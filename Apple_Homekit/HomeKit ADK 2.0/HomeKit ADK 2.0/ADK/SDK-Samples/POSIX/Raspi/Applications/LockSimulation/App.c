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

// An example that implements the lock homekit profile. The state of the accessory is stored in the
// global variable `accessoryConfiguration`. A basic lock simulation that simulates a secured lock
// by switching on an LED. A situation where the lock is blocked, i.e. secure/unsecure will fail,
// can be simulated. In addition to the Lock Mechanism service, the linked Lock Management service
// implements log output.
//
// This implementation is intended for POSIX platforms.
//
// To enable user interaction, POSIX signals are used:
//
// - The LED is used to simulate the lock state: a secured lock is indicated by switching LED to on.
// - Signal `SIGUSR1` will toggle the lock between secured and unsecured.
// - Signal `SIGUSR2` will toggle whether the lock is blocked, i.e. if subsequent lock (un-)secure
//   attempts will fail.
// - Signal `SIGTERM` will trigger a controlled shutdown.
//
// The code consists of multiple parts:
//
//   1. The definition of the accessory configuration and its internal state.
//
//   2. Helper functions to load and save the state of the accessory
//
//   3. The setup of the accessory HomeKit and the bridged accessories attribute database.
//
//   4. Device specific configuration, callbacks and LED switches.
//
//   5. The callbacks that implement the actual behavior of the accessory. If the
//      accessory state has changed, the corresponding device functions are called.
//
//   6. The initialization of the accessory state.
//
//   7. A callback that gets called when the server state is updated.

#ifdef LED_PORT
    #include <errno.h>
    #include <fcntl.h>
    #include <string.h>
    #include <stdio.h>
    #include <unistd.h>
#endif

#include <signal.h>

#include "HAP.h"
#include "HAPPlatform+Init.h"

#include "App.h"
#include "DB.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Domain used in the key value store for application data.
 *
 * Purged: On factory reset.
 */
#define kAppKeyValueStoreDomain_Configuration                                   ((HAPPlatformKeyValueStoreDomain) 0x00)

/**
 * Key used in the key value store to store the configuration state.
 *
 * Purged: On factory reset.
 */
#define kAppKeyValueStoreKey_Configuration_State                                ((HAPPlatformKeyValueStoreDomain) 0x00)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Device LED file descriptor
 */
typedef int LEDFileHandle;

/**
 * Global accessory state.
 */
typedef struct {
    struct {
        HAPCharacteristicValue_LockCurrentState currentState;
        HAPCharacteristicValue_LockTargetState targetState;
    } state;
    struct {
        LEDFileHandle led;
    } device;
    bool restoreFactorySettingsRequested;
    bool jammed;
    HAPAccessoryServerRef *server;
    HAPPlatformKeyValueStoreRef keyValueStore;
    HAPPlatformTimerRef identifyTimer;
} AccessoryConfiguration;

static AccessoryConfiguration accessoryConfiguration;

//----------------------------------------------------------------------------------------------------------------------

/**
 * Load the accessory state from persistent memory
 */
static void LoadAccessoryState(void)
{
    HAPPrecondition(accessoryConfiguration.keyValueStore);
    
    HAPError err;
    
    // Load persistent state if available
    bool found;
    size_t numBytes;
    
    err = HAPPlatformKeyValueStoreGet(accessoryConfiguration.keyValueStore,
        kAppKeyValueStoreDomain_Configuration, kAppKeyValueStoreKey_Configuration_State,
        &accessoryConfiguration.state, sizeof accessoryConfiguration.state, &numBytes, &found);
    
    if (err) {
        HAPAssert(err == kHAPError_Unknown);
        HAPFatalError();
    }
    if (!found || numBytes != sizeof accessoryConfiguration.state) {
        if (found) {
            HAPLogError(&kHAPLog_Default,
                "Unexpected app state found in key-value store. Resetting to default.");
        }
        HAPRawBufferZero(&accessoryConfiguration.state, sizeof accessoryConfiguration.state);
    }
}

/**
 * Save the accessory state to persistent memory
 */
