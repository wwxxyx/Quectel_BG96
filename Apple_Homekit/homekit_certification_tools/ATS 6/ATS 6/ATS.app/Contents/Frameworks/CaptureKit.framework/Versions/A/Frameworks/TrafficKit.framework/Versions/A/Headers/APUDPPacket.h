//
//  APUDPPacket.h
//  APTS
//
//  Created by Mark Hamlin on 6/26/12.
//  Copyright (c) 2012 Apple. All rights reserved.
//

#import "APPacket.h"

@interface APUDPPacket : APPacket

/*!
 @property  payloadSize
 @abstract  Number of bytes in the encapsulated payload
 */
@property (readonly, nonatomic, assign) unsigned int payloadSize;

/*!
 @property  payload
 @abstract  The raw data encapsulated in this UDP packet
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

@end
