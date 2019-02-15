//
//  MBBulkDataFrame.h
//  MikeyBusKit
//
//  Created by Wesley McCloy on 9/24/15.
//  Copyright © 2015 Apple Inc. All rights reserved.
//

/*!
 @header        MBBulkDataFrame
 @discussion    This header defines the properties inherent to a BulkDataFrame object as well as an interface to create BulkDataFrames and get appropriate strings
 */

#import <Foundation/Foundation.h>
#import <MikeyBusKit/MBTypes.h>

@class MBBulkDataPacket;

/// This enum declares the different sources of BulkDataPackets
typedef NS_ENUM(NSInteger, MBBulkDataPacketDirectionType) {
    /// Downlink is data flow from host (Apple device) to accessory.
    MBBulkDataPacketDirectionTypeDownlink,
    /// Uplink is data flow from accessory to host (Apple device).
    MBBulkDataPacketDirectionTypeUplink,
};

/// This enum declares the different packet types of BulkDataPackets
typedef NS_ENUM(NSInteger, MBBulkDataPacketType) {
    /// This packet’s payload is a MikeyBus Designator. [MBDesignatorPacket.h]
    MBBulkDataPacketTypeDesignator,
    /// This packet is an acknowledge for other Packet Types (non-acknowledge packets) with the same Segment Number.
    MBBulkDataPacketTypeACK,
};

extern const uint16_t MBBDFSOSUnEncodedA;
extern const uint16_t MBBDFSOSUnEncodedB;
extern const int MBBDFPayloadLengthIndex;
extern const int MBBDFSOSIndex;
extern const int MBBDFSOSValue;
extern const uint8_t MBBDFMinPacketSize;

/**
 Interface to represent bulk data and its direction.
 */
@interface MBBulkData : NSObject

/**
 Direction of which this bulk data was sent
 */
@property (nonatomic, readonly) MBBulkDataPacketDirectionType direction;

/**
 A complete bulk data
 */
@property (nonatomic, readonly) NSData *data;

/**
 The timestamp when the bulk data packet was fully formed
 */
@property (nonatomic, readonly) NSTimeInterval timestamp;

/**
 Method to initialize a bulk data with input data and direction.
 
 @param A complete Bulk Data
 @param Direction
 @param Timestamp of when the Bulk Data Packet was fully created
 */
- (instancetype)initWithData:(NSData *)data direction:(MBBulkDataPacketDirectionType)direction andTimestamp:(NSTimeInterval)timestamp;

@end


@interface MBBulkDataPacket : NSObject

/// Bulk Data
@property (nonatomic, readonly) NSData* data;

/// Accesses data with index 0
@property (nonatomic, readonly) uint8_t SOSByte;
/// Accesses data with index 1

@property (nonatomic, readonly) uint8_t reserved;

/// Accesses data with index 2
@property (nonatomic, readonly) uint8_t payloadLength;

/// Accesses data with index 3
@property (nonatomic, readonly) MBFunctionGroupID functionGroupID;

/// Accesses data with index 4
@property (nonatomic, readonly) uint8_t packetFlags;

/// Accesses data with index 5
@property (nonatomic, readonly) uint8_t segmentNumber;

/// Accesses data with index 6 onwards (depending on payloadLength)
@property (nonatomic, readonly) NSData* payload;

/// Whether the bulk data packet is a designator packet or ACK to a designator of the same segment number.
@property (nonatomic, readonly) MBBulkDataPacketType type;

/// Direction regarding origin: downlink or uplink channel
@property (nonatomic, readonly) MBBulkDataPacketDirectionType direction;

/// Timestamp relative to start of capture
@property (nonatomic, readonly) NSTimeInterval timestamp;

/// The validity of the frame is evaluated upon getting this property
@property (nonatomic, readonly) BOOL isValid;

/// Accompanying errors if the frame is deemed invalid
@property (nonatomic, strong) NSArray <NSError *> *errors;

/// Returns the string for the function group ID
@property (nonatomic, readonly) NSString *functionGroupIDString;

/// Returns  the string "UL" or "DL" depending on direction / source
@property (nonatomic, readonly) NSString *directionString;

/// Returns the string interpretation of the packet flags
@property (nonatomic, readonly) NSString *packetFlagsString;

/// Returns whether or not RST_SEG flag is set.
@property (nonatomic, readonly) BOOL isRST_SEGFlagSet;

/// Returns a string based on if there is an error. Supports missing SOS control byte, lack of payload, and payload length mismatch
@property (nonatomic, readonly) NSString *errorString;

/*!
 Initializes a bulkDataPacket
 
 @param data An NSData of the entire contents of the packet
 @param direction A MBBulkDataPacketDirectionType indicating the source of the data
 */
+ (instancetype)bulkDataPacketWithData:(NSData *)data direction:(MBBulkDataPacketDirectionType)direction andTimestamp:(NSTimeInterval)timestamp;

@end
