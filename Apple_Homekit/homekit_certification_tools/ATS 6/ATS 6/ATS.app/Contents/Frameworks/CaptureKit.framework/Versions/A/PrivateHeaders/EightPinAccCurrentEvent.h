//
//  EightPinAccCurrentEvent.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 8/2/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <CaptureKit/EightPinHardwareEvent.h>

/*!
 @class     EightPinAccCurrentEvent
 @abstract  Represents the current the accessory is drawing from the Apple device
 */
@interface EightPinAccCurrentEvent : EightPinHardwareEvent

/*!
 @property  currentInMilliamps
 @abstract  The measured current in milliamps
 */
@property (nonatomic, assign) double currentInMilliamps;

@end
