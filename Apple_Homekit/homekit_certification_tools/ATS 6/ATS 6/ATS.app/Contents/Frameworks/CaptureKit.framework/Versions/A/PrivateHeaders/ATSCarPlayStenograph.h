//
//  ATSCarPlayStenograph.h
//  ATSMacApp
//
//  Created by Carlos Frederico Azevedo on 3/30/15.
//  Copyright (c) 2015 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureRuleStenograph.h>

/** The Bonjour service name to match on when looking for CarPlay-related advertisements. Expected service name of a CarPlay Bonjour service instance. */
extern NSString *const kCarPlayBonjourServiceNamePattern;

@class CPStreamDescriptorRequestContainer;
@class I2MMessage;
@class CPInfoResponse;
@class BSUSBConfigurationDescriptor;
@class CarPlayControlRequestEvent;
@class USBResetEvent;
@class mDNSQueryTracker;

/** A Stenographer subclass with interfaces for storing and querying CarPlay rule engine state. */
@interface ATSCarPlayStenograph : CaptureRuleStenograph

/** The latest IdentificationInformation message. */
@property (strong) I2MMessage *identificationInformation;

/** The latest info response. */
@property (strong) CPInfoResponse *infoResponse;

/** The configurationDescriptor from the current USBTransferEvent, if it has one. */
@property (strong) BSUSBConfigurationDescriptor *configurationDescriptor;

/** YES if the apple device is the host in the USB connection. Can change value from NO to YES after the connection has been established through a role switch. */
@property BOOL isAppleDeviceHost;

/** The latest NCM interface number identified. */
@property UInt8 NCMInterfaceNumber;

/** Boolean that represents if the accessory supports Bluetooth. */
@property BOOL supportsBluetooth;

/** YES if the accessory supports Wired CarPlay; NO otherwise. */
@property (getter=isWiredCarPlayAccessory) BOOL wiredCarPlayAccessory;

/** YES if the accessory supports Wireless CarPlay; NO otherwise. */
@property (getter=isWirelessCarPlayAccessory) BOOL wirelessCarPlayAccessory;

/** A flag indicating whether or not the accessory uses the "audioType" key in its info response audio format dictionaries. Needed for determining which rule logic should apply when validating audio format requirements. <rdar://problem/25227284> !! [Rule fix] CarPlayWiredMinimumAudioFormatsNotSeen fires when all required formats are supplied */
@property BOOL supportsAudioTypes;

/** Difference between timestamps of estimated connection and the first Setup Request sent by the Apple Device. In seconds */
@property double timeToEstablishCarPlaySession;

/**
 Dictionary of source IP address to its mDNS query tracker. Everytime a new bonjour record with question / answer records is discovered, a new entry in the map is added.
 */
@property (nonatomic, strong, readonly) NSDictionary <NSString *, mDNSQueryTracker *> *queryRecordsTrackerMap;

/**
 Dictionary that contains every outstanding audio stream descriptor (from Setup Request) that is currently being streamed in a CarPlay session.
 */
@property (nonatomic, strong) NSMutableDictionary <NSNumber *, CPStreamDescriptorRequestContainer *> *audioTypeToStreamDescriptorMap;

/**
 Returns the string from the USBStringDescriptors array which has the same index as the parameter index.
 
 @param index The index of the desired string descriptor
 @return The string of the indexed string descriptor or nil if none possess that index
 */
- (NSString *)stringForUSBStringDescriptorWithIndex:(UInt8)index;

@end
