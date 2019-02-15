//
//  ProductPlanTypes.h
//  ATSMacApp
//
//  Created by Kip Nicol on 7/22/11.
//  Copyright (c) 2011 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

// String constants used in initializing the ProductPlan from a dictionary
extern NSString *const PPVersionNumber;
extern NSString *const PPData;
extern NSString *const PPProductID;
extern NSString *const PPAccessorySKU;
extern NSString *const PPAccessoryName;
extern NSString *const PPLightningModuleFunctionalVariant;
extern NSString *const PPValidationFailureMessage;
extern NSString *const PPConnectorType;
extern NSString *const PPiAPVersionType;
extern NSString *const PPUSBVbusPowerSourceType;
extern NSString *const PPResistorType;
extern NSString *const PPRFCertificationBitmask;
extern NSString *const PPTransportTestParameters;
extern NSString *const PPTransportType;
extern NSString *const PPAppleDeviceTestParameters;
extern NSString *const PPAppleDeviceType;
extern NSString *const PPFeatures;
extern NSString *const PPSelfCertTests;

/** Various dock connector types. */
typedef NS_ENUM(uint8_t, ConnectorType) {
    /** No dock connector (eg bluetooth accessories). */
    ConnectorTypeNone = 1,

    /** The 30-pin dock connector. */
    ConnectorType30Pin = 2,

    /** The 8-pin connector. */
    ConnectorType8Pin = 3,

    /** The Bluetooth connector. */
    ConnectorTypeBluetooth = 4,
};

/** iAP versions. */
typedef NS_ENUM(uint8_t, iAPVersionType) {
    /** iAP 1 version. */
    iAPVersionTypeiAP1 = 1,

    /** iAP 2 version. */
    iAPVersionTypeiAP2 = 2,

    /** Both iAP1 and iAP2 are supported. */
    iAPVersionTypeiAP1AndiAP2 = 3,

    /** No iAP version used. */
    iAPVersionTypeNone = 255,
};

/** Vbus charger types. */
typedef NS_ENUM(uint8_t, USBVbusPowerSourceType) {
    USBVbusPowerSourceTypeNone = 1,
    USBVbusPowerSourceType500mA = 2,
    USBVbusPowerSourceType1A = 3,
    USBVbusPowerSourceType2Point1A = 4,
    USBVbusPowerSourceType2Point4A = 5,
};

/** Resistor types. */
typedef NS_ENUM(uint8_t, ResistorType) {
    ResistorTypeNone = 1,
    ResistorType28k = 2,
    ResistorType191k = 3,
    ResistorType255kOrNone = 4,
    ResistorType549k = 5,
    ResistorType1M = 6,
    ResistorType360k = 7,
};

/** Transport types. */
typedef NS_ENUM(uint8_t, TransportType) {
    TransportTypeNone = 1,
    TransportTypeUART = 2,
    TransportTypeAppleDeviceIsUSBHost = 3,
    TransportTypeAppleDeviceIsUSBDevice = 4,
    TransportTypeBluetooth = 6,
    TransportTypeWiFi = 7,
};

