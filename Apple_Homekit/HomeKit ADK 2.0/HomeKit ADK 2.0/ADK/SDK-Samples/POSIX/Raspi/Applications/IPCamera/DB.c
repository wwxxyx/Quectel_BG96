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

// This file contains the accessory attribute database that defines the accessory information service, HAP Protocol
// Information Service, the Pairing service, the service signature and the RTP stream, microphone and speaker
// services exposed by the IP camera.

#include "App.h"
#include "DB.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * IID constants.
 */
#define kIID_AccessoryInformation                                                   ((uint64_t) 0x0001)
#define kIID_AccessoryInformationIdentify                                           ((uint64_t) 0x0002)
#define kIID_AccessoryInformationManufacturer                                       ((uint64_t) 0x0003)
#define kIID_AccessoryInformationModel                                              ((uint64_t) 0x0004)
#define kIID_AccessoryInformationName                                               ((uint64_t) 0x0005)
#define kIID_AccessoryInformationSerialNumber                                       ((uint64_t) 0x0006)
#define kIID_AccessoryInformationFirmwareRevision                                   ((uint64_t) 0x0007)
#define kIID_AccessoryInformationHardwareRevision                                   ((uint64_t) 0x0008)
#define kIID_AccessoryInformationADKVersion                                         ((uint64_t) 0x0009)

#define kIID_HAPProtocolInformation                                                 ((uint64_t) 0x0010)
#define kIID_HAPProtocolInformationServiceSignature                                 ((uint64_t) 0x0011)
#define kIID_HAPProtocolInformationVersion                                          ((uint64_t) 0x0012)

#define kIID_Pairing                                                                ((uint64_t) 0x0020)
#define kIID_PairingPairSetup                                                       ((uint64_t) 0x0022)
#define kIID_PairingPairVerify                                                      ((uint64_t) 0x0023)
#define kIID_PairingPairingFeatures                                                 ((uint64_t) 0x0024)
#define kIID_PairingPairingPairings                                                 ((uint64_t) 0x0025)

#define kIID_CameraRTPStreamManagement0                                             ((uint64_t) 0x0030)
#define kIID_CameraRTPStreamManagement0ServiceSignature                             ((uint64_t) 0x0031)
#define kIID_CameraRTPStreamManagement0StreamingStatus                              ((uint64_t) 0x0033)
#define kIID_CameraRTPStreamManagement0SupportedVideoStreamConfiguration            ((uint64_t) 0x0034)
#define kIID_CameraRTPStreamManagement0SupportedAudioStreamConfiguration            ((uint64_t) 0x0035)
#define kIID_CameraRTPStreamManagement0SupportedRTPConfiguration                    ((uint64_t) 0x0036)
#define kIID_CameraRTPStreamManagement0SetupEndpoints                               ((uint64_t) 0x0037)
#define kIID_CameraRTPStreamManagement0SelectedRTPStreamConfiguration               ((uint64_t) 0x0038)

#define kIID_CameraRTPStreamManagement1                                             ((uint64_t) 0x0040)
#define kIID_CameraRTPStreamManagement1StreamingStatus                              ((uint64_t) 0x0043)
#define kIID_CameraRTPStreamManagement1SupportedVideoStreamConfiguration            ((uint64_t) 0x0044)
#define kIID_CameraRTPStreamManagement1SupportedAudioStreamConfiguration            ((uint64_t) 0x0045)
#define kIID_CameraRTPStreamManagement1SupportedRTPConfiguration                    ((uint64_t) 0x0046)
#define kIID_CameraRTPStreamManagement1SetupEndpoints                               ((uint64_t) 0x0047)
#define kIID_CameraRTPStreamManagement1SelectedRTPStreamConfiguration               ((uint64_t) 0x0048)

#define kIID_Microphone                                                             ((uint64_t) 0x0050)
#define kIID_MicrophoneName                                                         ((uint64_t) 0x0052)
#define kIID_MicrophoneMute                                                         ((uint64_t) 0x0053)
#define kIID_MicrophoneVolume                                                       ((uint64_t) 0x0054)

#define kIID_Speaker                                                                ((uint64_t) 0x0060)
#define kIID_SpeakerName                                                            ((uint64_t) 0x0062)
#define kIID_SpeakerMute                                                            ((uint64_t) 0x0063)
#define kIID_SpeakerVolume                                                          ((uint64_t) 0x0064)

HAP_STATIC_ASSERT(kAttributeCount == 9 + 3 + 5 + 8 + 7 + 4 + 4, AttributeCount_mismatch);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const HAPBoolCharacteristic accessoryInformationIdentifyCharacteristic = {
    .format = kHAPCharacteristicFormat_Bool,
    .iid = kIID_AccessoryInformationIdentify,
    .characteristicType = &kHAPCharacteristicType_Identify,
    .debugDescription = kHAPCharacteristicDebugDescription_Identify,
    .manufacturerDescription = NULL,
    .properties = {
        .readable = false,
        .writable = true,
        .supportsEventNotification = false,
        .hidden = false,
        .requiresTimedWrite = false,
        .supportsAuthorizationData = false,
        .ip = {
            .controlPoint = false,
            .supportsWriteResponse = false
        },
        .ble = {
            .supportsBroadcastNotification = false,
            .supportsDisconnectedNotification = false,
            .readableWithoutSecurity = false,
            .writableWithoutSecurity = false
        }
    },
    .callbacks = {
        .handleRead = NULL,
        .handleWrite = HAPHandleAccessoryInformationIdentifyWrite
    }
};

const HAPStringCharacteristic accessoryInformationManufacturerCharacteristic = {
    .format = kHAPCharacteristicFormat_String,
    .iid = kIID_AccessoryInformationManufacturer,
    .characteristicType = &kHAPCharacteristicType_Manufacturer,
    .debugDescription = kHAPCharacteristicDebugDescription_Manufacturer,
    .manufacturerDescription = NULL,
    .properties = {
        .readable = true,
        .writable = false,
        .supportsEventNotification = false,
        .hidden = false,
        .requiresTimedWrite = false,
        .supportsAuthorizationData = false,
        .ip = {
            .controlPoint = false,
            .supportsWriteResponse = false
        },
        .ble = {
            .supportsBroadcastNotification = false,
            .supportsDisconnectedNotification = false,
            .readableWithoutSecurity = false,
            .writableWithoutSecurity = false
        }
    },
    .constraints = {
        .maxLength = 64
    },
    .callbacks = {
        .handleRead = HAPHandleAccessoryInformationManufacturerRead,
        .handleWrite = NULL
    }
};

