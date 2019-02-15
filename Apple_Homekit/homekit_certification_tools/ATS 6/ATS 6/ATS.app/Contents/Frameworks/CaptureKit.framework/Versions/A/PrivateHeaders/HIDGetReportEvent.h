//
//  HIDGetReportEvent.h
//  ATSMacApp
//
//  Created by Zachary Church on 3/17/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <CaptureKit/HIDEvent.h>
#import <BusSniffer/BSUSBHIDTypes.h>

@interface HIDGetReportEvent : HIDEvent

@property (readonly, nonatomic) UInt8 reportID;
@property (readonly, nonatomic) BSUSBHIDReportType reportType;

+ (instancetype)eventWithReportID:(UInt8)reportID andReportType:(UInt8)reportType onInterfaceName:(NSString *)interfaceName atTimestamp:(UInt64)timestamp;

@end
