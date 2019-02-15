//
//  IKConstants.h
//  ATSImportKit
//
//  Created by Mark Hamlin on 7/16/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

#define IK_BIT(i) ((1<<i))
typedef enum : UInt32 {
    IKImportTransportUART = IK_BIT(1),
    IKImportTransportUSBHID = IK_BIT(2),
    IKImportTransportUSBBulkPipe = IK_BIT(3),
    IKImportTransportUSBRawPacket = IK_BIT(4),
    IKImportTransportBluetooth = IK_BIT(5),
} IKImportTransport;

typedef enum : UInt32 {
    IKImportProtocoliAP1 = IK_BIT(1),
    IKImportProtocoliAP2 = IK_BIT(2),
} IKImportProtocol;

typedef enum : UInt8 {
    IKPacketSourceAccessory = 0x00,
    IKPacketSourceAppleDevice = 0x01,
    IKPacketSourceCaptureDevice = 0x02, // For example, the app using the framework
} IKPacketSource;
