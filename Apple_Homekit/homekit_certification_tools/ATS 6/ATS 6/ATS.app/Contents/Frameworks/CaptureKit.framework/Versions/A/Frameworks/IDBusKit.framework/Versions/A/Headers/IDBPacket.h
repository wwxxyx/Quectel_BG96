//
//  IDBPacket.h
//  IDBusKit
//
//  Created by Kip Nicol on 10/18/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <IDBusKit/IDBTypes.h>
#import <IDBusKit/IDBPacketField.h>

/*!
 The minimum length in bytes of a valid ID bus packet.
 */
#define IDBPacketMinimumSize 2

/*!
 The maximum length in bytes of a valid ID bus packet.
 */
#define IDBPacketMaximumSize 1024

/*!
 An enumeration of errors encountered while parsing and generating ID bus packets.
 */
typedef NS_ENUM(NSInteger, IDBPacketError) {
    /*! No error. */
    IDBPacketErrorNoError = 0,
    
    /*! Unexpected byte was seen while processing. */
    IDBPacketErrorUnexpectedByte = 8000,
    
    /*! Packet size for current command is unknown. */
    IDBPacketErrorUnknownPacketSize,
    
    /*! Too many bytes were processed before a packet was created. */
    IDBPacketErrorBufferOverflow,
    
    /*! An ID Bus break occured while processing was going on. */
    IDBPacketErrorUnexpectedBreak,
    
    /*! The 8-bit CRC check failed. */
    IDBPacketErrorBadCRC,
    
    /*! A variable packet size was provided which is invalid. */
    IDBPacketErrorInvalidVariablePacketSize,
};

/*!
 IDBPacket is the class that models an ID bus packet. It provides a description of the packet and any parsing errors, and it provides access to the raw packet bytes.
 */
@interface IDBPacket : NSObject

/*!
 The command for the packet.
 */
@property (nonatomic, readonly) IDBCommand command;

/*!
 Whether or not there are enough bytes for there to be a command byte.
 */
@property (nonatomic, readonly) BOOL hasCommand;

/*!
 A data object for the packet bytes, including the command and CRC bytes.
 */
@property (nonatomic, readonly) NSData *packetData;

/*!
 A reference to the buffer containing the raw packet bytes, including the command and CRC bytes.
 */
@property (nonatomic, readonly) const UInt8 *packetBytes;

/*!
 The number of bytes in the packet, including the command and CRC bytes.
 */
@property (nonatomic, readonly) NSUInteger packetLength;

/*!
 A data object for the packet payload bytes.
 */
@property (nonatomic, readonly) NSData *payloadData;

/*!
 A reference to the buffer containing just the packet's payload bytes.
 */
@property (nonatomic, readonly) const UInt8 *payloadBytes;

/*!
 The number of bytes in the packet's payload.
 */
@property (nonatomic, readonly) NSUInteger payloadLength;

/*!
 The CRC attached to the packet.
 */
@property (nonatomic, readonly) UInt8 CRC;

/*!
 Whether or not there are enough bytes for a CRC.
 */
@property (nonatomic, readonly) BOOL hasCRC;

/*!
 The error code for the packet, indicating any parsing errors that may have occurred while creating the packet object.
 */
@property (nonatomic, readonly) IDBPacketError packetError;

/*!
 A numeric representation of the packet.
 */
@property (nonatomic, readonly) NSNumber *numberValue;

/*!
 A string representation of the packet.
 */
@property (nonatomic, readonly) NSString *stringValue;

@property (nonatomic, readonly) NSNumber *responseLength;
@property (nonatomic, readonly) BOOL variableLengthResponse;

/*!
 Source of the packet. 
 */
@property (nonatomic, readonly) IDBSource source;

/*!
 A string representation of the source of the packet.
 */
@property (nonatomic, readonly) NSString *sourceDescription;

/*!
 Creates and returns a new IDBCommandID packet using data derived from the functional variant ID value.
 
 @param functionalVariantID The ID value from which to derive the packet's paylaod.
 @return The new IDBCommandID packet.
 */
