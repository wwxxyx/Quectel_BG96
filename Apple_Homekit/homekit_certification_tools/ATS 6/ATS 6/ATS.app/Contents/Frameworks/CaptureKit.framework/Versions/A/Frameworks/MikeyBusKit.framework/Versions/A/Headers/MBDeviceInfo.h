//
//  MBDeviceInfo.h
//  MikeyBusKit
//
//  Created by Vinod Madigeri on 10/6/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <MikeyBusKit/MBDeviceConfiguration.h>

@class MBDeviceProductDetails;
@class MBDeviceInterfaceChipDetails;
@class MBDeviceConfiguration;
@class MBConfigurationDesignatorPacket;

/**
 All of the product detail string types sent over as string indexes in retDeviceInfo.
 */
typedef NS_ENUM(NSUInteger, ProductDetailType) {
    ProductDetailTypeVendorName = 0x01,
    ProductDetailTypeName,
    ProductDetailTypeModelName,
    ProductDetailTypeHardwareRevision,
    ProductDetailTypeFirmwareRevision,
    ProductDetailTypeSerialNumber,
};

@interface MBDeviceInfo : NSObject

/**
 The version of MikeyBus specification implemented by the accessory.
 */
@property (nonatomic, copy) NSString *mikeyBusVersion;

/**
 Interface chip details sent over MBConfigurationCommandRetDeviceInfo.
 */
@property (nonatomic, strong) MBDeviceInterfaceChipDetails *interfaceChipDetails;

/**
 Accessory product details sent over MBConfigurationCommandRetDeviceInfo.
 */
@property (nonatomic, strong) MBDeviceProductDetails *productDetails;

/**
 Current active device configuration number sent over MBConfigurationCommandRetDeviceInfo.
 */
@property (nonatomic, strong) NSNumber *activeDeviceConfigurationNumber;

/**
 Current active device configuration communicated over device configuration function group ID.
 */
@property (nonatomic, readonly) MBDeviceConfiguration *activeDeviceConfiguration;

/**
 Device configuration number to configuration map.
 */
@property (nonatomic, strong) NSMutableDictionary <NSNumber *, MBDeviceConfiguration *> *deviceConfigurations;

/**
 String index to product detail type map to be able to know which index belongs to what kind of product detail type.
 */
@property (nonatomic, strong) NSMutableDictionary <NSNumber *, NSNumber *> *stringIndexToProductDetailTypeMap;

/**
 Method to read device info sent over Device Configuration Function Group ID with command MBConfigurationCommandRetDeviceInfo.
 
 @param retDeviceInfo Configuration designator packet with device information.
 */
- (void)processRetDeviceInfo:(MBConfigurationDesignatorPacket *)retDeviceInfo;

/**
 Method to associate each string index with the string values for each of the device info properties.
 
 @param retStrDesignator Configuration designator packet with string data.
 */
- (void)processRetStrDesignator:(MBConfigurationDesignatorPacket *)retStrDesignator;

/**
 Method to read the configuration number from MBConfigurationCommandSetConfiguration request.
 
 @param getConfiguration Configuration designator packet with configuration number.
 */
- (void)processSetConfiguration:(MBConfigurationDesignatorPacket *)setConfiguration;

/**
 Method to create and store configuration from MBConfigurationCommandRetConfiguration, and associate it with the configuration number.
 
 @param retConfiguration Configuration designator packet with configuration payload.
 @param configurationNumber Configuration number this configuration is associated it.
 @param delegate Delegate will be notified when there is a property-information data seen over retFunctionGroupPropertyInfo.
 
 @note Delegate is notified as soon as there is a Property-Info data for a particular type of bulkdata payload (HID / EA / …).
 */
- (void)processRetConfiguration:(MBConfigurationDesignatorPacket *)retConfiguration forConfigurationNumber:(NSNumber *)configurationNumber withConfigurationPropertyInformationDelegate:(id<MBDeviceConfigurationPropertyInformationDelegate>)delegate;


@end
