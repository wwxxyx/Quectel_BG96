//
//  CaptureConfigurationTypes.h
//  ATSMacApp
//
//  Created by Cody Brimhall on 7/24/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

/*!
 @header     CaptureConfigurationTypes
 @discussion This header file defines the different type enumerations and string
             constants used by the capture configuration manager. Types cover 
             the accessory connector, iAP protocol version, data transport, 
             capture configuration manager state, and various errors.
 */

/*!
 @const      CaptureConfigurationDefaultModeKey
 @abstract   The key for the default capture configuration mode value stored in 
             the standard user defaults store.
 */
extern NSString *const CaptureConfigurationDefaultModeKey;

/*!
 @typedef    CaptureConfigurationState
 @abstract   The different states the capture configuration assistant can be in 
             during the process of collecting capture document configuration 
             information from the user.
 @constant   CaptureConfigurationUnknownState
             An unknown or unspecified state.
 @constant   CaptureConfigurationStartState
             The capture configuration assistant has not started collecting
             information.
 @constant   CaptureConfigurationDeviceTypeState
             The capture configuration assistant is collecting the device type
             to start capture.
 @constant   CaptureConfigurationCaptureDeviceState
             The capture configuration assistant is collecting capture 
             device information.
 @constant   CaptureConfigurationTransportState
             The capture configuration assistant is collecting transport layer 
             information.
 @constant   CaptureConfigurationUSBModeState
             The capture configuration assistant is collecting USB mode 
             information.
 @constant   CaptureConfigurationBeagleState
             The capture configuration assistant is collecting information about 
             which Beagle device to use.
 @constant   CaptureConfigurationProtocolState
             The capture configuration assistant is collecting iAP protocol 
             version information.
 @constant   CaptureConfigurationVbusCurrentState
             The capture configuration assistant is collecting Vbus current 
             information.
 @constant   CaptureConfigurationBluetoothCaptureConfigurationState
             The capture configuration assistant is collecting the comprobe
             information.
 @constant   CaptureConfigurationBluetoothAppleDeviceState
             The capture configuration assistant is collecting the apple
             device information.
 @constant   CaptureConfigurationBluetoothAccessoryState
             The capture configuration assistant is collecting the accessory
             device information.
 @constant   CaptureConfigurationBluetoothPairingState
             The capture configuration assistant is collecting the pairing mode
 @constant   CaptureConfigurationFinishState
             The capture configuration assistant has finished collecting 
             configuration information.
 */
typedef enum {
    CaptureConfigurationUnknownState = 0,
    CaptureConfigurationStartState,
    CaptureConfigurationCaptureDeviceState,
    CaptureConfigurationTransportState,
    CaptureConfigurationUSBModeState,
    CaptureConfigurationBeagleState,
    CaptureConfigurationBluetoothCaptureConfigurationState,
    CaptureConfigurationBluetoothAppleDeviceState,
    CaptureConfigurationBluetoothAccessoryState,
    CaptureConfigurationBluetoothPairingState,
    CaptureConfigurationProtocolState,
    CaptureConfigurationSettingsState,
    CaptureConfigurationVbusCurrentState,
    CaptureConfigurationFinishState,
} CaptureConfigurationState;

typedef NS_OPTIONS(NSUInteger, CaptureConfigurationConnectorMask) {
    // No connector; unknown or unspecified conector type.
    CaptureConfigurationConnectorMaskNoConnector                = 0,
    // The OG 30-pin connector.
    CaptureConfigurationConnectorMask30PinConnector             = 1 << 0,   // 1
    // The fading-away 8-pin connector.
    CaptureConfigurationConnectorMask8PinConnector              = 1 << 1,   // 2
    // The logical Lighting Audio connector (not a real-world thing).
    CaptureConfigurationConnectorMaskLightningAudioConnector    = 1 << 2,   // 4
    // The ignored Smart Connector.
    CaptureConfigurationConnectorMaskSmartConnector             = 1 << 3,   // 8
    // The Bluetooth Radio.
    CaptureConfigurationConnectorMaskBluetoothRadioConnector    = 1 << 4,   // 16
    // The Wi-Fi Radio.
    CaptureConfigurationConnectorMaskWiFiRadioConnector         = 1 << 5,   // 32
};

