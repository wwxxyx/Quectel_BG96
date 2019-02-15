//
//  CaptureConfiguration.h
//  ATSMacApp
//
//  Created by Cody Brimhall on 7/24/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//
// https://gitlab.sd.apple.com/ats/CaptureKitDocumentation

#import <Foundation/Foundation.h>
#import <CaptureKit/CaptureConfigurationTypes.h>

NS_ASSUME_NONNULL_BEGIN

@class BeagleUSBCaptureDevice;
@class BluetoothDevice;

/** Version of ATS when Multi Transport Wireless Capture was introduced */
extern NSString *const kCaptureConfigurationMultiTransportWirelessCaptureIntroducedVersion;

#pragma mark - keys backing the configuration defaults
/** The key for the CaptureConfiguration connector property. */
extern NSString *const CaptureConfigurationConnectorMaskKey;

/** The key for the CaptureConfiguration protocolMask property. */
extern NSString *const CaptureConfigurationProtocolMaskKey;

/** The key for the CaptureConfiguration transportMask property. */
extern NSString *const CaptureConfigurationTransportMaskKey;

/** The key for the CaptureConfiguration usingBeagle property. */
extern NSString *const CaptureConfigurationUsingBeagleKey;

/** The key for the CaptureConfiguration usingCaptureDevice property. */
extern NSString *const CaptureConfigurationUsingCaptureDeviceKey;

/** The key for the CaptureConfiguration usingComprobe property. */
extern NSString *const CaptureConfigurationUsingComprobeKey;

/** The key for the CaptureConfiguration bluetoothAppleDeviceAddress property. */
extern NSString *const CaptureConfigurationBluetoothAppleDeviceAddressKey;

/** The key for the CaptureConfiguration bluetoothAccessoryAddress property. */
extern NSString *const CaptureConfigurationBluetoothAccessoryAddressKey;

/** The key for the CaptureConfiguration bluetoothLink property. */
extern NSString *const CaptureConfigurationBluetoothLinkKey;

/** The key for the CaptureConfiguration usingBluetoothTransport property. */
extern NSString *const CaptureConfigurationUsingBluetoothTransportKey;

#pragma mark - Capture Configuration Data Model
/** Encapsulates all the data necessary to configure a new caputure document. */
@interface CaptureConfiguration : NSObject <NSCopying>

/** A flag indicating whether or not the trace is configured for capturing data but with no connector (might be useful for imported data traces). */
@property (nonatomic, getter=isUsingNoConnector) BOOL usingNoConnector;

/** A flag indicating whether or not the trace is configured for capturing data from an 30-pin connector. */
@property (nonatomic, getter=isUsing30PinConnector) BOOL using30PinConnector;

/** A flag indicating whether or not the trace is configured for capturing data from an 8-pin connector. */
@property (nonatomic, getter=isUsing8PinConnector) BOOL using8PinConnector;

/** A flag indicating whether or not the trace is configured for capturing data from a Smart Connector. */
@property (nonatomic, getter=isUsingSmartConnector) BOOL usingSmartConnector;

/** A flag indicating whether or not the trace is configured for capturing data from a Lightning Audio connector. */
@property (nonatomic, getter=isUsingLightningAudioConnector) BOOL usingLightningAudioConnector;

/** A flag indicating whether or not the trace is configured for capturing data from an Bluetooth Radio connector. */
@property (nonatomic, getter=isUsingBluetoothRadioConnector) BOOL usingBluetoothRadioConnector;

/** A flag indicating whether or not the trace is configured for capturing data from an Wi-Fi Radio connector. */
@property (nonatomic, getter=isUsingWiFiRadioConnector) BOOL usingWiFiRadioConnector;

/** The connector types used by the accessory to communicate with an Apple device. */
@property (nonatomic, assign) CaptureConfigurationConnectorMask connectorMask;

/** The version of the iAP protocol used by the accessory to communicate with an Apple device. */
@property (nonatomic, assign) CaptureConfigurationProtocolMask protocolMask;

/** The amperage of electrical current the accessory provides to an Apple device. */
@property (nonatomic, assign) CaptureConfigurationVbusCurrent VbusCurrent;

/** The kind of transport used by the accessory for sending and receiving iAP messages. */
@property (nonatomic, assign) CaptureConfigurationTransportMask transportMask;

