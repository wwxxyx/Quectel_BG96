//
//  EightPinAccOrientationStateEvent.h.h
//  ATSMacApp
//
//  Created by Kip Nicol on 8/20/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <CaptureKit/EightPinHardwareEvent.h>
#import <ATSR3BoxSupport/ATSR3BoxSupport.h>

@interface EightPinAccOrientationStateEvent : EightPinHardwareEvent

/*!
 @property  orientationState
 @abstract  The orientation state of the accessory.
 */
@property (nonatomic, assign) ATFR3AccessoryOrientation orientationState;

@end
