//
//  PhysicalBeagleUSBCaptureDevice.h
//  ATS
//
//  Created by Joe Basu on 2/10/09.
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <CaptureKit/BeagleUSBCaptureDevice.h>

@interface PhysicalBeagleUSBCaptureDevice : BeagleUSBCaptureDevice

/**
 Type of the Beagle capture hardware
 */
@property (nonatomic) ATSUSBBeagleType beagleType;

@end
