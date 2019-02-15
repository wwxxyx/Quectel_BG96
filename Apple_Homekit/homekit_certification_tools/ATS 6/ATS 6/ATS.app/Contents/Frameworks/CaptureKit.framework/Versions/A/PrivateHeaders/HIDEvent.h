//
//  HIDEvent.h
//  ATSMacApp
//
//  Created by Zachary Church on 3/15/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureEvent.h>
#import <BusSniffer/BSiAPTypes.h>

@interface HIDEvent : CaptureEvent

@property (readonly, nonatomic) NSString *title;
@property (readonly, nonatomic, strong) NSString *interfaceName;
@property (readonly) BSiAPSource source;

- (instancetype)initWithInterfaceName:(NSString *)interfaceName withTimestamp:(UInt64)timestamp;
+ (instancetype)eventWithInterfaceName:(NSString *)interfaceName withTimestamp:(UInt64)timestamp;

@end
