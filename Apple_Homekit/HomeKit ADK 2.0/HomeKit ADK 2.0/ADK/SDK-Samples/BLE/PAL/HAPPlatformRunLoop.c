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

#include "app_scheduler.h"
#include "nrf_log_ctrl.h"
#include "nrf_sdh_soc.h"

#include "HAPPlatformKeyValueStore+Init.h"
#include "HAPPlatformRunLoop+Init.h"

static const HAPLogObject logObject = {
    .subsystem = kHAPPlatform_LogSubsystem,
    .category = "RunLoop"
};

/**
 * State of the run loop.
 */
HAP_ENUM_BEGIN(uint8_t, HAPPlatformRunLoopState) {
    /**
     * The run loop is not yet started or finished stopping.
     */
    kHAPPlatformRunLoopState_Idle,

    /**
     * The run loop is running.
     */
    kHAPPlatformRunLoopState_Running,

    /**
     * The run loop is stopping.
     */
    kHAPPlatformRunLoopState_Stopping
} HAP_ENUM_END(uint8_t, HAPPlatformRunLoopState);

static HAPPlatformRunLoopState state;

static HAPPlatformKeyValueStoreRef keyValueStore;

void HAPPlatformRunLoopCreate(
    const HAPPlatformRunLoopOptions *options)
{
    HAPPrecondition(options);
    HAPPrecondition(options->keyValueStore);

    keyValueStore = options->keyValueStore;
}

void HAPPlatformRunLoopRun(void)
{
    HAPPrecondition(state == kHAPPlatformRunLoopState_Idle);
    
    HAPLogInfo(&logObject, "Entering run loop.");
    state = kHAPPlatformRunLoopState_Running;
    
    for (;;) {
        // Execute pending events.
        app_sched_execute();
        
        // Exit run loop if requested.
        if (state != kHAPPlatformRunLoopState_Running && !HAPPlatformKeyValueStoreIsBusy(keyValueStore)) {
            HAPAssert(state == kHAPPlatformRunLoopState_Stopping);
            
            HAPLogInfo(&logObject, "Exiting run loop.");
            state = kHAPPlatformRunLoopState_Idle;
            return;
        }
        
        if (NRF_LOG_PROCESS() == false) {
            // Power manage.
            ret_code_t e = sd_app_evt_wait();
            if (e) {
                HAPLogError(&logObject, "sd_app_evt_wait failed: 0x%04x.", (unsigned int) e);
                HAPFatalError();
            }
        }
    }
}

void HAPPlatformRunLoopStop(void)
{
    if (state == kHAPPlatformRunLoopState_Running) {
        HAPLogInfo(&logObject, "Registering request to stop the run loop.");
        state = kHAPPlatformRunLoopState_Stopping;
    }
}

static void AppSchedEventHandler(
    void *p_event_data,
    uint16_t event_size)
{
    HAPPrecondition(p_event_data);
    HAPPrecondition(event_size >= sizeof (HAPPlatformRunLoopCallback));

    uint8_t *bytes = p_event_data;
    HAPPlatformRunLoopCallback callback;
    HAPRawBufferCopyBytes(&callback, bytes, sizeof callback);

    event_size -= sizeof callback;
    HAPRawBufferCopyBytes(bytes, bytes + sizeof callback, event_size);

    callback(event_size ? bytes : NULL, event_size);
}

HAP_RESULT_USE_CHECK
HAPError HAPPlatformRunLoopScheduleCallback(
    HAPPlatformRunLoopCallback callback,
    void *_Nullable const context,
    size_t contextSize)
{
    HAPPrecondition(callback);
    HAPPrecondition(!contextSize || context);

    if (contextSize > UINT8_MAX) {
        HAPLogError(&logObject, "Contexts larger than UINT8_MAX are not supported.");
        return kHAPError_OutOfResources;
    }

    // Serialize event context.
    // Format: Callback pointer followed by context data.
    // Context length is not serialized as it is kept track of by the nRF5 SDK.
    // Context is copied to offset 0 when invoking the callback to ensure proper alignment.
    uint8_t bytes[sizeof callback + UINT8_MAX];
    size_t numBytes = 0;
    HAPRawBufferCopyBytes(&bytes[numBytes], &callback, sizeof callback); numBytes += sizeof callback;
    if (context) {
        HAPRawBufferCopyBytes(&bytes[numBytes], context, contextSize); numBytes += contextSize;
    }
    HAPAssert(numBytes <= sizeof bytes);

    // Schedule event.
    uint32_t e = app_sched_event_put(bytes, (uint16_t) numBytes, AppSchedEventHandler);
    if (e) {
        HAPLogError(&logObject, "app_sched_event_put failed: %lu.", (unsigned long) e);
        return kHAPError_OutOfResources;
    }

    return kHAPError_None;
}
