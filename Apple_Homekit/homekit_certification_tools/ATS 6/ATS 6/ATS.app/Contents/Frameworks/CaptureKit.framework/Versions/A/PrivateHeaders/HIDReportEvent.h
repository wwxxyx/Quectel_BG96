//
//  HIDReportEvent.h
//  ATSMacApp
//
//  Created by Zachary Church on 3/15/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <CaptureKit/HIDEvent.h>
#import <BusSniffer/BSUSBHIDTypes.h>

@class BSHIDReport;
@class HIDGetReportEvent;
@class HIDReportDescriptorEvent;

@interface HIDReportEvent : HIDEvent

@property (readonly, strong) BSHIDReport *report;
@property (readwrite, strong) HIDGetReportEvent *getReportEvent;
@property (readonly, strong) HIDReportDescriptorEvent *descriptorEvent;

+ (instancetype)eventWithReport:(BSHIDReport *)report reportDescriptorEvent:(HIDReportDescriptorEvent *)descriptorEvent onInterfaceName:(NSString *)interfaceName fromSource:(BSiAPSource)source atTimestamp:(UInt64)timestamp;

@end