static void SaveAccessoryState(void)
{
    HAPPrecondition(accessoryConfiguration.keyValueStore);
    
    HAPError err;
    err = HAPPlatformKeyValueStoreSet(accessoryConfiguration.keyValueStore,
        kAppKeyValueStoreDomain_Configuration, kAppKeyValueStoreKey_Configuration_State,
        &accessoryConfiguration.state, sizeof accessoryConfiguration.state);
    if (err) {
        HAPAssert(err == kHAPError_Unknown);
        HAPFatalError();
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**
 * HomeKit accessory that provides the Lock service.
 */
static const HAPAccessory accessory = {
    .aid = 1,
    .category = kHAPAccessoryCategory_Locks,
    .name = "Acme Lock Simulation",
    .manufacturer = "Acme",
    .model = "Lock1,1",
    .serialNumber = "099DB48E9E28",
    .firmwareVersion = "1",
    .hardwareVersion = "1",
    .services = (const HAPService *const[]) {
        &accessoryInformationService,
        &hapProtocolInformationService,
        &pairingService,
        &lockMechanismService,
        &lockManagementService,
        NULL
    },
    .cameraStreamConfigurations = NULL,
    .callbacks = {
        .identify = IdentifyAccessory
    }
};

//----------------------------------------------------------------------------------------------------------------------

/**
 * Configure the LED
 */
static void DeviceConfigureLED(void)
{
    HAPLogInfo(&kHAPLog_Default, "%s", __func__);
    
    #ifdef LED_PORT
        HAPPrecondition(accessoryConfiguration.device.led == -1);

        int fd;
        do {
            fd = open(LED_PORT, O_RDWR);
        } while (fd == -1 && errno == EINTR);
        if (fd < 0) {
            int _errno = errno;
            HAPAssert(fd == -1);
            HAPLogError(&kHAPLog_Default, "%s: Opening the LED port failed: %d", __func__, _errno);
            return;
        }
        accessoryConfiguration.device.led = fd;
        HAPAssert(accessoryConfiguration.device.led >= 0);

        #ifdef LED_TRIGGER
        {
            // Create empty LED configuration.
            HAPLogInfo(&kHAPLog_Default, "%s: Disable LED trigger.", __func__);
            do {
                fd = open(LED_TRIGGER, O_RDWR);
            } while (fd == -1 && errno == EINTR);
            if (fd < 0) {
                int _errno = errno;
                HAPAssert(fd == -1);
                HAPLogError(&kHAPLog_Default, "%s: Disabling LED trigger failed: %d", __func__, _errno);
                return;
            }
            
            // Write "none" in the trigger.
            const char *bytes = "none";
            size_t numBytes = strlen(bytes);
            ssize_t n;
            do {
                n = write(fd, bytes, numBytes);
            } while (n == -1 && errno == EINTR);
            HAPAssert(n == (ssize_t)numBytes);
            (void) close(fd);
        }
        #endif
        
        // Initialize LED with off, otherwise it fails to work the first time.
        ssize_t n;
        const char* bytes = "0"; // brightness
        size_t numBytes = strlen(bytes);
        do {
            n = write(accessoryConfiguration.device.led, bytes, numBytes);
        } while (n == -1 && errno == EINTR);
        HAPAssert(n == (ssize_t)numBytes);
        
        // The configuration takes some time.
        sleep(1);
    #endif
}

/**
 * Unconfigure the LED.
 */
static void DeviceUnconfigureLED(void)
{
    HAPLogInfo(&kHAPLog_Default, "%s", __func__);
    #ifdef LED_PORT
        if (accessoryConfiguration.device.led != -1) {
            close(accessoryConfiguration.device.led);
            accessoryConfiguration.device.led = -1;
        }
    #endif
}

/**
 * Enable the LED on the device.
 */
static void DeviceEnableLED(void)
{
    HAPLogInfo(&kHAPLog_Default, "%s", __func__);
    #ifdef LED_PORT
        HAPPrecondition(accessoryConfiguration.device.led != -1);

        const char* bytes = "255"; // brightness
        const size_t numBytes = strlen(bytes);
        ssize_t len;
        do {
            len = write(accessoryConfiguration.device.led, bytes, numBytes);
        } while (len == -1 && errno == EINTR);
        HAPAssert(len == (ssize_t)numBytes);
    #endif
}

/**
 * Disable the LED on the device.
 */
static void DeviceDisableLED(void)
{
    HAPLogInfo(&kHAPLog_Default, "%s", __func__);
    #ifdef LED_PORT
        HAPPrecondition(accessoryConfiguration.device.led != -1);

        const char* bytes = "0"; // brightness
        const size_t numBytes = strlen(bytes);
        ssize_t len;
        do {
            len = write(accessoryConfiguration.device.led, bytes, numBytes);
        } while (len == -1 && errno == EINTR);
        HAPAssert(len == (ssize_t)numBytes);
    #endif
}

/**
 * Secure the lock.
 */
static void DeviceSecureLock(void)
{
    HAPLogInfo(&kHAPLog_Default,"%s", __func__);
    DeviceEnableLED();
}

/**
 * Unsecure the lock.
 */
static void DeviceUnsecureLock(void)
{
    HAPLogInfo(&kHAPLog_Default,"%s", __func__);
    DeviceDisableLED();
}

/**
 * Block the lock - next (un-)secure lock will result in jammed state.
 */
static void DeviceBlockLock(void)
{
    HAPLogInfo(&kHAPLog_Default,"%s", __func__);
    accessoryConfiguration.jammed = true;
}

/**
 * Unblock the lock - next (un-)secure lock will succeed.
 */
static void DeviceUnblockLock(void)
{
    HAPLogInfo(&kHAPLog_Default,"%s", __func__);
    accessoryConfiguration.jammed = false;
}

//----------------------------------------------------------------------------------------------------------------------

/**
 * Set the lock state.
 */
static void SetLockState(
    HAPCharacteristicValue_LockTargetState targetState)
{
    HAPLogInfo(&kHAPLog_Default,"%s", __func__);
    
    HAPCharacteristicValue_LockCurrentState currentState = 0;
    if (accessoryConfiguration.jammed) {
        currentState = kHAPCharacteristicValue_LockCurrentState_Jammed;
    } else {
        switch (targetState) {
            case kHAPCharacteristicValue_LockTargetState_Secured: {
                currentState = kHAPCharacteristicValue_LockCurrentState_Secured;
            } break;
            case kHAPCharacteristicValue_LockTargetState_Unsecured: {
                currentState = kHAPCharacteristicValue_LockCurrentState_Unsecured;
            } break;
        }
    }
    switch (currentState) {
        case kHAPCharacteristicValue_LockCurrentState_Secured:
            DeviceSecureLock();
            break;
        case kHAPCharacteristicValue_LockCurrentState_Unsecured:
            DeviceUnsecureLock();
            break;
        case kHAPCharacteristicValue_LockCurrentState_Jammed:
        case kHAPCharacteristicValue_LockCurrentState_Unknown:
            break;
    }
    
    if (accessoryConfiguration.state.targetState != targetState ||
        accessoryConfiguration.state.currentState != currentState)
    {
        if (accessoryConfiguration.state.targetState != targetState) {
            accessoryConfiguration.state.targetState = targetState;
            
            HAPAccessoryServerRaiseEvent(
                accessoryConfiguration.server,
                &lockMechanismLockTargetStateCharacteristic,
                &lockMechanismService,
                &accessory);
        }
        if (accessoryConfiguration.state.currentState != currentState) {
            accessoryConfiguration.state.currentState = currentState;
            
            HAPAccessoryServerRaiseEvent(
                accessoryConfiguration.server,
                &lockMechanismLockCurrentStateCharacteristic,
                &lockMechanismService,
                &accessory);
        }
        
        SaveAccessoryState();
    }
}

/**
 * Toggle the lock state.
 */
static void ToggleLockState(void)
{
    HAPLogInfo(&kHAPLog_Default,"%s", __func__);
    switch (accessoryConfiguration.state.targetState) {
        case kHAPCharacteristicValue_LockTargetState_Secured: {
            SetLockState(kHAPCharacteristicValue_LockTargetState_Unsecured);
        } break;
        case kHAPCharacteristicValue_LockTargetState_Unsecured: {
            SetLockState(kHAPCharacteristicValue_LockTargetState_Secured);
        } break;
    }
}

/**
 * Toggle if the lock should be jammed.
 */
static void ToggleLockJammed(void)
{
    HAPLogInfo(&kHAPLog_Default,"%s", __func__);
    
    accessoryConfiguration.jammed = !accessoryConfiguration.jammed;
    if (accessoryConfiguration.jammed) {
        DeviceBlockLock();
    } else {
        DeviceUnblockLock();
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**
 * Global signal handler.
 */
static void HandleSignalCallback(
    void *_Nullable context,
    size_t contextSize)
{
    // Invoked from the run loop.
    
    HAPAssert(context);
    HAPAssert(contextSize == sizeof (int));
    
    int signum = *((int *) context);
    
    switch (signum) {
        case SIGUSR1: {
            ToggleLockState();
        } break;
        case SIGUSR2: {
            ToggleLockJammed();
        } break;
        case SIGTERM: {
            HAPAccessoryServerStop(accessoryConfiguration.server);
        } break;
        default: {
        } break;
    }
}

static void HandleSignal(
    int signum)
{
    HAPError err;
    
    err = HAPPlatformRunLoopScheduleCallback(
        HandleSignalCallback, &signum, sizeof signum);
    if (err) {
        HAPLogError(&kHAPLog_Default, "SignalHandler failed");
        HAPFatalError();
    }
}

/**
 * Initialize Signal Handlers.
 */
static void InitializeSignalHandlers()
{
    if (signal(SIGUSR1, HandleSignal) == SIG_ERR) {
        HAPLogError(&kHAPLog_Default, "SignalHandler initialization failed");
        HAPFatalError();
    }
    if (signal(SIGUSR2, HandleSignal) == SIG_ERR) {
        HAPLogError(&kHAPLog_Default, "SignalHandler initialization failed");
        HAPFatalError();
    }
    if (signal(SIGTERM, HandleSignal) == SIG_ERR) {
        HAPLogError(&kHAPLog_Default, "SignalHandler initialization failed");
        HAPFatalError();
    }
}

/**
 * Configure platform specific IO.
 */
static void ConfigureIO(void)
{
    HAPLogInfo(&kHAPLog_Default, "%s", __func__);
    
    DeviceConfigureLED();
    InitializeSignalHandlers();
}

/**
 * Unconfigure platform specific IO.
 */
static void UnconfigureIO(void)
{
    HAPLogInfo(&kHAPLog_Default, "%s", __func__);
    
    DeviceUnconfigureLED();
}

//----------------------------------------------------------------------------------------------------------------------

HAP_RESULT_USE_CHECK
HAPError IdentifyAccessory(
    HAPAccessoryServerRef *server HAP_UNUSED,
    const HAPAccessoryIdentifyRequest *request HAP_UNUSED,
    void *_Nullable context HAP_UNUSED)
{
    HAPLogInfo(&kHAPLog_Default, "%s", __func__);
    return kHAPError_None;
}

/**
 * Handle read request to the 'Lock Current State' characteristic of the Lock Mechanism service.
 */
HAP_RESULT_USE_CHECK
HAPError HandleLockMechanismLockCurrentStateRead(
    HAPAccessoryServerRef *server HAP_UNUSED,
    const HAPUInt8CharacteristicReadRequest *request HAP_UNUSED,
    uint8_t *value,
    void *_Nullable context HAP_UNUSED)
{
    HAPLogInfo(&kHAPLog_Default, "%s", __func__);
    *value = accessoryConfiguration.state.currentState;
    switch (*value) {
        case kHAPCharacteristicValue_LockCurrentState_Secured: {
            HAPLogInfo(&kHAPLog_Default, "%s: %s", __func__, "LockCurrentState_Secured");
        } break;
        case kHAPCharacteristicValue_LockCurrentState_Unsecured: {
            HAPLogInfo(&kHAPLog_Default, "%s: %s", __func__, "LockCurrentState_Unsecured");
        } break;
        case kHAPCharacteristicValue_LockCurrentState_Jammed: {
            HAPLogInfo(&kHAPLog_Default, "%s: %s", __func__, "LockCurrentState_Jammed");
        } break;
        case kHAPCharacteristicValue_LockCurrentState_Unknown: {
            HAPLogInfo(&kHAPLog_Default, "%s: %s", __func__, "LockCurrentState_Unknown");
        } break;
    }
    return kHAPError_None;
}

/**
 * Handle read request to the 'Lock Target State' characteristic of the Lock Mechanism service.
 */
HAP_RESULT_USE_CHECK
HAPError HandleLockMechanismLockTargetStateRead(
    HAPAccessoryServerRef *server HAP_UNUSED,
    const HAPUInt8CharacteristicReadRequest *request HAP_UNUSED,
    uint8_t *value,
    void *_Nullable context HAP_UNUSED)
{
    HAPLogInfo(&kHAPLog_Default, "%s", __func__);
    *value = accessoryConfiguration.state.targetState;
    switch (*value) {
        case kHAPCharacteristicValue_LockTargetState_Secured: {
            HAPLogInfo(&kHAPLog_Default, "%s: %s", __func__, "LockTargetState_Secured");
        } break;
        case kHAPCharacteristicValue_LockTargetState_Unsecured: {
            HAPLogInfo(&kHAPLog_Default, "%s: %s", __func__, "LockTargetState_Unsecured");
        } break;
    }
    return kHAPError_None;
}

/**
 * Handle write request to the 'Lock Target State' characteristic of the Lock Mechanism service.
 */
HAP_RESULT_USE_CHECK
HAPError HandleLockMechanismLockTargetStateWrite(
    HAPAccessoryServerRef *server HAP_UNUSED,
    const HAPUInt8CharacteristicWriteRequest *request HAP_UNUSED,
    uint8_t value,
    void *_Nullable context HAP_UNUSED)
{
    HAPLogInfo(&kHAPLog_Default, "%s", __func__);
    HAPCharacteristicValue_LockTargetState targetState = (HAPCharacteristicValue_LockTargetState) value;
    switch (targetState) {
        case kHAPCharacteristicValue_LockTargetState_Secured: {
            HAPLogInfo(&kHAPLog_Default, "%s: %s", __func__, "LockTargetState_Secured");
        } break;
        case kHAPCharacteristicValue_LockTargetState_Unsecured: {
            HAPLogInfo(&kHAPLog_Default, "%s: %s", __func__, "LockTargetState_Unsecured");
        } break;
    }
    SetLockState(targetState);
    return kHAPError_None;
}

/**
 * Handle write request to the 'Lock Control Point' characteristic of the Lock Management service.
 */
HAP_RESULT_USE_CHECK
HAPError HandleLockManagementLockControlPointWrite(
    HAPAccessoryServerRef *server HAP_UNUSED,
    const HAPTLV8CharacteristicWriteRequest *request HAP_UNUSED,
    HAPTLVReaderRef *requestReader,
    void *_Nullable context HAP_UNUSED)
{
    HAPPrecondition(requestReader);
    
    HAPLogInfo(&kHAPLog_Default, "%s", __func__);
    
    HAPError err;
    
    // Simply validate input.
    err = HAPTLVReaderGetAll(requestReader, (HAPTLV *const[]) {
        NULL
    });
    if (err) {
        HAPAssert(err == kHAPError_InvalidData);
        return err;
    }
    return kHAPError_None;
}

/**
 * Handle read request to the 'Version' characteristic of the Lock Management service.
 */
HAP_RESULT_USE_CHECK
HAPError HandleLockManagementVersionRead(
    HAPAccessoryServerRef *server HAP_UNUSED,
    const HAPStringCharacteristicReadRequest *request HAP_UNUSED,
    char *value,
    size_t maxBytes,
    void *_Nullable context HAP_UNUSED)
{
    HAPLogInfo(&kHAPLog_Default, "%s", __func__);
    const char *stringToCopy = "1.0";
    size_t numBytes = HAPStringGetNumBytes(stringToCopy);
    if (numBytes >= maxBytes) {
        HAPLogError(&kHAPLog_Default, "Not enough space to store %s (needed: %zu, available: %zu).",
            "Version", numBytes + 1, maxBytes);
        return kHAPError_OutOfResources;
    }
    HAPRawBufferCopyBytes(value, stringToCopy, numBytes);
    value[numBytes] = '\0';
    return kHAPError_None;
}

//----------------------------------------------------------------------------------------------------------------------

void AppCreate(
    HAPAccessoryServerRef *server,
    HAPPlatformKeyValueStoreRef keyValueStore)
{
    HAPPrecondition(server);
    HAPPrecondition(keyValueStore);
    
    HAPLogInfo(&kHAPLog_Default, "%s", __func__);
    
    HAPRawBufferZero(&accessoryConfiguration, sizeof accessoryConfiguration);
    accessoryConfiguration.server = server;
    accessoryConfiguration.keyValueStore = keyValueStore;
    accessoryConfiguration.device.led = -1;
    
    ConfigureIO();
    
    LoadAccessoryState();
    
    switch (accessoryConfiguration.state.currentState) {
        case kHAPCharacteristicValue_LockCurrentState_Secured: {
            DeviceSecureLock();
        } break;
        case kHAPCharacteristicValue_LockCurrentState_Unsecured: {
            DeviceUnsecureLock();
        } break;
        case kHAPCharacteristicValue_LockCurrentState_Jammed:
        case kHAPCharacteristicValue_LockCurrentState_Unknown:
          break;
    }
}

void AppRelease(void)
{
    UnconfigureIO();
}

void AppAccessoryServerStart(void)
{
    HAPAccessoryServerStart(accessoryConfiguration.server, &accessory);
    
    // Enter pairing mode.
    if (HAVE_NFC) {
        HAPLogInfo(&kHAPLog_Default, "Entering pairing mode for 5 minutes...");
        HAPAccessoryServerEnterPairingMode(accessoryConfiguration.server);
    }
}

//----------------------------------------------------------------------------------------------------------------------

void AccessoryServerHandleUpdatedState(
    HAPAccessoryServerRef *server,
    void *_Nullable context)
{
    HAPPrecondition(server);
    HAPPrecondition(!context);
    
    HAPError err;
    
    switch (HAPAccessoryServerGetState(server)) {
        case kHAPAccessoryServerState_Idle: {
            HAPLogInfo(&kHAPLog_Default, "Accessory Server State did update: Idle.");
            // AccessoryServer stop requested by signal.
            if (accessoryConfiguration.restoreFactorySettingsRequested) {
                HAPLogInfo(&kHAPLog_Default, "A factory reset has been requested.");
                
                // Purge app state.
                err = HAPPlatformKeyValueStorePurgeDomain(
                    accessoryConfiguration.keyValueStore,
                    kAppKeyValueStoreDomain_Configuration);
                if (err) {
                    HAPAssert(err == kHAPError_Unknown);
                    HAPFatalError();
                }
                
                // Reset HomeKit state.
                err = HAPRestoreFactorySettings(accessoryConfiguration.keyValueStore);
                if (err) {
                    HAPAssert(err == kHAPError_Unknown);
                    HAPFatalError();
                }
                
                // Restore platform specific factory settings.
                RestorePlatformFactorySettings();
                
                // De-initialize App.
                AppRelease();
                
                accessoryConfiguration.restoreFactorySettingsRequested = false;
                
                // Re-initialize App.
                AppCreate(server, accessoryConfiguration.keyValueStore);
                
                // Restart accessory server.
                AppAccessoryServerStart();
                
                return;
            }
            
            // Signal run loop to exit.
            HAPPlatformRunLoopStop();
        } return;
        case kHAPAccessoryServerState_Running: {
            HAPLogInfo(&kHAPLog_Default, "Accessory Server State did update: Running.");
        } return;
        case kHAPAccessoryServerState_Stopping: {
            HAPLogInfo(&kHAPLog_Default, "Accessory Server State did update: Stopping.");
        } return;
    }
    HAPFatalError();
}