/** A flag indicating whether or not the Apple device acts as a USB host when using USB for transport. */
@property (nonatomic, assign, getter=isAppleDeviceActingAsUSBHost) BOOL appleDeviceActingAsUSBHost;

/** A flag indicating whether or not a Beagle USB protocol analyzer will be in use during the capture. */
@property (nonatomic, assign, getter=isUsingBeagle) BOOL usingBeagle;

/** A flag indicating whether or not a Bluetooth Comprobe box will be in use during the capture. */
@property (nonatomic, getter=isUsingComprobe) BOOL usingComprobe;

/** A flag indicating whether or not a saved bluetooth capture configuration will be in use during the capture. */
@property (nonatomic, assign, getter=isUsingSavedBluetoothCaptureConfiguration) BOOL usingSavedBluetoothCaptureConfiguration;

/** A flag indicating whether or not a CaptureDevice will be in use during the capture.
 
 @note This represents ATS Box only.
 */
@property (nonatomic, assign, getter=isUsingCaptureDevice) BOOL usingCaptureDevice;

/** A Legacy flag indicating whether or not a CaptureDevice will be in use during the capture. */
@property (nonatomic, readonly) BOOL isUsingATSBox_Deprecated;

/**
 The serial number of the Apple Device that will be in use during the capture, or nil if no data is being captured directly from the device.
 */
@property (nonatomic, copy, nullable) NSString *deviceUDID;

/**
 The type of ATSBoxDeviceType capture device that the data model is configured to use.
 
 Capture devices can be physical, emulated, or virtual.
 */
@property (nonatomic, assign) CaptureConfigurationDeviceType ATSBoxDeviceType;

/**
 The serial number of the ATS (30-pin or Lightning) Box  that will be in use during the capture, or nil if no ATS boxes are detected.
 
 Only physical ATS boxes have serial numbers.
 */
@property (nonatomic, copy, nullable) NSString *ATSBoxSerialNumber;

/**
 The device path of the ATS (30-pin or Lightning or Smart Connector) box that will be in use during the capture, or nil if no ATS boxes are detected.
 
 Only physical ATS boxes have device paths.
 */
@property (nonatomic, copy, nullable) NSString *ATSBoxDevicePath;

/**
 The identifier of the ATS (30-pin or Lightning or Smart Connector) box that will be in use during the capture, or nil if no ATS boxes are detected.
 
 Only virtual ATS boxes have identifiers.
 */
@property (nonatomic, copy, nullable) NSString *ATSBoxIdentifier;

/**
 The port name of the ATS (30-pin or Lightning or Smart Connector) box that will be in use during the capture, or nil if no ATS boxes are detected.
 
 Only virtual ATS boxes have port names.
 */
@property (nonatomic, copy, nullable) NSString *ATSBoxPortName;

/**
 The type of Beagle USB protocol analyzer that the data model is configured to use.
 
 Capture devices can be physical, emulated or virtual.
 */
@property (nonatomic, assign) CaptureConfigurationDeviceType beagleDeviceType;

/**
 The serial number of the Beagle USB protocol analyzer that will be in use during the capture, or nil if no Beagle devices are detected.
 
 Only physical Beagle devices have serial numbers.
 */
@property (nonatomic, copy, nullable) NSString *beagleSerialNumber;

/** A flag indicating whether or not the Beagle USB protocol analyzer that will be in use during the capture is a Posh Beagle device. */
@property (nonatomic, assign, getter=isUsingPoshBeagle) BOOL usingPoshBeagle;

/** A flag indicating whether or not the accessory will be using an ATS 30-pin Box during the capture. */
@property (nonatomic, readonly) BOOL isUsing30PinATSBox;

/** A flag indicating whether or not the accessory will be using an ATS Lightning Box during the capture. */
@property (nonatomic, readonly) BOOL isUsingATSLightningBox;

/** A flag indicating whether or not the accessory will be using an ATS Smart Connector Box during the capture. */
@property (nonatomic, readonly) BOOL isUsingATSSmartConnectorBox;

/** A flag indicating whether or not the accessory will be using an ATS Lightning Audio Box during the capture. */
@property (nonatomic, readonly) BOOL isUsingATSLightningAudioBox;

/** A flag indicating whether or not the accessory will be using a transport to send and receive iAP messages. */
@property (nonatomic, readonly) BOOL isUsingTransport;

