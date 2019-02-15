//
//  BeagleUSBCaptureDevice.h
//  ATSMacApp
//
//  Created by Joe Basu on 10/20/10.
//  Copyright © 2010 Apple Inc. All Rights Reserved.
//

#import <CaptureKit/CaptureDevice.h>

/**
 Type of the Beagle capture hardware
 */
typedef NS_ENUM(NSUInteger, ATSUSBBeagleType) {
    ATSUSBBeagleType480,
    ATSUSBBeagleType480Power,
    ATSUSBBeagleType5000,
};

@interface BeagleUSBCaptureDevice : CaptureDevice

/**
 Type of the Beagle capture hardware
 */
@property (nonatomic) ATSUSBBeagleType beagleType;

/**
 Whether or not this is a Posh Beagle; Keeping this for historical reasons (old traces)
 */
@property (nonatomic) BOOL poshBeagle;

@end
