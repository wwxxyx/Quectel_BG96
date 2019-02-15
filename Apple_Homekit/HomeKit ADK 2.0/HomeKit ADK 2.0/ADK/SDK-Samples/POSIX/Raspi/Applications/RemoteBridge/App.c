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

// This Linux remote example is configured to advertise a set of example buttons to demonstrate a remote that can
// control Apple TV devices.
//
// The keyboard may be used to simulate button presses and releases and to cycle through the controllable targets.
// Updates to the remote state are logged as part of the regular diagnostic log for development purposes.
// The remote may be factory reset by sending the SIGUSR2 signal to the process.
//
// /!\ It is strongly recommended not to modify Remote.c or Siri.c, to simplify the integration of revised
// versions of these files.
//
// App.c (this file):
// - The list of supported buttons needs to be updated to match the buttons available by the hardware.
// - The keyboard handling code needs to be replaced with actual button input handling code.
// - The remote UI needs to be updated when the remote configuration changes.
// - The factory reset functionality needs to be adjusted.
//
// HAPPlatformAudio.c
// - The microphone code may need adjustment. The reference example relies on the Linux ALSA component and GStreamer.

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <sys/stat.h>
#include <termios.h>

#include "HAP.h"
#include "HAPPlatform+Init.h"
#include "HAPPlatformFileHandle.h"
#include "HAPPlatformKeyValueStore+Init.h"
#include "HAPPlatformMicrophone+Init.h"

#include "App.h"
#include "DB.h"
#include "Remote.h"
#include "Siri.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const HAPAccessory accessory;
static const HAPAccessory *_Nonnull const *_Nullable bridgedAccessories;
static RemoteRef remotes[2];
static HAPPlatformKeyValueStore remoteKeyValueStores[HAPArrayCount(remotes)];
static HAPPlatformMicrophone remoteMicrophone;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * HomeKit Accessory instance ID of bridge.
 */
#define kAppAID_Bridge                                                          ((size_t) 1)

/**
 * HomeKit Accessory instance ID of first bridged accessory.
 */
#define kAppAid_BridgedAccessories_Start                                        ((size_t) 2)

extern const HAPAccessory accessory;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * IDs of supported buttons.
 */
HAP_ENUM_BEGIN(uint8_t, ButtonID) {
    kButtonID_01_Menu = 1,
    kButtonID_02_PlayPause,
    kButtonID_03_TVHome,
    kButtonID_04_Select,
    kButtonID_05_ArrowUp,
    kButtonID_06_ArrowRight,
    kButtonID_07_ArrowDown,
    kButtonID_08_ArrowLeft,
    kButtonID_09_VolumeUp,
    kButtonID_10_VolumeDown,
    kButtonID_11_Siri,
    kButtonID_12_Power,
    kButtonID_13_Generic
} HAP_ENUM_END(uint8_t, ButtonID);

/**
 * List of supported buttons that are physically available on remote 1.
 *
 * - When a button is pressed or released the function RemoteRaiseButtonEvent must be called
 *   to process the button event.
 *
 * - Note that each target that is controlled by the remote may reconfigure the buttons
 *   to have non-default names or different types specific to that target.
 *
 * - Keys 14 to 42 are not mapped.
 */
static const RemoteSupportedButton supportedButtonsRemote0[] = {
    {kButtonID_01_Menu,       kRemoteButtonType_Menu},
    {kButtonID_02_PlayPause,  kRemoteButtonType_PlayPause},
    {kButtonID_03_TVHome,     kRemoteButtonType_TVHome},
    {kButtonID_04_Select,     kRemoteButtonType_Select},
    {kButtonID_05_ArrowUp,    kRemoteButtonType_ArrowUp},
    {kButtonID_06_ArrowRight, kRemoteButtonType_ArrowRight},
    {kButtonID_07_ArrowDown,  kRemoteButtonType_ArrowDown},
    {kButtonID_08_ArrowLeft,  kRemoteButtonType_ArrowLeft},
    {kButtonID_09_VolumeUp,   kRemoteButtonType_VolumeUp},
    {kButtonID_10_VolumeDown, kRemoteButtonType_VolumeDown},
    {kButtonID_11_Siri,       kRemoteButtonType_Siri},
    {kButtonID_12_Power,      kRemoteButtonType_Power},
    {kButtonID_13_Generic,    kRemoteButtonType_Generic}
};
HAP_STATIC_ASSERT(HAPArrayCount(supportedButtonsRemote0) <= kRemote_MaxButtons, supportedButtonsRemote0);

