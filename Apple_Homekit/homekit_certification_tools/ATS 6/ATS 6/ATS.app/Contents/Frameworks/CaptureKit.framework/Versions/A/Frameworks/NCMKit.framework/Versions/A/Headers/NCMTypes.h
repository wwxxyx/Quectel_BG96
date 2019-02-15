//
//  NCMTypes.h
//  NCMKit
//
//  Created by Mark Hamlin on 5/17/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

typedef enum : UInt8 {
    NCMSourceAccessory        = 0x00,
    NCMSourceAppleDevice      = 0x01,
    NCMSourceUnknown          = 0xFF,
} NCMSource;

#pragma pack(1) // Tightly pack
typedef struct {
    uint32_t dwSignature;
    uint16_t wHeaderLength;
    uint16_t wSequence;
    uint16_t wBlockLength;
    uint16_t wFpIndex;
} NCMTransferHeader16;
#pragma pack() // Reset


#pragma pack(1) // Tightly pack
typedef struct {
    uint32_t dwSignature;
    uint16_t wHeaderLength;
    uint16_t wSequence;
    uint32_t wBlockLength;
    uint32_t dwFpIndex;
} NCMTransferHeader32;
#pragma pack() // Reset

#pragma pack(1) // Tightly pack
typedef struct {
    uint16_t wLength;
    uint16_t bmNtbFormatsSupported;
    uint32_t dwNtbInMaxSize;
    uint16_t wNdpInDivisor;
    uint16_t wNdpInPayloadRemainder;
    uint16_t wNdpInAlignment;
    uint16_t reserved0;
    uint32_t dwNtbOutMaxSize;
    uint16_t wNdpOutDivisor;
    uint16_t wNdpOutPayloadRemainder;
    uint16_t wNdpOutAlignment;
    uint16_t reserved1;
} NCMGetNTBParametersStructure;
#pragma pack() // Reset

typedef enum : uint8_t {
    NCMRequestSetEthernetMulticastFilters = 0x40,
    NCMRequestSetEthernetPowerManagementPatternFilter = 0x41,
    NCMRequestGetEthernetPowerManagementPatternFilter = 0x42,
    NCMRequestSetEthernetPacketFilter = 0x43,
    NCMRequestGetEthernetStatistic = 0x44,
    NCMRequestGetNTBParameters = 0x80,
    NCMRequestGetNetAddress = 0x81,
    NCMRequestSetNetAddress = 0x82,
    NCMRequestGetNTBFormat = 0x83,
    NCMRequestSetNTBFormat = 0x84,
    NCMRequestGetNTBInputSize = 0x85,
    NCMRequestSetNTBInputSize = 0x86,
    NCMRequestGetMaxDatagramSize = 0x87,
    NCMRequestSetMaxDatagramSize = 0x88,
    NCMRequestGetCRCMode = 0x89,
    NCMRequestSetCRCMode = 0x9A,
} NCMRequestCode;

/*!
 @function  NCMStringFromRequestCode
 @abstract  Returns the human readable name from a request code, or just the hex value of the code if it is unknown
 */
NSString *NCMStringFromRequestCode(NCMRequestCode code);
