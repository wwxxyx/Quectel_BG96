//
//  BSiAP2LinkPacket.h
//  BusSniffer
//
//  Created by Joe Basu on 11/28/11.
//  Copyright (c) 2011 Apple Inc. All rights reserved.
//

#import <BusSniffer/BSiAPTypes.h>
#import <BusSniffer/BSiAP2LinkTypes.h>

@protocol BSiAP2LinkPacket <NSObject>

@property (nonatomic, readonly) BSiAPSource source;
@property (nonatomic, readonly) BSiAPTransport transport;
@property (nonatomic, readonly) UInt64 timestamp;
@property (nonatomic, readonly) NSData *data;
@property (nonatomic, readonly) BSiAP2LinkPacketStatus status;

@property (nonatomic, readonly) BOOL hasPacketLength;
@property (nonatomic, readonly) BOOL hasControlByte;
@property (nonatomic, readonly) BOOL hasPacketSequenceNumber;
@property (nonatomic, readonly) BOOL hasPacketAcknowledgementNumber;
@property (nonatomic, readonly) BOOL hasSessionID;
@property (nonatomic, readonly) BOOL hasHeaderChecksum;

@property (nonatomic, readonly) UInt16 packetLength;
@property (nonatomic, readonly) UInt8 controlByte;
@property (nonatomic, readonly) UInt8 packetSequenceNumber;
@property (nonatomic, readonly) UInt8 packetAcknowledgementNumber;
@property (nonatomic, readonly) UInt8 sessionID;
@property (nonatomic, readonly) SInt8 headerChecksum;

// optional fields
@property (nonatomic, readonly) BOOL hasPayloadData;
@property (nonatomic, readonly) BOOL hasPayloadChecksum;

@property (nonatomic, readonly) NSData *payloadData;
@property (nonatomic, readonly) SInt8 payloadChecksum;

@end


@interface BSiAP2LinkPacket : NSObject <BSiAP2LinkPacket> {
	BSiAPSource _source;
	BSiAPTransport _transport;
	UInt64 _timestamp;
    NSData *_data;
    BSiAP2LinkPacketStatus _status;
}

+ (id)packetWithTimestamp:(UInt64)timestamp
                   source:(BSiAPSource)source
                transport:(BSiAPTransport)transport
                     data:(NSData *)data;

- (id)initWithTimestamp:(UInt64)timestamp
                 source:(BSiAPSource)source
              transport:(BSiAPTransport)transport
                   data:(NSData *)data;

/*!
 @method    isACKBitSet
 @abstract  verify if the ACK bit is set in the control byte
 @return    YES if ACK bit is set, NO otherwisw
 */
- (BOOL)isACKBitSet;

/*!
 @method    isEAKBitSet
 @abstract  verify if the EAK bit is set in the control byte
 @return    YES if EAK bit is set, NO otherwisw
 */
- (BOOL)isEAKBitSet;

/*!
 @method    isSYNBitSet
 @abstract  verify if the SYN bit is set in the control byte
 @return    YES if SYN bit is set, NO otherwisw
 */
- (BOOL)isSYNBitSet;

/*!
 @method    isRSTBitSet
 @abstract  verify if the RST bit is set in the control byte
 @return    YES if RST bit is set, NO otherwisw
 */
- (BOOL)isRSTBitSet;

/*!
 @method    isSLPBitSet:
 @abstract  verify if the SLP bit is set in the control byte
 @return    YES if SLP bit is set, NO otherwisw
 */
- (BOOL)isSLPBitSet;

@end
