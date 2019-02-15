//
//  APIPv6Packet.h
//  TrafficKit
//
//  Created by Mark Hamlin on 6/28/12.
//  Copyright (c) 2012 Apple. All rights reserved.
//

#import "APPacket.h"

@interface APIPv6Packet : APPacket

/*!
 @property  payloadSize
 @abstract  Size of the payload, excluding this header and extensions
 */
@property (readonly, nonatomic, assign) unsigned int payloadSize;

/*!
 @property  sourceAddress
 @abstract  Source address, following ipv6 format guidelines
 @see       https://en.wikipedia.org/wiki/IPv6#Address_format
 */
@property (readonly, nonatomic, strong) NSString *sourceAddress;

/*!
 @property  destAddress
 @abstract  Destination address, following ipv6 format guidelines
 @see       https://en.wikipedia.org/wiki/IPv6#Address_format
 */
@property (readonly, nonatomic, strong) NSString *destAddress;

/*!
 @method    stringFromIPv6Address:
 @abstract  Formats an IPv6 address
 */
+ (NSString *)stringFromIPv6Address:(const uint8_t *)address;

@end