const HAPStringCharacteristic accessoryInformationModelCharacteristic = {
    .format = kHAPCharacteristicFormat_String,
    .iid = kIID_AccessoryInformationModel,
    .characteristicType = &kHAPCharacteristicType_Model,
    .debugDescription = kHAPCharacteristicDebugDescription_Model,
    .manufacturerDescription = NULL,
    .properties = {
        .readable = true,
        .writable = false,
        .supportsEventNotification = false,
        .hidden = false,
        .requiresTimedWrite = false,
        .supportsAuthorizationData = false,
        .ip = {
            .controlPoint = false,
            .supportsWriteResponse = false
        },
        .ble = {
            .supportsBroadcastNotification = false,
            .supportsDisconnectedNotification = false,
            .readableWithoutSecurity = false,
            .writableWithoutSecurity = false
        }
    },
    .constraints = {
        .maxLength = 64
    },
    .callbacks = {
        .handleRead = HAPHandleAccessoryInformationModelRead,
        .handleWrite = NULL
    }
};

const HAPStringCharacteristic accessoryInformationNameCharacteristic = {
    .format = kHAPCharacteristicFormat_String,
    .iid = kIID_AccessoryInformationName,
    .characteristicType = &kHAPCharacteristicType_Name,
    .debugDescription = kHAPCharacteristicDebugDescription_Name,
    .manufacturerDescription = NULL,
    .properties = {
        .readable = true,
        .writable = false,
        .supportsEventNotification = false,
        .hidden = false,
        .requiresTimedWrite = false,
        .supportsAuthorizationData = false,
        .ip = {
            .controlPoint = false,
            .supportsWriteResponse = false
        },
        .ble = {
            .supportsBroadcastNotification = false,
            .supportsDisconnectedNotification = false,
            .readableWithoutSecurity = false,
            .writableWithoutSecurity = false
        }
    },
    .constraints = {
        .maxLength = 64
    },
    .callbacks = {
        .handleRead = HAPHandleAccessoryInformationNameRead,
        .handleWrite = NULL
    }
};

const HAPStringCharacteristic accessoryInformationSerialNumberCharacteristic = {
    .format = kHAPCharacteristicFormat_String,
    .iid = kIID_AccessoryInformationSerialNumber,
    .characteristicType = &kHAPCharacteristicType_SerialNumber,
    .debugDescription = kHAPCharacteristicDebugDescription_SerialNumber,
    .manufacturerDescription = NULL,
    .properties = {
        .readable = true,
        .writable = false,
        .supportsEventNotification = false,
        .hidden = false,
        .requiresTimedWrite = false,
        .supportsAuthorizationData = false,
        .ip = {
            .controlPoint = false,
            .supportsWriteResponse = false
        },
        .ble = {
            .supportsBroadcastNotification = false,
            .supportsDisconnectedNotification = false,
            .readableWithoutSecurity = false,
            .writableWithoutSecurity = false
        }
    },
    .constraints = {
        .maxLength = 64
    },
    .callbacks = {
        .handleRead = HAPHandleAccessoryInformationSerialNumberRead,
        .handleWrite = NULL
    }
};

const HAPStringCharacteristic accessoryInformationFirmwareRevisionCharacteristic = {
    .format = kHAPCharacteristicFormat_String,
    .iid = kIID_AccessoryInformationFirmwareRevision,
    .characteristicType = &kHAPCharacteristicType_FirmwareRevision,
    .debugDescription = kHAPCharacteristicDebugDescription_FirmwareRevision,
    .manufacturerDescription = NULL,
    .properties = {
        .readable = true,
        .writable = false,
        .supportsEventNotification = false,
        .hidden = false,
        .requiresTimedWrite = false,
        .supportsAuthorizationData = false,
        .ip = {
            .controlPoint = false,
            .supportsWriteResponse = false
        },
        .ble = {
            .supportsBroadcastNotification = false,
            .supportsDisconnectedNotification = false,
            .readableWithoutSecurity = false,
            .writableWithoutSecurity = false
        }
    },
    .constraints = {
        .maxLength = 64
    },
    .callbacks = {
        .handleRead = HAPHandleAccessoryInformationFirmwareRevisionRead,
        .handleWrite = NULL
    }
};

const HAPStringCharacteristic accessoryInformationHardwareRevisionCharacteristic = {
    .format = kHAPCharacteristicFormat_String,
    .iid = kIID_AccessoryInformationHardwareRevision,
    .characteristicType = &kHAPCharacteristicType_HardwareRevision,
    .debugDescription = kHAPCharacteristicDebugDescription_HardwareRevision,
    .manufacturerDescription = NULL,
    .properties = {
        .readable = true,
        .writable = false,
        .supportsEventNotification = false,
        .hidden = false,
        .requiresTimedWrite = false,
        .supportsAuthorizationData = false,
        .ip = {
            .controlPoint = false,
            .supportsWriteResponse = false
        },
        .ble = {
            .supportsBroadcastNotification = false,
            .supportsDisconnectedNotification = false,
            .readableWithoutSecurity = false,
            .writableWithoutSecurity = false
        }
    },
    .constraints = {
        .maxLength = 64
    },
    .callbacks = {
        .handleRead = HAPHandleAccessoryInformationHardwareRevisionRead,
        .handleWrite = NULL
    }
};

