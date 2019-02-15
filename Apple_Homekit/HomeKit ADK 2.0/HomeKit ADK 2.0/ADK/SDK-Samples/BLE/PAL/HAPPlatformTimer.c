//
// Package: HomeKit-ADK
// Version: 2.0 (16A62)
//
// Disclaimer: IMPORTANT: This Apple software is supplied to you, by Apple Inc. ("Apple"), in your
// capacity as a current, and in good standing, Licensee in the MFi Licensing Program. Use of this
// Apple software is governed by and subject to the terms and conditions of your MFi License,
// including, but not limited to, the restrictions specified in the provision entitled "Public
// Software", and is further subject to your agreement to the following additional terms, and your
// agreement that the use, installation, modification or redistribution of this Apple software
// constitutes acceptance of these additional terms. If you do not agree with these additional terms,
// you may not use, install, modify or redistribute this Apple software.
//
// Subject to all of these terms and in consideration of your agreement to abide by them, Apple grants
// you, for as long as you are a current and in good-standing MFi Licensee, a personal, non-exclusive
// license, under Apple's copyrights in this Apple software (the "Apple Software"), to use,
// reproduce, and modify the Apple Software in source form, and to use, reproduce, modify, and
// redistribute the Apple Software, with or without modifications, in binary form, in each of the
// foregoing cases to the extent necessary to develop and/or manufacture "Proposed Products" and
// "Licensed Products" in accordance with the terms of your MFi License. While you may not
// redistribute the Apple Software in source form, should you redistribute the Apple Software in binary
// form, you must retain this notice and the following text and disclaimers in all such redistributions
// of the Apple Software. Neither the name, trademarks, service marks, or logos of Apple Inc. may be
// used to endorse or promote products derived from the Apple Software without specific prior written
// permission from Apple. Except as expressly stated in this notice, no other rights or licenses,
// express or implied, are granted by Apple herein, including but not limited to any patent rights that
// may be infringed by your derivative works or by other works in which the Apple Software may be
// incorporated. Apple may terminate this license to the Apple Software by removing it from the list
// of Licensed Technology in the MFi License, or otherwise in accordance with the terms of such MFi License.
//
// Unless you explicitly state otherwise, if you provide any ideas, suggestions, recommendations, bug
// fixes or enhancements to Apple in connection with this software ("Feedback"), you hereby grant to
// Apple a non-exclusive, fully paid-up, perpetual, irrevocable, worldwide license to make, use,
// reproduce, incorporate, modify, display, perform, sell, make or have made derivative works of,
// distribute (directly or indirectly) and sublicense, such Feedback in connection with Apple products
// and services. Providing this Feedback is voluntary, but if you do provide Feedback to Apple, you
// acknowledge and agree that Apple may exercise the license granted above without the payment of
// royalties or further consideration to Participant.
//
// The Apple Software is provided by Apple on an "AS IS" basis. APPLE MAKES NO WARRANTIES, EXPRESS OR
// IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY
// AND FITNESS FOR A PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE OR
// IN COMBINATION WITH YOUR PRODUCTS.
//
// IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION, MODIFICATION
// AND/OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER UNDER THEORY OF CONTRACT, TORT
// (INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// Copyright (C) 2018 Apple Inc. All Rights Reserved.
//

#include "app_timer.h"

#include "HAPPlatform.h"

static const HAPLogObject logObject = {
    .subsystem = kHAPPlatform_LogSubsystem,
    .category = "Timer"
};

#define kTimerStorage_MaxTimers ((size_t) 32)

typedef struct {
    /**
     * ID. 0 if timer has never been used.
     */
    HAPPlatformTimerRef id;

    /**
     * Deadline after which the timer expires.
     */
    HAPTime deadline;

    /**
     * Callback. NULL if timer inactive.
     */
    HAPPlatformTimerCallback _Nullable callback;

    /**
     * The context parameter given to the HAPPlatformTimerRegister function.
     */
    void *_Nullable context;
} HAPPlatformTimer;

