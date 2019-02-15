//
//  IKBluetoothRFCOMMImportPacket.h
//  ATSImportKit
//
//  Created by Mark Hamlin on 5/8/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import "IKImportPacket.h"

@interface IKBluetoothRFCOMMImportPacket : IKImportPacket

/*!
 @property  bluetoothImportPacketRFCOMMPayload
 @abstract  The payload of the rfcomm packet
 */
@property (nonatomic, retain) NSData *rfcommPayload;

@end
