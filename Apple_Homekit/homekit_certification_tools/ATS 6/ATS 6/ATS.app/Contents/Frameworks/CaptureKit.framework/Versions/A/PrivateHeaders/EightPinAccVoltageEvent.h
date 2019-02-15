//
//  EightPinAccVoltageEvent.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 8/2/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <CaptureKit/EightPinHardwareEvent.h>

@interface EightPinAccVoltageEvent : EightPinHardwareEvent

/*!
 @property  voltageInMillivolts
 @abstract  The measured voltage in millivolts
 */
@property (nonatomic, assign) double voltageInMillivolts;

@end
