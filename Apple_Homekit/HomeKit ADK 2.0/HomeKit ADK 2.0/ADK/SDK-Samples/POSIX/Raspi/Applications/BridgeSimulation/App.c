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

// Basic bridge example: A bridge accessory that bridges two lightbulbs (see kAppState_NumLightbulbs).
//
// This implementation is intended for POSIX platforms.
//
// To enable user interaction, POSIX signals are used:
//
// - Signal `SIGUSR1` toggle whether the first lightbulb is reachable behind the bridge.
// - Signal `SIGUSR2` toggle whether the second lightbulb is reachable behind the bridge.
// - Signal `SIGTERM` will trigger a controlled shutdown.
//
// The code consists of multiple parts:
//
//   1. The definition of the accessory configuration, its internal state, and bridged accessories state.
//
//   2. Helper functions to load and save the state of the accessory.
//
//   3. The setup of the accessory HomeKit and the bridged accessories attribute database.
//
//   4. Functions to trigger the bridged accessories' reachability.
//
//   5. The callbacks that implement the actual behavior of the accessory, in this
//      case here they merely access the global accessory state variable and write
//      to the log to make the behavior easily observable.
//
//   6. The initialization of the accessory state.
//
//   7. A callback that gets called when the server state has changed.

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
 * HomeKit Accessory instance ID of bridge.
 */
#define kAppAID_Bridge                                                          ((size_t) 1)

/**
 * HomeKit Accessory instance ID of first bridged accessory.
 */
#define kAppAid_BridgedAccessories_Start                                        ((size_t) 2)

/**
 * Global accessory configuration.
 */
typedef struct {
    bool restoreFactorySettingsRequested;
    HAPAccessoryServerRef *server;
    HAPPlatformKeyValueStoreRef keyValueStore;
} AccessoryConfiguration;

static AccessoryConfiguration accessoryConfiguration;

/**
 * Global bridged accessory state.
 */
typedef struct {
    bool lightbulbOn;
} AccessoryState;

static AccessoryState accessoryState[kAppState_NumLightbulbs];

/**
 * Obtain the accessory state based on the HAP Accessory ID used.
 */
static AccessoryState *GetAccessoryState(
    const HAPAccessory *accessoryBridged)
{
    HAPPrecondition(accessoryBridged);
    HAPPrecondition(accessoryBridged->aid >= kAppAid_BridgedAccessories_Start);
    HAPPrecondition((size_t) (accessoryBridged->aid - kAppAid_BridgedAccessories_Start) < kAppState_NumLightbulbs);
    return &accessoryState[accessoryBridged->aid - kAppAid_BridgedAccessories_Start];
}


/**
 * Global bridged accessory reachability condition.
 */
typedef struct {
    bool reachable;
} ReachabilityCondition;

static ReachabilityCondition reachabilityCondition[kAppState_NumLightbulbs] = {
    { .reachable = true }, // Lightbulb 0
    { .reachable = true }  // Lightbulb 1
};

/**
 * Obtain the accessory reachability condition based on the HAP Accessory ID used.
 */
static ReachabilityCondition *GetAccessoryReachabilityCondition(
    const HAPAccessory *accessoryBridged)
{
    HAPPrecondition(accessoryBridged);
    HAPPrecondition(accessoryBridged->aid >= kAppAid_BridgedAccessories_Start);
    HAPPrecondition((size_t) (accessoryBridged->aid - kAppAid_BridgedAccessories_Start) < kAppState_NumLightbulbs);
    return &reachabilityCondition[accessoryBridged->aid - kAppAid_BridgedAccessories_Start];
}

//----------------------------------------------------------------------------------------------------------------------

/**
 * Load the accessory state from persistent memory.
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
        &accessoryState, sizeof accessoryState, &numBytes, &found);
    
    if (err) {
        HAPAssert(err == kHAPError_Unknown);
        HAPFatalError();
    }
    if (!found || numBytes != sizeof accessoryState) {
        if (found) {
            HAPLogError(&kHAPLog_Default,
                "Unexpected app state found in key-value store. Resetting to default.");
        }
        HAPRawBufferZero(&accessoryState, sizeof accessoryState);
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
        &accessoryState, sizeof accessoryState);
    if (err) {
        HAPAssert(err == kHAPError_Unknown);
        HAPFatalError();
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**
 * HomeKit accessory that provides the Bridge service.
 */
static const HAPAccessory accessory = {
    .aid = kAppAID_Bridge,
    .category = kHAPAccessoryCategory_Bridges,
    .name = "Acme Bridge Simulation",
    .manufacturer = "Acme",
    .model = "Bridge1,1",
    .serialNumber = "099DB48E9E28",
    .firmwareVersion = "1",
    .hardwareVersion = "1",
    .services = (const HAPService *const[]) {
        &accessoryInformationService,
        &hapProtocolInformationService,
        &pairingService,
        NULL
    },
    .cameraStreamConfigurations = NULL,
    .callbacks = {
        .identify = IdentifyAccessory
    }
};

