//
//  APIPv4Packet.h
//  APTS
//
//  Created by Mark Hamlin on 6/26/12.
//  Copyright (c) 2012 Apple. All rights reserved.
//

#import "APPacket.h"

@interface APIPv4Packet : APPacket

/*!
 @property  payloadSize
 @abstract  Size of the payload, as defined in the header (total length minus header length)
 */
@property (readonly, nonatomic, assign) unsigned int payloadSize;

/*!
 @property  headerSize
 @abstract  Size of the header
 */
@property (readonly, nonatomic, assign) unsigned int headerSize;

/*!
 @property  sourceAddress
 @abstract  Source address, formatted as xxx.xxx.xxx.xxx
 */
@property (readonly, nonatomic, strong) NSString *sourceAddress;

/*!
 @property  destAddress
 @abstract  Destination address, formatted as xxx.xxx.xxx.xxx
 */
@property (readonly, nonatomic, strong) NSString *destAddress;

+ (NSString *)stringFromIPv4Address:(uint32_t)address;

@end
