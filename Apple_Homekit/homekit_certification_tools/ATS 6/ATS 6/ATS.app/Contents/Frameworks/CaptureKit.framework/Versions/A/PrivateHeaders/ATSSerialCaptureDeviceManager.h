//
//  ATSSerialCaptureDeviceManager.h
//  ATSMacApp
//
//  Created by Vinod Madigeri on 10/16/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface ATSSerialCaptureDeviceManager : NSObject

/**
 Vendor ID of the ATS Serial (Smart Connector / Lightning / Lightning Audio) Box
 */
@property (nonatomic, readonly) NSUInteger vendorID;

/**
 Product ID of the ATS Serial (Smart Connector / Lightning / Lightning Audio) Box
 */
@property (nonatomic, readonly) NSUInteger productID;

/**
 List of available devices (Smart Connector / Lightning / Lightning Audio boxes) with non connected status
 */
@property (nonatomic, copy, readonly) NSArray *availableDevices;

/**
 The product name of capture devices that this instance of device manager is going to keep track of.
 */
@property (nonatomic, readonly) NSString *productName;

/**
 Product code of the capture hardware.
 */
@property (nonatomic, readonly) NSString *captureHardwareProductCode;

@end
