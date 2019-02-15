//
//  BonjourServiceInstanceEvent.h
//  ATSMacApp
//
//  Created by Vinod Madigeri on 6/30/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureEvent.h>
#import <BusSniffer/BSiAPTypes.h>

@class APBonjourServiceInstance;

/**
 Enum to define all the types of Bonjour Service sources.
 */
typedef NS_ENUM(NSUInteger, BonjourServiceInstanceType) {
    /** CarPlay accessory Bonjour Service Instance */
    BonjourServiceInstanceTypeCarPlayAccessory,
    /** CarPlay Apple device Bonjour Service Instance */
    BonjourServiceInstanceTypeAppleDevice,
    /* AirPlay accessory Bonjour Service Instance */
    BonjourServiceInstanceTypeAirPlayAccessory,
    /** non-CarPlay Bonjour Service Instance source */
    BonjourServiceInstanceTypeOther,
};

/**
 Event with the CarPlay accessory details found by Bonjour service.
 */
@interface BonjourServiceInstanceEvent : CaptureEvent

/**
 The type of Bonjour Service instance.
 */
@property (nonatomic, readonly) BonjourServiceInstanceType bonjourServiceInstanceType;

/**
 Whether or not this Bonjour Service belongs to a wireless HU.
 */
@property (nonatomic, readonly) BOOL isWireless;

/**
 Bonjour service instance associated with the CarPlay accessory.
 */
@property (nonatomic, strong, readonly) APBonjourServiceInstance *bonjourServiceInstance;

/**
 The title of the Bonjour service instance event.
 */
@property (nonatomic, readonly) NSString *title;

/**
 Source description of the Bonjour Service Event.
 */
- (NSString *)sourceDescription;

/**
 Instantiates a Bonjour Service Instance.
 */
+ (instancetype)eventWithBonjourServiceInstance:(APBonjourServiceInstance *)bonjourServiceInstance
                    bonjourServiceInstanceType:(BonjourServiceInstanceType)bonjourServiceInstanceType
                                    isWireless:(BOOL)isWireless
                                   atTimestamp:(UInt64)timestamp;

/**
 Class method to return a set of optional CarPlay service keys expected over TXT record.
 */
+ (NSSet<NSString *> *)optionalCarPlayServiceKeys:(BOOL)isWireless;

/**
 Class method to return a set of required CarPlay service keys expected over TXT record.
 */
+ (NSSet<NSString *> *)requiredCarPlayServiceKeys:(BOOL)isWireless;

@end
