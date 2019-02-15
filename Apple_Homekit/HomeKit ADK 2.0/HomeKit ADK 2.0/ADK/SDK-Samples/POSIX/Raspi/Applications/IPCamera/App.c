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

// An accessory that represents an IP Camera.
//
// This implementation is intended for POSIX platforms.
//
// To enable user interaction, POSIX signals are used:
//
// - Send SIGUSR1 to enter WAC mode.
// - Send SIGUSR2 to the process to trigger a factory reset.
// - Send SIGTERM to the process to trigger a controlled shutdown.
//
// The code consists of multiple parts:
//
//   1. The definition of the accessory configuration and its internal state.
//
//   2. Helper functions to load and save the state of the accessory.
//
//   3. The callbacks that implement the actual behavior of the accessory.
//
//   4. The video stream configurations for stream #1 and #2.
//
//   5. The signal handlers.
//
//   6. The setup of the accessory HomeKit and the bridged accessories attribute database.
//
//   7. The initialization of the accessory state.
//
//   8. A callback that gets called when the server state is updated.

#include <signal.h>

#include "HAP.h"
#include "HAPPlatform+Init.h"
#include "HAPPlatformCamera+Init.h"

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
 * Global accessory configuration.
 */
typedef struct {
    HAPAccessoryServerRef *server;
    HAPPlatformKeyValueStoreRef keyValueStore;
    HAPPlatformCamera *camera;
    bool restoreFactorySettingsRequested;
} AccessoryConfiguration;

static AccessoryConfiguration accessoryConfiguration;

//----------------------------------------------------------------------------------------------------------------------

/**
 * Initialize the default accessory state
 */
static void SetupDefaultAccessoryState(void)
{
    accessoryConfiguration.camera->state.microphone.mute = false;
    accessoryConfiguration.camera->state.microphone.volume = 50;
    
    accessoryConfiguration.camera->state.speaker.mute = false;
    accessoryConfiguration.camera->state.speaker.volume = 50;
}

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
        &accessoryConfiguration.camera->state, sizeof accessoryConfiguration.camera->state, &numBytes, &found);
    
    if (err) {
        HAPAssert(err == kHAPError_Unknown);
        HAPFatalError();
    }
    if (!found || numBytes != sizeof accessoryConfiguration.camera->state) {
        if (found) {
            HAPLog(&kHAPLog_Default, "Unexpected app state found in key-value store. Resetting to default.");
        }
        SetupDefaultAccessoryState();
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
        &accessoryConfiguration.camera->state, sizeof accessoryConfiguration.camera->state);
    if (err) {
        HAPAssert(err == kHAPError_Unknown);
        HAPFatalError();
    }
}

//----------------------------------------------------------------------------------------------------------------------


/**
 * Identify routine. Used to locate the accessory.
 */
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
 * Handle read request to the 'Mute' characteristic of the Microphone service.
 */
HAP_RESULT_USE_CHECK
HAPError HandleMicrophoneMuteRead(
    HAPAccessoryServerRef *server HAP_UNUSED,
    const HAPBoolCharacteristicReadRequest *request HAP_UNUSED,
    bool *value,
    void *_Nullable context HAP_UNUSED)
{
    *value = accessoryConfiguration.camera->state.microphone.mute;
    HAPLogInfo(&kHAPLog_Default, "%s: %s", __func__, *value ? "true" : "false");
    return kHAPError_None;
}

/**
 * Handle write request to the 'Mute' characteristic of the Microphone service.
 */
HAP_RESULT_USE_CHECK
HAPError HandleMicrophoneMuteWrite(
    HAPAccessoryServerRef *server,
    const HAPBoolCharacteristicWriteRequest *request,
    bool value,
    void *_Nullable context HAP_UNUSED)
{
    HAPLogInfo(&kHAPLog_Default, "%s: %s", __func__, value ? "true" : "false");
    if (accessoryConfiguration.camera->state.microphone.mute != value) {
        accessoryConfiguration.camera->state.microphone.mute = value;
        SaveAccessoryState();
        HAPPlatformCameraSetMicrophone(accessoryConfiguration.camera);
        HAPAccessoryServerRaiseEvent(
            server,
            request->characteristic,
            request->service,
            request->accessory);
    }
    return kHAPError_None;
}

