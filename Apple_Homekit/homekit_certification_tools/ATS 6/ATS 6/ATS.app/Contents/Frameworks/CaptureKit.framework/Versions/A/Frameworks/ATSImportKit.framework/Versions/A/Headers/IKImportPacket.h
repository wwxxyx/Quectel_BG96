//
//  ImportPacket.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 5/8/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "IKConstants.h"

@interface IKImportPacket : NSObject

/// The backing for this packet.
@property (nonatomic, readonly) NSMutableDictionary *backing;

/// Returns an instance of ImportPacket.
+ (instancetype)importPacket;

/// Creates an import packet from the data (using the class type declared in the data), or nil on failure.
+ (instancetype)importPacketWithData:(NSData *)data;

/// Returns the serialized data for this packet, including type information.
- (NSData *)serializedData;

/*!
 Assigns the relative timestamp in NS, populating timestamp
 */
- (void)assignRelativeTimestampInNS:(UInt64)timestampInNS;

/*!
 Assigns the absolute timestamp (since 1970), populating timestampSince1970
 */
- (void)assignAbsoluteTimestampSince1970:(UInt64)timestampSince1970;

/*!
 The timestamp for the imported packet
 */
@property (nonatomic, readonly) UInt64 relativeTimestamp;

/*!
 The absolute timestamp (since 1970) for the imported packet
 */
@property (nonatomic, readonly) UInt64 timestampSince1970;

/// The source for the imported packet.
@property (nonatomic, assign) IKPacketSource source;

@end