const HAPStringCharacteristic accessoryInformationADKVersionCharacteristic = {
    .format = kHAPCharacteristicFormat_String,
    .iid = kIID_AccessoryInformationADKVersion,
    .characteristicType = &kHAPCharacteristicType_ADKVersion,
    .debugDescription = kHAPCharacteristicDebugDescription_ADKVersion,
    .manufacturerDescription = NULL,
    .properties = {
        .readable = true,
        .writable = false,
        .supportsEventNotification = false,
        .hidden = true,
        .requiresTimedWrite = false,
        .supportsAuthorizationData = false,
        .ip = {
            .controlPoint = false,
            .supportsWriteResponse = false
        },
        .ble = {
            .supportsBroadcastNotification = false,
            .supportsDisconnectedNotification = false,
            .readableWithoutSecurity = false,
            .writableWithoutSecurity = false
        }
    },
    .constraints = {
        .maxLength = 64
    },
    .callbacks = {
        .handleRead = HAPHandleAccessoryInformationADKVersionRead,
        .handleWrite = NULL
    }
};

const HAPService accessoryInformationService = {
    .iid = kIID_AccessoryInformation,
    .serviceType = &kHAPServiceType_AccessoryInformation,
    .debugDescription = kHAPServiceDebugDescription_AccessoryInformation,
    .name = NULL,
    .properties = {
        .primaryService = false,
        .hidden = false,
        .ble = {
            .supportsConfiguration = false
        }
    },
    .linkedServices = NULL,
    .characteristics = (const HAPCharacteristic *const []) {
        &accessoryInformationIdentifyCharacteristic,
        &accessoryInformationManufacturerCharacteristic,
        &accessoryInformationModelCharacteristic,
        &accessoryInformationNameCharacteristic,
        &accessoryInformationSerialNumberCharacteristic,
        &accessoryInformationFirmwareRevisionCharacteristic,
        &accessoryInformationHardwareRevisionCharacteristic,
        &accessoryInformationADKVersionCharacteristic,
        NULL
    }
};

//----------------------------------------------------------------------------------------------------------------------

static const HAPDataCharacteristic hapProtocolInformationServiceSignatureCharacteristic = {
    .format = kHAPCharacteristicFormat_Data,
    .iid = kIID_HAPProtocolInformationServiceSignature,
    .characteristicType = &kHAPCharacteristicType_ServiceSignature,
    .debugDescription = kHAPCharacteristicDebugDescription_ServiceSignature,
    .manufacturerDescription = NULL,
    .properties = {
        .readable = true,
        .writable = false,
        .supportsEventNotification = false,
        .hidden = false,
        .requiresTimedWrite = false,
        .supportsAuthorizationData = false,
        .ip = {
            .controlPoint = true
        },
        .ble = {
            .supportsBroadcastNotification = false,
            .supportsDisconnectedNotification = false,
            .readableWithoutSecurity = false,
            .writableWithoutSecurity = false
        }
    },
    .constraints = {
        .maxLength = 2097152
    },
    .callbacks = {
        .handleRead = HAPHandleServiceSignatureRead,
        .handleWrite = NULL
    }
};

static const HAPStringCharacteristic hapProtocolInformationVersionCharacteristic = {
    .format = kHAPCharacteristicFormat_String,
    .iid = kIID_HAPProtocolInformationVersion,
    .characteristicType = &kHAPCharacteristicType_Version,
    .debugDescription = kHAPCharacteristicDebugDescription_Version,
    .manufacturerDescription = NULL,
    .properties = {
        .readable = true,
        .writable = false,
        .supportsEventNotification = false,
        .hidden = false,
        .requiresTimedWrite = false,
        .supportsAuthorizationData = false,
        .ip = {
            .controlPoint = false,
            .supportsWriteResponse = false
        },
        .ble = {
            .supportsBroadcastNotification = false,
            .supportsDisconnectedNotification = false,
            .readableWithoutSecurity = false,
            .writableWithoutSecurity = false
        }
    },
    .constraints = {
        .maxLength = 64
    },
    .callbacks = {
        .handleRead = HAPHandleHAPProtocolInformationVersionRead,
        .handleWrite = NULL
    }
};

const HAPService hapProtocolInformationService = {
    .iid = kIID_HAPProtocolInformation,
    .serviceType = &kHAPServiceType_HAPProtocolInformation,
    .debugDescription = kHAPServiceDebugDescription_HAPProtocolInformation,
    .name = NULL,
    .properties = {
        .primaryService = false,
        .hidden = false,
        .ble = {
            .supportsConfiguration = true
        }
    },
    .linkedServices = NULL,
    .characteristics = (const HAPCharacteristic *const []) {
        &hapProtocolInformationServiceSignatureCharacteristic,
        &hapProtocolInformationVersionCharacteristic,
        NULL
    }
};

//----------------------------------------------------------------------------------------------------------------------

static const HAPTLV8Characteristic pairingPairSetupCharacteristic = {
    .format = kHAPCharacteristicFormat_TLV8,
    .iid = kIID_PairingPairSetup,
    .characteristicType = &kHAPCharacteristicType_PairSetup,
    .debugDescription = kHAPCharacteristicDebugDescription_PairSetup,
    .manufacturerDescription = NULL,
    .properties = {
        .readable = false,
        .writable = false,
        .supportsEventNotification = false,
        .hidden = false,
        .requiresTimedWrite = false,
        .supportsAuthorizationData = false,
        .ip = {
            .controlPoint = true
        },
        .ble = {
            .supportsBroadcastNotification = false,
            .supportsDisconnectedNotification = false,
            .readableWithoutSecurity = true,
            .writableWithoutSecurity = true
        }
    },
    .callbacks = {
        .handleRead = HAPHandlePairingPairSetupRead,
        .handleWrite = HAPHandlePairingPairSetupWrite
    }
};

static const HAPTLV8Characteristic pairingPairVerifyCharacteristic = {
    .format = kHAPCharacteristicFormat_TLV8,
    .iid = kIID_PairingPairVerify,
    .characteristicType = &kHAPCharacteristicType_PairVerify,
    .debugDescription = kHAPCharacteristicDebugDescription_PairVerify,
    .manufacturerDescription = NULL,
    .properties = {
        .readable = false,
        .writable = false,
        .supportsEventNotification = false,
        .hidden = false,
        .requiresTimedWrite = false,
        .supportsAuthorizationData = false,
        .ip = {
            .controlPoint = true
        },
        .ble = {
            .supportsBroadcastNotification = false,
            .supportsDisconnectedNotification = false,
            .readableWithoutSecurity = true,
            .writableWithoutSecurity = true
        }
    },
    .callbacks = {
        .handleRead = HAPHandlePairingPairVerifyRead,
        .handleWrite = HAPHandlePairingPairVerifyWrite
    }
};

