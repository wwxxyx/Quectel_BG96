//
//  IKUSBPacketImportPacket.h
//  ATSImportKit
//
//  Created by Mark Hamlin on 5/8/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import "IKImportPacket.h"

@interface IKUSBPacketImportPacket : IKImportPacket

/*!
 @property  state
 @abstract  The state change information
 */
@property (nonatomic, assign) UInt32 state;

/*!
 @property  packetData
 @abstract  The data for the uart packet
 */
@property (nonatomic, retain) NSData *packetData;

@end