/*!
 @typedef    CaptureConfigurationConnector_Deprecated
 @abstract   The different types of connectors that an accessory can use to 
             communicate with an Apple device.
 @constant   CaptureConfigurationNoConnector_Deprecated
             An unknown or unspecified conector type.
 @constant   CaptureConfiguration30PinConnector_Deprecated
             The original 30-pin connector.
 @constant   CaptureConfiguration8PinConnector_Deprecated
             The 8-pin connector.
 @constant   CaptureConfigurationBluetoothRadioConnector_Deprecated
             A Bluetooth radio.
 @constant   CaptureConfigurationWiFiRadioConnector_Deprecated
             A WiFi radio.
 @constant   CaptureConfigurationSmartConnector_Deprecated
             A Smart connector.
 @constant   CaptureConfigurationLightningAudioConnector_Deprecated
             A Lightning Audio connector.
 */
typedef NS_ENUM(NSUInteger, CaptureConfigurationConnector_Deprecated) {
    CaptureConfigurationNoConnector_Deprecated                 = 0,
    CaptureConfiguration30PinConnector_Deprecated              = 1,
    CaptureConfiguration8PinConnector_Deprecated               = 2,
    CaptureConfigurationBluetoothRadioConnector_Deprecated     = 3,
    CaptureConfigurationWiFiRadioConnector_Deprecated          = 4,
    CaptureConfigurationSmartConnector_Deprecated              = 5,
    CaptureConfigurationLightningAudioConnector_Deprecated     = 6,
};

/**
 The different protocols the accessory can use to communicate with an Apple device.
 */
typedef NS_OPTIONS(NSUInteger, CaptureConfigurationProtocolMask) {
    /** None */
    CaptureConfigurationProtocolMaskNoProtocol = 0,
    /** iAP 1 protocol */
    CaptureConfigurationProtocolMaskiAP1Protocol = (1 << 0),
    /** iAP 2 protocol */
    CaptureConfigurationProtocolMaskiAP2Protocol = (1 << 1),
    /** CarPlay protocol */
    CaptureConfigurationProtocolMaskCarPlayProtocol = (1 << 2),
    /** AirPlay protocol */
    CaptureConfigurationProtocolMaskAirPlayProtocol = (1 << 3),
};

/*!
 @function  CaptureConfigurationiAPStringFromProtocolMask
 @abstract  Returns a string for the selected iAP versions.
 */
NSString *CaptureConfigurationiAPStringFromProtocolMask(CaptureConfigurationProtocolMask protocolMask);

/*!
 The different kinds of transport available for use for iAP.
 */
typedef NS_OPTIONS(NSUInteger, CaptureConfigurationTransportMask) {
    /// An unknown or unspecified transport.
    CaptureConfigurationTransportMaskNone = 0,
    /// UART transport for iAP.
    CaptureConfigurationTransportMaskUART = 1 << 0,
    /// USB transport for iAP.
    CaptureConfigurationTransportMaskUSB = 1 << 1,
    /// Bluetooth transport for iAP.
    CaptureConfigurationTransportMaskBluetooth = 1 << 2,
    /// WiFi transport for iAP, CarPlay, and AirPlay 2.
    CaptureConfigurationTransportMaskWiFi = 1 << 3,
    /// MikeyBus/Lightning Audio transport for iAP2/iAP1/HID.
    CaptureConfigurationTransportMaskLightningAudio = 1 << 4,
};

