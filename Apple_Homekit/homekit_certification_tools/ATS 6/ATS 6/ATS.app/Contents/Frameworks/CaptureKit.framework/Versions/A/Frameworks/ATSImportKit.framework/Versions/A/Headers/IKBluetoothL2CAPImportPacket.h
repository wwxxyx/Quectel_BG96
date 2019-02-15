//
//  IKBluetoothL2CAPImportPacket.h
//  ATSImportKit
//
//  Created by Edgard Lima on 10/17/18.
//  Copyright © 2018 Apple Inc. All rights reserved.
//

#import <ATSImportKit/ATSImportKit.h>

@interface IKBluetoothL2CAPImportPacket : IKImportPacket

/*!
 The payload of the L2CAP packet
 */
@property (strong) NSData *frameData;

@end