static const HAPUInt8Characteristic pairingPairingFeaturesCharacteristic = {
    .format = kHAPCharacteristicFormat_UInt8,
    .iid = kIID_PairingPairingFeatures,
    .characteristicType = &kHAPCharacteristicType_PairingFeatures,
    .debugDescription = kHAPCharacteristicDebugDescription_PairingFeatures,
    .manufacturerDescription = NULL,
    .properties = {
        .readable = false,
        .writable = false,
        .supportsEventNotification = false,
        .hidden = false,
        .requiresTimedWrite = false,
        .supportsAuthorizationData = false,
        .ip = {
            .controlPoint = false,
            .supportsWriteResponse = false
        },
        .ble = {
            .supportsDisconnectedNotification = false,
            .supportsBroadcastNotification = false,
            .readableWithoutSecurity = true,
            .writableWithoutSecurity = false
        }
    },
    .units = kHAPCharacteristicUnits_None,
    .constraints = {
        .minimumValue = 0,
        .maximumValue = UINT8_MAX,
        .stepValue = 0,
        .validValues = NULL,
        .validValuesRanges = NULL
    },
    .callbacks = {
        .handleRead = HAPHandlePairingPairingFeaturesRead,
        .handleWrite = NULL
    }
};

static const HAPTLV8Characteristic pairingPairingPairingsCharacteristic = {
    .format = kHAPCharacteristicFormat_TLV8,
    .iid = kIID_PairingPairingPairings,
    .characteristicType = &kHAPCharacteristicType_PairingPairings,
    .debugDescription = kHAPCharacteristicDebugDescription_PairingPairings,
    .manufacturerDescription = NULL,
    .properties = {
        .readable = true,
        .writable = true,
        .supportsEventNotification = false,
        .hidden = false,
        .requiresTimedWrite = false,
        .supportsAuthorizationData = false,
        .ip = {
            .controlPoint = true
        },
        .ble = {
            .supportsBroadcastNotification = false,
            .supportsDisconnectedNotification = false,
            .readableWithoutSecurity = false,
            .writableWithoutSecurity = false
        }
    },
    .callbacks = {
        .handleRead = HAPHandlePairingPairingPairingsRead,
        .handleWrite = HAPHandlePairingPairingPairingsWrite
    }
};

const HAPService pairingService = {
    .iid = kIID_Pairing,
    .serviceType = &kHAPServiceType_Pairing,
    .debugDescription = kHAPServiceDebugDescription_Pairing,
    .name = NULL,
    .properties = {
        .primaryService = false,
        .hidden = false,
        .ble = {
            .supportsConfiguration = false
        }
    },
    .linkedServices = NULL,
    .characteristics = (const HAPCharacteristic *const []) {
        &pairingPairSetupCharacteristic,
        &pairingPairVerifyCharacteristic,
        &pairingPairingFeaturesCharacteristic,
        &pairingPairingPairingsCharacteristic,
        NULL
    }
};

//----------------------------------------------------------------------------------------------------------------------

/**
 * The 'Service Signature' characteristic of the first Camera RTP Stream Management service.
 */
const HAPDataCharacteristic cameraRTPStreamManagement0ServiceSignatureCharacteristic = {
    .format = kHAPCharacteristicFormat_Data,
    .iid = kIID_CameraRTPStreamManagement0ServiceSignature,
    .characteristicType = &kHAPCharacteristicType_ServiceSignature,
    .debugDescription = kHAPCharacteristicDebugDescription_ServiceSignature,
    .manufacturerDescription = NULL,
    .properties = {
        .readable = true,
        .writable = false,
        .supportsEventNotification = false,
        .hidden = false,
        .requiresTimedWrite = false,
        .supportsAuthorizationData = false,
        .ip = {
            .controlPoint = true
        },
        .ble = {
            .supportsBroadcastNotification = false,
            .supportsDisconnectedNotification = false,
            .readableWithoutSecurity = false,
            .writableWithoutSecurity = false
        }
    },
    .constraints = {
        .maxLength = 2097152
    },
    .callbacks = {
        .handleRead = HAPHandleServiceSignatureRead,
        .handleWrite = NULL
    }
};

/**
 * The 'Streaming Status' characteristic of the first Camera RTP Stream Management service.
 */
const HAPTLV8Characteristic cameraRTPStreamManagement0StreamingStatusCharacteristic = {
    .format = kHAPCharacteristicFormat_TLV8,
    .iid = kIID_CameraRTPStreamManagement0StreamingStatus,
    .characteristicType = &kHAPCharacteristicType_StreamingStatus,
    .debugDescription = kHAPCharacteristicDebugDescription_StreamingStatus,
    .manufacturerDescription = NULL,
    .properties = {
        .readable = true,
        .writable = false,
        .supportsEventNotification = true,
        .hidden = false,
        .requiresTimedWrite = false,
        .supportsAuthorizationData = false,
        .ip = {
            .controlPoint = false,
            .supportsWriteResponse = false
        },
        .ble = {
            .supportsBroadcastNotification = false,
            .supportsDisconnectedNotification = false,
            .readableWithoutSecurity = false,
            .writableWithoutSecurity = false
        }
    },
    .callbacks = {
        .handleRead = HAPHandleCameraRTPStreamManagementStreamingStatusRead,
        .handleWrite = NULL
    }
};

/**
 * The 'Supported Video Stream Configuration' characteristic of the first Camera RTP Stream Management service.
 */
