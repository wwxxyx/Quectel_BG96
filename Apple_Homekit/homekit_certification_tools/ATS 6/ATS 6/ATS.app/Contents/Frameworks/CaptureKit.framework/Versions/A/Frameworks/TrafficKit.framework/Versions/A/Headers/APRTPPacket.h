//
//  APRTPPacket.h
//  TrafficKit
//
//  Created by Mark Hamlin on 7/12/12.
//  Copyright (c) 2012 Apple. All rights reserved.
//

#import <TrafficKit/TrafficKit.h>

@interface APRTPPacket : APPacket

/*!
 @property  sequenceNumber
 @abstract  The sequence number for the packet, incremented by 1 each packet.
 */
@property (nonatomic, assign) uint16_t sequenceNumber;

/*!
 @property  timestamp
 @abstract  Packets timestamp, contents are application specific
 */
@property (nonatomic, assign) uint32_t rtpTimestamp;

/*!
 @property  ssrcIdentifier
 @abstract  Unique identifier for this stream
 */
@property (nonatomic, assign) uint32_t ssrcIdentifier;

/*!
 @property      payload
 @abstract      RTP payload, everything after the 12-byte minimum header
 @discussion    Currently only the minimum 12-byte header is parsed, so if there are extensions (or csrc identifiers), then this payload will contain them.
 */
@property (nonatomic, strong) NSData *payload;

@end
