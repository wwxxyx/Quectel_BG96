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

#include "App.h"
#include "DB.h"
#include "Remote.h"
#include "Siri.h"

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

#define kIID_TargetControlManagement                                                ((uint64_t) 0x0030)
#define kIID_TargetControlManagementServiceSignature                                ((uint64_t) 0x0031)
#define kIID_TargetControlSupportedConfiguration                                    ((uint64_t) 0x0032)
#define kIID_TargetControlList                                                      ((uint64_t) 0x0033)

#define kIID_TargetControl                                                          ((uint64_t) 0x0040)
#define kIID_TargetControlServiceSignature                                          ((uint64_t) 0x0041)
#define kIID_ActiveIdentifier                                                       ((uint64_t) 0x0042)
#define kIID_Active                                                                 ((uint64_t) 0x0043)
#define kIID_ButtonEvent                                                            ((uint64_t) 0x0044)
#define kIID_Name                                                                   ((uint64_t) 0x0045)

#define kIID_DataStreamTransportManagement                                          ((uint64_t) 0x0050)
#define kIID_DataStreamTransportManagementServiceSignature                          ((uint64_t) 0x0051)
#define kIID_DataStreamTransportManagementSupportedDataStreamTransportConfiguration ((uint64_t) 0x0052)
#define kIID_DataStreamTransportManagementSetupDataStreamTransport                  ((uint64_t) 0x0053)
#define kIID_DataStreamTransportManagementVersion                                   ((uint64_t) 0x0054)

#define kIID_AudioStreamManagement                                                  ((uint64_t) 0x0060)
#define kIID_AudioStreamManagementServiceSignature                                  ((uint64_t) 0x0061)
#define kIID_AudioStreamManagementSupportedAudioStreamConfiguration                 ((uint64_t) 0x0062)
#define kIID_AudioStreamManagementSelectedAudioStreamConfiguration                  ((uint64_t) 0x0063)

#define kIID_Siri                                                                   ((uint64_t) 0x0070)
#define kIID_SiriServiceSignature                                                   ((uint64_t) 0x0071)
#define kIID_SiriInputType                                                          ((uint64_t) 0x0072)

HAP_STATIC_ASSERT(kAttributeCount ==
    9 + 3 + 5 + (4 + 6 + 5 + 4 + 3) * kAppState_NumRemotes, AttributeCount_mismatch);

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

static const HAPDataCharacteristic targetControlManagementServiceSignatureCharacteristic = {
    .format = kHAPCharacteristicFormat_Data,
    .iid = kIID_TargetControlManagementServiceSignature,
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

const HAPTLV8Characteristic targetControlSupportedConfigurationCharacteristic = {
    .format = kHAPCharacteristicFormat_TLV8,
    .iid = kIID_TargetControlSupportedConfiguration,
    .characteristicType = &kHAPCharacteristicType_TargetControlSupportedConfiguration,
    .debugDescription = kHAPCharacteristicDebugDescription_TargetControlSupportedConfiguration,
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
        },
    },
    .callbacks = {
        .handleRead = RemoteHandleTargetControlSupportedConfigurationRead,
        .handleWrite = NULL
    }
};

const HAPTLV8Characteristic targetControlListCharacteristic = {
    .format = kHAPCharacteristicFormat_TLV8,
    .iid = kIID_TargetControlList,
    .characteristicType = &kHAPCharacteristicType_TargetControlList,
    .debugDescription = kHAPCharacteristicDebugDescription_TargetControlList,
    .manufacturerDescription = NULL,
    .properties = {
        .readable = true,
        .writable = true,
        .supportsEventNotification = false,
        .hidden = false,
        .requiresAdminPermissions = true,
        .requiresTimedWrite = false,
        .supportsAuthorizationData = false,
        .ip = {
            .controlPoint = true,
            .supportsWriteResponse = true
        },
        .ble = {
            .supportsBroadcastNotification = false,
            .supportsDisconnectedNotification = false,
            .readableWithoutSecurity = false,
            .writableWithoutSecurity = false
        }
    },
    .callbacks = {
        .handleRead = RemoteHandleTargetControlListRead,
        .handleWrite = RemoteHandleTargetControlListWrite
    }
};

const HAPService targetControlManagementService = {
    .iid = kIID_TargetControlManagement,
    .serviceType = &kHAPServiceType_TargetControlManagement,
    .debugDescription = kHAPServiceDebugDescription_TargetControlManagement,
    .properties = {
        .primaryService = true,
        .hidden = false,
        .ble = {
            .supportsConfiguration = false
        }
    },
    .linkedServices = (const uint16_t[]) {
        kIID_TargetControl,
        kIID_DataStreamTransportManagement,
        kIID_AudioStreamManagement,
        kIID_Siri,
        0
    },
    .characteristics = (const HAPCharacteristic *const[]) {
        &targetControlManagementServiceSignatureCharacteristic,
        &targetControlSupportedConfigurationCharacteristic,
        &targetControlListCharacteristic,
        NULL
    }
};