const HAPTLV8Characteristic cameraRTPStreamManagement0SupportedVideoStreamConfigurationCharacteristic = {
    .format = kHAPCharacteristicFormat_TLV8,
    .iid = kIID_CameraRTPStreamManagement0SupportedVideoStreamConfiguration,
    .characteristicType = &kHAPCharacteristicType_SupportedVideoStreamConfiguration,
    .debugDescription = kHAPCharacteristicDebugDescription_SupportedVideoStreamConfiguration,
    .manufacturerDescription = NULL,
    .properties = {
        .readable = true,
        .writable = false,
        .supportsEventNotification = false,
        .hidden = false,
        .requiresTimedWrite = false,
        .supportsAuthorizationData = false,
        .ip = {
            .controlPoint = false,
            .supportsWriteResponse = false
        },
        .ble = {
            .supportsBroadcastNotification = false,
            .supportsDisconnectedNotification = false,
            .readableWithoutSecurity = false,
            .writableWithoutSecurity = false
        }
    },
    .callbacks = {
        .handleRead = HAPHandleCameraRTPStreamManagementSupportedVideoStreamConfigurationRead,
        .handleWrite = NULL
    }
};

/**
 * The 'Supported Audio Stream Configuration' characteristic of the first Camera RTP Stream Management service.
 */
const HAPTLV8Characteristic cameraRTPStreamManagement0SupportedAudioStreamConfigurationCharacteristic = {
    .format = kHAPCharacteristicFormat_TLV8,
    .iid = kIID_CameraRTPStreamManagement0SupportedAudioStreamConfiguration,
    .characteristicType = &kHAPCharacteristicType_SupportedAudioStreamConfiguration,
    .debugDescription = kHAPCharacteristicDebugDescription_SupportedAudioStreamConfiguration,
    .manufacturerDescription = NULL,
    .properties = {
        .readable = true,
        .writable = false,
        .supportsEventNotification = false,
        .hidden = false,
        .requiresTimedWrite = false,
        .supportsAuthorizationData = false,
        .ip = {
            .controlPoint = false,
            .supportsWriteResponse = false
        },
        .ble = {
            .supportsBroadcastNotification = false,
            .supportsDisconnectedNotification = false,
            .readableWithoutSecurity = false,
            .writableWithoutSecurity = false
        }
    },
    .callbacks = {
        .handleRead = HAPHandleCameraRTPStreamManagementSupportedAudioStreamConfigurationRead,
        .handleWrite = NULL
    }
};

/**
 * The 'Supported RTP Configuration' characteristic of the first Camera RTP Stream Management service.
 */
const HAPTLV8Characteristic cameraRTPStreamManagement0SupportedRTPConfigurationCharacteristic = {
    .format = kHAPCharacteristicFormat_TLV8,
    .iid = kIID_CameraRTPStreamManagement0SupportedRTPConfiguration,
    .characteristicType = &kHAPCharacteristicType_SupportedRTPConfiguration,
    .debugDescription = kHAPCharacteristicDebugDescription_SupportedRTPConfiguration,
    .manufacturerDescription = NULL,
    .properties = {
        .readable = true,
        .writable = false,
        .supportsEventNotification = false,
        .hidden = false,
        .requiresTimedWrite = false,
        .supportsAuthorizationData = false,
        .ip = {
            .controlPoint = false,
            .supportsWriteResponse = false
        },
        .ble = {
            .supportsBroadcastNotification = false,
            .supportsDisconnectedNotification = false,
            .readableWithoutSecurity = false,
            .writableWithoutSecurity = false
        }
    },
    .callbacks = {
        .handleRead = HAPHandleCameraRTPStreamManagementSupportedRTPConfigurationRead,
        .handleWrite = NULL
    }
};

/**
 * The 'Setup Endpoints' characteristic of the first Camera RTP Stream Management service.
 */
const HAPTLV8Characteristic cameraRTPStreamManagement0SetupEndpointsCharacteristic = {
    .format = kHAPCharacteristicFormat_TLV8,
    .iid = kIID_CameraRTPStreamManagement0SetupEndpoints,
    .characteristicType = &kHAPCharacteristicType_SetupEndpoints,
    .debugDescription = kHAPCharacteristicDebugDescription_SetupEndpoints,
    .manufacturerDescription = NULL,
    .properties = {
        .readable = true,
        .writable = true,
        .supportsEventNotification = false,
        .hidden = false,
        .requiresTimedWrite = false,
        .supportsAuthorizationData = false,
        .ip = {
            .controlPoint = true
        },
        .ble = {
            .supportsBroadcastNotification = false,
            .supportsDisconnectedNotification = false,
            .readableWithoutSecurity = false,
            .writableWithoutSecurity = false
        }
    },
    .callbacks = {
        .handleRead = HAPHandleCameraRTPStreamManagementSetupEndpointsRead,
        .handleWrite = HAPHandleCameraRTPStreamManagementSetupEndpointsWrite
    }
};

/**
 * The 'Selected RTP Stream Configuration' characteristic of the first Camera RTP Stream Management service.
 */
const HAPTLV8Characteristic cameraRTPStreamManagement0SelectedRTPStreamConfigurationCharacteristic = {
    .format = kHAPCharacteristicFormat_TLV8,
    .iid = kIID_CameraRTPStreamManagement0SelectedRTPStreamConfiguration,
    .characteristicType = &kHAPCharacteristicType_SelectedRTPStreamConfiguration,
    .debugDescription = kHAPCharacteristicDebugDescription_SelectedRTPStreamConfiguration,
    .manufacturerDescription = NULL,
    .properties = {
        .readable = true,
        .writable = true,
        .supportsEventNotification = false,
        .hidden = false,
        .requiresTimedWrite = false,
        .supportsAuthorizationData = false,
        .ip = {
            .controlPoint = true
        },
        .ble = {
            .supportsBroadcastNotification = false,
            .supportsDisconnectedNotification = false,
            .readableWithoutSecurity = false,
            .writableWithoutSecurity = false
        }
    },
    .callbacks = {
        .handleRead = HAPHandleCameraRTPStreamManagementSelectedRTPStreamConfigurationRead,
        .handleWrite = HAPHandleCameraRTPStreamManagementSelectedRTPStreamConfigurationWrite
    }
};

/**
 * The first Camera RTP Stream Management service that contains the characteristics for the first IP camera stream.
 */
