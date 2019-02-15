//
//  EightPinDeviceCurrentEvent.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 8/2/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <CaptureKit/EightPinHardwareEvent.h>

/*!
 @class     EightPinDeviceCurrentEvent
 @abstract  Represents the current the Accessory is providing to the Apple device
 */
@interface EightPinDeviceCurrentEvent : EightPinHardwareEvent

/*!
 @property  currentInMilliamps
 @abstract  The measured current in milliamps
 */
@property (nonatomic, assign) double currentInMilliamps;

@end
