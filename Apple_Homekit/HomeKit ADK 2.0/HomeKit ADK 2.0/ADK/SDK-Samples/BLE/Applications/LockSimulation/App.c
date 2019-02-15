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
// This implementation is intended for a Nordic nRF52832 board that provides four LEDs and four
// Buttons.
//
// To enable user interaction, buttons and LEDs on the development board are used:
//
// - LED 1 is used to simulate the lock state: a secured lock is indicated by switching LED 1 to on.
// - LED 2 is used to indicate that the lock is blocked: it is switched on while the lock is blocked.
// - Button 1 will toggle the lock between secured and unsecured.
// - Button 2 will toggle if the lock is blocked, i.e. if subsequent lock (un-)secure attempts will
//   fail.
// - Button 3 will trigger a factory reset.
//
// The code consists of multiple parts:
//
//   1. The definition of the accessory configuration and its internal state.
//
//   2. Helper functions to load and save the state of the accessory
//
//   3. The definitions for the HomeKit attribute database.
//
//   4. Device specific configuration, callbacks and LED switches.
//
//   5. The callbacks that implement the actual behavior of the accessory. If the
//      accessory state has changed, the corresponding device functions are called.
//
//   6. The initialization of the accessory state.
//
//   7. A callback that gets called when the server state is updated.


#include "HAP.h"
#include "HAPPlatform+Init.h"

#include "app_button.h"
#include "boards.h"

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
 * Global accessory state.
 */
