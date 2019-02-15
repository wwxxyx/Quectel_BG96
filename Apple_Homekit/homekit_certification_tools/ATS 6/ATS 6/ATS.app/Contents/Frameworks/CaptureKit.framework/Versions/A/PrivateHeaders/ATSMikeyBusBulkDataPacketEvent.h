//
//  ATSMikeyBusBulkDataPacketEvent.h
//  ATSMacApp
//
//  Created by Wesley McCloy on 12/14/15.
//  Copyright © 2015 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureEvent+Protected.h>
#import <MikeyBusKit/MikeyBusKit.h>

@interface ATSMikeyBusBulkDataPacketEvent : CaptureEvent

/**
 Mikeybus designator packet sent over bulk data packet.
 */
@property (nonatomic, strong) id <MBDesignator> mbDesignatorPacket;

/**
 MikeyBus bulk data packet reference; used to interpret the bulk data events.
 */
@property (nonatomic, strong) MBBulkDataPacket *mbBulkDataPacket;

/**
 The ACK pair. If this event is of a designator type, then the counter part
 will be the ACK, and vice versa
 */
@property (nonatomic, weak) ATSMikeyBusBulkDataPacketEvent *ackPair;

/**
 Direction of the bulk data packet event.
 */
@property (nonatomic, readonly) NSString *direction;

/**
 Function group ID of the bulk data packet event.
 */
@property (nonatomic, readonly) NSString *functionGroupID;

/**
 Packet flags of the bulk data packet event.
 */
@property (nonatomic, readonly) NSString *packetFlags;

/**
 Command ID of the bulk data packet event.
 */
@property (nonatomic, readonly) NSString *commandID;

/**
 Command Details of the bulk data packet event.
 */
@property (nonatomic, readonly) NSString *commandDescription;

/**
 Segment number of the bulk data packet event.
 */
@property (nonatomic, readonly) NSString *segmentNumber;

@end