/** Apple device types. */
typedef NS_ENUM(uint8_t, AppleDeviceType) {
    AppleDeviceTypeiPad2 = 1,
    AppleDeviceTypeiPad = 2,
    AppleDeviceTypeiPhone4 = 3,
    AppleDeviceTypeiPhone3GS = 4,
    AppleDeviceTypeiPhone3G = 5,
    AppleDeviceTypeiPhone = 6,
    AppleDeviceTypeiPodNano6G = 7,
    AppleDeviceTypeiPodNano5G = 8,
    AppleDeviceTypeiPodNano4G = 9,
    AppleDeviceTypeiPodNano3G = 10,
    AppleDeviceTypeiPodNano2G = 11,
    AppleDeviceTypeiPodNano1G = 12,
    AppleDeviceTypeiPodMini = 13,
    AppleDeviceTypeiPodTouch4G = 14,
    AppleDeviceTypeiPodTouch3G = 15,
    AppleDeviceTypeiPodTouch2G = 16,
    AppleDeviceTypeiPodTouch1G = 17,
    AppleDeviceTypeiPodClassic = 18,
    AppleDeviceTypeiPodWithVideo = 19,
    AppleDeviceTypeiPodPhoto = 20,
    AppleDeviceTypeiPod4G = 21,
    AppleDeviceTypeiPhone4S = 22,
    AppleDeviceTypeiPad3rdGeneration = 23,
    AppleDeviceTypeiPodNano7G = 24,
    AppleDeviceTypeiPhone5 = 25,
    AppleDeviceTypeiPodTouch5G = 26,
    AppleDeviceTypeiPad4thGeneration = 27,
    AppleDeviceTypeiPadMini = 28,
    AppleDeviceTypeiPhone5s = 29,
    AppleDeviceTypeiPhone5c = 30,
    AppleDeviceTypeiPadAir = 31,
    AppleDeviceTypeiPadMini2 = 32,
    AppleDeviceTypeiPhone6 = 33,
    AppleDeviceTypeiPhone6Plus = 34,
    AppleDeviceTypeiPadAir2 = 35,
    AppleDeviceTypeiPadMini3 = 36,
    AppleDeviceTypeiPodTouch6G = 37,
    AppleDeviceTypeiPhone6s = 38,
    AppleDeviceTypeiPhone6sPlus = 39,
    AppleDeviceTypeiPadMini4 = 40,
    AppleDeviceTypeiPadPro12dot9Inch1stGeneration = 41,
    AppleDeviceTypeiPhoneSE = 42,
    AppleDeviceTypeiPadPro9dot7Inch = 43,
    AppleDeviceTypeAppleTV4G = 44,
    AppleDeviceTypeiPhone7 = 45,
    AppleDeviceTypeiPhone7Plus = 46,
    AppleDeviceTypeiPad5thGeneration = 47,
    AppleDeviceTypeiPadPro12dot9Inch2ndGeneration = 48,
    AppleDeviceTypeiPadPro10dot5Inch = 49,
    AppleDeviceTypeiPhone8 = 50,
    AppleDeviceTypeiPhone8Plus = 51,
    AppleDeviceTypeiPhoneX = 52,
    AppleDeviceTypeiPad6thGeneration = 53,
    AppleDeviceTypeiPhoneXR = 54,
    AppleDeviceTypeiPhoneXS = 55,
    AppleDeviceTypeiPhoneXSMax = 56,
    AppleDeviceTypeiPadPro12dot9Inch3rdGeneration = 57,
    AppleDeviceTypeiPadPro11Inch = 58,
};

