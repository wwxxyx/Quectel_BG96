/*
 *  BSiAPTypes.h
 *  BusSniffer
 *
 *  Created by Joe Basu on 6/13/09.
 *  Copyright © 2009 Apple Inc. All Rights Reserved.
 *
 */

typedef NS_ENUM(UInt8, BSiAPSource) {
    BSiAPSourceAccessory    = 0x00,
    BSiAPSourceAppleDevice  = 0x01,
    BSiAPSourceUnknown      = 0xFF,
};

enum {
    BSiAPTransportUART          = 0x00,
    BSiAPTransportUSBHID        = 0x01,
    BSiAPTransportBluetooth     = 0x02,
    BSiAPTransportUSBBulkPipe   = 0x03,
    BSiAPTransportUnknown       = 0xFF,
};

typedef UInt8 BSiAPTransport;

enum {
    BSiAPByteSync   = 0xFF,
    BSiAPByteSOP    = 0x55,
};

enum {
    BSiAPLingoGeneral   = 0x00,
};

enum {
    BSiAPCommandGeneralRequestIdentify          = 0x00,
    BSiAPCommandGeneralIdentify                 = 0x01,
    BSiAPCommandGeneraliPodAck                  = 0x02,
    BSiAPCommandGeneralIdentifyDeviceLingoes    = 0x13,
    BSiAPCommandGeneralStartIDPS                = 0x38,
    BSIAPCommandGeneralDetectSequence           = 0xEE,
};

enum {
    BSiAPPacketStatusOK                 = 0x00,
    BSiAPPacketStatusSingleSyncByte     = 0x01,
    BSiAPPacketStatusTooFewBytes        = 0x02,
    BSiAPPacketStatusInvalidSOP         = 0x03,
    BSiAPPacketStatusLengthMismatch     = 0x04,
    BSiAPPacketStatusInvalidChecksum    = 0x05,
    BSiAPPacketStatusInvalidSyncByte    = 0x06,
    BSiAPPacketStatusEmpty              = 0x07,

};

typedef UInt8 BSiAPPacketStatus;

enum {
    BSUSBInterfaceSubClassMFiAccessory   = 0xF0,
};

#define BSUSBInterfaceSubClassBulkiAP BSUSBInterfaceSubClassMFiAccessory

enum {
    BSUSBInterfaceProtocolBulkiAP             = 0x00,
    BSUSBInterfaceProtocolEANativeTransport   = 0x01,
};
