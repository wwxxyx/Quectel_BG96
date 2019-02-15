//
//  BSiAPSessionTypes.h
//  BusSniffer
//
//  Created by Joe Basu on 4/3/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

enum {
    BSiAP2SessionTypeControl                     = 0,
    BSiAP2SessionTypeFileTransfer                = 1,
    BSiAP2SessionTypeExternalAccessoryProtocol   = 2,
};

typedef UInt8 BSiAP2SessionType;

enum {
    BSiAP2SessionDefinitionStatusOK                  = 0x00,
    BSiAP2SessionDefinitionStatusTooFewBytes,
    BSiAP2SessionDefinitionStatusInvalidReservedByte,
};

typedef UInt8 BSiAP2SessionDefinitionStatus;

typedef NS_ENUM(NSUInteger, BSiAP2FileTransferSessionDatagramType) {
    BSiAP2FileTransferSessionDatagramTypeSetup            = 0x04,
    BSiAP2FileTransferSessionDatagramTypeStart            = 0x01,
    BSiAP2FileTransferSessionDatagramTypeFirstData        = 0x80,
    BSiAP2FileTransferSessionDatagramTypeFirstAndOnlyData = 0xC0,
    BSiAP2FileTransferSessionDatagramTypeData             = 0x00,
    BSiAP2FileTransferSessionDatagramTypeLastData         = 0x40,
    BSiAP2FileTransferSessionDatagramTypeCancel           = 0x02,
    BSiAP2FileTransferSessionDatagramTypePause            = 0x03,
    BSiAP2FileTransferSessionDatagramTypeSuccess          = 0x05,
    BSiAP2FileTransferSessionDatagramTypeFailure          = 0x06,
    /** This is not a specified type. It is just used to mark packets with type not parsed (due to small datagram) */
    BSiAP2FileTransferSessionDatagramTypeInvalid          = 0xFF,
};