/** Self-cert test types. */
typedef NS_ENUM(uint8_t, SelfCertTestType) {
    // iAP1 Tests
    SelfCertTestTypeAutomaticTestsAppleDeviceOnly = 1,
    SelfCertTestTypeAutomaticTestsAccessoryOnly = 2,
    SelfCertTestTypeSimpleRemote = 3,
    SelfCertTestTypeRemoteUI = 4,
    SelfCertTestTypeStorageLingo = 5,
    SelfCertTestTypeCardioEquipment = 6,
    SelfCertTestTypeLineOut = 7,
    SelfCertTestTypeVideoOut = 8,
    SelfCertTestTypeiOSSDK = 9,
    SelfCertTestTypeLocationLingo = 10,
    SelfCertTestTypeiPodOut = 11,
    SelfCertTestTypeVoiceOver = 12,
    SelfCertTestTypeWiFiNetworkLoginSharing = 13,
    SelfCertTestTypeAssistiveTouch = 14,
    SelfCertTestTypeDisplayRemote = 15,
    SelfCertTestTypeAutomaticTestsAppleDeviceAndAccessory = 59,

    // iAP2 Tests
    SelfCertTestTypeiAP2AutomaticTestsWithAppleDevice = 16,
    SelfCertTestTypeiAP2AutomaticTestsAccessoryOnly = 17,
    SelfCertTestTypeiAP2AppLaunch = 18,
    SelfCertTestTypeiAP2AssistiveTouch = 19,
    SelfCertTestTypeiAP2BluetoothConnection = 20,
    SelfCertTestTypeiAP2DeviceAuthentication = 21,
    SelfCertTestTypeiAP2DeviceNotification = 22,
    SelfCertTestTypeiAP2PowerDevicePowered = 23,
    SelfCertTestTypeiAP2DigitalAudio = 24,
    SelfCertTestTypeiAP2DigitalAudioDeviceMode = 25,
    SelfCertTestTypeiAP2DigitalAudioHostMode = 26,
    SelfCertTestTypeiAP2EASessioniAP2 = 27,
    SelfCertTestTypeiAP2EANativeTransport = 28,
    SelfCertTestTypeiAP2HIDSessioniAP2 = 29,
    SelfCertTestTypeiAP2HIDSessioniAP2WithHIDFunctionAssistiveSwitchControl = 30,
    SelfCertTestTypeiAP2HIDSessioniAP2withHIDFunctionAssistiveTouchPointer = 31,
    SelfCertTestTypeiAP2HIDSessioniAP2WithHIDFunctionGameController = 32,
    SelfCertTestTypeiAP2HIDSessioniAP2WithHIDFunctionHeadphone = 34,
    SelfCertTestTypeiAP2HIDSessioniAP2withHIDFunctionKeyboard = 35,
    SelfCertTestTypeiAP2HIDSessioniAP2withHIDFunctionMediaPlaybackRemote = 36,
    SelfCertTestTypeiAP2HIDNativeTransport = 37,
    SelfCertTestTypeiAP2HIDNativeTransportWithHIDFunctionAssistiveSwitchControl = 38,
    SelfCertTestTypeiAP2HIDNativeTransportWithHIDFunctionAssistiveTouchPointer = 39,
    SelfCertTestTypeiAP2HIDNativeTransportWithHIDFunctionGameController = 40,
    SelfCertTestTypeiAP2HIDNativeTransportWithHIDFunctionHeadphone = 42,
    SelfCertTestTypeiAP2HIDNativeTransportWithHIDFunctionKeyboard = 43,
    SelfCertTestTypeiAP2HIDNativeTransportWithHIDFunctionMediaPlaybackRemote = 44,
    SelfCertTestTypeiAP2Location = 45,
    SelfCertTestTypeiAP2MediaLibraryInformation = 46,
    SelfCertTestTypeiAP2MediaLibraryUpdates = 47,
    SelfCertTestTypeiAP2MediaLibraryPlayback = 48,
    SelfCertTestTypeiAP2NowPlaying = 49,
    SelfCertTestTypeiAP2Telephony = 50,
    SelfCertTestTypeiAP2VehicleStatus = 51,
    SelfCertTestTypeiAP2VoiceOver = 52,
    SelfCertTestTypeiAP2Wifi = 53,
    SelfCertTestTypeiAP2MultipleEASessions = 54,
    SelfCertTestTypeiAP2CallStateUpdates = 55,
    SelfCertTestTypeiAP2CommunicationsUpdates = 56,
    SelfCertTestTypeiAP2CallControls = 57,
    SelfCertTestTypeiAP2ListUpdates = 58,
    SelfCertTestTypeiAP2DisconnectAccessory = 60,
    SelfCertTestTypeBluetoothDelay = 61,
};

/** Old-Self-cert test types. */
typedef NS_ENUM(uint8_t, OldSelfCertTestType) {
    
    // iAP2 Tests
    OldSelfCertTestTypeiAP2AllEncompassing = 16,
    OldSelfCertTestTypeiAP2AutomaticTestsWithAppleDevice = 17, // Automatic test with Apple device
    OldSelfCertTestTypeiAP2AutomaticTestsAccessoryOnly = 18,
};