/**
 * Handle read request to the 'Volume' characteristic of the Microphone service.
 */
HAP_RESULT_USE_CHECK
HAPError HandleMicrophoneVolumeRead(
    HAPAccessoryServerRef *server HAP_UNUSED,
    const HAPUInt8CharacteristicReadRequest *request HAP_UNUSED,
    uint8_t *value,
    void *_Nullable context HAP_UNUSED)
{
    *value = accessoryConfiguration.camera->state.microphone.volume;
    HAPLogInfo(&kHAPLog_Default, "%s: %s", __func__, *value ? "true" : "false");
    return kHAPError_None;
}

/**
 * Handle write request to the 'Volume' characteristic of the Microphone service.
 */
HAP_RESULT_USE_CHECK
HAPError HandleMicrophoneVolumeWrite(
    HAPAccessoryServerRef *server,
    const HAPUInt8CharacteristicWriteRequest *request,
    uint8_t value,
    void *_Nullable context HAP_UNUSED)
{
    HAPLogInfo(&kHAPLog_Default, "%s: %s", __func__, value ? "true" : "false");
    if (accessoryConfiguration.camera->state.microphone.volume != value) {
        accessoryConfiguration.camera->state.microphone.volume = value;
        SaveAccessoryState();
        HAPPlatformCameraSetMicrophone(accessoryConfiguration.camera);
        HAPAccessoryServerRaiseEvent(
            server,
            request->characteristic,
            request->service,
            request->accessory);
    }
    return kHAPError_None;
}

/**
 * Handle read request to the 'Mute' characteristic of the Speaker service.
 */
HAP_RESULT_USE_CHECK
HAPError HandleSpeakerMuteRead(
    HAPAccessoryServerRef *server HAP_UNUSED,
    const HAPBoolCharacteristicReadRequest *request HAP_UNUSED,
    bool *value,
    void *_Nullable context HAP_UNUSED)
{
    *value = accessoryConfiguration.camera->state.speaker.mute;
    HAPLogInfo(&kHAPLog_Default, "%s: %s", __func__, *value ? "true" : "false");
    return kHAPError_None;
}

/**
 * Handle write request to the 'Mute' characteristic of the Speaker service.
 */
HAP_RESULT_USE_CHECK
HAPError HandleSpeakerMuteWrite(
    HAPAccessoryServerRef *server,
    const HAPBoolCharacteristicWriteRequest *request,
    bool value,
    void *_Nullable context HAP_UNUSED)
{
    HAPLogInfo(&kHAPLog_Default, "%s: %s", __func__, value ? "true" : "false");
    if (accessoryConfiguration.camera->state.speaker.mute != value) {
        accessoryConfiguration.camera->state.speaker.mute = value;
        SaveAccessoryState();
        HAPPlatformCameraSetSpeaker(accessoryConfiguration.camera);
        HAPAccessoryServerRaiseEvent(
            server,
            request->characteristic,
            request->service,
            request->accessory);
    }
    return kHAPError_None;
}

/**
 * Handle read request to the 'Volume' characteristic of the Speaker service.
 */
HAP_RESULT_USE_CHECK
HAPError HandleSpeakerVolumeRead(
    HAPAccessoryServerRef *server HAP_UNUSED,
    const HAPUInt8CharacteristicReadRequest *request HAP_UNUSED,
    uint8_t *value,
    void *_Nullable context HAP_UNUSED)
{
    *value = accessoryConfiguration.camera->state.speaker.volume;
    HAPLogInfo(&kHAPLog_Default, "%s: %s", __func__, *value ? "true" : "false");
    return kHAPError_None;
}

/**
 * Handle write request to the 'Volume' characteristic of the Speaker service.
 */
