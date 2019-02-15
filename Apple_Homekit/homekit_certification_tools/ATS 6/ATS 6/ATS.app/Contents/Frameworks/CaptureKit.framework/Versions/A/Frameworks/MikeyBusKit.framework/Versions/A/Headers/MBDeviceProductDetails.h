//
//  MBDeviceProductDetails.h
//  MikeyBusKit
//
//  Created by Vinod Madigeri on 10/8/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface MBDeviceProductDetails : NSObject

/**
 Device's product vendor ID.
 */
@property (nonatomic, copy) NSString *vendorID;

/**
 Device's product vendor name.
 */
@property (nonatomic, copy) NSString *vendorName;

/**
 Device's product ID.
 */
@property (nonatomic, copy) NSString *productID;

/**
 Device's product name.
 */
@property (nonatomic, copy) NSString *productName;

/**
 Device's product model name.
 */
@property (nonatomic, copy) NSString *productModelName;

/**
 Device's product hardware revision.
 */
@property (nonatomic, copy) NSString *productHardwareRevision;

/**
 Device's product firmware revision.
 */
@property (nonatomic, copy) NSString *productFirmwareRevision;

/**
 Device's product serial number.
 */
@property (nonatomic, copy) NSString *productSerialNumber;

@end
