//
//  APTCPPacket.h
//  APTS
//
//  Created by Mark Hamlin on 6/26/12.
//  Copyright (c) 2012 Apple. All rights reserved.
//

#import "APPacket.h"

@interface APTCPPacket : APPacket

/*!
 @property  payloadSize
 @abstract  Number of bytes in the encapsulated payload
 */
@property (readonly, nonatomic, assign) unsigned int payloadSize;

/*!
 @property      payload
 @abstract      Raw TCP payload data
 @discussion    Could be nil if the declared ip payload length doesn't match the actual capture length, or if there's no payload
 */
@property (readonly, nonatomic, strong) NSData *payload;

/*!
 Source IP address
 */
@property (readonly, nonatomic, strong) NSString *sourceIPAddress;

/*!
 Source IP address
 */
@property (readonly, nonatomic, strong) NSString *destinationIPAddress;

/*!
 @property  sourcePort
 @abstract  Source port
 */
@property (readonly, nonatomic, assign) unsigned short sourcePort;

/*!
 @property  destPort
 @abstract  Destination port
 */
@property (readonly, nonatomic, assign) unsigned short destPort;

/*!
 @property  seqNumber
 @abstract  Sequence number
 */
@property (readonly, nonatomic, assign) uint32_t seqNumber;

/*!
 @property      ackNumber
 @abstract      Acknowledgement number
 @discussion    Only set if the ackFlag is YES
 */
@property (readonly, nonatomic, assign) uint32_t ackNumber;

/*!
 @property  ackFlag
 @abstract  Boolean indicating if the ack flag is set
 */
@property (readonly, nonatomic, assign) BOOL ackFlag;

/*!
 @property  synFlag
 @abstract  Boolean indicating if the syn flag is set
 */
@property (readonly, nonatomic, assign) BOOL synFlag;

/*!
 @property  finFlag
 @abstract  Boolean indicating if the fin flag is set
 */
@property (readonly, nonatomic, assign) BOOL finFlag;

/*!
 Boolean indicating if the RST flag is set
 */
@property (readonly, nonatomic, assign) BOOL rstFlag;

@end