static HAPPlatformTimer timers[kTimerStorage_MaxTimers];
static size_t numActiveTimers;
static size_t numExpiredTimers;

APP_TIMER_DEF(hwTimer);
static HAPTime hwTimerDeadline = UINT64_MAX;

static void ScheduleNextTimer(void)
{
    // Do not call any functions that may lead to reentrancy!

    HAPAssert(!numExpiredTimers);
    HAPTime nextDeadline = UINT64_MAX;
    if (numActiveTimers) {
        nextDeadline = timers[0].deadline;
    }

    // If HW is already set to next timer, we are done.
    if (nextDeadline == hwTimerDeadline) {
        return;
    }

    // Calculate delta until next timer.
    HAPTime now = HAPPlatformClockGetCurrent();
    if (nextDeadline < now) {
        nextDeadline = now;
    }

    // Stop old timer.
    uint32_t e = app_timer_stop(hwTimer);
    if (e) {
        HAPLogError(&logObject, "app_timer_stop failed: 0x%04x.", (unsigned int) e);
        HAPFatalError();
    }

    if (nextDeadline == UINT64_MAX) {
        // No timer needed. Turn off HW. Note: Clock won't continue in this time.
        // It is only allowed to suspend the clock while no connection is active.
        // However, for BLE, a timer is always active during a HAP connection.
        // Therefore, we don't have to check with the BLE module here.
        HAPLogInfo(&logObject, "No timers scheduled - Clock suspended.");
        NRF_RTC2->TASKS_STOP = 1;
    } else {
        // Turn on HW if necessary.
        if (hwTimerDeadline == UINT64_MAX) {
            NRF_RTC2->PRESCALER = 0;
            NRF_RTC2->TASKS_START = 1;
            HAPLogInfo(&logObject, "Clock resumed.");
        }

        // Start timer.
        uint32_t numTicks = APP_TIMER_TICKS(nextDeadline - now + 2);
        if (numTicks < 5) {
            numTicks = 5;
        }
        e = app_timer_start(hwTimer, numTicks, NULL);
        if (e) {
            HAPLogError(&logObject, "app_timer_start failed: 0x%04x.", (unsigned int) e);
            HAPFatalError();
        }
    }

    // HW has been updated.
    hwTimerDeadline = nextDeadline;
}

static bool needsSchedule;

static void HandleHWTimerExpired(
    void *context HAP_UNUSED)
{
    // Reentrancy note - Callbacks may lead to reentrant add / remove timer invocations.
    // Do not call any functions that may lead to reentrancy!
    //
    // The idea is that timers 0 ..< numExpiredTimers are managed here.
    // add / remove must only move timers numExpiredTimers ..< numActiveTimers.
    // Timers added through reentrancy are allocated after the expired timers.
    // Timers removed through reentrancy have their callback set to NULL.

    needsSchedule = true;

    // Get current time, and, by checking, make sure that it is updated.
    HAPTime now = HAPPlatformClockGetCurrent();
    if (hwTimerDeadline > now) {
        HAPLog(&logObject,
            "app_timer returned early (%lu ms). Scheduling again.", (unsigned long) (hwTimerDeadline - now));
        hwTimerDeadline = now;
    }

    // Find number of expired timers.
    for (numExpiredTimers = 0; numExpiredTimers < numActiveTimers; numExpiredTimers++) {
        if (timers[numExpiredTimers].deadline > now) {
            break;
        }
    }

    // Invoke callbacks.
    for (size_t i = 0; i < numExpiredTimers; i++) {
        if (timers[i].callback) {
            timers[i].callback(timers[i].id, timers[i].context);
            timers[i].callback = NULL;
        }
    }

    // Free memory.
    HAPAssert(numExpiredTimers <= numActiveTimers);
    while (numExpiredTimers) {
        HAPPlatformTimerRef id = timers[0].id;
        HAPRawBufferCopyBytes(&timers[0], &timers[1], (numActiveTimers - 1) * sizeof timers[0]);
        numActiveTimers--;
        numExpiredTimers--;
        timers[numActiveTimers].id = id;
    }
    HAPAssert(!numExpiredTimers);

    // Schedule next timer.
    needsSchedule = false;
    ScheduleNextTimer();
}

