//
//  BSiAP2LinkTypes.h
//  BusSniffer
//
//  Created by Joe Basu on 11/28/11.
//  Copyright (c) 2011 Apple Inc. All rights reserved.
//


enum {
    BSiAP2LinkByteSOPHiByte      = 0xFF,
    BSiAP2LinkByteSOPLoByte      = 0x5A,
};

enum {
    BSiAP2LinkPacketStatusOK             = 0x00,
    BSiAP2LinkPacketStatusSingleSyncByte = 0x01,
    BSiAP2LinkPacketStatusDetectSequence,
    BSiAP2LinkPacketStatusBadACKSequence,
    BSiAP2LinkPacketStatusRequestIdentifySequence,
    BSiAP2LinkPacketStatusAccUnsupportedSequence,
    BSiAP2LinkPacketStatusStartIDPSSequence,
    BSiAP2LinkPacketStatusTooFewBytes,
    BSiAP2LinkPacketStatusSOPHiByteIncorrect,
    BSiAP2LinkPacketStatusSOPLoByteIncorrect,
    BSiAP2LinkPacketStatusInvalidLength,
    BSiAP2LinkPacketStatusInvalidType,
    BSiAP2LinkPacketStatusInvalidHeaderChecksum,
    BSiAP2LinkPacketStatusInvalidPayloadChecksum,
    BSiAP2LinkPacketStatusTooManyBytes,
    BSiAP2LinkPacketStatusMalformedSpecialByteSequence,
    BSiAP2LinkPacketStatusTooFewBytesForSYN,
    BSiAP2LinkPacketStatusIncompleteSessionDefintionInSYN,
    BSiAP2LinkPacketStatusSLPSentByAccessory,
    BSiAP2LinkPacketStatusSLPSentWithPayload,
    BSiAP2LinkPacketStatusRSTSentByAccessory,
    BSiAP2LinkPacketStatusRSTSentWithPayload,
    BSiAP2LinkPacketStatusSessionIdentifierWithoutACK,
    BSiAP2LinkPacketStatusAcknowledgementNumberWithoutACK,
    BSiAP2LinkPacketStatusMaximumPayloadDataSizeExceeded,
    BSiAP2LinkPacketStatusInvalidMaximumNumberOfOutstandingPackets,
    BSiAP2LinkPacketStatusInvalidMaximumReceivedPacketLength,
    BSiAP2LinkPacketStatusInvalidRetransmissionTimeout,
    BSiAP2LinkPacketStatusInvalidCumulativeAcknowledgementTimeout,
    BSiAP2LinkPacketStatusInvalidMaximumNumberOfRetransmissions,
    BSiAP2LinkPacketStatusInvalidMaximumCumulativeAcknowledgements,
    BSiAP2LinkPacketStatusInvalidSessionID,
    BSiAP2LinkPacketStatusInvalidLinkVersion,
    BSiAP2LinkPacketStatusInvalidSessionVersion,
    BSiAP2LinkPacketStatusNoSessionIdentifier,
    BSiAP2LinkPacketStatusUnknownSessionType,
    BSiAP2LinkPacketStatusDuplicatedSessionID,
};

typedef UInt8 BSiAP2LinkPacketStatus;

enum {
    BSiAP2LinkPacketTypeSYN      = 0x80,
    BSiAP2LinkPacketTypeACK      = 0x40,
    BSiAP2LinkPacketTypeSYNACK   = BSiAP2LinkPacketTypeSYN | BSiAP2LinkPacketTypeACK,
    BSiAP2LinkPacketTypeEAK      = 0x20,
    BSiAP2LinkPacketTypeACKEAK   = BSiAP2LinkPacketTypeACK | BSiAP2LinkPacketTypeEAK,
    BSiAP2LinkPacketTypeRST      = 0x10,
    BSiAP2LinkPacketTypeSLP      = 0x08,
};

typedef UInt8 BSiAP2LinkPacketType;
