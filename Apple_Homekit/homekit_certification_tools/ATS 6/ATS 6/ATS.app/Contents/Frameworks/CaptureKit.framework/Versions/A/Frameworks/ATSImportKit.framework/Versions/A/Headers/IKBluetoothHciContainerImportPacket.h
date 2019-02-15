//
//  IKBluetoothHciContainerImportPacket.h
//  ATSImportKit
//
//  Created by Edgard Lima on 10/17/18.
//  Copyright © 2018 Apple Inc. All rights reserved.
//

#import <ATSImportKit/ATSImportKit.h>
#import <ATSImportKit/Hci.h>

//  Borrowed form ATSPacketLoggerFileImporter.m
typedef NS_ENUM(UInt8, IKBluetoothHciContainerType) {
    IKBluetoothHciContainerTypeHciCommand = 0x00,
    IKBluetoothHciContainerTypeHciEvent = 0x01,
    IKBluetoothHciContainerTypeACLSent = 0x02,
    IKBluetoothHciContainerTypeACLReceived = 0x03,
    IKBluetoothHciContainerTypeTCICommand = 0x04,
    IKBluetoothHciContainerTypeTCIEvent = 0x05,
    IKBluetoothHciContainerTypeTCIDataSent = 0x06,
    IKBluetoothHciContainerTypeTCIDataReceived = 0x07,
    IKBluetoothHciContainerTypeSCOSent = 0x08,
    IKBluetoothHciContainerTypeSCOReceived = 0x09,
    IKBluetoothHciContainerTypeLMPSent = 0x0A,
    IKBluetoothHciContainerTypeLMPReceived = 0x0B,
    IKBluetoothHciContainerTypeErrorRawData = 0xEF, // Data, but don't attempt to decode
    
    // These are all notes and not data passed over Bluetooth
    kIKBluetoothHciContainerTypeKernel = 0xF8,
    kIKBluetoothHciContainerTypeKernelDebug = 0xF9, // Hidden in PacketLogger by default
    kIKBluetoothHciContainerTypeError = 0xFA,
    kIKBluetoothHciContainerTypePower = 0xFB,
    kIKBluetoothHciContainerTypeNote = 0xFC,
    kIKBluetoothHciContainerTypeControllerUnavailable = 0xFD,
    kIKBluetoothHciContainerTypeControllerAvailable = 0xFE,
    kIKBluetoothHciContainerTypeUndefined = 0xFF
};

@interface IKBluetoothHciContainerImportPacket : IKImportPacket

/*!
 The original length of the Hci packet
 */
@property (nonatomic, assign) UInt32 originalLength;

/*!
 The included length of the Hci packet
 */
@property (nonatomic, assign) UInt32 includedLength;

/*!
 The cumulative drops of the Hci packet
 */
@property (nonatomic, assign) UInt32 cumulativeDrops;

/*!
 The Bluetooth Hci Container Type
 */
@property (nonatomic, assign) IKBluetoothHciContainerType type;

/*!
 The header of payload of the Hci packet
 */
@property (nonatomic, assign) UInt16 headerPayloadLength;

/*!
 The payload of the Hci packet
 */
@property (strong) NSData *payload;

@end