HAP_RESULT_USE_CHECK
HAPError HandleSpeakerVolumeWrite(
    HAPAccessoryServerRef *server,
    const HAPUInt8CharacteristicWriteRequest *request,
    uint8_t value,
    void *_Nullable context HAP_UNUSED)
{
    HAPLogInfo(&kHAPLog_Default, "%s: %s", __func__, value ? "true" : "false");
    if (accessoryConfiguration.camera->state.speaker.volume != value) {
        accessoryConfiguration.camera->state.speaker.volume = value;
        SaveAccessoryState();
        HAPPlatformCameraSetSpeaker(accessoryConfiguration.camera);
        HAPAccessoryServerRaiseEvent(
            server,
            request->characteristic,
            request->service,
            request->accessory);
    }
    return kHAPError_None;
}

//----------------------------------------------------------------------------------------------------------------------

/**
 * H264 video codec parameters
 */
static const HAPH264VideoCodecParameters h264VideoCodecParameters = {
    .profile = kHAPH264VideoCodecProfile_Main,
    .level = (HAPH264VideoCodecProfileLevel) (
        kHAPH264VideoCodecProfileLevel_3_1 |
        kHAPH264VideoCodecProfileLevel_4
    ),
    .packetizationMode = kHAPH264VideoCodecPacketizationMode_NonInterleaved,
    .cvo = false,
    .cvoID = 0
};

/**
 * Available video parameters
 */
static const HAPVideoAttributes videoAttributes[] = {
    [0] = { .width = 1920, .height = 1080, .maxFrameRate = 30 },
    [1] = { .width = 1280, .height =  720, .maxFrameRate = 30 },
    [2] = { .width =  640, .height =  360, .maxFrameRate = 30 },
    [3] = { .width =  480, .height =  270, .maxFrameRate = 30 },
    [4] = { .width =  320, .height =  180, .maxFrameRate = 30 },
    [5] = { .width = 1280, .height =  960, .maxFrameRate = 30 },
    [6] = { .width = 1024, .height =  768, .maxFrameRate = 30 },
    [7] = { .width =  640, .height =  480, .maxFrameRate = 30 },
    [8] = { .width =  480, .height =  360, .maxFrameRate = 30 },
    [9] = { .width =  320, .height =  240, .maxFrameRate = 30 }
};

/**
 * Full screen video parameters
 */
static const HAPVideoAttributes *fullVideoAttributes[] = {
        &videoAttributes[0],
        &videoAttributes[1],
        &videoAttributes[2],
        &videoAttributes[3],
        &videoAttributes[4],
        &videoAttributes[5],
        &videoAttributes[6],
        &videoAttributes[7],
        &videoAttributes[8],
        &videoAttributes[9],
        NULL
};

/**
 * Small screen video parameters
 */
static const HAPVideoAttributes *smallVideoAttributes[] = {
        // no 1080p resolution
        &videoAttributes[1],
        &videoAttributes[2],
        &videoAttributes[3],
        &videoAttributes[4],
        // no 960p resolution
        &videoAttributes[6],
        &videoAttributes[7],
        &videoAttributes[8],
        &videoAttributes[9],
        NULL
};

/**
 * H264 video codec configuration
 */
static const HAPCameraSupportedVideoCodecConfiguration h264FullVideoCodecConfiguration = {
    .codecType = kHAPVideoCodecType_H264,
    .codecParameters = &h264VideoCodecParameters,
    .attributes = fullVideoAttributes
};

/**
 * H264 video codec configuration with restricted resolution
 */
static const HAPCameraSupportedVideoCodecConfiguration h264SmallVideoCodecConfiguration = {
    .codecType = kHAPVideoCodecType_H264,
    .codecParameters = &h264VideoCodecParameters,
    .attributes = smallVideoAttributes
};

/**
 * Opus audio codec parameters
 */
static const HAPAudioCodecParameters varAudioCodecParameters = {
    .numberOfChannels = 1,
    .bitRateMode = kHAPAudioCodecBitRateControlMode_Variable,
    .sampleRate = (HAPAudioCodecSampleRate) (
        kHAPAudioCodecSampleRate_16KHz |
        kHAPAudioCodecSampleRate_24KHz
    )
};

/**
 * Opus audio configuration
 */
static const HAPCameraSupportedAudioCodecConfiguration opusAudioCodecConfiguration = {
    .codecType = kHAPAudioCodecType_Opus,
    .codecParameters = &varAudioCodecParameters
};