/**
* Array of bridged accessories exposed by the bridge accessory. NULL-terminated.
*/
static const HAPAccessory *_Nonnull const *_Nullable bridgedAccessories = (const HAPAccessory *const[]) {
    &(const HAPAccessory) {
        .aid = kAppAid_BridgedAccessories_Start,
        .category = kHAPAccessoryCategory_BridgedAccessory,
        .name = "Acme Lightbulb",
        .manufacturer = "Acme",
        .model = "lightbulb1,1",
        .serialNumber = "099DB48E9E29A",
        .firmwareVersion = "1",
        .hardwareVersion = "1",
        .services = (const HAPService *const[]) {
            &accessoryInformationService,
            &pairingService,
            &lightbulbService,
            NULL
        },
        .cameraStreamConfigurations = NULL,
        .callbacks = {
            .identify = IdentifyAccessory
        }
    },
    &(const HAPAccessory) {
        .aid = kAppAid_BridgedAccessories_Start + 1,
        .category = kHAPAccessoryCategory_BridgedAccessory,
        .name = "Acme Lightbulb",
        .manufacturer = "Acme",
        .model = "lightbulb1,1",
        .serialNumber = "099DB48E9E29B",
        .firmwareVersion = "1",
        .hardwareVersion = "1",
        .services = (const HAPService *const[]) {
            &accessoryInformationService,
            &pairingService,
            &lightbulbService,
            NULL
        },
        .cameraStreamConfigurations = NULL,
        .callbacks = {
            .identify = IdentifyAccessory
        }
    },
    NULL
};

//----------------------------------------------------------------------------------------------------------------------

/**
 * Toggle if the lightbulb is reachable behind the bridge.
 */
static void ToggleLightbulbReachable(
    const HAPAccessory *accessoryBridged)
{
    HAPPrecondition(accessoryBridged);

    ReachabilityCondition* condition = GetAccessoryReachabilityCondition(accessoryBridged);
    condition->reachable = !condition->reachable;

    HAPLogInfo(&kHAPLog_Default, "%s lightbulb %i: %s",
        __func__,
        (int) (accessoryBridged->aid - kAppAid_BridgedAccessories_Start + 1), // First bridged lightbulb is 1
        condition->reachable ? "true" : "false"
    );
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
    
    if (signum == SIGUSR1) {
        ToggleLightbulbReachable(bridgedAccessories[0]);
    } else if (signum == SIGUSR2) {
        ToggleLightbulbReachable(bridgedAccessories[1]);
    } else if (signum == SIGTERM) {
        HAPAccessoryServerStop(accessoryConfiguration.server);
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
static void InitializeSignalHandlers(void)
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

HAP_RESULT_USE_CHECK
HAPError HandleLightbulbOnRead(
    HAPAccessoryServerRef *server HAP_UNUSED,
    const HAPBoolCharacteristicReadRequest *request,
    bool *value,
    void *_Nullable context HAP_UNUSED)
{
    HAPError error;
    uint64_t lightbulbID = request->accessory->aid - 1;
    AccessoryState* state = GetAccessoryState(request->accessory);
    if (GetAccessoryReachabilityCondition(request->accessory)->reachable) {
        *value = state->lightbulbOn;
        HAPLogInfo(&kHAPLog_Default, "%s: [Lightbulb %lu] %s", __func__,
            (unsigned long) lightbulbID, *value ? "true" : "false");
        error = kHAPError_None;
    } else {
        HAPLogInfo(&kHAPLog_Default, "%s: [Lightbulb %lu] %s", __func__,
            (unsigned long) lightbulbID, "not reachable");
        error = kHAPError_InvalidState;
    }
    return error;
}

HAP_RESULT_USE_CHECK
HAPError HandleLightbulbOnWrite(
    HAPAccessoryServerRef *server,
    const HAPBoolCharacteristicWriteRequest *request,
    bool value,
    void *_Nullable context HAP_UNUSED)
{
    HAPError error;
    uint64_t lightbulbID = request->accessory->aid - 1;
    HAPLogInfo(&kHAPLog_Default, "%s: [Lightbulb %lu] %s", __func__,
        (unsigned long) lightbulbID, value ? "true" : "false");
    AccessoryState* state = GetAccessoryState(request->accessory);
    if (GetAccessoryReachabilityCondition(request->accessory)->reachable) {
        if (state->lightbulbOn != value) {
            state->lightbulbOn = value;
            HAPAccessoryServerRaiseEvent(server,
                request->characteristic, request->service, request->accessory);
            SaveAccessoryState();
        }
        error = kHAPError_None;
    } else {
        HAPLogInfo(&kHAPLog_Default, "%s: [Lightbulb %lu] %s", __func__,
            (unsigned long) lightbulbID, "not reachable");
        error = kHAPError_InvalidState;
    }
    return error;
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
    
    InitializeSignalHandlers();
    
    LoadAccessoryState();
}

void AppRelease(void)
{
}

void AppAccessoryServerStart(void)
{
    HAPAccessoryServerStartBridge(
        accessoryConfiguration.server,
        &accessory,
        bridgedAccessories,
        /* configurationChanged: */ false);
    
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
