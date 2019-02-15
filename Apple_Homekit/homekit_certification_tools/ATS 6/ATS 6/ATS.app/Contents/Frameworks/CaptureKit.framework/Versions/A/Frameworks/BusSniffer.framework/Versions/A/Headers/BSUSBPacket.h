//
//  BSUSBPacket.h
//  BusSniffer
//
//  Created by Joe Basu on 2/25/09.
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <Foundation/Foundation.h>
#import <BusSniffer/BSUSBTypes.h>
#import <BusSniffer/BSUSBTransaction.h>

/*!
 @protocol BSUSBPacket
 @abstract The BSUSBPacket protocol defines common properties of USB packets.
*/

@protocol BSUSBPacket <NSObject>

/*!
 @property address
 @abstract The address of the device the packet was intended for. This property only returns valid data for SETUP, IN, OUT, PING token packets.
*/
@property (nonatomic, readonly) UInt8 address;

/*!
 @property crc
 @abstract The CRC5 value for a token packet (SOF, SETUP, IN, OUT, PING, or SPLIT) or the CRC16 value for a data packet (DATA0, DATA1, DATA2, or MDATA). This property only returns valid values for token or data packets.
 */
@property (nonatomic, readonly) UInt16 crc;

/*!
 @property data
 @abstract The raw data representing the packet, including the packet ID, plus payload and CRC checksum if applicable.
 */
@property (nonatomic, readonly, copy) NSData *data;

/*!
 @property address
 @abstract The endpoint of the device the packet was targeted for. This property only returns valid data for SETUP, IN, OUT or PING token packets.
 */
@property (nonatomic, readonly) UInt8 endpoint;

/*!
 @property packetID
 @abstract The packet ID (4 bits) and its complement (4 bits).
 */
@property (nonatomic, readonly) BSUSBPacketID packetID;

/*!
 @property packetIDString
 @abstract A string representation of the packet ID suitable for display.
 */
@property (nonatomic, readonly, copy) NSString *packetIDString;

/*!
 @property timestamp
 @abstract The time the packet appeared on the USB in units of microseconds.
 */
@property (nonatomic, readonly) UInt64 timestamp;

/*!
 @property status
 @abstract An status code indicating if the packet is malformed
 */
@property (nonatomic, readonly) BSUSBPacketStatus status;

@end

/*!
 @class BSUSBPacket
 @discussion A BSUSBPacket instance is an immutable model class representing a USB packet that follows the BSUSBPacket protocol.
*/

@interface BSUSBPacket : NSObject <BSUSBPacket> {
	NSData *_data;
	UInt64 _timestamp;
	BSUSBPacketStatus _status;
}

/*!
 @method usbPacketWithTimestamp:data:
 @abstract Creates and returns a USB packet initialized with a given timestamp in microseconds and raw packet data.
 */
+ (id)usbPacketWithTimestamp:(UInt64)timestamp
						data:(NSData *)data;

+ (id)usbTokenPacketWithTimestamp:(UInt64)timestamp
						 packetID:(BSUSBPacketID)packetID
						  address:(UInt8)address
						 endpoint:(UInt8)endpoint
							  crc:(UInt8)crc;

+ (id)usbHandshakePacketWithTimestamp:(UInt64)timestamp
							 packetID:(BSUSBPacketID)packetID;

+ (id)usbDataPacketWithTimestamp:(UInt64)timestamp
						packetID:(BSUSBPacketID)packetID
					 payloadData:(NSData *)payloadData
							 crc:(UInt16)crc;

/*!
 @method initWithTimestamp:data:
 @abstract Returns a USB packet initialized with a given timestamp in microseconds and raw packet data.
 */
- (id)initWithTimestamp:(UInt64)timestamp
				   data:(NSData *)data;

@end

BSUSBPacketID BSUSBPacketLightweightPacketID(const void *bytes, UInt16 length);
UInt8 BSUSBPacketLightweightTokenPacketAddress(const void *bytes, UInt16 length);

