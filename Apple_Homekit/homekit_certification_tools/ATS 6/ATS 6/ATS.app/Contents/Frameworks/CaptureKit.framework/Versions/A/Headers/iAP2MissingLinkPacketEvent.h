//
//  iAP2MissingLinkPacketEvent.h
//  ATSMacApp
//
//  Created by Natália Cabral on 18/06/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <CaptureKit/iAP2Event.h>
#import <CaptureKit/CaptureKit.h>
@import BusSniffer.BSiAPTypes;

/** iAP2MissingLinkPacketEvent encapsulates information about a point in time at which missing traffic was detected. */
@interface iAP2MissingLinkPacketEvent : iAP2Event<ReportAnalyzerDisplayProtocol>

/** The source of the missing packet. */
@property (nonatomic, readonly) BSiAPSource source;

/** The packet sequence number that wasn't seen. */
@property (nonatomic, readonly) UInt8 packetSequenceNumber;

/** The list of packets that are consecutives to the missing packet. */
@property (nonatomic, readonly, copy) NSArray *relatedPackets;

/** An error object appropriate for APIs that accept packet errors. */
@property (readonly) NSError *error;

/**
 Initializes and returns a new iAP2MissingLinkPacketEvent with the given source, quantity and timestamp.
 
 @param source The source of the missing packet.
 @param packetSequenceNumber The missing sequence number.
 @param relatedPackets The list of packets that are consecutives to the missing packet.
 @param timestamp The timestamp when the event was created.
 @return The new event.
 */
+ (instancetype)iAP2MissingLinkPacketEventWithSource:(BSiAPSource)source packetSequenceNumber:(UInt8)packetSequenceNumber relatedPackets:(NSArray *)relatedPackets timestamp:(UInt64)timestamp;

/**
 Initializes and returns a new iAP2MissingLinkPacketEvent with the given source, quantity and timestamp.
 
 @param source The source of the missing packet.
 @param packetSequenceNumber The missing sequence number.
 @param relatedPackets The list of packets that are consecutives to the missing packet.
 @param timestamp The timestamp when the event was created.
 @return The new event.
 */
- (instancetype)initWithSource:(BSiAPSource)source packetSequenceNumber:(UInt8)packetSequenceNumber relatedPackets:(NSArray *)relatedPackets timestamp:(UInt64)timestamp;

@end