+ (instancetype)packetFromFunctionalVariantID:(IDBFunctionalVariantID)functionalVariantID;

/*!
 Creates and returns a new IDBCommandInterfaceDeviceInfo packet using data from the device info struct.
 
 @param interfaceDeviceInfo The device info struct from which to derive the packet's payload.
 @return The new IDBCommandInterfaceDeviceInfo packet.
 */
+ (instancetype)packetFromInterfaceDeviceInfo:(const IDBInterfaceDeviceInfo *)interfaceDeviceInfo;

/*!
 Creates and returns a new IDBCommandInterfaceModuleSerialNumber packet using data from the module serial number info struct.
 
 @param interfaceModuleSerialNumberInfo The module serial number info struct from which to derive the packet's payload.
 @return The new IDBCommandInterfaceModuleSerialNumber packet.
 */
+ (instancetype)packetFromInterfaceModuleSerialNumberInfo:(const IDBInterfaceModuleSerialNumberInfo *)interfaceModuleSerialNumberInfo;

/*!
 Creates and returns a new IDBCommandAccessorySerialNumber packet using data from the accessory serial number info struct.
 
 @param accessorySerialNumberInfo The accessory serial number info struct from which to derive the packet's payload.
 @return The new IDBCommandAccessorySerialNumber packet.
 */
+ (instancetype)packetFromAccessorySerialNumberInfo:(const IDBAccessorySerialNumberInfo *)accessorySerialNumberInfo;

/*!
 Creates and returns a new ID bus packet object using a copy of the given packet bytes.
 
 @param packetBytes The packet buffer that will be copied.
 @param packetLength The number of bytes in the packet buffer.
 @return The new packet object.
 */
+ (instancetype)packetWithPacketBytes:(const UInt8 *)packetBytes packetLength:(NSUInteger)packetLength;

/*!
 Creates and returns a new ID bus packet object using a copy of the given packet bytes and the packet error.
 
 @param packetBytes The packet buffer that will be copied.
 @param packetLength The number of bytes in the packet buffer.
 @param packetError The packet error type that should be used by the new packet object to indicate a parsing error.
 @return The new packet object.
 */
+ (instancetype)packetWithPacketBytes:(const UInt8 *)packetBytes packetLength:(NSUInteger)packetLength packetError:(IDBPacketError)packetError;

/*!
 Creates and returns a new ID bus packet object using a copy of the given packet bytes.
 
 @param packetBytes The packet buffer that will be copied.
 @param packetLength The number of bytes in the packet buffer.
 @return The new packet object.
 */
- (instancetype)initWithPacketBytes:(const UInt8 *)packetBytes packetLength:(NSUInteger)packetLength;

/*!
 Creates and returns a new ID bus packet object using a copy of the given packet bytes and the packet error.
 
 This is the designated initializer.
 
 @param packetBytes The packet buffer that will be copied.
 @param packetLength The number of bytes in the packet buffer.
 @param packetError The packet error type that should be used by the new packet object to indicate a parsing error.
 @return The new packet object.
 */
- (instancetype)initWithPacketBytes:(const UInt8 *)packetBytes packetLength:(NSUInteger)packetLength packetError:(IDBPacketError)packetError;

/*!
 Returns fields in the packet parsed in key/value form, as an array of IDBPacketField instances.
 */
- (NSArray *)parsedKeyValueFields;

- (NSDictionary *)fields;

- (NSNumber *)UInt8FieldAtOffset:(unsigned)offset;
- (NSNumber *)UInt8FieldAtOffset:(unsigned)offset withMask:(UInt8)mask;
- (NSNumber *)UInt16FieldAtOffset:(unsigned)offset;
- (NSData *)dataFieldWithOffset:(unsigned)offset length:(unsigned)length;
- (NSString *)CStringFieldWithOffset:(unsigned int)offset;
- (BOOL)boolFieldAtByteOffset:(unsigned)byteOffset bitOffset:(UInt8)bitOffset;

@end