/** Feature types. */
typedef NS_ENUM(uint8_t, FeatureType) {
    FeatureTypeUnknown = 0,
    
    // iAP1 Features
    FeatureTypeVoiceOverEvents = 1, //  // USB Device Mode Audio
    FeatureTypeAssistiveTouch = 2, // AssistiveTouch
    FeatureTypeCommunicationWithiOSApplications = 3, // Communicates with iOS Application
    FeatureTypeEqualizer = 4, // Equalizer
    FeatureTypeDisplayRemote = 5, // Display Remote
    FeatureTypeExtendedInterfaceMode = 6, // Extended Interface Mode
    FeatureTypeiPodOutMode = 7, // iPod Out Mode
    FeatureTypeiTunesTagging = 8, // iTunes Tagging
    FeatureTypeLocation = 9, // Location
    FeatureTypeRFTuner = 10, // RF Tuner
    FeatureTypeSimpleRemote = 11, // Simple Remote
    FeatureTypeSports = 12, // Sports
    FeatureTypeUSBDeviceModeAudio = 13,  // USB Device Mode Audio
    FeatureTypeStorage = 14,    // Derived feature
    FeatureTypeAnalogAudioOut = 15, // VM: I don't see these anymore in MFi
    FeatureTypeAnalogVideoOut = 16, // VM: I don't see these anymore in MFi
    FeatureTypeWiFiNetworkLoginSharing = 17, // Wi-Fi Network Login Sharing
    FeatureTypeUSBHostModeLingo = 18,   // Derived feature

    // iAP2 Features
    FeatureTypeiAP2AppLaunch = 50, //    App Launch
    FeatureTypeiAP2AssistiveTouch = 51, //    Assistive Touch
    FeatureTypeiAP2BluetoothConnection = 52, //    Bluetooth Connection
    FeatureTypeiAP2DeviceAuthentication = 53, //    Device Authentication (Deprecated)
    FeatureTypeiAP2DigitalAudioHostMode = 54, //    Digital Audio USB Host Mode
    FeatureTypeiAP2DigitalAudioDeviceMode = 55, //    Digital Audio USB Device Mode
    FeatureTypeiAP2EASessioniAP2 = 56, //    EA iAP2 Session
    FeatureTypeiAP2EANativeTransport = 57, //    EA Native Transport
    FeatureTypeiAP2HIDSessioniAP2 = 58, //    HID over iAP2 Control Session
    FeatureTypeiAP2HIDNativeTransport = 59, //    HID over USB Native Transport
    FeatureTypeiAP2Location = 60, //    Location
    FeatureTypeiAP2MediaLibraryInformation = 61, //    Media Library Information
    FeatureTypeiAP2MediaLibraryUpdates = 62, //    Media Library Updates
    FeatureTypeiAP2MediaLibraryPlayback = 63, //    Media Library Playback
    FeatureTypeiAP2NowPlaying = 64, //    Now Playing
    FeatureTypeiAP2PowerDevicePowered = 65, //    Device Powered
    FeatureTypeiAP2VoiceOver = 66, //    VoiceOver
    FeatureTypeiAP2WiFi = 67, //    Wi-Fi Information Sharing
    FeatureTypeiAP2HIDSessioniAP2WithHIDFunctionKeyboard = 68, //    HID Keyboard over iAP2 Control Session
    FeatureTypeiAP2HIDSessioniAP2WithHIDFunctionMediaPlaybackRemote = 69, //    HID Media Playback Remote over iAP2 Control Session
    FeatureTypeiAP2HIDSessioniAP2WithHIDFunctionAssistiveTouchPointer = 70, //    HID AssistiveTouch Pointer over iAP2 Control Session
    FeatureTypeiAP2HIDNativeTransportWithHIDFunctionKeyboard = 71, //    HID Keyboard over USB Native Transport
    FeatureTypeiAP2HIDNativeTransportWithHIDFunctionMediaPlaybackRemote = 72, //    HID Media Playback Remote over USB Native Transport
    FeatureTypeiAP2HIDNativeTransportWithHIDFunctionAssistiveTouchPointer = 73, //    HID AssistiveTouch Pointer over USB Native Transport
    FeatureTypeiAP2DeviceNotifications = 74, //    Device Notifications
    //FeatureTypeiAP2HIDSessioniAP2WithHIDFunctionGameControllerStandard = 75, // HID Game Controller Standard
    FeatureTypeiAP2HIDSessioniAP2WithHIDFunctionGameController = 76, //    HID Game Controller Extended over iAP2 Control Session
    FeatureTypeiAP2HIDSessioniAP2WithHIDFunctionAssistiveSwitchControl = 77, //    HID Assistive Switch Control over iAP2 Control Session
    // FeatureTypeiAP2HIDNativeTransportWithHIDFunctionGameControllerStandard = 78, //    HID Game Controller Standard over USB Native Transport
    FeatureTypeiAP2HIDNativeTransportWithHIDFunctionGameController = 79, //    HID Game Controller Extended over USB Native Transport
    FeatureTypeiAP2HIDNativeTransportWithHIDFunctionAssistiveSwitchControl = 80,  //    HID Assistive Switch Control over USB Native Transport
    FeatureTypeiAP2Telephony = 81, //    Telephony
    FeatureTypeiAP2VehicleStatus = 82, //    Vehicle status
    FeatureTypeiAP2HIDSessioniAP2WithHIDFunctionHeadphone = 83, //    HID Headset over iAP2 Control Session
    FeatureTypeiAP2HIDNativeTransportWithHIDFunctionHeadphone = 84, //    HID Headset over USB Native Transport
    FeatureTypeiAP2MultipleEASessions = 85, //    Multiple iAP2 EA sessions
    FeatureTypeiAP2CallControls = 86, //    Call Controls
    FeatureTypeiAP2CallStateUpdates = 87, //    Call State Updates
    FeatureTypeiAP2CommunicationsUpdates = 88, //    Communications Updates
    FeatureTypeiAP2ListUpdates = 89, //    List Updates
    FeatureTypeOutOfBandBluetoothPairing = 90, // MFi does not support this feature; The value has no extrensic meaning.
    // FeatureTypeHIDoverSmartConnectorModuleI2CInterface = 91, //    HID over Smart Connector Module I2C interface; currently not supported in CA. The value 91 has no extrensic meaning.
    
    // Meta-Features - Shown in the UI to group and control related features
    FeatureTypeiAP2DigitalAudio = 100,
    FeatureTypeiAP2Communications = 101,
};

