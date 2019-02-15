//
//  BSiAPPacket.h
//  BusSniffer
//
//  Created by Joe Basu on 6/13/09.
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <BusSniffer/BSiAPTypes.h>

@protocol BSiAPPacket <NSObject>

// FIXME: <rdar://problem/19046683> Bluetooth: one message was sent from the incorrect source
// Temporary work-around: parser will update the packet source if it looks like the ComProbe got it wrong
@property (nonatomic, readwrite) BSiAPSource source;
@property (nonatomic, readonly) BSiAPTransport transport;
@property (nonatomic, readonly, copy) NSData *data;
@property (nonatomic, readonly) BOOL hasTransactionID;
@property (nonatomic, readonly) UInt64 timestamp;
@property (nonatomic, readonly) BSiAPPacketStatus status;

@end

@interface BSiAPPacket : NSObject <BSiAPPacket> {
    BSiAPSource _source;
    BSiAPTransport _transport;
    BOOL _hasTransactionID;
    NSData *_data;
    UInt64 _timestamp;
    BSiAPPacketStatus _status;
    UInt8 _syncByteOffset;
}

@property (nonatomic, readonly) BOOL usesLargePacketFormat;
@property (nonatomic, readonly) UInt16 specifiedLength;
@property (nonatomic, readonly) UInt8 lingoID;
@property (nonatomic, readonly) UInt16 commandID;
@property (nonatomic, readonly) UInt16 transactionID;

+ (id)iAPPacketWithTimestamp:(UInt64)timestamp
                      source:(BSiAPSource)source
                   transport:(BSiAPTransport)transport
                        data:(NSData *)data
            hasTransactionID:(BOOL)hasTransactionID;

- (id)initWithTimestamp:(UInt64)timestamp
                 source:(BSiAPSource)source
              transport:(BSiAPTransport)transport
                   data:(NSData *)data
       hasTransactionID:(BOOL)hasTransactionID;

@end
