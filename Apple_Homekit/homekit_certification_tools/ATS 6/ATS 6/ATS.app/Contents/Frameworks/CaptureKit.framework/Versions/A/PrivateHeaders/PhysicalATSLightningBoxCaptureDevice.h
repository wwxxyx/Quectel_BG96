//
//  PhysicalATSLightningBoxCaptureDevice.h
//  ATSMacApp
//
//  Created by Zachary Church on 7/23/15.
//  Copyright (c) 2015 Apple Inc. All rights reserved.
//

#import <CaptureKit/PhysicalATFR3CaptureDevice.h>

/**
 Product ID of ATS Lightning box
 */
extern const NSUInteger ATSLightningBoxProductId;

/**
 Vendor ID of ATS Lightning box (Apple Inc.)
 */
extern const NSUInteger ATSLightningBoxVendorId;

@interface PhysicalATSLightningBoxCaptureDevice : PhysicalATFR3CaptureDevice

@end