/*!
 @typedef    CaptureConfigurationTransport
 @abstract   The different kinds of transport available for use for iAP.
 @constant   CaptureConfigurationUnknownTransport
 An unknown or unspecified transport.
 @constant   CaptureConfigurationUARTTransport
 UART transport for iAP.
 @constant   CaptureConfigurationUSBTransport
 USB transport for iAP.
 @constant   CaptureConfigurationBluetoothTransport
 Bluetooth transport for iAP.
 @constant   CaptureConfigurationWiFiTransport
 WiFi transport for iAp and CarPlay.
 @constant   CaptureConfigurationLightningAudioTransport
 MikeyBus transport for MikeyBus.
 */
typedef NS_ENUM(NSUInteger, CaptureConfigurationTransport_Deprecated) {
    CaptureConfigurationUnknownTransport_Deprecated         = 0,
    CaptureConfigurationUARTTransport_Deprecated            = 1,
    CaptureConfigurationUSBTransport_Deprecated             = 2,
    CaptureConfigurationBluetoothTransport_Deprecated       = 3,
    CaptureConfigurationWiFiTransport_Deprecated            = 4,
    CaptureConfigurationLightningAudioTransport_Deprecated  = 5,
};

/*!
 @typedef    CaptureConfigurationDeviceType
 @abstract   The different kinds of capture devices that can be used to capture 
             data.
 @constant   CaptureConfigurationUnknownDeviceType
             An unknown or unspecified capture device type.
 @constant   CaptureConfigurationPhysicalDeviceType
             A physical piece of hardware attached to the capture setup.
 @constant   CaptureConfigurationEmulatedDeviceType
             An emulated capture device, such as the ones used to support
             Packetizer.
 @constant   CaptureConfigurationVirtualDeviceType
             A virtual capture device, such as the ones created by iAPCommander.
 */
typedef enum {
    CaptureConfigurationUnknownDeviceType = 0,
    CaptureConfigurationPhysicalDeviceType,
    CaptureConfigurationEmulatedDeviceType,
    CaptureConfigurationVirtualDeviceType,
} CaptureConfigurationDeviceType;

/*!
 @typedef    CaptureConfigurationVbusCurrent
 @abstract   The different amperages of electrical current that an accessory can 
             provide to an Apple device.
 @constant   CaptureConfigurationUnknownVbusCurrent
             An unknown or unspecified electrical current.
 @constant   CaptureConfiguration1AVbusCurrent
             The accessory provides 1 A of electrical current to the Apple
             device.
 @constant   CaptureConfiguration2Point1AVbusCurrent
             The accessory provides 2.1 A of electrical current to the Apple 
             device.
 @constant   CaptureConfiguration2Point4AVbusCurrent
             The accessory provides 2.4 A of electrical current to the Apple
             device.
 */
typedef enum {
    CaptureConfigurationUnknownVbusCurrent = 0,
    CaptureConfiguration1AVbusCurrent,
    CaptureConfiguration2Point1AVbusCurrent,
    CaptureConfiguration2Point4AVbusCurrent,
} CaptureConfigurationVbusCurrent;

/*!
 @const      CaptureConfigurationErrorDomain
 @abstract   The domain for errors that occur within capture configuration operations.
 @discussion When possible, CaptureConfigurationErrorDomain errors will provide 
             a full localized description and a reference to the underlying error.
 */
extern NSString *const CaptureConfigurationErrorDomain;

/*!
 @typedef    CaptureConfigurationError
 @abstract   The different types of errors that can be encountered while 
             executing capture configuration operations.
 @constant   CaptureConfigurationUnknownError
             An unknown error.
 @constant   CaptureConfigurationInsufficientInformationError
             The capture configuration manager was not provided with all of the 
             information needed to configure a capture document.
 @constant   CaptureConfigurationValidationError
             The capture configuration assistant settings are invalid.
 */
typedef enum {
    CaptureConfigurationUnknownError = 0,
    CaptureConfigurationInsufficientInformationError,
    CaptureConfigurationValidationError,
} CaptureConfigurationError;
