//
//  ATSPowerAvailableEvent.h
//  ATSMacApp
//
//  Created by Vinod Madigeri on 6/20/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureEvent.h>
#import <BusSniffer/BSiAPTypes.h>

@class BSPowerDeliveryPacket;

/**
 Event to indicate whether the power is available or pending to the Apple device while displaying the max voltage and max current.
 
 @note This event is displayed in both old and new kind of TVCs. Implemented both ways of describing the event.
 */
@interface ATSPowerAvailableEvent : CaptureEvent

/**
 Create an event with input Power Delivery packet with SourceCapabilities data, source and timestamp.
 
 @param packet Input Power delivery packet with SourceCapabilities data.
 @param source Source of the packet.
 @param timestamp Time stamp of the event.
 */
+ (instancetype)eventWithPacket:(BSPowerDeliveryPacket *)packet fromSource:(BSiAPSource)source atTimestamp:(UInt64)timestamp;

@end