const HAPService cameraRTPStreamManagement0Service = {
    .iid = kIID_CameraRTPStreamManagement0,
    .serviceType = &kHAPServiceType_CameraRTPStreamManagement,
    .debugDescription = kHAPServiceDebugDescription_CameraRTPStreamManagement,
    .name = NULL,
    .properties = {
        .primaryService = true,
        .hidden = false,
        .ble = {
            .supportsConfiguration = false
        }
    },
    .linkedServices = NULL,
    .characteristics = (const HAPCharacteristic *const[]) {
        &cameraRTPStreamManagement0ServiceSignatureCharacteristic,
        &cameraRTPStreamManagement0StreamingStatusCharacteristic,
        &cameraRTPStreamManagement0SupportedVideoStreamConfigurationCharacteristic,
        &cameraRTPStreamManagement0SupportedAudioStreamConfigurationCharacteristic,
        &cameraRTPStreamManagement0SupportedRTPConfigurationCharacteristic,
        &cameraRTPStreamManagement0SetupEndpointsCharacteristic,
        &cameraRTPStreamManagement0SelectedRTPStreamConfigurationCharacteristic,
        NULL
    }
};

//----------------------------------------------------------------------------------------------------------------------

/**
 * The 'Streaming Status' characteristic of the second Camera RTP Stream Management service.
 */
const HAPTLV8Characteristic cameraRTPStreamManagement1StreamingStatusCharacteristic = {
    .format = kHAPCharacteristicFormat_TLV8,
    .iid = kIID_CameraRTPStreamManagement1StreamingStatus,
    .characteristicType = &kHAPCharacteristicType_StreamingStatus,
    .debugDescription = kHAPCharacteristicDebugDescription_StreamingStatus,
    .manufacturerDescription = NULL,
    .properties = {
        .readable = true,
        .writable = false,
        .supportsEventNotification = true,
        .hidden = false,
        .requiresTimedWrite = false,
        .supportsAuthorizationData = false,
        .ip = {
            .controlPoint = false,
            .supportsWriteResponse = false
        },
        .ble = {
            .supportsBroadcastNotification = false,
            .supportsDisconnectedNotification = false,
            .readableWithoutSecurity = false,
            .writableWithoutSecurity = false
        }
    },
    .callbacks = {
        .handleRead = HAPHandleCameraRTPStreamManagementStreamingStatusRead,
        .handleWrite = NULL
    }
};

/**
 * The 'Supported Video Stream Configuration' characteristic of the second Camera RTP Stream Management service.
 */
const HAPTLV8Characteristic cameraRTPStreamManagement1SupportedVideoStreamConfigurationCharacteristic = {
    .format = kHAPCharacteristicFormat_TLV8,
    .iid = kIID_CameraRTPStreamManagement1SupportedVideoStreamConfiguration,
    .characteristicType = &kHAPCharacteristicType_SupportedVideoStreamConfiguration,
    .debugDescription = kHAPCharacteristicDebugDescription_SupportedVideoStreamConfiguration,
    .manufacturerDescription = NULL,
    .properties = {
        .readable = true,
        .writable = false,
        .supportsEventNotification = false,
        .hidden = false,
        .requiresTimedWrite = false,
        .supportsAuthorizationData = false,
        .ip = {
            .controlPoint = false,
            .supportsWriteResponse = false
        },
        .ble = {
            .supportsBroadcastNotification = false,
            .supportsDisconnectedNotification = false,
            .readableWithoutSecurity = false,
            .writableWithoutSecurity = false
        }
    },
    .callbacks = {
        .handleRead = HAPHandleCameraRTPStreamManagementSupportedVideoStreamConfigurationRead,
        .handleWrite = NULL
    }
};

/**
 * The 'Supported Audio Stream Configuration' characteristic of the second Camera RTP Stream Management service.
 */
const HAPTLV8Characteristic cameraRTPStreamManagement1SupportedAudioStreamConfigurationCharacteristic = {
    .format = kHAPCharacteristicFormat_TLV8,
    .iid = kIID_CameraRTPStreamManagement1SupportedAudioStreamConfiguration,
    .characteristicType = &kHAPCharacteristicType_SupportedAudioStreamConfiguration,
    .debugDescription = kHAPCharacteristicDebugDescription_SupportedAudioStreamConfiguration,
    .manufacturerDescription = NULL,
    .properties = {
        .readable = true,
        .writable = false,
        .supportsEventNotification = false,
        .hidden = false,
        .requiresTimedWrite = false,
        .supportsAuthorizationData = false,
        .ip = {
            .controlPoint = false,
            .supportsWriteResponse = false
        },
        .ble = {
            .supportsBroadcastNotification = false,
            .supportsDisconnectedNotification = false,
            .readableWithoutSecurity = false,
            .writableWithoutSecurity = false
        }
    },
    .callbacks = {
        .handleRead = HAPHandleCameraRTPStreamManagementSupportedAudioStreamConfigurationRead,
        .handleWrite = NULL
    }
};

/**
 * The 'Supported RTP Configuration' characteristic of the second Camera RTP Stream Management service.
 */
const HAPTLV8Characteristic cameraRTPStreamManagement1SupportedRTPConfigurationCharacteristic = {
    .format = kHAPCharacteristicFormat_TLV8,
    .iid = kIID_CameraRTPStreamManagement1SupportedRTPConfiguration,
    .characteristicType = &kHAPCharacteristicType_SupportedRTPConfiguration,
    .debugDescription = kHAPCharacteristicDebugDescription_SupportedRTPConfiguration,
    .manufacturerDescription = NULL,
    .properties = {
        .readable = true,
        .writable = false,
        .supportsEventNotification = false,
        .hidden = false,
        .requiresTimedWrite = false,
        .supportsAuthorizationData = false,
        .ip = {
            .controlPoint = false,
            .supportsWriteResponse = false
        },
        .ble = {
            .supportsBroadcastNotification = false,
            .supportsDisconnectedNotification = false,
            .readableWithoutSecurity = false,
            .writableWithoutSecurity = false
        }
    },
    .callbacks = {
        .handleRead = HAPHandleCameraRTPStreamManagementSupportedRTPConfigurationRead,
        .handleWrite = NULL
    }
};