/** Various Lightning module functional variants. */
typedef NS_ENUM(uint8_t, LightningModuleFunctionalVariant) {
    LightningModuleFunctionalVariantNone = 1,
    LightningModuleFunctionalVariantC10A = 2,
    LightningModuleFunctionalVariantC10B = 3,
    LightningModuleFunctionalVariantC10C = 4,
    LightningModuleFunctionalVariantC10D = 5,
    LightningModuleFunctionalVariantC11A = 6,
    LightningModuleFunctionalVariantC11B = 7,
    LightningModuleFunctionalVariantC11C = 8,
    LightningModuleFunctionalVariantC11D = 9,
    LightningModuleFunctionalVariantB141 = 10,
    LightningMoudleFunctionalVariantB139B164 = 11,
    LightningModuleFunctionalVariantC12A = 12,
    LightningModuleFunctionalVariantC12B = 13,
    LightningModuleFunctionalVariantC12C = 14,
    LightningModuleFunctionalVariantC12D = 15,
    LightningModuleFunctionalVariantC48B = 16,
    LightningModuleFunctionalVariantC48A = 17,
    LightningModuleFunctionalVariantC48C = 18,
    LightningModuleFunctionalVariantC68A = 19,
    LightningModuleFunctionalVariantC68B = 20,
    LightningModuleFunctionalVariantC68C = 21,
    LightningModuleFunctionalVariantC10F = 22,
    LightningModuleFunctionalVariantC11F = 23,
    LightningModuleFunctionalVariantC12F = 24,
    LightningModuleFunctionalVariantC68F = 25,
    LightningModuleFunctionalVariantC12H = 26,
    LightningModuleFunctionalVariantC68H = 27,
};

/** Question types. */
typedef NS_ENUM(uint8_t, QuestionType) {
    QuestionTypeTransport = 1,
    QuestionTypeCharger = 2,
    QuestionTypeFeatures = 3
};

/*!
 Returns the feature that supports the specified message.
 
 @param messageID The ID for the specified message.
 @return The feature that supports the given message.
 */
FeatureType const featureTypeFromMessageID(UInt16 messageID);

/*!
 Returns a list of all iAP2 SelfCertTest types.
 
 @return The array that contains the SelfCertTest types.
 */
NSArray<NSNumber *> * ATSSelfCertTestTypesForProtocolVersionType(iAPVersionType type);