/** A flag indicating whether or not the accessory is not using a transport. */
@property (nonatomic, getter=isUsingNoTransport) BOOL usingNoTransport;

/** A flag indicating whether or not the accessory will be using either USB transport or UART transport with the potential to upgrade to USB transport. */
@property (nonatomic, getter=isUsingUSBTransport) BOOL usingUSBTransport;

/** A flag indicating whether or not the accessory will be using UART transport, with or without the option to upgrade to USB transport. */
@property (nonatomic, getter=isUsingUARTTransport) BOOL usingUARTTransport;

/** A flag indicating whether or not the accessory will be using Bluetooh transport. */
@property (nonatomic, getter=isUsingBluetoothTransport) BOOL usingBluetoothTransport;

/** A flag indicating whether or not the accessory will be using Wi-Fi transport. */
@property (nonatomic, getter=isUsingWiFiTransport) BOOL usingWiFiTransport;

/** A flag indicating whether or not the accessory will be using MikeyBus transport. */
@property (nonatomic, getter=isUsingLightningAudioTransport) BOOL usingLightningAudioTransport;

/** The selected bluetooth apple device address. */
@property (nonatomic, copy, nullable) NSString *bluetoothAppleDeviceAddress;

/** The selected bluetooth accessory address. */
@property (nonatomic, copy, nullable) NSString *bluetoothAccessoryAddress;

/** The link key to be used as the Bluetooth SSP pairing mode. */
@property (nonatomic, copy, nullable) NSString *bluetoothLink;

/** A flag indicating whether or not the accessory will be using no protocol; just electrical measurements (for charger bricks). */
@property (nonatomic, getter=isNoProtocol) BOOL noProtocol;

/** A flag indicating whether or not the accessory will be using iAP1. */
@property (nonatomic, getter=isiAP1Trace) BOOL iAP1Trace;

/** A flag indicating whether or not the accessory will be using iAP2. */
@property (nonatomic, getter=isiAP2Trace) BOOL iAP2Trace;

/** A flag indicating whether or not the accessory will be using CarPlay.
 
 @note Until version 5.17.2 we used to manually (additionally) add iAP2 protocol mask when configuring for CarPlay. Now, starting ATS 6, we are purely using CarPlay protocol mask in vanilla state and won't be implicitly adding iAP2 mask to the protocol mask. Rest of the code (Capture Transports) is modified such that with CarPlay mask, it's implicitly assumed — in the rest of the code — that iAP2 should also be considered.
 */
@property (nonatomic, getter=isCarPlayTrace) BOOL carPlayTrace;

/** A flag indicating whether or not the accessory will be using AirPlay. */
@property (nonatomic, getter=isAirPlayTrace) BOOL airPlayTrace;

/** A flag indicating whether or not the ethernet filter is enabled. When enabled, CarPlay NCM traffic is filtered from the saved USB data, and CarPlay control traffic is saved in a separate capture file. Checking the "Include streaming media" checkbox during capture configuration causes this flag to be NO. */
@property (nonatomic, assign, getter=isEthernetFilterEnabled) BOOL ethernetFilterEnabled;

/** A flag indicating whether or not the capture is configured to sniff WiFi. */
@property (nonatomic, readonly) BOOL isUsingWiFi_Deprecated;

/**
 The mutable dictionary that backs all of the data model properties.
 
 @note use as readonly. Exposed to share with the category
 */
@property (nonatomic, readonly) NSMutableDictionary *configurationDefaults;

/**
 The version string for the version of ATS used to create the data model.
 
 @note use as readonly. Exposed to share with the category
 */
@property (nonatomic, readonly) NSString *atsVersion;

/**
 Conveinence method to make a new empty capture configuration up for configuring.
 */
+ (instancetype)emptyConfiguration;

/**
 Returns a capture configuration data model that has been populated with the default properties from the standard user defaults store.
 
 @return A capture configuration data model that has been populated with the default properties from the standard user defaults store.
 */
- (instancetype)initUsingSavedDefaults;

/**
 Returns a capture configuration data model that has been populated with the contents of the file at the passed path, as previously stored by writeToFile:atomically:
 
 @param path Path to the file to restore from
 @return A capture configuration data model with a configuration that matches that of the file. Returns nil if the data model could not be restored from file
 */
- (instancetype)initWithContentsOfPlist:(NSString *)path;

