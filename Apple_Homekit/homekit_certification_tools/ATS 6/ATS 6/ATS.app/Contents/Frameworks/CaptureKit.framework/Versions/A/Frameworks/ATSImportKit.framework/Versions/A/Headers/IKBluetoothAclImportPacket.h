//
//  IKBluetoothAclImportPacket.h
//  ATSImportKit
//
//  Created by Edgard Lima on 10/17/18.
//  Copyright © 2018 Apple Inc. All rights reserved.
//

#import <ATSImportKit/ATSImportKit.h>
#import <ATSImportKit/Hci.h>

@interface IKBluetoothAclImportPacket : IKImportPacket

/*!
 The main data of the Acl packet (header + payload)
 */
@property (strong) NSData *frameData;

/*!
 The header payload length
 */
@property (nonatomic, assign) UInt16 headerPayloadLength;

/*!
 The payload of the Acl packet
 */
@property (strong) NSData *payload;

/*!
 The identification of the packet boundary L2CAP PDU
 */
@property (nonatomic, assign) AclPL2capPacketBoundary packetBoundary;

@end
