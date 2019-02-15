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

// Basic bridge example: A bridge accessory that bridges two lightbulbs (see kAppState_NumLightbulbs). It can serve
// as a basic implementation for any platform. The accessory logic implementation is reduced to internal state
// updates and log output.
//
// This implementation is platform-independent.
//
// The code consists of multiple parts:
//
//   1. The definition of the accessory configuration, its internal, and bridged accessories state.
//
//   2. Helper functions to load and save the state of the accessory.
//
//   3. The definitions for the HomeKit attribute database.
//
//   4. The callbacks that implement the actual behavior of the accessory, in this
//      case here they merely access the global accessory state variable and write
//      to the log to make the behavior easily observable.
//
//   5. The initialization of the accessory state.
//
//   6. Callbacks that notify the server in case their associated value has changed.

#include "HAP.h"

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
    .name = "Acme Bridge",
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
    uint64_t lightbulbID = request->accessory->aid - 1;
    AccessoryState* state = GetAccessoryState(request->accessory);
    *value = state->lightbulbOn;
    HAPLogInfo(&kHAPLog_Default, "%s: [Lightbulb %lu] %s", __func__,
        (unsigned long) lightbulbID, *value ? "true" : "false");
    return kHAPError_None;
}

HAP_RESULT_USE_CHECK
HAPError HandleLightbulbOnWrite(
    HAPAccessoryServerRef *server,
    const HAPBoolCharacteristicWriteRequest *request,
    bool value,
    void *_Nullable context HAP_UNUSED)
{
    uint64_t lightbulbID = request->accessory->aid - 1;
    HAPLogInfo(&kHAPLog_Default, "%s: [Lightbulb %lu] %s", __func__,
        (unsigned long) lightbulbID, value ? "true" : "false");
    AccessoryState* state = GetAccessoryState(request->accessory);
    if (state->lightbulbOn != value) {
        state->lightbulbOn = value;
        HAPAccessoryServerRaiseEvent(server,
            request->characteristic, request->service, request->accessory);
        SaveAccessoryState();
    }
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
}

//----------------------------------------------------------------------------------------------------------------------

void AccessoryServerHandleUpdatedState(
    HAPAccessoryServerRef *server,
    void *_Nullable context)
{
    HAPPrecondition(server);
    HAPPrecondition(!context);

    switch (HAPAccessoryServerGetState(server)) {
        case kHAPAccessoryServerState_Idle: {
            HAPLogInfo(&kHAPLog_Default, "Accessory Server State did update: Idle.");
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