/**
 * The 'Setup Endpoints' characteristic of the second Camera RTP Stream Management service.
 */
const HAPTLV8Characteristic cameraRTPStreamManagement1SetupEndpointsCharacteristic = {
    .format = kHAPCharacteristicFormat_TLV8,
    .iid = kIID_CameraRTPStreamManagement1SetupEndpoints,
    .characteristicType = &kHAPCharacteristicType_SetupEndpoints,
    .debugDescription = kHAPCharacteristicDebugDescription_SetupEndpoints,
    .manufacturerDescription = NULL,
    .properties = {
        .readable = true,
        .writable = true,
        .supportsEventNotification = false,
        .hidden = false,
        .requiresTimedWrite = false,
        .supportsAuthorizationData = false,
        .ip = {
            .controlPoint = true
        },
        .ble = {
            .supportsBroadcastNotification = false,
            .supportsDisconnectedNotification = false,
            .readableWithoutSecurity = false,
            .writableWithoutSecurity = false
        }
    },
    .callbacks = {
        .handleRead = HAPHandleCameraRTPStreamManagementSetupEndpointsRead,
        .handleWrite = HAPHandleCameraRTPStreamManagementSetupEndpointsWrite
    }
};

/**
 * The 'Selected RTP Stream Configuration' characteristic of the second Camera RTP Stream Management service.
 */
const HAPTLV8Characteristic cameraRTPStreamManagement1SelectedRTPStreamConfigurationCharacteristic = {
    .format = kHAPCharacteristicFormat_TLV8,
    .iid = kIID_CameraRTPStreamManagement1SelectedRTPStreamConfiguration,
    .characteristicType = &kHAPCharacteristicType_SelectedRTPStreamConfiguration,
    .debugDescription = kHAPCharacteristicDebugDescription_SelectedRTPStreamConfiguration,
    .manufacturerDescription = NULL,
    .properties = {
        .readable = true,
        .writable = true,
        .supportsEventNotification = false,
        .hidden = false,
        .requiresTimedWrite = false,
        .supportsAuthorizationData = false,
        .ip = {
            .controlPoint = true
        },
        .ble = {
            .supportsBroadcastNotification = false,
            .supportsDisconnectedNotification = false,
            .readableWithoutSecurity = false,
            .writableWithoutSecurity = false
        }
    },
    .callbacks = {
        .handleRead = HAPHandleCameraRTPStreamManagementSelectedRTPStreamConfigurationRead,
        .handleWrite = HAPHandleCameraRTPStreamManagementSelectedRTPStreamConfigurationWrite
    }
};

/**
 * The second Camera RTP Stream Management service that contains the characteristics for the second IP camera stream.
 */
const HAPService cameraRTPStreamManagement1Service = {
    .iid = kIID_CameraRTPStreamManagement1,
    .serviceType = &kHAPServiceType_CameraRTPStreamManagement,
    .debugDescription = kHAPServiceDebugDescription_CameraRTPStreamManagement,
    .name = NULL,
    .properties = {
        .primaryService = false,
        .hidden = false,
        .ble = {
            .supportsConfiguration = false
        }
    },
    .linkedServices = NULL,
    .characteristics = (const HAPCharacteristic *const[]) {
        &cameraRTPStreamManagement1StreamingStatusCharacteristic,
        &cameraRTPStreamManagement1SupportedVideoStreamConfigurationCharacteristic,
        &cameraRTPStreamManagement1SupportedAudioStreamConfigurationCharacteristic,
        &cameraRTPStreamManagement1SupportedRTPConfigurationCharacteristic,
        &cameraRTPStreamManagement1SetupEndpointsCharacteristic,
        &cameraRTPStreamManagement1SelectedRTPStreamConfigurationCharacteristic,
        NULL
    }
};

//----------------------------------------------------------------------------------------------------------------------

/**
 * The 'Name' characteristic of the Microphone service.
 */
static const HAPStringCharacteristic microphoneNameCharacteristic = {
    .format = kHAPCharacteristicFormat_String,
    .iid = kIID_MicrophoneName,
    .characteristicType = &kHAPCharacteristicType_Name,
    .debugDescription = kHAPCharacteristicDebugDescription_Name,
    .manufacturerDescription = NULL,
    .properties = {
        .readable = true,
        .writable = false,
        .supportsEventNotification = false,
        .hidden = false,
        .requiresTimedWrite = false,
        .supportsAuthorizationData = false,
        .ip = {
            .controlPoint = false,
            .supportsWriteResponse = false
        },
        .ble = {
            .supportsBroadcastNotification = false,
            .supportsDisconnectedNotification = false,
            .readableWithoutSecurity = false,
            .writableWithoutSecurity = false
        }
    },
    .constraints = {
        .maxLength = 64
    },
    .callbacks = {
        .handleRead = HAPHandleNameRead,
        .handleWrite = NULL
    }
};

/**
 * The 'Mute' characteristic of the Microphone service.
 */
const HAPBoolCharacteristic microphoneMuteCharacteristic = {
    .format = kHAPCharacteristicFormat_Bool,
    .iid = kIID_MicrophoneMute,
    .characteristicType = &kHAPCharacteristicType_Mute,
    .debugDescription = kHAPCharacteristicDebugDescription_Mute,
    .manufacturerDescription = NULL,
    .properties = {
        .readable = true,
        .writable = true,
        .supportsEventNotification = true,
        .hidden = false,
        .requiresTimedWrite = false,
        .supportsAuthorizationData = false,
        .ip = {
            .controlPoint = false,
            .supportsWriteResponse = false
        },
        .ble = {
            .supportsBroadcastNotification = false,
            .supportsDisconnectedNotification = false,
            .readableWithoutSecurity = false,
            .writableWithoutSecurity = false
        }
    },
    .callbacks = {
        .handleRead = HandleMicrophoneMuteRead,
        .handleWrite = HandleMicrophoneMuteWrite
    }
};

/**
 * The 'Volume' characteristic of the Microphone service.
 */
