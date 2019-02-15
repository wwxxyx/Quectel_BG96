//
//  IDBPacketizer.h
//  IDBusKit
//
//  Created by Kip Nicol on 10/18/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@class IDBPacket;

/*!
 IDBPacketizer iteratively accepts bytes as input. When a complete ID bus packet has been processed, it generates a new packet object.
 */
@interface IDBPacketizer : NSObject

/*!
 Processes one ID Bus byte and returns a packet if neccessary.
 
 @param byte The ID bus packet byte to process.
 @return A new ID bus packet object, or nil if the packetizer hasn't received a complete ID bus packet.
 */
- (IDBPacket *)processByte:(UInt8)byte;

/*!
 Processes an ID Bus break and returns a packet with an error if necessary.
 
 @return A packet with an error if the packetizer receives the break while it is in the middle of processing a packet, or nil.
 */
- (IDBPacket *)processBreak;

@end
