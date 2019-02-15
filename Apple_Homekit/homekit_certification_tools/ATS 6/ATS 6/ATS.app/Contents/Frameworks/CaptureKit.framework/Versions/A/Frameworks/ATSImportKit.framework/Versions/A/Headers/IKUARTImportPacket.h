//
//  IKUARTImportPacket.h
//  ATSImportKit
//
//  Created by Mark Hamlin on 5/8/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import "IKImportPacket.h"

@interface IKUARTImportPacket : IKImportPacket

/*!
 @property  packetData
 @abstract  The data for the uart packet
 */
@property (nonatomic, retain) NSData *packetData;

@end