/**
 * List of supported buttons that are physically available on the remote 2.
 *
 * - When a button is pressed or released the function RemoteRaiseButtonEvent must be called
 *   to process the button event.
 *
 * - Note that each target that is controlled by the remote may reconfigure the buttons
 *   to have non-default names or different types specific to that target.
 *
 * - Keys 14 to 42 are not mapped.
 */
static const RemoteSupportedButton supportedButtonsRemote1[] = {
    {kButtonID_01_Menu,       kRemoteButtonType_Menu},
    {kButtonID_02_PlayPause,  kRemoteButtonType_PlayPause},
    {kButtonID_03_TVHome,     kRemoteButtonType_TVHome},
    {kButtonID_04_Select,     kRemoteButtonType_Select},
    {kButtonID_05_ArrowUp,    kRemoteButtonType_ArrowUp},
    {kButtonID_06_ArrowRight, kRemoteButtonType_ArrowRight},
    {kButtonID_07_ArrowDown,  kRemoteButtonType_ArrowDown},
    {kButtonID_08_ArrowLeft,  kRemoteButtonType_ArrowLeft},
    {kButtonID_09_VolumeUp,   kRemoteButtonType_VolumeUp},
    {kButtonID_10_VolumeDown, kRemoteButtonType_VolumeDown},
    {kButtonID_11_Siri, kRemoteButtonType_Siri},
    {kButtonID_12_Power,      kRemoteButtonType_Power},
    {kButtonID_13_Generic,    kRemoteButtonType_Generic}
};
HAP_STATIC_ASSERT(HAPArrayCount(supportedButtonsRemote1) <= kRemote_MaxButtons, supportedButtonsRemote1);

/**
 * Gets a description for a given button type.
 *
 * - The descriptions may be used in the remote UI if no target-specific name has been configured.
 *
 * @param      value                Button type.
 * @return                          Description of the button type.
 */
HAP_RESULT_USE_CHECK
static const char *GetButtonTypeDescription(
    RemoteButtonType value)
{
    switch (value) {
        case kRemoteButtonType_Menu:
            return "Menu";
        case kRemoteButtonType_PlayPause:
            return "Play/Pause";
        case kRemoteButtonType_TVHome:
            return "TV/Home";
        case kRemoteButtonType_Select:
            return "Select";
        case kRemoteButtonType_ArrowUp:
            return "Arrow Up";
        case kRemoteButtonType_ArrowRight:
            return "Arrow Right";
        case kRemoteButtonType_ArrowDown:
            return "Arrow Down";
        case kRemoteButtonType_ArrowLeft:
            return "Arrow Left";
        case kRemoteButtonType_VolumeUp:
            return "Volume Up";
        case kRemoteButtonType_VolumeDown:
            return "Volume Down";
        case kRemoteButtonType_Siri:
            return "Siri";
        case kRemoteButtonType_Power:
            return "Power";
        case kRemoteButtonType_Generic:
            return "Generic";
    }
    
    HAPLogError(&kHAPLog_Default, "Invalid button type %u.", value);
    HAPFatalError();
}

/**
 * Prints the list of supported buttons to the diagnostic log.
 *
 * @param      buttons              Supported buttons.
 * @param      numButtons           Number of supported buttons.
 */
static void PrintSupportedButtons(
    const RemoteSupportedButton *buttons,
    size_t numButtons)
{
    HAPLogInfo(&kHAPLog_Default, "Supported Buttons");
    for (size_t i = 0; i < numButtons; i++) {
        HAPLogInfo(&kHAPLog_Default, "  Button %d", buttons[i].buttonID);
        HAPLogInfo(&kHAPLog_Default, "   - Type: %s", GetButtonTypeDescription(buttons[i].buttonType));
    }
}

/**
 * Global accessory configuration.
 */
/**@{*/
typedef struct {
    HAPTime startTime;
    HAPAccessoryServerRef *server;
    HAPPlatformKeyValueStoreRef keyValueStore;
    HAPPlatformFileHandleRef stdinFileHandle;
    int stdinFileDescriptor;
    bool restoreFactorySettingsRequested;
} AccessoryConfiguration;

static AccessoryConfiguration accessoryConfiguration;
/**@}*/

/**
 * Bridge Accessory state.
 */
/**@{*/
typedef struct {
    size_t id;
    bool reachable;
} BridgedAccessoryState;
static BridgedAccessoryState accessoryState[kAppState_NumRemotes];

