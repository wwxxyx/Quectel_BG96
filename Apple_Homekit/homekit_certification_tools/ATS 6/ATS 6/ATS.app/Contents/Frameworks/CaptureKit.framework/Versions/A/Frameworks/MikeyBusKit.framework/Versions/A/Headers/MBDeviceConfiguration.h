//
//  MBDeviceConfiguration.h
//  MikeyBusKit
//
//  Created by Vinod Madigeri on 10/7/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <MikeyBusKit/MBTypes.h>

@class MBDeviceFunctionGroup;
@class MBConfigurationDesignatorPacket;
@class MBDeviceConfiguration;

@protocol MBDeviceConfigurationPropertyInformationDelegate <NSObject>

/**
 Delegate will be notified whenever there is EA property info data seen over device configuration function group ID.
 
 @param deviceConfiguration Device configuration instance that processes MBConfigurationCommandRetFunctionGroupPropertyInfo for property info data.
 @param eaData EA property info data — EA app ID.
 @param functionGroupID Function Group ID that EA can be sent over. It'll either be MBFunctionGroupIDEAUpdate or MBFunctionGroupIDHID3PP.
 @param propertyID Property ID for the property info.
 @param source Where the data was sent from the iOS or accessory.
 
 @note Ret is sent from accessory and Set is sent from iOS.
 */
- (void)deviceConfiguration:(MBDeviceConfiguration *)deviceConfiguration didFindEAPropertyInformation:(NSData *)eaData forFunctionGroupID:(MBFunctionGroupID)functionGroupID andPropertyID:(NSNumber *)propertyID fromSource:(MBSource)source;

/**
 Delegate will be notified whenever there is HID property info data seen over device configuration function group ID.
 
 @param deviceConfiguration Device configuration instance that processes MBConfigurationCommandRetFunctionGroupPropertyInfo for property info data.
 @param hidData HID property info data — HID report descriptor
 @param functionGroupID Function Group ID that HID can be sent over. It'll either be MBFunctionGroupIDHIDX90 or MBFunctionGroupIDHID3PP.
 @param propertyID Property ID for the property info.
 @param source Where the data was sent from the iOS or accessory.
 
 @note Ret is sent from accessory and Set is sent from iOS.
 */
- (void)deviceConfiguration:(MBDeviceConfiguration *)deviceConfiguration didFindHIDPropertyInformation:(NSData *)hidData forFunctionGroupID:(MBFunctionGroupID)functionGroupID andPropertyID:(NSNumber *)propertyID fromSource:(MBSource)source;

/**
 Delegate will be notified whenever there is Vendor Specific (UART) property info data seen over device configuration function group ID.
 
 @param deviceConfiguration Device configuration instance that processes MBConfigurationCommandRetFunctionGroupPropertyInfo for property info data.
 @param vendorSpecificData Vendor Specific property info data
 @param functionGroupID Function Group ID that HID can be sent over. It'll be MBFunctionGroupIDVendorSpecific.
 @param propertyID Property ID for the property info.
 @param source Where the data was sent from the iOS or accessory.
 
 @note Ret is sent from accessory and Set is sent from iOS.
 */
- (void)deviceConfiguration:(MBDeviceConfiguration *)deviceConfiguration didFindVendorSpecificPropertyInformation:(NSData *)vendorSpecificData forFunctionGroupID:(MBFunctionGroupID)functionGroupID andPropertyID:(NSNumber *)propertyID fromSource:(MBSource)source;

@end

@interface MBDeviceConfiguration : NSObject

/**
 Value representing configuration class of the MikeyBus accessory.
 */
@property (nonatomic, strong) NSNumber *configurationClass;

/**
 Value representing configuration Subclass of the MikeyBus accessory.
 */
@property (nonatomic, strong) NSNumber *configurationSubclass;

/**
 Value representing the number of function group supported by this device configuration instance.
 */
@property (nonatomic, strong) NSNumber *numberOfFunctionGroups;

/**
 Delegate will be notified when there is a property-information data seen over retFunctionGroupPropertyInfo.
 */
@property (nonatomic, weak) id <MBDeviceConfigurationPropertyInformationDelegate> delegate;

/**
 Create an instance of device configuration with a delegate to receive property-information data.(HID report descriptior and EA app ID).
 @param delegate Delegate will be notified when there is a property-information data seen over retFunctionGroupPropertyInfo.
 */
- (instancetype)initWithPropertyInformationDelegate:(id<MBDeviceConfigurationPropertyInformationDelegate>)delegate;

/**
 Function group number to function group map.
 */
@property (nonatomic, strong) NSMutableDictionary <NSNumber *, MBDeviceFunctionGroup *> *functionGroups;

/**
 Method to read the configuration from MBConfigurationCommandRetConfiguration response.
 
 @param retConfiguration Configuration designator packet with device configuration.
 */
- (void)processRetConfiguration:(MBConfigurationDesignatorPacket *)retConfiguration;

/**
 Method to read the function group properties for the input function group number.
 
 @param retFunctionGroupProperties Configuration designator packet with function group properties.
 @param functionGroupNumber function group number for which this funciton group properties response was sent for.
 */
- (void)processRetFunctionGroupProperties:(MBConfigurationDesignatorPacket *)retFunctionGroupProperties forFunctionGroupNumber:(NSNumber *)functionGroupNumber;

/**
 Method to read the function group property info data from MBConfigurationCommandRetFunctionGroupPropertyInfo response and report the data in the delegate after storing it in the right function group number and property ID.
 
 @param retFunctionGroupPropertyInfo Configuration designator packet with function group property info data.
 @param functionGroupNumber function group number for which this funciton group properties info response was sent for.
 @param propertyID property ID for which this funciton group properties info response was sent for.
 */
- (void)processRetFunctionGroupPropertyInfo:(MBConfigurationDesignatorPacket *)retFunctionGroupPropertyInfo forFunctionGroupNumber:(NSNumber *)functionGroupNumber forPropertyID:(NSNumber *)propertyID;

/**
 Method to read the function group property info data from MBConfigurationCommandSetFunctionGroupPropertyInfo from host.
 
 @param setFunctionGroupPropertyInfo Configuration designator packet with function group property info data.
 */
- (void)processSetFunctionGroupPropertyInfo:(MBConfigurationDesignatorPacket *)setFunctionGroupPropertyInfo;

/**
 Method returns string name for the input function group type.
 
 @param functionGroupType Function group type to get the string representation for.
 
 @return String name for the input function group type.
 */
+ (NSString *)stringDescriptionForFunctionGroupType:(MBFunctionGroupType)functionGroupType;

@end
