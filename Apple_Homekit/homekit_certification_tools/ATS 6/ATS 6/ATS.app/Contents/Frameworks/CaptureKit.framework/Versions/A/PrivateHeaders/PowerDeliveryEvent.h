//
//  PowerDeliveryEvent.h
//  ATSMacApp
//
//  Created by Zachary Church on 3/28/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureEvent.h>
#import <BusSniffer/BSiAPTypes.h>

@class BSPowerDeliveryPacket;

@interface PowerDeliveryEvent : CaptureEvent

@property (readonly, strong) PowerDeliveryEvent *sourceCapabilities;
@property (readonly, strong) BSPowerDeliveryPacket *packet;
@property (readonly) BSiAPSource source;
@property (readonly) NSString *commandType;

+ (instancetype)eventWithPacket:(BSPowerDeliveryPacket *)packet fromSource:(BSiAPSource)source atTimestamp:(UInt64)timestamp sourceCapabilities:(PowerDeliveryEvent *)sourceCapabilities;

@end