//----------------------------------------------------------------------------------------------------------------------

static const HAPDataCharacteristic targetControlServiceSignatureCharacteristic = {
    .format = kHAPCharacteristicFormat_Data,
    .iid = kIID_TargetControlServiceSignature,
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

const HAPUInt32Characteristic activeIdentifierCharacteristic = {
    .format = kHAPCharacteristicFormat_UInt32,
    .iid = kIID_ActiveIdentifier,
    .characteristicType = &kHAPCharacteristicType_ActiveIdentifier,
    .debugDescription = kHAPCharacteristicDebugDescription_ActiveIdentifier,
    .manufacturerDescription = NULL,
    .properties = {
        .readable = true,
        .writable = false,
        .supportsEventNotification = true,
        .hidden = false,
        .requiresTimedWrite = false,
        .ip = {
            .controlPoint = false,
        },
        .ble = {
            .supportsBroadcastNotification = false,
            .supportsDisconnectedNotification = false,
            .readableWithoutSecurity = false,
            .writableWithoutSecurity = false
        }
    },
    .callbacks = {
        .handleRead = RemoteHandleActiveIdentifierRead,
        .handleWrite = NULL
    },
    .constraints = {
        .minimumValue = 0,
        .maximumValue = UINT32_MAX,
        .stepValue = 1
    }
};

const HAPUInt8Characteristic activeCharacteristic = {
    .format = kHAPCharacteristicFormat_UInt8,
    .iid = kIID_Active,
    .characteristicType = &kHAPCharacteristicType_Active,
    .debugDescription = kHAPCharacteristicDebugDescription_Active,
    .manufacturerDescription = NULL,
    .properties = {
        .readable = true,
        .writable = true,
        .supportsEventNotification = true,
        .hidden = false,
        .requiresTimedWrite = false,
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
        .handleRead = RemoteHandleActiveRead,
        .handleWrite = RemoteHandleActiveWrite
    },
    .constraints = {
        .minimumValue = 0,
        .maximumValue = 1,
        .stepValue = 1
    }
};

const HAPTLV8Characteristic buttonEventCharacteristic = {
    .format = kHAPCharacteristicFormat_TLV8,
    .iid = kIID_ButtonEvent,
    .characteristicType = &kHAPCharacteristicType_ButtonEvent,
    .debugDescription = kHAPCharacteristicDebugDescription_ButtonEvent,
    .manufacturerDescription = NULL,
    .properties = {
        .readable = true,
        .writable = false,
        .supportsEventNotification = true,
        .hidden = false,
        .requiresAdminPermissions = true,
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
        .handleRead = RemoteHandleButtonEventRead,
        .handleWrite = NULL,
        .handleSubscribe = RemoteHandleButtonEventSubscribe,
        .handleUnsubscribe = RemoteHandleButtonEventUnsubscribe
    }
};

const HAPStringCharacteristic nameCharacteristic = {
    .format = kHAPCharacteristicFormat_String,
    .iid = kIID_Name,
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

const HAPService targetControlService = {
    .iid = kIID_TargetControl,
    .serviceType = &kHAPServiceType_TargetControl,
    .debugDescription = kHAPServiceDebugDescription_TargetControl,
    .name = "Remote",
    .properties = {
        .primaryService = false,
        .hidden = false,
        .ble = {
            .supportsConfiguration = false
        }
    },
    .linkedServices = NULL,
    .characteristics = (const HAPCharacteristic *const[]) {
        &targetControlServiceSignatureCharacteristic,
        &activeIdentifierCharacteristic,
        &activeCharacteristic,
        &buttonEventCharacteristic,
        &nameCharacteristic,
        NULL
    }
};

//----------------------------------------------------------------------------------------------------------------------

static const HAPDataCharacteristic dataStreamTransportManagementServiceSignatureCharacteristic = {
    .format = kHAPCharacteristicFormat_Data,
    .iid = kIID_DataStreamTransportManagementServiceSignature,
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

static const HAPTLV8Characteristic supportedDataStreamTransportConfigurationCharacteristic = {
    .format = kHAPCharacteristicFormat_TLV8,
    .iid = kIID_DataStreamTransportManagementSupportedDataStreamTransportConfiguration,
    .characteristicType = &kHAPCharacteristicType_SupportedDataStreamTransportConfiguration,
    .debugDescription = kHAPCharacteristicDebugDescription_SupportedDataStreamTransportConfiguration,
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
        .handleRead = HAPHandleDataStreamTransportManagementSupportedDataStreamTransportConfigurationRead,
        .handleWrite = NULL
    }
};

static const HAPTLV8Characteristic setupDataStreamTransportCharacteristic = {
    .format = kHAPCharacteristicFormat_TLV8,
    .iid = kIID_DataStreamTransportManagementSetupDataStreamTransport,
    .characteristicType = &kHAPCharacteristicType_SetupDataStreamTransport,
    .debugDescription = kHAPCharacteristicDebugDescription_SetupDataStreamTransport,
    .manufacturerDescription = NULL,
    .properties = {
        .readable = true,
        .writable = true,
        .supportsEventNotification = false,
        .hidden = false,
        .requiresTimedWrite = false,
        .supportsAuthorizationData = false,
        .ip = {
            .controlPoint = true,
            .supportsWriteResponse = true
        },
        .ble = {
            .supportsBroadcastNotification = false,
            .supportsDisconnectedNotification = false,
            .readableWithoutSecurity = false,
            .writableWithoutSecurity = false
        }
    },
    .callbacks = {
        .handleRead = HAPHandleDataStreamTransportManagementSetupDataStreamTransportRead,
        .handleWrite = HAPHandleDataStreamTransportManagementSetupDataStreamTransportWrite
    }
};

static const HAPStringCharacteristic dataStreamTransportManagementVersionCharacteristic = {
    .format = kHAPCharacteristicFormat_String,
    .iid = kIID_DataStreamTransportManagementVersion,
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
        .handleRead = HAPHandleDataStreamTransportManagementVersionRead,
        .handleWrite = NULL
    }
};

const HAPService dataStreamTransportManagementService = {
    .iid = kIID_DataStreamTransportManagement,
    .serviceType = &kHAPServiceType_DataStreamTransportManagement,
    .debugDescription = kHAPServiceDebugDescription_DataStreamTransportManagement,
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
        &dataStreamTransportManagementServiceSignatureCharacteristic,
        &supportedDataStreamTransportConfigurationCharacteristic,
        &setupDataStreamTransportCharacteristic,
        &dataStreamTransportManagementVersionCharacteristic,
        NULL
    }
};

