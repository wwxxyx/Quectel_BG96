//
//  PhysicalATSLightningAudioBoxCaptureDevice.h
//  ATSMacApp
//
//  Created by Vinod Madigeri on 10/16/16.
//  Copyright (c) 2016 Apple Inc. All rights reserved.
//

#import <CaptureKit/ATSSerialCaptureDevice.h>

/**
 Product ID of ATS Lightning Audio Box
 */
extern const NSUInteger ATSLightningAudioBoxProductId;

/**
 Vendor ID of ATS Lightning Audio Box by Future Technology Devices International Limited 
 */
extern const NSUInteger ATSLightningAudioBoxVendorId;

@interface PhysicalATSLightningAudioBoxCaptureDevice : ATSSerialCaptureDevice

@end
