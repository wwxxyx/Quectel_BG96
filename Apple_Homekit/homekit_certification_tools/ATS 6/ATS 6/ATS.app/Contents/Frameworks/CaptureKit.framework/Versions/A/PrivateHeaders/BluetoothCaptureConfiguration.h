//
//  BluetoothCaptureConfiguration.h
//  ATSMacApp
//
//  Created by Natália Cabral on 07/01/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CaptureKit/CaptureConfigurationTypes.h>

@interface BluetoothCaptureConfiguration : NSObject <NSCoding>

/*!
 @property   appleDeviceAddress
 @abstract   The address of the apple device related to the configuration
 */
@property (nonatomic, copy) NSString *appleDeviceAddress;

/*!
 @property   accessoryAddress
 @abstract   The address of the accessory related to the configuration
 */
@property (nonatomic, copy) NSString *accessoryAddress;

/*!
 @property   appleDeviceName
 @abstract   The name of the apple device related to the configuration
 */
@property (nonatomic, copy) NSString *appleDeviceName;

/*!
 @property   accessoryName
 @abstract   The name of the accessory related to the configuration
 */
@property (nonatomic, copy) NSString *accessoryName;

/*!
 @property   linkKey
 @abstract   The link key related to the configuration
 */
@property (nonatomic, copy) NSString *linkKey;

/*!
 @property   protocolMask
 @abstract   The protocol mask related to the configuration
 */
@property (nonatomic) CaptureConfigurationProtocolMask protocolMask;

/*!
 @method    initWithAppleDevice:accessory:linkKey:protocol:
 @abstract  Initialize bluetooth capture configuration with given apple device, accessory, link key and protocol.
 */
+ (id)initWithAppleDeviceName:(NSString *)appleDeviceName appleDeviceAddress:(NSString *)appleDeviceAddress accessoryName:(NSString *)accessoryName accessoryAddress:(NSString *)accessoryAddress linkKey:(NSString *)linkKey protocolMask:(CaptureConfigurationProtocolMask)protocolMask;

@end