//----------------------------------------------------------------------------------------------------------------------

static const HAPDataCharacteristic audioStreamManagementServiceSignatureCharacteristic = {
    .format = kHAPCharacteristicFormat_Data,
    .iid = kIID_AudioStreamManagementServiceSignature,
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

static const HAPTLV8Characteristic supportedAudioStreamConfigurationCharacteristic = {
    .format = kHAPCharacteristicFormat_TLV8,
    .iid = kIID_AudioStreamManagementSupportedAudioStreamConfiguration,
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
        .handleRead = HandleAudioStreamManagementSupportedAudioStreamConfigurationRead,
        .handleWrite = NULL
    }
};

static const HAPTLV8Characteristic selectedAudioStreamConfigurationCharacteristic = {
    .format = kHAPCharacteristicFormat_TLV8,
    .iid = kIID_AudioStreamManagementSelectedAudioStreamConfiguration,
    .characteristicType = &kHAPCharacteristicType_SelectedAudioStreamConfiguration,
    .debugDescription = kHAPCharacteristicDebugDescription_SelectedAudioStreamConfiguration,
    .manufacturerDescription = NULL,
    .properties = {
        .readable = true,
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
        .handleRead = HandleAudioStreamManagementSelectedAudioStreamConfigurationRead,
        .handleWrite = HandleAudioStreamManagementSelectedAudioStreamConfigurationWrite
    }
};

const HAPService audioStreamManagementService = {
    .iid = kIID_AudioStreamManagement,
    .serviceType = &kHAPServiceType_AudioStreamManagement,
    .debugDescription = kHAPServiceDebugDescription_AudioStreamManagement,
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
        &audioStreamManagementServiceSignatureCharacteristic,
        &supportedAudioStreamConfigurationCharacteristic,
        &selectedAudioStreamConfigurationCharacteristic,
        NULL
    }
};

//----------------------------------------------------------------------------------------------------------------------

static const HAPDataCharacteristic siriServiceSignatureCharacteristic = {
    .format = kHAPCharacteristicFormat_Data,
    .iid = kIID_SiriServiceSignature,
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

static const HAPUInt8Characteristic siriInputTypeCharacteristic = {
    .format = kHAPCharacteristicFormat_UInt8,
    .iid = kIID_SiriInputType,
    .characteristicType = &kHAPCharacteristicType_SiriInputType,
    .debugDescription = kHAPCharacteristicDebugDescription_SiriInputType,
    .manufacturerDescription = NULL,
    .properties = {
        .readable = true,
        .writable = false,
        .supportsEventNotification = false,
        .hidden = false,
        .requiresTimedWrite = false,
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
        .handleRead = HandleSiriInputTypeRead,
        .handleWrite = NULL
    }
};

const HAPService siriService = {
    .iid = kIID_Siri,
    .serviceType = &kHAPServiceType_Siri,
    .debugDescription = kHAPServiceDebugDescription_Siri,
    .name = NULL,
    .properties = {
        .primaryService = false,
        .hidden = false,
        .ble = {
            .supportsConfiguration = false
        }
    },
    .linkedServices = (const uint16_t[]) {
        kIID_AudioStreamManagement,
        kIID_DataStreamTransportManagement,
        0
    },
    .characteristics = (const HAPCharacteristic *const []) {
        &siriServiceSignatureCharacteristic,
        &siriInputTypeCharacteristic,
        NULL
    }
};
