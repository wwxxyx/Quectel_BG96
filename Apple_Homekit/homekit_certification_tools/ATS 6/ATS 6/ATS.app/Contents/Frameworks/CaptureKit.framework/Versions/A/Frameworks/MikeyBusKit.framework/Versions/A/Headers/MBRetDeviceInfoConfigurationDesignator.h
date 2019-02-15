//
//  MBRetDeviceInfoConfigurationDesignator.h
//  MikeyBusKit
//
//  Created by Vinod Madigeri on 11/5/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <MikeyBusKit/MBConfigurationDesignatorPacket.h>

/*
 String Keys to be able to access each value inside parsedPayloadFields
 */
extern NSString * const MBMikeyBusVersionKey;
extern NSString * const MBInterfaceChipVendorIDKey;
extern NSString * const MBInterfaceChipProductIDKey;
extern NSString * const MBInterfaceChipRevisionKey;
extern NSString * const MBInterfaceChipSerialNumberKey;
extern NSString * const MBProductVendorIDKey;
extern NSString * const MBProductVendorNameStringIndexKey;
extern NSString * const MBProductIDKey;
extern NSString * const MBProductNameStringIndexKey;
extern NSString * const MBProductModelNameStringIndexKey;
extern NSString * const MBProductHardwareRevisionStringIndexKey;
extern NSString * const MBProductFirmwareRevisionStringIndexKey;
extern NSString * const MBProductSerialNumberStringIndexKey;
extern NSString * const MBTotalNumberOfConfigurationsKey;
extern NSString * const MBCurrentActiveConfigurationKey;

@interface MBRetDeviceInfoConfigurationDesignator : MBConfigurationDesignatorPacket

@end
