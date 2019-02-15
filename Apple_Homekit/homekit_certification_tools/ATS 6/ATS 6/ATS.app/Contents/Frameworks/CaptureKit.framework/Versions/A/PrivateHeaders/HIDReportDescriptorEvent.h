//
//  HIDReportDescriptorEvent.h
//  ATSMacApp
//
//  Created by Zachary Church on 3/15/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <CaptureKit/HIDEvent.h>

@class BSUSBHIDReportDescriptor;

@interface HIDReportDescriptorEvent : HIDEvent

@property (nonatomic, readonly, strong) BSUSBHIDReportDescriptor *descriptor;
@property (nonatomic, readonly, strong) NSError *error;

+ (instancetype)eventWithDescriptor:(BSUSBHIDReportDescriptor *)descriptor onInterfaceName:(NSString *)interfaceName error:(NSError *)error atTimestamp:(UInt64)timestamp;

@end