static void Initialize(void)
{
    static bool initialized;
    if (initialized) {
        return;
    }

    // Do not call any functions that may lead to reentrancy!

    uint32_t e = app_timer_create(&hwTimer, APP_TIMER_MODE_SINGLE_SHOT, HandleHWTimerExpired);
    if (e) {
        HAPLogError(&logObject, "app_timer_create failed: 0x%04x.", (unsigned int) e);
        HAPFatalError();
    }
    initialized = true;
}

HAP_RESULT_USE_CHECK
HAPError HAPPlatformTimerRegister(
    HAPPlatformTimerRef *timer,
    HAPTime deadline,
    HAPPlatformTimerCallback callback,
    void *_Nullable context)
{
    HAPPrecondition(timer);
    HAPPrecondition(callback);

    Initialize();

    // Do not call any functions that may lead to reentrancy!

    if (numActiveTimers == sizeof timers / sizeof timers[0]) {
        HAPLog(&logObject, "Cannot allocate more timers.");
        return kHAPError_OutOfResources;
    }

    // Find timer slot.
    size_t i;
    for (i = numExpiredTimers; i < numActiveTimers; i++) {
        if (timers[i].deadline > deadline) {
            // Search condition must be '>' and not '>=' to ensure that timers fire in ascending order of their
            // deadlines and that timers registered with the same deadline fire in order of registration.
            break;
        }
    }

    // Move timers.
    HAPPlatformTimerRef id = timers[numActiveTimers].id;
    HAPRawBufferCopyBytes(&timers[i + 1], &timers[i], (numActiveTimers - i) * sizeof timers[0]);
    timers[i].id = id;
    numActiveTimers++;

    // Prepare timer.
    static HAPPlatformTimerRef peakNumTimers;
    if (!timers[i].id) {
        timers[i].id = ++peakNumTimers;
        HAPAssert(timers[i].id <= sizeof timers / sizeof timers[0]);
        HAPLogInfo(&logObject, "New maximum of concurrent timers: %u (%u%%).",
            (unsigned int) peakNumTimers, 100 * peakNumTimers / (sizeof timers / sizeof timers[0]));
    }

    // Store client data.
    timers[i].deadline = deadline;
    timers[i].callback = callback;
    timers[i].context = context;

    // Store timer ID.
    *timer = timers[i].id;

    // Schedule next timer.
    if (!needsSchedule) {
        ScheduleNextTimer();
    }

    return kHAPError_None;
}

void HAPPlatformTimerDeregister(
    HAPPlatformTimerRef timer)
{
    HAPPrecondition(timer);

    Initialize();

    // Do not call any functions that may lead to reentrancy!

    // Find timer.
    for (size_t i = 0; i < numActiveTimers; i++) {
        if (timers[i].id == timer) {
            HAPAssert(timers[i].callback);
            timers[i].callback = NULL;

            if (i >= numExpiredTimers) {
                // Move remaining timers.
                HAPRawBufferCopyBytes(&timers[i], &timers[i + 1], (numActiveTimers - i - 1) * sizeof timers[i]);
                numActiveTimers--;
                timers[numActiveTimers].id = timer;
            }

            // Schedule next timer.
            if (!needsSchedule) {
                ScheduleNextTimer();
            }
            return;
        }
    }

    // Timer not found.
    HAPLogError(&logObject, "Timer not found: %lu.", (unsigned long) timer);
    HAPFatalError();
}