HAP_RESULT_USE_CHECK
static BridgedAccessoryState *GetAccessoryState(
    const HAPAccessory *accessoryBridged)
{
    HAPPrecondition(accessoryBridged);
    HAPPrecondition(accessoryBridged->aid >= kAppAid_BridgedAccessories_Start);
    HAPPrecondition((size_t) (accessoryBridged->aid - kAppAid_BridgedAccessories_Start) < kAppState_NumRemotes);
    return &accessoryState[accessoryBridged->aid - kAppAid_BridgedAccessories_Start];
}
/**@}*/

//----------------------------------------------------------------------------------------------------------------------

static void PrintTargetButtonsCallback(
    void *_Nullable context HAP_UNUSED,
    RemoteRef *remote HAP_UNUSED,
    const RemoteTargetConfiguration *targetConfiguration HAP_UNUSED,
    const RemoteButtonConfiguration *buttonConfiguration,
    bool *shouldContinue)
{
    *shouldContinue = true;
    
    HAPLogInfo(&kHAPLog_Default, "  Button %u", buttonConfiguration->buttonID);
    if (buttonConfiguration->buttonType.isDefined) {
        HAPLogInfo(&kHAPLog_Default, "   - Type: %s", GetButtonTypeDescription(buttonConfiguration->buttonType.value));
    }
    if (buttonConfiguration->buttonName.isDefined) {
        HAPLogInfo(&kHAPLog_Default, "   - Name: %s", buttonConfiguration->buttonName.bytes);
    }
}

static void PrintTargetsCallback(
    void *_Nullable context HAP_UNUSED,
    RemoteRef *remote,
    const RemoteTargetConfiguration *targetConfiguration,
    bool *shouldContinue)
{
    HAPPrecondition(remote);
    HAPPrecondition(targetConfiguration);
    
    *shouldContinue = true;
    
    HAPLogInfo(&kHAPLog_Default, "Target: %lu", (unsigned long) targetConfiguration->targetIdentifier);
    if (targetConfiguration->targetCategory) {
        HAPLogInfo(&kHAPLog_Default, "  Category: %u", targetConfiguration->targetCategory);
    }
    if (targetConfiguration->targetName.isDefined) {
        HAPLogInfo(&kHAPLog_Default, "  Name:     %s", targetConfiguration->targetName.bytes);
    }
    
    RemoteEnumerateButtons(
        remote,
        targetConfiguration->targetIdentifier,
        PrintTargetButtonsCallback,
        /* context: */ NULL);
}

static void PrintTargets(
    RemoteRef *remote)
{
    HAPPrecondition(remote);
    
    HAPLogInfo(&kHAPLog_Default, "Targets configured for remote %p", (const void *) remote);
    RemoteEnumerateTargets(remote, PrintTargetsCallback, /* context: */ NULL);
}

//----------------------------------------------------------------------------------------------------------------------

/**
 * Identify routine. Used to locate the accessory.
 */