typedef struct {
    struct {
        HAPCharacteristicValue_LockCurrentState currentState;
        HAPCharacteristicValue_LockTargetState targetState;
    } state;
    struct {
        uint32_t lockPin;
        uint32_t jammedPin;
    } device;
    bool restoreFactorySettingsRequested;
    bool jammed;
    HAPAccessoryServerRef *server;
    HAPPlatformKeyValueStoreRef keyValueStore;
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
 * HomeKit accessory that provides the Lightbulb service.
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
 * Enable LED on the device.
 */
static void DeviceEnableLED(
    uint32_t pin)
{
    HAPLogInfo(&kHAPLog_Default, "%s", __func__);
    nrf_gpio_pin_clear(pin);
}

/**
 * Disable LED on the device.
 */
static void DeviceDisableLED(
    uint32_t pin)
{
    HAPLogInfo(&kHAPLog_Default, "%s", __func__);
    nrf_gpio_pin_set(pin);
}

//----------------------------------------------------------------------------------------------------------------------

/**
 * Secure the lock.
 */
static void SecureLock(
    HAPCharacteristicValue_LockCurrentState *effectiveState)
{
    HAPLogInfo(&kHAPLog_Default, "%s", __func__);
    DeviceEnableLED(accessoryConfiguration.device.lockPin);
    *effectiveState = kHAPCharacteristicValue_LockCurrentState_Secured;
}

/**
 * Unsecure the lock.
 */
static void UnsecureLock(
    HAPCharacteristicValue_LockCurrentState *effectiveState)
{
    HAPLogInfo(&kHAPLog_Default, "%s", __func__);
    DeviceDisableLED(accessoryConfiguration.device.lockPin);
    *effectiveState = kHAPCharacteristicValue_LockCurrentState_Unsecured;
}

/**
 * Block the lock - next (un-)secure lock will result in jammed state.
 */
static void BlockLock(void)
{
    HAPLogInfo(&kHAPLog_Default, "%s", __func__);
    DeviceEnableLED(accessoryConfiguration.device.jammedPin);
}

/**
 * Unblock the lock - next (un-)secure lock will succeed.
 */
static void UnblockLock(void)
{
    HAPLogInfo(&kHAPLog_Default, "%s", __func__);
    DeviceDisableLED(accessoryConfiguration.device.jammedPin);
}

/**
 * Toggle the lock state.
 */
static void SetLockState(
    HAPCharacteristicValue_LockTargetState targetState)
{
    HAPLogInfo(&kHAPLog_Default, "%s", __func__);
    
    HAPCharacteristicValue_LockCurrentState currentState;
    if (accessoryConfiguration.jammed) { // simulate that DeviceSecureLock / DeviceUnsecureLock failed
        currentState = kHAPCharacteristicValue_LockCurrentState_Jammed;
    } else {
        currentState = kHAPCharacteristicValue_LockCurrentState_Unknown;
        switch (targetState) {
            case kHAPCharacteristicValue_LockTargetState_Secured: {
                SecureLock(&currentState);
            } break;
            case kHAPCharacteristicValue_LockTargetState_Unsecured: {
                UnsecureLock(&currentState);
            } break;
        }
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
    HAPLogInfo(&kHAPLog_Default, "%s", __func__);
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
    HAPLogInfo(&kHAPLog_Default, "%s", __func__);
    
    accessoryConfiguration.jammed = !accessoryConfiguration.jammed;
    if (accessoryConfiguration.jammed) {
        BlockLock();
    } else {
        UnblockLock();
    }
}

/**
 * Flag a factory reset, which is triggered in the run loop
 */
static void RestoreFactorySettings(void)
{
    HAPLogInfo(&kHAPLog_Default, "%s", __func__);
    
    accessoryConfiguration.restoreFactorySettingsRequested = true;
    HAPAccessoryServerStop(accessoryConfiguration.server);
}

//----------------------------------------------------------------------------------------------------------------------


typedef struct {
    uint8_t pin;
    uint8_t action;
} ButtonEvent;

/**
 * Global button handler.
 */
static void HandleButtonEventCallback(
    void *_Nullable context,
    size_t contextSize)
{
    HAPPrecondition(context);
    HAPAssert(contextSize == sizeof (ButtonEvent));
    
    HAPLogInfo(&kHAPLog_Default, "%s", __func__);
    
    ButtonEvent buttonEvent = *((ButtonEvent *) context);
    if (buttonEvent.action == APP_BUTTON_RELEASE) {
        switch (buttonEvent.pin) {
            case BUTTON_1: {
                ToggleLockState();
            } break;
            case BUTTON_2: {
                ToggleLockJammed();
            } break;
            case BUTTON_3: {
                RestoreFactorySettings();
            } break;
            default: {
            } break;
        }
    }
}

static void HandleButtonEvent(
    uint8_t pin_no,
    uint8_t button_action)
{
    HAPError err;
    ButtonEvent buttonEvent = {
        .pin = pin_no,
        .action = button_action
    };
    
    err = HAPPlatformRunLoopScheduleCallback(HandleButtonEventCallback, &buttonEvent, sizeof buttonEvent);
    if (err) {
        HAPFatalError();
    }
}

static void ConfigureIO(void)
{
    HAPLogInfo(&kHAPLog_Default, "%s", __func__);
    
    // Configure GPIO pins.
    accessoryConfiguration.device.lockPin = LED_1;
    accessoryConfiguration.device.jammedPin = LED_2;
    
    nrf_gpio_pin_set(accessoryConfiguration.device.lockPin); // LED off
    nrf_gpio_cfg_output(accessoryConfiguration.device.lockPin);
    
    nrf_gpio_pin_set(accessoryConfiguration.device.jammedPin); // LED off
    nrf_gpio_cfg_output(accessoryConfiguration.device.jammedPin);
    
    // Configure buttons
    static app_button_cfg_t buttonConfigs[] = {
        {
            .pin_no = BUTTON_1,
            .active_state = APP_BUTTON_ACTIVE_LOW,
            .pull_cfg = NRF_GPIO_PIN_PULLUP,
            .button_handler = HandleButtonEvent
        },
        {
            .pin_no = BUTTON_2,
            .active_state = APP_BUTTON_ACTIVE_LOW,
            .pull_cfg = NRF_GPIO_PIN_PULLUP,
            .button_handler = HandleButtonEvent
        },
        {
            .pin_no = BUTTON_3,
            .active_state = APP_BUTTON_ACTIVE_LOW,
            .pull_cfg = NRF_GPIO_PIN_PULLUP,
            .button_handler = HandleButtonEvent
        }
    };
    
    uint32_t e = app_button_init(buttonConfigs, HAPArrayCount(buttonConfigs), /* detection_delay: */ 1000);
    if (e) {
        HAPLogError(&kHAPLog_Default, "app_button_init failed: 0x%04x.", (unsigned int) e);
        HAPFatalError();
    }
    e = app_button_enable();
    if (e) {
        HAPLogError(&kHAPLog_Default, "app_button_enable failed: 0x%04x.", (unsigned int) e);
        HAPFatalError();
    }
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
    
    LoadAccessoryState();
 
    ConfigureIO();

    HAPCharacteristicValue_LockCurrentState currentState;
    switch (accessoryConfiguration.state.currentState) {
        case kHAPCharacteristicValue_LockCurrentState_Secured: {
            SecureLock(&currentState);
        } break;
        case kHAPCharacteristicValue_LockCurrentState_Unsecured: {
            UnsecureLock(&currentState);
        } break;
        default: {
            currentState = accessoryConfiguration.state.currentState;
        } break;
    }
    HAPAssert(currentState == accessoryConfiguration.state.currentState);
}

void AppRelease(void)
{
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
            }
            
            // Signal run loop to exit.
            HAPPlatformRunLoopStop();
        } return;
        case kHAPAccessoryServerState_Running: {
            HAPLogInfo(&kHAPLog_Default, "Accessory Server State did update: Running.");
        } return;
        case kHAPAccessoryServerState_Stopping: {
            HAPLogInfo(&kHAPLog_Default, "Accessory Server State did update: Stopping.");
            SaveAccessoryState();
        } return;
    }
    HAPFatalError();
}
