//
//  APEthernetPacket.h
//  APTS
//
//  Created by Mark Hamlin on 6/22/12.
//  Copyright (c) 2012 Apple. All rights reserved.
//

#import "APPacket.h"

/*!
 @class     APEthernetPacket
 @abstract  Represents an ethernet packet
 */
@interface APEthernetPacket : APPacket

/*!
 @property  payloadType
 @abstract  The payload type https://en.wikipedia.org/wiki/EtherType
 */
@property (readonly, nonatomic, assign) uint16_t payloadType;

/*!
 @property  macSource
 @abstract  Source mac address, formatted as "xx:xx:xx:xx:xx:xx:"
 */
@property (readonly, nonatomic, strong) NSString *macSource;

/*!
 @property  macDest
 @abstract  Destination mac address, formatted as "xx:xx:xx:xx:xx:xx:"
 */
@property (readonly, nonatomic, strong) NSString *macDest;

/*!
 @method    payloadTypeName
 @abstract  Returns the name of the payload type, if known. Otherwise @"Unknown"
 */
- (NSString *)payloadTypeName;

@end