HAP_RESULT_USE_CHECK
HAPError IdentifyAccessory(
    HAPAccessoryServerRef *server HAP_UNUSED,
    const HAPAccessoryIdentifyRequest *request,
    void *_Nullable context HAP_UNUSED)
{
    if (request->accessory->aid == 1) {
        HAPLogInfo(&kHAPLog_Default, "%s: Identifying Bridge", __func__);
    }
    else {
        BridgedAccessoryState *state = GetAccessoryState(request->accessory);
        HAPAssert(state);
        HAPLogInfo(&kHAPLog_Default, "%s: Remote [%zu]", __func__, state->id);
    }
    return kHAPError_None;
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
    HAPAssert(contextSize == sizeof(int));

    int signum = *((int *) context);

    switch (signum) {
        case SIGUSR1: {
            RemoteSwitchToNextTarget(&remotes[0], /* context: */ NULL);
        } break;
        case SIGUSR2: {
            accessoryConfiguration.restoreFactorySettingsRequested = true;
            HAPAccessoryServerStop(accessoryConfiguration.server);
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

//----------------------------------------------------------------------------------------------------------------------

static void PrintButtonConfiguration(void)
{
    HAPLogInfo(&kHAPLog_Default, "Button Configuration");
    HAPLogInfo(&kHAPLog_Default, "Action                      Down Up");
    HAPLogInfo(&kHAPLog_Default, "===================================");
    HAPLogInfo(&kHAPLog_Default, "Toggle Identifier:          m");
    HAPLogInfo(&kHAPLog_Default, "Set target to non/homekit:  0");
    HAPLogInfo(&kHAPLog_Default, "Siri:                       1    2");
    HAPLogInfo(&kHAPLog_Default, "Menu:                       q    w");
    HAPLogInfo(&kHAPLog_Default, "Play / Pause:               e    r");
    HAPLogInfo(&kHAPLog_Default, "TV / Home:                  t    y");
    HAPLogInfo(&kHAPLog_Default, "Select:                     u    i");
    HAPLogInfo(&kHAPLog_Default, "Arrow Up:                   o    p");
    HAPLogInfo(&kHAPLog_Default, "Arrow Right:                a    s");
    HAPLogInfo(&kHAPLog_Default, "Arrow Down:                 d    f");
    HAPLogInfo(&kHAPLog_Default, "Arrow Left:                 g    h");
    HAPLogInfo(&kHAPLog_Default, "Volume Up:                  j    k");
    HAPLogInfo(&kHAPLog_Default, "Volume Down:                l    ;");
    HAPLogInfo(&kHAPLog_Default, "Power:                      c    v");
    HAPLogInfo(&kHAPLog_Default, "Generic:                    b    n");
}

//----------------------------------------------------------------------------------------------------------------------

static void FileHandleCallback(
    HAPPlatformFileHandleRef fileHandle,
    HAPPlatformFileHandleEvent fileHandleEvents,
    void *_Nullable context)
{
    HAPAssert(fileHandle);
    HAPAssert(fileHandleEvents.isReadyForReading);
    HAPAssert(context);
    
    int *fileDescriptor = (int*) context;
    
    for (;;) {
        char singleChar;
        ssize_t n;
        do {
            n = read(*fileDescriptor, &singleChar, sizeof singleChar);
        } while (n == -1 && errno == EINTR);
        if (n == 0) {
            return;
        }
        if (n == -1 && errno == EAGAIN) {
            return;
        }
    
        size_t remoteIndex = 0;
        // Handle mapping for second remote
        
        // Map upper case to lower case
        if (singleChar >= 'A' && singleChar < 'a') {
            remoteIndex = 1;
            singleChar += ('a' - 'A');
        }
        
        // Special cases
        if (singleChar == '!') {
            remoteIndex = 1;
            singleChar = '1';
        }
        if (singleChar == '@') {
            remoteIndex = 1;
            singleChar = '2';
        }
        if (singleChar == ':') {
            remoteIndex = 1;
            singleChar = ';';
        }
        if (singleChar == ')') {
            remoteIndex = 1;
            singleChar = '0';
        }
        
        RemoteRef *targetRemote = &remotes[remoteIndex];
        HAPLogDebug(&kHAPLog_Default, "%s: Read action %c for remote #%u (%p)",
            __func__, singleChar, (uint32_t) (remoteIndex + 1), (const void *) targetRemote);
        
        HAPTime ts = HAPPlatformClockGetCurrent();
    
        switch (singleChar) {
            // Toggle Identifier
            case 'm': {
                RemoteSwitchToNextTarget(targetRemote, /* context: */ NULL);
            } break;
        
            // Set the identifier to non-HomeKit (0).
            case '0': {
                RemoteSetActiveTargetIdentifier(targetRemote, kRemoteTargetIdentifier_NonHomeKit, /* context: */ NULL);
            } break;

            // Button: AudioInput
            case '1': {
                RemoteRaiseButtonEvent(targetRemote,
                    kButtonID_11_Siri, kRemoteButtonState_Down, ts);
            } break;
            
            case '2': {
                RemoteRaiseButtonEvent(targetRemote,
                    kButtonID_11_Siri, kRemoteButtonState_Up, ts);
            } break;
            
            // Button: Menu
            case 'q': {
                RemoteRaiseButtonEvent(targetRemote,
                    kButtonID_01_Menu, kRemoteButtonState_Down, ts);
            } break;
            
            case 'w': {
                RemoteRaiseButtonEvent(targetRemote,
                    kButtonID_01_Menu, kRemoteButtonState_Up, ts);
            } break;
            
            // Button: Play/Pause
            case 'e': {
                RemoteRaiseButtonEvent(targetRemote,
                    kButtonID_02_PlayPause, kRemoteButtonState_Down, ts);
            } break;
            
            case 'r': {
                RemoteRaiseButtonEvent(targetRemote,
                    kButtonID_02_PlayPause, kRemoteButtonState_Up, ts);
            } break;
            
            // Button: TV/Home
            case 't': {
                RemoteRaiseButtonEvent(targetRemote,
                    kButtonID_03_TVHome, kRemoteButtonState_Down, ts);
            } break;
            
            case 'y': {
                RemoteRaiseButtonEvent(targetRemote,
                    kButtonID_03_TVHome, kRemoteButtonState_Up, ts);
            } break;
            
            // Button: Select
            case 'u': {
                RemoteRaiseButtonEvent(targetRemote,
                    kButtonID_04_Select, kRemoteButtonState_Down, ts);
            } break;
            
            case 'i': {
                RemoteRaiseButtonEvent(targetRemote,
                    kButtonID_04_Select, kRemoteButtonState_Up, ts);
            } break;
            
            // Button: ArrowUp
            case 'o': {
                RemoteRaiseButtonEvent(targetRemote,
                    kButtonID_05_ArrowUp, kRemoteButtonState_Down, ts);
            } break;
            
            case 'p': {
                RemoteRaiseButtonEvent(targetRemote,
                    kButtonID_05_ArrowUp, kRemoteButtonState_Up, ts);
            } break;
            
            // Button: ArrowRight
            case 'a': {
                RemoteRaiseButtonEvent(targetRemote,
                    kButtonID_06_ArrowRight, kRemoteButtonState_Down, ts);
            } break;
            
            case 's': {
                RemoteRaiseButtonEvent(targetRemote,
                    kButtonID_06_ArrowRight, kRemoteButtonState_Up, ts);
            } break;
            
            // Button: ArrowDown
            case 'd': {
                RemoteRaiseButtonEvent(targetRemote,
                    kButtonID_07_ArrowDown, kRemoteButtonState_Down, ts);
            } break;
            
            case 'f': {
                RemoteRaiseButtonEvent(targetRemote,
                    kButtonID_07_ArrowDown, kRemoteButtonState_Up, ts);
            } break;
            
            // Button: ArrowLeft
            case 'g': {
                RemoteRaiseButtonEvent(targetRemote,
                    kButtonID_08_ArrowLeft, kRemoteButtonState_Down, ts);
            } break;
            
            case 'h': {
                RemoteRaiseButtonEvent(targetRemote,
                    kButtonID_08_ArrowLeft, kRemoteButtonState_Up, ts);
            } break;
            
            // Button: VolumeUp
            case 'j': {
                RemoteRaiseButtonEvent(targetRemote,
                    kButtonID_09_VolumeUp, kRemoteButtonState_Down, ts);
            } break;
            
            case 'k': {
                RemoteRaiseButtonEvent(targetRemote,
                    kButtonID_09_VolumeUp, kRemoteButtonState_Up, ts);
            } break;
            
            // Button: VolumeDown
            case 'l': {
                RemoteRaiseButtonEvent(targetRemote,
                    kButtonID_10_VolumeDown, kRemoteButtonState_Down, ts);
            } break;
            
            case ';': {
                RemoteRaiseButtonEvent(targetRemote,
                    kButtonID_10_VolumeDown, kRemoteButtonState_Up, ts);
            } break;
            
            // Button: Power
            case 'c': {
                RemoteRaiseButtonEvent(targetRemote,
                    kButtonID_12_Power, kRemoteButtonState_Down, ts);
            } break;
            
            case 'v': {
                RemoteRaiseButtonEvent(targetRemote,
                    kButtonID_12_Power, kRemoteButtonState_Up, ts);
            } break;
            
            // Button: Generic
            case 'b': {
                RemoteRaiseButtonEvent(targetRemote,
                    kButtonID_13_Generic, kRemoteButtonState_Down, ts);
            } break;
            
            case 'n': {
                RemoteRaiseButtonEvent(targetRemote,
                    kButtonID_13_Generic, kRemoteButtonState_Up, ts);
            } break;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

static HAPTargetControlDataStreamProtocol targetControlDataStreamProtocol = {
    .base = &kHAPTargetControlDataStreamProtocol_Base,
    .callbacks = {
        .handleIdentifierUpdate = RemoteHandleTargetControlIdentifierUpdate
    }
};

static HAPDataSendDataStreamProtocolContextRef dataSendDataStreamProtocolContexts[kApp_NumDataStreams];
static HAPDataSendDataStreamProtocol dataSendDataStreamProtocol = {
    .base = &kHAPDataSendDataStreamProtocol_Base,
    .storage = {
        .numDataStreams = kApp_NumDataStreams,
        .protocolContexts = dataSendDataStreamProtocolContexts
    },
    .callbacks = {
        .handleAccept = SiriHandleDataSendAccept,
        .handleInvalidate = SiriHandleDataSendInvalidate
    }
};

static HAPDataStreamDescriptorRef dataStreamDescriptors[kApp_NumDataStreams];
static HAPDataStreamDispatcherStorage dataStreamDispatcherStorage = {
    .numDataStreams = kApp_NumDataStreams,
    .dataStreamDescriptors = dataStreamDescriptors,
    .dataStreamProtocols = (HAPDataStreamProtocol *const[]) {
        &targetControlDataStreamProtocol,
        &dataSendDataStreamProtocol,
        NULL
    }
};
static HAPDataStreamDispatcherRef dataStreamDispatcher;

//----------------------------------------------------------------------------------------------------------------------

/**
 * HomeKit accessory that provides the Bridge service.
 */
static const HAPAccessory accessory = {
    .aid = kAppAID_Bridge,
    .category = kHAPAccessoryCategory_Bridges,
    .name = "Acme Remote Bridge",
    .manufacturer = "Acme",
    .model = "Remote1,1",
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
        .name = "1 - Remote",
        .manufacturer = "Acme",
        .model = "Remote1,1",
        .serialNumber = "099DB48E9E28",
        .firmwareVersion = "1",
        .hardwareVersion = "1",
        .services = (const HAPService *const[]) {
            &accessoryInformationService,
            &pairingService,
            &targetControlManagementService,
            &targetControlService,
            &dataStreamTransportManagementService,
            &audioStreamManagementService,
            &siriService,
            NULL
        },
        .dataStream.delegate = {
            .callbacks = &kHAPDataStreamDispatcher_DataStreamCallbacks,
            .context = &dataStreamDispatcher
        },
        .cameraStreamConfigurations = NULL,
        .callbacks = {
            .identify = IdentifyAccessory
        }
    },
    &(const HAPAccessory) {
        .aid = kAppAid_BridgedAccessories_Start + 1,
        .category = kHAPAccessoryCategory_BridgedAccessory,
        .name = "2 - Remote",
        .manufacturer = "Acme",
        .model = "Remote1,1",
        .serialNumber = "099DB48E9E28",
        .firmwareVersion = "1",
        .hardwareVersion = "1",
        .services = (const HAPService *const[]) {
            &accessoryInformationService,
            &pairingService,
            &targetControlManagementService,
            &targetControlService,
            &dataStreamTransportManagementService,
            &audioStreamManagementService,
            &siriService,
            NULL
        },
        .dataStream.delegate = {
            .callbacks = &kHAPDataStreamDispatcher_DataStreamCallbacks,
            .context = &dataStreamDispatcher
        },
        .cameraStreamConfigurations = NULL,
        .callbacks = {
            .identify = IdentifyAccessory
        }
    },
    NULL
};

//----------------------------------------------------------------------------------------------------------------------

static void HandleActiveChange(
    RemoteRef *remote,
    void *_Nullable context HAP_UNUSED)
{
    if (RemoteIsActive(remote)) {
        RemoteTargetIdentifier targetIdentifier = RemoteGetActiveTargetIdentifier(remote);
        RemoteTargetConfiguration targetConfiguration;
        bool found;
        RemoteGetTargetConfiguration(remote, targetIdentifier, &targetConfiguration, &found);
        
        HAPLog(&kHAPLog_Default, "Remote %p active. Target \"%s\" (%lu), category %u",
            (const void *) remote,
            targetConfiguration.targetName.isDefined? targetConfiguration.targetName.bytes : "Unknown",
            (unsigned long) targetConfiguration.targetIdentifier,
            targetConfiguration.targetCategory);
    } else {
        HAPLog(&kHAPLog_Default, "%s: Remote %p inactive.", __func__, (const void *) remote);
    }
}

static void HandleConfigurationChange(
    RemoteRef *remote,
    void *_Nullable context HAP_UNUSED)
{
    HAPLog(&kHAPLog_Default, "Remote %p configured targets changed", (const void *) remote);
    PrintTargets(remote);
}

//----------------------------------------------------------------------------------------------------------------------

typedef struct {
    struct termios values;
    bool isValid;
} TerminalAttributes;
static TerminalAttributes terminalAttributes;

static void StdinPipeCreate(void)
{
    HAPError err;
    
    HAPLog(&kHAPLog_Default, "%s", __func__);
    
    int e;
    
    // Setup terminal
    struct termios t;
    do {
        e = tcgetattr(STDIN_FILENO, &t);
        terminalAttributes.values = t;
        terminalAttributes.isValid = true;
    } while (e == -1 && errno == EINTR);
    if (e) {
        int _errno = errno;
        HAPAssert(e == -1);
        HAPLogError(&kHAPLog_Default, "%s: tcgetattr failed: %d", __func__, _errno);
        terminalAttributes.isValid = false;
    }
    e = cfsetispeed(&t, B115200);
    if (e) {
        int _errno = errno;
        HAPAssert(e == -1);
        HAPLogError(&kHAPLog_Default, "%s: cfsetispeed failed: %d", __func__, _errno);
    }
    e = cfsetospeed(&t, B115200);
    if (e) {
        int _errno = errno;
        HAPAssert(e == -1);
        HAPLogError(&kHAPLog_Default, "%s: cfsetospeed failed: %d", __func__, _errno);
    }
    
    // Make the terminal raw.
    t.c_iflag &= (tcflag_t) ~(BRKINT | INPCK | ISTRIP );
    t.c_lflag &= (tcflag_t) ~(ECHO | ICANON);
    t.c_cflag |= (CS8);
    
    t.c_cc[VMIN]  = 1;
    t.c_cc[VTIME] = 0;
    
    struct termios t_expected = t;
    do {
        e = tcsetattr(STDIN_FILENO, TCSAFLUSH, &t);
    } while (e == -1 && errno == EINTR);
    if (e) {
        int _errno = errno;
        HAPAssert(e == -1);
        HAPLogError(&kHAPLog_Default, "%s: tcsetattr failed: %d", __func__, _errno);
    }
    
    do {
        e = tcgetattr(STDIN_FILENO, &t);
    } while (e == -1 && errno == EINTR);
    if (e) {
        int _errno = errno;
        HAPAssert(e == -1);
        HAPLogError(&kHAPLog_Default, "%s: tcgetattr failed: %d", __func__, _errno);
    }

    if (!HAPRawBufferAreEqual(&t, &t_expected, sizeof(struct termios))) {
        HAPAssert(e == -1);
        HAPLogError(&kHAPLog_Default, "%s: tcsetattr did not apply all requested changes.", __func__);
    }
    
    e = fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
    if (e == -1) {
        HAPLogError(&kHAPLog_Default,
            "%s: System call 'fcntl' to set STDIN file descriptor 0 flags to 'non-blocking' failed.", __func__);
        HAPFatalError();
    }
    
    accessoryConfiguration.stdinFileDescriptor = STDIN_FILENO;
    
    err = HAPPlatformFileHandleRegister(
        &accessoryConfiguration.stdinFileHandle,
        accessoryConfiguration.stdinFileDescriptor,
        (HAPPlatformFileHandleEvent) {
            .isReadyForReading = true,
            .isReadyForWriting = false,
            .hasErrorConditionPending = false
        },
        FileHandleCallback,
        &accessoryConfiguration.stdinFileDescriptor);
    if (err) {
        HAPLogError(&kHAPLog_Default, "%s: Unable to register Filehandle.", __func__);
        HAPFatalError();
    }
}

static void StdinPipeRelease(void)
{
    HAPLog(&kHAPLog_Default, "%s", __func__);
    
    if (terminalAttributes.isValid) {
        int e;
        do {
            e = tcsetattr(STDIN_FILENO, TCSANOW, &terminalAttributes.values);
        } while (e == -1 && errno == EINTR);
        if (e) {
            int _errno = errno;
            HAPAssert(e == -1);
            HAPLogError(&kHAPLog_Default, "%s: tcsetattr failed: %d", __func__, _errno);
        }
    }
    HAPPlatformFileHandleDeregister(accessoryConfiguration.stdinFileHandle);
}

//----------------------------------------------------------------------------------------------------------------------

void AppCreate(
    HAPAccessoryServerRef *server,
    HAPPlatformKeyValueStoreRef keyValueStore)
{
    HAPPrecondition(server);
    HAPPrecondition(keyValueStore);

    HAPDataStreamDispatcherCreate(server, &dataStreamDispatcher,
        &(const HAPDataStreamDispatcherOptions) {
            .storage = &dataStreamDispatcherStorage
        });
    
    HAPRawBufferZero(&accessoryConfiguration, sizeof accessoryConfiguration);
    accessoryConfiguration.server = server;
    accessoryConfiguration.keyValueStore = keyValueStore;
    accessoryConfiguration.startTime = HAPPlatformClockGetCurrent();
    
    PrintButtonConfiguration();
    
    StdinPipeCreate();
    
    InitializeSignalHandlers();
    
    static RemoteCallbacks remoteCallbacks = {
        .handleActiveChange = HandleActiveChange,
        .handleConfigurationChange = HandleConfigurationChange
    };

    static HAPPlatformMicrophoneStream microphoneStreams[HAPArrayCount(remotes)];
    HAPPlatformMicrophoneCreate(&remoteMicrophone,
        &(const HAPPlatformMicrophoneOptions) {
            .audioInputDevice = "plughw:1",
            .microphoneStreams = microphoneStreams,
            .numMicrophoneStreams = HAPArrayCount(microphoneStreams)
        });
    
    // Remote 1
    {
        HAPPlatformKeyValueStoreCreate(&remoteKeyValueStores[0], &(const HAPPlatformKeyValueStoreOptions) {
            .rootDirectory = ".Remote1"
        });
        
        static RemoteOptions remoteOptions;
        HAPRawBufferZero(&remoteOptions, sizeof remoteOptions);
        
        remoteOptions.type = kRemoteType_Hardware;
        remoteOptions.server = server;
        remoteOptions.accessory = bridgedAccessories[0];
        remoteOptions.supportedButtons = supportedButtonsRemote0;
        remoteOptions.numSupportedButtons = HAPArrayCount(supportedButtonsRemote0);
        remoteOptions.remoteKeyValueStore = &remoteKeyValueStores[0];
        remoteOptions.remoteMicrophone = &remoteMicrophone;
        remoteOptions.remoteCallbacks = &remoteCallbacks;
        
        RemoteCreate(&remotes[0], &remoteOptions, /* context: */ NULL);
    
        PrintSupportedButtons(supportedButtonsRemote0, HAPArrayCount(supportedButtonsRemote0));
        PrintTargets(&remotes[0]);
    }
    
    // Remote 2
    {
        HAPPlatformKeyValueStoreCreate(&remoteKeyValueStores[1], &(const HAPPlatformKeyValueStoreOptions) {
            .rootDirectory = ".Remote2"
        });
        
        static RemoteOptions remoteOptions;
        HAPRawBufferZero(&remoteOptions, sizeof remoteOptions);
        
        remoteOptions.type = kRemoteType_Hardware;
        remoteOptions.server = server;
        remoteOptions.accessory = bridgedAccessories[1];
        remoteOptions.supportedButtons = supportedButtonsRemote1;
        remoteOptions.numSupportedButtons = HAPArrayCount(supportedButtonsRemote1);
        remoteOptions.remoteKeyValueStore = &remoteKeyValueStores[1];
        remoteOptions.remoteMicrophone = &remoteMicrophone;
        remoteOptions.remoteCallbacks = &remoteCallbacks;
    
        RemoteCreate(&remotes[1], &remoteOptions, /* context: */ NULL);
    
        PrintSupportedButtons(supportedButtonsRemote1, HAPArrayCount(supportedButtonsRemote1));
        PrintTargets(&remotes[1]);
    }
}

void AppRelease(void)
{
    // Microphone.
    HAPPlatformMicrophoneRelease(&remoteMicrophone);
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
    
    if (!HAPAccessoryServerIsPaired(server)) {
        RemoteHandleUnpair(&remotes[0], context);
        PrintTargets(&remotes[0]);
        RemoteHandleUnpair(&remotes[1], context);
        PrintTargets(&remotes[1]);
    }
    
    switch (HAPAccessoryServerGetState(server)) {
        case kHAPAccessoryServerState_Idle: {
            HAPLogInfo(&kHAPLog_Default, "Accessory Server State did update: Idle.");
            // AccessoryServer stop requested by signal.
            
            // Release remotes.
            RemoteRelease(&remotes[0], context);
            RemoteRelease(&remotes[1], context);
            
            if (accessoryConfiguration.restoreFactorySettingsRequested) {
                HAPLogInfo(&kHAPLog_Default, "A factory reset has been requested.");
                
                // Restore factory settings in remote.
                RemoteRestoreFactorySettings(&remoteKeyValueStores[0]);
                RemoteRestoreFactorySettings(&remoteKeyValueStores[1]);
                
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
            StdinPipeRelease();

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