/**
 * First video stream:
 *  - Video: H264, Level 3.1 and 4, all resolutions
 *  - Audio: Opus, 16kHz and 24kHz sample rate
 */
static const HAPCameraStreamSupportedConfigurations supportedCameraStreamConfigurations0 = {
    .videoStream = {
        .configurations = (const HAPCameraSupportedVideoCodecConfiguration *const[]) {
            &h264FullVideoCodecConfiguration,
            NULL
        }
    },
    .audioStream = {
        .configurations = (const HAPCameraSupportedAudioCodecConfiguration *const[]) {
            &opusAudioCodecConfiguration,
            NULL
        },
        .comfortNoise = {
            .supported = false
        }
    },
    .rtp = {
        .srtpCryptoSuites = (HAPSRTPCryptoSuite) (
            kHAPSRTPCryptoSuite_AES_CM_128_HMAC_SHA1_80 |
            kHAPSRTPCryptoSuite_AES_256_CM_HMAC_SHA1_80 |
            kHAPSRTPCryptoSuite_Disabled
        )
    }
};

/**
 * Second video stream
 *  - Video: H264, Level 3.1 and 4, up to 720p resolution
 *  - Audio: Opus, 16kHz and 24kHz sample rate
 */
static const HAPCameraStreamSupportedConfigurations supportedCameraStreamConfigurations1 = {
    .videoStream = {
        .configurations = (const HAPCameraSupportedVideoCodecConfiguration *const[]) {
            &h264SmallVideoCodecConfiguration,
            NULL
        }
    },
    .audioStream = {
        .configurations = (const HAPCameraSupportedAudioCodecConfiguration *const[]) {
            &opusAudioCodecConfiguration,
            NULL
        },
        .comfortNoise = {
            .supported = false
        }
    },
    .rtp = {
        .srtpCryptoSuites = (HAPSRTPCryptoSuite) (
            kHAPSRTPCryptoSuite_AES_CM_128_HMAC_SHA1_80 |
            kHAPSRTPCryptoSuite_AES_256_CM_HMAC_SHA1_80 |
            kHAPSRTPCryptoSuite_Disabled
        )
    }
};

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
            HAPAccessoryServerEnterWACMode(accessoryConfiguration.server);
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

/**
 * HomeKit accessory that provides the streaming services and the Microphone and Speaker services.
 */
static const HAPAccessory accessory = {
    .aid = 1,
    .category = kHAPAccessoryCategory_IPCameras,
    .name = "Acme IP Camera",
    .manufacturer = "Acme",
    .model = "IPCamera1,1",
    .serialNumber = "099DB48E9E28",
    .firmwareVersion = "1",
    .hardwareVersion = "1",
    .services = (const HAPService *const[]) {
        &accessoryInformationService,
        &hapProtocolInformationService,
        &pairingService,
        &cameraRTPStreamManagement0Service,
        &cameraRTPStreamManagement1Service,
        &microphoneService,
        &speakerService,
        NULL
    },
    .cameraStreamConfigurations = (const HAPCameraStreamSupportedConfigurations *const[]) {
        &supportedCameraStreamConfigurations0,
        &supportedCameraStreamConfigurations1,
        NULL
    },
    .callbacks = {
        .identify = IdentifyAccessory
    }
};

//----------------------------------------------------------------------------------------------------------------------

void AppCreate(
    HAPAccessoryServerRef *server,
    HAPPlatformKeyValueStoreRef keyValueStore,
    HAPPlatformCameraRef camera)
{
    HAPPrecondition(server);
    HAPPrecondition(keyValueStore);
    
    HAPLogInfo(&kHAPLog_Default, "%s", __func__);
    
    HAPRawBufferZero(&accessoryConfiguration, sizeof accessoryConfiguration);
    accessoryConfiguration.server = server;
    accessoryConfiguration.keyValueStore = keyValueStore;
    accessoryConfiguration.camera = (HAPPlatformCamera *) camera;
    
    InitializeSignalHandlers();
    
    LoadAccessoryState();
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
                AppCreate(server, accessoryConfiguration.keyValueStore, accessoryConfiguration.camera);
                
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
