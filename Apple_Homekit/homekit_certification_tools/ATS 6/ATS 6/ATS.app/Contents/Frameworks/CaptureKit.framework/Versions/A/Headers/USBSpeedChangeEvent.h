//
//  USBSpeedChangeEvent.h
//  ATSMacApp
//
//  Created by Zachary Church on 1/5/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <CaptureKit/USBEvent.h>
@import BusSniffer.BSUSBTypes;
@import iAP2MessageKit.I2MTypeUtils;

@interface USBSpeedChangeEvent : USBEvent

@property (readonly) BSUSBSpeed newSpeed;

+ (instancetype)eventWithTimestamp:(UInt64)timestamp newSpeed:(BSUSBSpeed)speed;

@end
