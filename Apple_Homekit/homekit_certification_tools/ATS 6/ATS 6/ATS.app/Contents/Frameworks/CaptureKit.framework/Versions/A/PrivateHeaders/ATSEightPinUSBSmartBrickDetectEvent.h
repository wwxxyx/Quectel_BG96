//
//  ATSEightPinUSBSmartBrickDetectEvent.h
//  ATSMacApp
//
//  Created by Vinod Madigeri on 6/1/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <CaptureKit/EightPinHardwareEvent.h>
#import <ATSR3BoxSupport/ATSR3BoxSupport.h>

@interface ATSEightPinUSBSmartBrickDetectEvent : EightPinHardwareEvent

#pragma mark - Raw Measurements

/**
 vbus voltage
 */
@property (nonatomic) double vbus;

/**
 D- shunt to ground voltage
 */
@property (nonatomic) double dMinusShuntToGround;

/**
 D+ voltage
 */
@property (nonatomic) double dPlus;

/**
 D- voltage
 */
@property (nonatomic) double dMinus;

@end
