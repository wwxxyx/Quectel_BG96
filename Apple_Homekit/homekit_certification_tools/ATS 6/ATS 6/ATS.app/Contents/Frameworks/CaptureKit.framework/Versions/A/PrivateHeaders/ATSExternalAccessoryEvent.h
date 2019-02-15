//
//  ATSExternalAccessoryEvent.h
//  ATSMacApp
//
//  Created by Vinod Madigeri on 4/28/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureEvent.h>
#import <BusSniffer/BSiAPTypes.h>
#import <BusSniffer/BSExternalAccessoryPacket.h>

@class BSExternalAccessoryPacket;

@interface ATSExternalAccessoryEvent : CaptureEvent

/**
 Source this event't packet came from.
 */
@property (nonatomic) BSiAPSource source;

/**
 Packet associated with this event.
 */
@property (nonatomic, strong) BSExternalAccessoryPacket *packet;

/**
 Create a External Accessory event from packet, source and timestamp.
 
 @param packet External Accessory packet
 @param source Event source
 @param timestamp timestamp for this event.
 @param isSuitableForRuleEngine Whether or not the event created using the data is suitable for rule engine.
 */
+ (instancetype)eventWithPacket:(BSExternalAccessoryPacket *)packet
                     fromSource:(BSiAPSource)source
                    atTimestamp:(UInt64)timestamp
          suitableForRuleEngine:(BOOL)isSuitableForRuleEngine;

@end
