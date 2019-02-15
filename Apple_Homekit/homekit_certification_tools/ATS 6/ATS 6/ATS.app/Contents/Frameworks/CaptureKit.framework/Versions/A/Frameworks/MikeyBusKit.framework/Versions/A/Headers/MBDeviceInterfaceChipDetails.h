//
//  MBDeviceInterfaceChipDetails.h
//  MikeyBusKit
//
//  Created by Vinod Madigeri on 10/8/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface MBDeviceInterfaceChipDetails : NSObject

/**
 Device's interface chip vendor ID.
 */
@property (nonatomic, copy) NSString *interfaceChipVendorID;

/**
 Device's interface chip product ID.
 */
@property (nonatomic, copy) NSString *interfaceChipProductID;

/**
 Device's interface chip Revision.
 */
@property (nonatomic, copy) NSString *interfaceChipRevision;

/**
 Device's interface chip serial number.
 */
@property (nonatomic, copy) NSString *interfaceChipSerialNumber;

@end
