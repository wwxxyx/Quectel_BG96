//
//  BtsnoopRecord.h
//  Header file for BTSnoop class for a Record.
//
//  Created by Edgard Lima on 09/12/18.
//  Copyright (c) 2018 Apple. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CaptureKit/BtsnoopTypes.h>
#import <ATSImportKit/IKBluetoothHciContainerImportPacket.h>
#import <ATSImportKit/ATSImportLogger.h>

typedef NS_ENUM(UInt32, BtsnoopDirection) {
    /// Packet was sent from Host to Controller (Sent)
    BtsnoopDirectionHostToController,
    /// Packet was sent form Controller to Host (Received)
    BtsnoopDirectionControllerToHost,
};

typedef NS_ENUM(UInt8, BtsnoopType) {
    /// A Data Packet
    BtsnoopTypeData = 0x00,
    /// A Command (Always kBtsnoopDirectionHostToController)
    BtsnoopTypeCommand = 0x01,
    /// A Event (Always kBtsnoopDirectionControllerToHost)
    BtsnoopTypeEvent = 0x02,
    /// Undefined
    BtsnoopTypeUndefined = 0xFF,
    
};

@interface BtsnoopRecord : NSObject

/// Bytes captured from Network.
@property NSUInteger originalLength;
/// Length packetData. If captured data was truncaded includedLength < originalLength.
@property NSUInteger includedLength;
/// Direction (Host to Controller or Controller to Host), and If it is Command/Event or Data.
@property NSUInteger flags;
/// number of packets lost since the first packet.
@property NSUInteger cumulativeDrops;
/// nanoseconds since January 1, 1970, when the packet arrived.
@property UInt64 timestamp;
/// Record Data
@property NSData * data;

/// The packet direction. From Host to Controller (i.e. in case of data, from host to accessory) or From Controller to Host (i.e. in case of data, from accessory to HCI device).
@property (readonly) BtsnoopDirection direction;

/// The type of the HCI packet.
@property (readonly) BtsnoopType type;

/// Creates a new BtsnoopRecord object from its properties and payload. See writeToFileHandle.
+ (instancetype)btSnoopRecordFromPayload:(NSData*)payload originalLength:(NSUInteger)originalLength includedLength:(NSUInteger)includedLength direction:(BtsnoopDirection)direction type:(BtsnoopType)type cumulativeDrops:(NSUInteger)cumulativeDrops timestamp:(UInt64)timestamp;

/// Just set all properties to zero or nil.
- (void)reset;

/// Writes a raw BtSnoop record in the file. See also writeToFileHandle.
+ (void)writeRecordToFileHandleWithData:(NSFileHandle *)fileHandle data:(NSData*)data direction:(BtsnoopDirection)direction type:(BtsnoopType)type timestampMicroseconds:(UInt64)timestampMicroseconds;

/// Writes the BtsnoopRecord object as a raw BtSnoop record in the file. See also writeRecordToFileHandleWithData.
- (void)writeToFileHandle:(NSFileHandle *)fileHandle;

/// may throw exception if file is not consistent
+ (instancetype)btsnoopRecordFromFileHandle:(NSFileHandle*)fileHandle readBytesPtr:(NSUInteger*)readBytesPtr log:(ATSImportLogger*)log;

/// Converts from BtSnoop Hci packet type into Hci packet type. This mighty be useful to create a IKImportPacket or HciContainerEvent objects while importing from BtSnoop.
+ (IKBluetoothHciContainerType)btsnoopRecordTypeToHciContainerType:(BtsnoopType)btsnooType accessoryIsSender:(BOOL)accessoryIsSender source:(IKPacketSource)source;

/// Converts from Hci packet Type into BtSnoop Hci packet. This mighty be useful to create a BtsnoopRecord object while exporting HciContainerEvent objects to BtSnoop.
+ (BtsnoopType)hciContainerTypeToBtsnoopRecordType:(IKBluetoothHciContainerType)hciContainerType;

@end
