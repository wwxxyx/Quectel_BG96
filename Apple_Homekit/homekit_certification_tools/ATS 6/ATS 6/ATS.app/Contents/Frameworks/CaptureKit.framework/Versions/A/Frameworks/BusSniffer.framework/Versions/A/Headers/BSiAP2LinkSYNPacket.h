//
//  BSiAP2LinkSYNPacket.h
//  BusSniffer
//
//  Created by Joe Basu on 11/28/11.
//  Copyright (c) 2011 Apple Inc. All rights reserved.
//

#import <BusSniffer/BSiAP2LinkPacket.h>

@protocol BSiAP2LinkSYNPacket <BSiAP2LinkPacket>

@property (nonatomic, readonly) BOOL hasLinkVersion;
@property (nonatomic, readonly) BOOL hasMaxNumberOfOutstandingPackets;
@property (nonatomic, readonly) BOOL hasMaximumReceivedPacketLength;
@property (nonatomic, readonly) BOOL hasRetransmissionTimeout;
@property (nonatomic, readonly) BOOL hasCumulativeAcknowledgementTimeout;
@property (nonatomic, readonly) BOOL hasMaximumNumberOfRetransmissions;
@property (nonatomic, readonly) BOOL hasMaximumCumulativeAcknowledgements;

@property (nonatomic, readonly) UInt8 linkVersion;
@property (nonatomic, readonly) UInt8 maxNumberOfOutstandingPackets;
@property (nonatomic, readonly) UInt16 maximumReceivedPacketLength;
@property (nonatomic, readonly) UInt16 retransmissionTimeout;
@property (nonatomic, readonly) UInt16 cumulativeAcknowledgementTimeout;
@property (nonatomic, readonly) UInt8 maximumNumberOfRetransmissions;
@property (nonatomic, readonly) UInt8 maximumCumulativeAcknowledgements;

@property (nonatomic, readonly) NSUInteger sessionDefinitionCount;
@property (nonatomic, readonly) NSArray *sessionDefinitions;

@end

@interface BSiAP2LinkSYNPacket : BSiAP2LinkPacket <BSiAP2LinkSYNPacket> {
    NSArray *_sessionDefinitions;
}

@end