const HAPUInt8Characteristic microphoneVolumeCharacteristic = {
    .format = kHAPCharacteristicFormat_UInt8,
    .iid = kIID_MicrophoneVolume,
    .characteristicType = &kHAPCharacteristicType_Volume,
    .debugDescription = kHAPCharacteristicDebugDescription_Volume,
    .manufacturerDescription = NULL,
    .properties = {
        .readable = true,
        .writable = true,
        .supportsEventNotification = true,
        .hidden = false,
        .requiresTimedWrite = false,
        .supportsAuthorizationData = false,
        .ip = {
            .controlPoint = false,
            .supportsWriteResponse = false
        },
        .ble = {
            .supportsBroadcastNotification = false,
            .supportsDisconnectedNotification = false,
            .readableWithoutSecurity = false,
            .writableWithoutSecurity = false
        }
    },
    .units = kHAPCharacteristicUnits_Percentage,
    .constraints = {
        .minimumValue = 0,
        .maximumValue = 100,
        .stepValue = 1,
        .validValues = NULL,
        .validValuesRanges = NULL
    },
    .callbacks = {
        .handleRead = HandleMicrophoneVolumeRead,
        .handleWrite = HandleMicrophoneVolumeWrite
    }
};

/**
 * The Microphone service that contains the 'Mute' and the 'Volume' characteristics.
 */
const HAPService microphoneService = {
    .iid = kIID_Microphone,
    .serviceType = &kHAPServiceType_Microphone,
    .debugDescription = kHAPServiceDebugDescription_Microphone,
    .name = "Microphone",
    .properties = {
        .primaryService = false,
        .hidden = false,
        .ble = {
            .supportsConfiguration = false
        }
    },
    .linkedServices = NULL,
    .characteristics = (const HAPCharacteristic *const[]) {
        &microphoneNameCharacteristic,
        &microphoneMuteCharacteristic,
        &microphoneVolumeCharacteristic,
        NULL
    }
};

//----------------------------------------------------------------------------------------------------------------------

/**
 * The 'Name' characteristic of the Speaker service.
 */
static const HAPStringCharacteristic speakerNameCharacteristic = {
    .format = kHAPCharacteristicFormat_String,
    .iid = kIID_SpeakerName,
    .characteristicType = &kHAPCharacteristicType_Name,
    .debugDescription = kHAPCharacteristicDebugDescription_Name,
    .manufacturerDescription = NULL,
    .properties = {
        .readable = true,
        .writable = false,
        .supportsEventNotification = false,
        .hidden = false,
        .requiresTimedWrite = false,
        .supportsAuthorizationData = false,
        .ip = {
            .controlPoint = false,
            .supportsWriteResponse = false
        },
        .ble = {
            .supportsBroadcastNotification = false,
            .supportsDisconnectedNotification = false,
            .readableWithoutSecurity = false,
            .writableWithoutSecurity = false
        }
    },
    .constraints = {
        .maxLength = 64
    },
    .callbacks = {
        .handleRead = HAPHandleNameRead,
        .handleWrite = NULL
    }
};

/**
 * The 'Mute' characteristic of the Speaker service.
 */
const HAPBoolCharacteristic speakerMuteCharacteristic = {
    .format = kHAPCharacteristicFormat_Bool,
    .iid = kIID_SpeakerMute,
    .characteristicType = &kHAPCharacteristicType_Mute,
    .debugDescription = kHAPCharacteristicDebugDescription_Mute,
    .manufacturerDescription = NULL,
    .properties = {
        .readable = true,
        .writable = true,
        .supportsEventNotification = true,
        .hidden = false,
        .requiresTimedWrite = false,
        .supportsAuthorizationData = false,
        .ip = {
            .controlPoint = false,
            .supportsWriteResponse = false
        },
        .ble = {
            .supportsBroadcastNotification = false,
            .supportsDisconnectedNotification = false,
            .readableWithoutSecurity = false,
            .writableWithoutSecurity = false
        }
    },
    .callbacks = {
        .handleRead = HandleSpeakerMuteRead,
        .handleWrite = HandleSpeakerMuteWrite
    }
};

/**
 * The 'Volume' characteristic of the Speaker service.
 */
const HAPUInt8Characteristic speakerVolumeCharacteristic = {
    .format = kHAPCharacteristicFormat_UInt8,
    .iid = kIID_SpeakerVolume,
    .characteristicType = &kHAPCharacteristicType_Volume,
    .debugDescription = kHAPCharacteristicDebugDescription_Volume,
    .manufacturerDescription = NULL,
    .properties = {
        .readable = true,
        .writable = true,
        .supportsEventNotification = true,
        .hidden = false,
        .requiresTimedWrite = false,
        .supportsAuthorizationData = false,
        .ip = {
            .controlPoint = false,
            .supportsWriteResponse = false
        },
        .ble = {
            .supportsBroadcastNotification = false,
            .supportsDisconnectedNotification = false,
            .readableWithoutSecurity = false,
            .writableWithoutSecurity = false
        }
    },
    .units = kHAPCharacteristicUnits_Percentage,
    .constraints = {
        .minimumValue = 0,
        .maximumValue = 100,
        .stepValue = 1,
        .validValues = NULL,
        .validValuesRanges = NULL
    },
    .callbacks = {
        .handleRead = HandleSpeakerVolumeRead,
        .handleWrite = HandleSpeakerVolumeWrite
    }
};

/**
 * The Speaker service that contains the 'Mute' and the 'Volume' characteristics.
 */
const HAPService speakerService = {
    .iid = kIID_Speaker,
    .serviceType = &kHAPServiceType_Speaker,
    .debugDescription = kHAPServiceDebugDescription_Speaker,
    .name = "Speaker",
    .properties = {
        .primaryService = false,
        .hidden = false,
        .ble = {
            .supportsConfiguration = false
        }
    },
    .linkedServices = NULL,
    .characteristics = (const HAPCharacteristic *const[]) {
        &speakerNameCharacteristic,
        &speakerMuteCharacteristic,
        &speakerVolumeCharacteristic,
        NULL
    }
};
