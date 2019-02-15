//
//  Rfcomm.h
//  Rfcomm Types
//
//  Created by Edgard Lima on 08/13/18.
//  Copyright (c) 2018 Apple. All rights reserved.
//

#pragma pack(push,1)
#ifdef __LITTLE_ENDIAN__  // Usually bit and byte endieness are the same
typedef struct {
    uint8_t ea:1;
    uint8_t cr:1;
    uint8_t d:1;
    uint8_t serverChannel:5;
} RfcommAddressField;

typedef union {
    struct {
        uint8_t l:4;
        uint8_t pf:1;
        uint8_t h:3;
    } f;
    uint8_t value;
} RfcommControlField;

typedef struct {
    uint8_t ea:1;
    uint8_t value:7;
} RfcommShortLengthField;

typedef struct {
    RfcommAddressField addr;
    RfcommControlField control;
    RfcommShortLengthField length;
} RfcommShortHeader;

typedef struct {
    uint16_t ea:1;
    uint16_t value:15;
} RfcommLongLengthField;

typedef struct {
    RfcommAddressField addr;
    RfcommControlField control;
    RfcommLongLengthField length;
} RfcommLongHeader;

// From https://www.etsi.org/deliver/etsi_ts/101300_101399/101369/06.05.00_60/ts_101369v060500p.pdf page 17 Frame Type for Control field. And page 43.
typedef NS_ENUM(uint8_t, RfcommFrameType) {
    /// Set Asynchronous Balanced Mode
    RfcommFrameTypeSabm = 0x2f,
    /// Unnumbered Acknowledgement
    RfcommFrameTypeUa = 0x63,
    /// Disconnected Mode
    RfcommFrameTypeDm = 0x0f,
    /// Disconnect
    RfcommFrameTypeDisc = 0x43,
    /// Unnumbered Information with Header check
    RfcommFrameTypeUih = 0xef,
    /// Unnumbered Information
    RfcommFrameTypeUi = 0x03,
    /// Information (error recovery mode)
    RfcommFrameTypeI = 0x00,
    /// Receive Ready (error recovery mode)
    RfcommFrameTypeRr = 0x01,
    /// Receive Not Ready (error recovery mode)
    RfcommFrameTypeRnr = 0x05,
    /// Reject (error recovery mode)
    RfcommFrameTypeRej = 0x09,
};

#else
#error "HCI structures haven't been defined for Big-endian."
#endif
#pragma pack(pop)