/**
 Returns a capture configuration data model that has been populated with the contents of the file at the passed path, as previously stored by writeToFile:atomically:
 
 @param path Path to the file to restore from
 @param atsVersion The version string for the version of ATS that created the data model file.
 @return A capture configuration data model with a configuration that matches that of the file. Returns nil if the data model could not be restored from file
 */
- (instancetype)initWithContentsOfPlist:(NSString *)path forATSVersion:(NSString *)atsVersion;

/**
 Returns a capture configuration data model with a configuration that matches the configuration of the given data model.
 
 @param configuration The data model to base the new data model's configuration on.
 @return A capture configuration data model with a configuration that matches the configuration of the given data model.
 */
- (instancetype)initWithCaptureConfiguration:(CaptureConfiguration *)configuration;

/**
 Initialize an instance with the input configuration dictionary. The dictionary can come from user defaults or from reading a plist file.

 @param configurationDictionary Configuration dictionary that contains the key value pairs that represents the configuration
 @return Instance of Capture Configuration
 */
- (instancetype)initWithDictionary:(NSDictionary *)configurationDictionary;

/**
 Stores the receiver's property values in the standard user defaults store.
 */
- (void)saveToDefaults;

/**
 Writes the receiver's property values to the given path as a property list file.
 
 @param path The path as which to write the file.
 @param atomically A flag indicating whether or not the property list file should be written atomically.
 @return YES if the file is written successfully; NO otherwise.
 */
- (BOOL)writeToFile:(NSString *)path atomically:(BOOL)atomically;

/**
 Returns a flag indicating if the data model is in a valid state and can be used to configure a capture document.
 
 @param error An error out parameter that will be pointed as an error object on return if the data model validation fails. Pass nil if no error object is required.
 @return YES if the data model validation succeeds and the data model can be used to configure a capture document; NO otherwise.
 */
- (BOOL)validate:(NSError ** _Nullable)error;

/**
 Returns a flag indicating whether or not the receiver has any configurable properties for the given capture configuration state.
 
 A data model property may not be configurable for a given Capture Assistant state if the propety value is determined by the value of another data model property on which it is dependent. For example, the data model is not configurable for the CaptureConfigurationProtocolState when the data model's connector property is set to CaptureConfiguration30PinConnector, because iAP1 is the only valid protocol for accessories that use the 30-pin connector.
 
 @param The capture configuration state for which the receiver will indicate whether or not it has any configurable properties.
 @return YES if the receiver has configurable properties for the given capture configuration state; NO otherwise.
 */
- (BOOL)isConfigurableForState:(CaptureConfigurationState)state;

/**
 Adds any additional configurations that might not be set that can be inferred.
 */
- (void)finalizeConfiguration;

/**
 Updates the values for all of the data model's ATS-box-related properties based on the given ATS box object. Returns a flag indicating whether or not the data model was successfully configured.
 
 There are physical, emulated and virtual ATS box types. The three types for the original ATS box have no immediate common parent class in common with the three types for the ATS Lightning Box / ATS Smart Connector Box, which is why the ATSBox parameter is of type id.
 
 @param ATSBox The object representing the ATS box that the data model should use to configure itself.
 @return YES if the data model was succesfully configured and has all the information it needs about the ATS box; NO otherwise.
 */
- (BOOL)configureForATSBox:(nullable id)ATSBox;


/**
 Convenience method to copy all the ATS Box related configurations from one instance to another
 */
+ (void)fillATSBoxConfigurationFrom:(CaptureConfiguration *)from to:(CaptureConfiguration *)to;

/**
 Updates the values for all of the data model's Beagle-related properties based on the given BeagleUSBCaptureDevice object. Returns a flag indicating whether or not the data model was successfully configured.
 
 @param beagle The object representing the Beagle USB protocol analyzer that the data model should use to configure itself.
 @return YES if the data model was successfully configured and has all the information it needs about the Beagle device; NO otherwise. @discussion There are physical and emulated Beagle devices.
 */
- (BOOL)configureForBeagle:(nullable BeagleUSBCaptureDevice *)beagle;

/**
 Convenience method to copy all the Beagle related configurations from one instance to another
 */
+ (void)fillBeagleConfigurationFrom:(CaptureConfiguration *)from to:(CaptureConfiguration *)to;

@end

NS_ASSUME_NONNULL_END
