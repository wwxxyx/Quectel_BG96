//
//  ComprobeSupportConstants.h
//  ComprobeSupport
//
//  Created by Natália Cabral on 02/12/13.
//  Copyright (c) 2013 Apple. All rights reserved.
//

#define CSMaxIAPDataLengthPerFrame 127

/*!
 @const     CSBasebandHeaderLength
 @abstract  constant to represent the length of a baseband header.
 */
#define CSBasebandHeaderLength 15

/*!
 @const     CSBluetoothAddressLength
 @abstract  The Bluetooth address length.
 */
#define CSBluetoothAddressLength 6

/*!
 @const     CSL2capHeaderLength
 @abstract  constant to represent the length of a L2CAP header.
 */
#define CSL2capHeaderLength 4

/*!
 @const     CSL2capConnectionRequestHeaderLength
 @abstract  constant to represent the length of a l2cap connection request header.
 */
#define CSL2capConnectionRequestHeaderLength 12

/*!
 @const     CSL2capConnectionResponseHeaderLength
 @abstract  constant to represent the length of a l2cap connection response header.
 */
#define CSL2capConnectionResponseHeaderLength 16

/*!
 @const     CSL2capDisconnectionResponseHeaderLength
 @abstract  constant to represent the length of a l2cap disconnection response header.
 */
#define CSL2capDisconnectionResponseHeaderLength 12

/*!
 @const     CSL2capEmptyCodeHeaderLength
 @abstract  constant to represent the length of a l2cap message with only code, identifier and length.
 */
#define CSL2capEmptyCodeHeaderLength 8

/*!
 @const     CSRfcommInvalidChannelNumber
 @abstract  Constant to represent an invalid channel number. Valid rfcomm channels are 1-30, so 0xFF is well outside of that range
 */
#define CSRfcommInvalidChannelNumber 0xFF

/*!
 @const     CSRfcommControlChannelNumber
 @abstract  The RFCOMM control channel
 */
#define CSRfcommControlChannelNumber 0

/*!
 @const     CSRfcommHeaderLength
 @abstract  constant to represent the length of a RFCOMM header.
 */
#define CSRfcommHeaderLength  4

/*!
 @const     CSRfcommHeaderAndChecksumLength
 @abstract  constant to represent the length of a RFCOMM header and the checksum byte.
 */
#define CSRfcommHeaderAndChecksumLength (CSRfcommHeaderLength + 1)

/*!
 @const     CSL2capRfcommHeaderLength
 @abstract  constant to represent the length of L2CAP and RFCOMM headers.
 */
#define CSL2capRfcommHeaderLength (CSL2capHeaderLength + CSRfcommHeaderAndChecksumLength)

/*!
 @const     CSBasebandl2capRfcommHeaderLength
 @abstract  constant to represent the length of baseband, L2CAP and RFCOMM headers.
 */
#define CSBasebandl2capRfcommHeaderLength (CSBasebandHeaderLength + CSL2capHeaderLength + CSRfcommHeaderLength)

/*!
 @const     CSSdpHeaderLength
 @abstract  constant to represent the length of a SDP header.
 */
#define CSSdpHeaderLength 3

/*!
 @const     CSL2capSdpHeaderLength
 @abstract  constant to represent the length of L2CAP and SDP headers.
 */
#define CSL2capSdpHeaderLength (CSL2capHeaderLength + CSSdpHeaderLength)

/*!
 @const     CSL2capSdpServiceAttributeResponseHeaderLengthWithoutAttributeListCount
 @abstract  constant to represent the length of L2CAP and SDP Service Attribute Response headers.
 */
#define CSL2capSdpServiceAttributeResponseHeaderLengthWithoutAttributeListCount (CSL2capHeaderLength + CSSdpHeaderLength + 5)

/*!
 @const     CSBasebandl2capSdpHeaderLength
 @abstract  constant to represent the length of baseband, L2CAP and RFCOMM headers.
 */
#define CSBasebandl2capSdpHeaderLength (CSBasebandHeaderLength + CSL2capSdpHeaderLength)

/*!
 @const     CSLmpHeaderLength
 @abstract  constant to represent the length of a LMP header.
 */
#define CSLmpHeaderLength 1

/*!
 @const     CSSdpAttributeListPacketLength
 @abstract  constant to represent the minimum length of a SDP attribute list packet.
 */
#define CSSdpAttributeListPacketLength (CSSdpHeaderLength + 3)

/*!
 @const     CSSdpServiceAttributeRequestPacketLength
 @abstract  constant to represent the minimum length of a SDP Service Attribute Request packet.
 */
#define CSSdpServiceAttributeRequestPacketLength (CSSdpHeaderLength + 9)

/*!
 @const     CSSdpServiceSearchRequestPacketLength
 @abstract  constant to represent the minimum length of a SDP Service Search Request packet.
 */
#define CSSdpServiceSearchRequestPacketLength (CSSdpHeaderLength + 3)

/*!
 @const     CSSdpServiceSearchResponsePacketLength
 @abstract  constant to represent the minimum length of a SDP Service Search Response packet.
 */
#define CSSdpServiceSearchResponsePacketLength (CSSdpHeaderLength + 7)

/*!
 @const     CSSdpUUIDSize
 @abstract  constant to represent size of an SDP UUID in bytes
 */
#define CSSdpUUIDSize 16

/*!
 @const     CSSdpServiceRecordHandleSize
 @abstract  constant to represent size of an SDP service record handle
 */
#define CSSdpServiceRecordHandleSize 4

/*!
 @const     CSBasebandPacketStatusOk
 @abstract  constant to represent Baseband code for packet status OK.
 */
#define CSBasebandPacketStatusOk 0x00

/*!
 @const     CSL2capConnectionStatusOk
 @abstract  constant to represent L2CAP code for successfully connection result.
 */
#define CSL2capConnectionStatusOk 0x0000

/*!
 @const     CSL2capSignalingChannelID
 @abstract  constant to represent L2CAP channel for signaling operations.
 */
#define CSL2capSignalingChannelID 0x0001

/*!
 @enumtype  CSEmulatedChannels
 @enum      CSEmulatedRFCOMMSourceChannel
            RFCOMM source channel used by emulator comprobe device.
 @enum      CSEmulatedRFCOMMDestinationChannel
            RFCOMM destination channel used by emulator comprobe device.
 @enum      CSEmulatedIAPChannel
            L2CAP code for disconnection response.
 */
enum
{
    CSEmulatedSDPSourceChannel = 0x0115,
    CSEmulatedSDPDestinationChannel = 0x0225,
    CSEmulatedRFCOMMSourceChannel = 0x0205,
    CSEmulatedRFCOMMDestinationChannel = 0x0055,
    CSEmulatedIAPChannel = 0x02,
};
typedef UInt8 CSEmulatedChannels;

/*!
 @enumtype  CSProtocolServiceMultiplexe
 @enum      CSRfcommProtocolServiceMultiplexer
            constant to represent RFCOMM protocol/service multiplexer.
 @enum      CSSdpProtocolServiceMultiplexer
            constant to represent SDP protocol/service multiplexer.
 */
enum
{
    CSRfcommProtocolServiceMultiplexer  = 0x0003,
    CSSdpProtocolServiceMultiplexer     = 0x0001,
    CSHIDProtocolServiceMultiplexer     = 0x0011,
    CSHIDLiteProtocolServiceMultiplexer = 0x0013,
};
typedef UInt8 CSProtocolServiceMultiplexe;

/*!
 @enumtype  CSL2capFrameCode
 @enum      CSL2capConnectionRequest
            L2CAP code for connection request.
 @enum      CSL2capConnectionResponse
            L2CAP code for connection response.
 @enum      CSL2capDisconnectionResponse
            L2CAP code for disconnection response.
 */
enum
{
    CSL2capConnectionRequest = 0x02,
    CSL2capConnectionResponse = 0x03,
    CSL2capDisconnectionResponse = 0x07,
    CSL2capEchoRequest = 0x08,
    CSL2capEchoResponse = 0x09,
};
typedef UInt8 CSL2capFrameCode;

/*!
 @enumtype  CSSdpFrameCode
 @enum      CSSdpServiceAttributeResponse
            SDP code for service attribute response.
 */
enum
{
    CSSdpServiceSearchRequest = 0x02,
    CSSdpServiceSearchResponse = 0x03,
    CSSdpServiceAttributeRequest = 0x04,
    CSSdpServiceAttributeResponse = 0x05,
    CSSdpServiceSearchAttributeRequest = 0x06,
    CSSdpServiceSearchAttributeResponse = 0x07,
};
typedef UInt8 CSSdpFrameCode;

/*!
 @enumtype  CSBasebandFrameIndexes
 @enum      CSBasebandFrameClockIndex
            index where the clock is placed in the baseband frame.
 @enum      CSBasebandFrameStatusIndex
            index where the packet status is placed in the baseband frame.
 @enum      CSBasebandFrameLogicalLinkIDIndex
            index where the logical link ID is placed in the baseband frame.
 @enum      CSBasebandFrameSEQNIndex
            index where the SEQN is placed in the baseband frame.
 @enum      CSBasebandFrameL2CAPFlowIndex
            index where the L2cap flow is placed in the baseband frame.
 @enum      CSBasebandFramePayloadLengthIndex
            index where the payload length is placed in the baseband frame.
 */
enum
{
    CSBasebandFrameClockIndex = 1,
    CSBasebandFrameStatusIndex = 4,
    CSBasebandFrameLogicalLinkIDIndex = 6,
    CSBasebandFrameSEQNIndex = CSBasebandFrameLogicalLinkIDIndex,
    CSBasebandFrameL2CAPFlowIndex = CSBasebandFrameLogicalLinkIDIndex,
    CSBasebandFramePayloadLengthIndex = CSBasebandFrameLogicalLinkIDIndex,
};
typedef UInt8 CSBasebandFrameIndexes;

/*!
 @enumtype  CSBasebandLogicalLink
 @enum      CSBasebandLogicalLinkContinuation
            bit value that indicates the the payload contains the continuation of a l2cap packet
 @enum      CSBasebandLogicalLinkStart
            bit value that indicates the the payload contains the start of a l2cap packet
 @enum      basebandLogicalLMP
            bit value that indicates the the payload LMP data
 
 */
enum
{
    CSBasebandLogicalLinkContinuation = 1,
    CSBasebandLogicalLinkStart = 2,
    CSBasebandLogicalLinkLMP = 3
};
typedef UInt8 CSBasebandLogicalLink;

/*!
 @enumtype  CSLMPFrameIndexes
 @enum      CSLMPFrameOpcodeIndex
            index where the opcode is placed in the l2cap frame.
 @enum      CSLMPFrameSourceIndex
            index where the source is placed in the l2cap frame.
 @enum      CSLMPFrameOriginalOpcodeIndex
            index where the original opcode is placed in the l2cap frame.
 */
enum
{
    CSLMPFrameOpcodeIndex = 0,
    CSLMPFrameSourceIndex = CSLMPFrameOpcodeIndex,
    CSLMPFrameOriginalOpcodeIndex = 1,
};
typedef UInt8 CSLMPFrameIndexes;

/*!
 @enumtype  CSLMPOpcode
 @enum      CSLMPAcceptedOpcode
            opcode that represent LMP_accepted
 @enum      CSLMPDetachOpcode
            opcode that represent LMP_detach
 @enum      CSLMPSetupCompleteOpcode
            opcode that represent LMP_setup_complete
 @enum      CSLMPHostConnectionReqOpcode
            opcode that represent LMP_host_connection_req
 */
enum
{
    CSLMPAcceptedOpcode = 3,
    CSLMPDetachOpcode = 7,
    CSLMPSetupCompleteOpcode = 49,
    CSLMPHostConnectionReqOpcode = 51,
};
typedef UInt8 CSLMPOpcode;

/*!
 @enumtype  CSLMPFrameSource
 @enum      CSLMPFrameSourceMaster
            bit to indicate that the frame source is master
 @enum      CSLMPFrameSourceSlave
            bit to indicate that the frame source is slave
 */
enum
{
    CSLMPFrameSourceMaster = 0,
    CSLMPFrameSourceSlave = 1,
};
typedef UInt8 CSLMPFrameSource;

/*!
 @enumtype  CSL2capFrameIndexes
 @enum      CSL2capFrameLengthIndex
            index where the length is placed in the l2cap frame.
 @enum      CSL2capFrameChannelIDIndex
            index where the channel ID is placed in the l2cap frame.
 @enum      CSL2capFrameCodeIndex
            index where the code is placed in the l2cap frame.
 */
enum
{
    CSL2capFrameLengthIndex = 0,
    CSL2capFrameChannelIDIndex = 2,
    CSL2capFrameCodeIndex = 4,
};
typedef UInt8 CSL2capFrameIndexes;

/*!
 @enumtype  CSL2capRequestFrameIndexes
 @enum      CSL2capRequestFrameProtocolIndex
            index where the protocol is placed in the l2cap request frame.
 @enum      CSL2capRequestFrameSourceChannelIDIndex
            index where the source channel ID is placed in the l2cap request frame.
 */
enum
{
    CSL2capRequestFrameProtocolIndex = 8,
    CSL2capRequestFrameSourceChannelIDIndex = 10,
};
typedef UInt8 CSL2capRequestFrameIndexes;

/*!
 @enumtype  CSL2capResponseFrameIndexes
 @enum      CSL2capResponseFrameDestinationChannelIDIndex
            index where the destination channel ID is placed in the l2cap response frame.
 @enum      CSL2capResponseFrameSourceChannelIDIndex
            index where the source channel ID is placed in the l2cap response frame.
 @enum      CSL2capResponseFrameConnectionResultIndex
            index where the connection result is placed in the l2cap response frame.
 */
enum
{
    CSL2capResponseFrameDestinationChannelIDIndex = 8,
    CSL2capResponseFrameSourceChannelIDIndex = 10,
    CSL2capResponseFrameConnectionResultIndex = 12,
};
typedef UInt8 CSL2capResponseFrameIndexes;

/*!
 @enumtype  CSSdpFrameIndexes
 @enum      CSSdpFramePDUIDIndex
            index where the PDUID is placed in the SDP frame.
 @enum      CSSdpFrameTransactionIDIndex
            index where the transaction ID is placed in the SDP frame.
 @enum      CSSdpFrameAttributeListByteCountIndex
            index where the attribute list byte count is placed in the SDP frame.
 @enum      CSSdpFrameAttributeListIndex
            index where the attribute list is placed in the SDP frame.
 @enum      CSSdpFrameSearchAttributeListParameterLengthIndex
            index where the search attribute list parameter length is placed in the SDP frame.
 @enum      CSSdpFrameSearchAttributeListIndex
            index where the search attribute list is placed in the SDP frame.
 @enum      CSSdpFrameServiceRecordHandleIndex
            index of service record handle in a service attribute request
 @enum      CSSdpFrameServiceSearchPatternIndex
            index of the start of the service search pattern in a service search request
 @enum      CSSdpFrameTotalServiceRecordCountIndex
            Index of the start of the Total Service Record Count in a service search response
 @enum      CSSdpFrameCurrentServiceRecordCountIndex
            Index of the start of the Current Service Record Count in a service search response
 @enum      CSSdpFrameServiceRecordHandleListIndex
            Index of the start of the Service Record Handle List in a service search response
 */
enum
{
    CSSdpFramePDUIDIndex = 0,
    CSSdpFrameTransactionIDIndex = 1,
    CSSdpFrameAttributeListByteCountIndex = 5,
    CSSdpFrameAttributeListIndex = 7,
    CSSdpFrameSearchAttributeListParameterLengthIndex = 3,
    CSSdpFrameSearchAttributeListIndex = CSSdpFrameAttributeListByteCountIndex,
    CSSdpFrameServiceRecordHandleIndex = 5,
    CSSdpFrameServiceSearchPatternIndex = 5,
    CSSdpFrameTotalServiceRecordCountIndex = 5,
    CSSdpFrameCurrentServiceRecordCountIndex = 7,
    CSSdpFrameServiceRecordHandleListIndex = 9,
};
typedef UInt8 CSSdpFrameIndexes;

/*!
 @enumtype  CSRfcommFrameIndexes
 @enum      CSRfcommFrameChannelIndex
            index where the channel is placed in the RFCOMM frame.
 @enum      CSRfcommFramePayloadLengthIndex
            index where the payload length is placed in the RFCOMM frame.
 */
enum
{
    CSRfcommFrameChannelIndex = 0,
    CSRfcommFrameControlIndex = 1,
    CSRfcommFramePayloadLengthIndex = 2,
};
typedef UInt8 CSRfcommFrameIndexes;

/*!
 @enumtype  CSRfcommControlBytes
 @abstract  Each RFCOMM control value
 */
enum CSRfcommControlBytes : UInt8 {
    CSRfcommControlByteSABM = 0x2F,
    CSRfcommControlByteUA = 0x63,
    CSRfcommControlByteDM = 0xF,
    CSRfcommControlByteDISC = 0x43,
    CSRfcommControlByteUIH = 0xEF,
};

#define kCSRFCOMMControlByteNoPFBitMask 0xEF

/*!
 @const     serviceClassIDListAttributeID
 @abstract  constant to represent the attribute ID of the service class ID list.
 */
static const UInt8 CSServiceClassIListDAttributeID[] = {0x00, 0x01};

/*!
 @const     CSProtocolDescriptorListAttributeID
 @abstract  constant to represent the attribute ID of the protocol descriptor list.
 */
static const UInt8 CSProtocolDescriptorListAttributeID[] = {0x00, 0x04};

/*!
 @const     CSRfcommProtocolDescriptorAttributeID
 @abstract  constant to represent the attribute ID of the rfcomm protocol descriptor.
 */
static const UInt8 CSRfcommProtocolDescriptorAttributeID[] = {0x00, 0x03};


/*!
 Constant to represent the service class UUID for iAP data.
 
 @note      From AISpec-search for 0xFFCACADEAFDECADEDEFACADE00000000
 */
static const UInt8 CSIAPProtocolAccessoryUUID[] = {0x00, 0x00, 0x00, 0x00, 0xde, 0xca, 0xfa, 0xde, 0xde, 0xca, 0xde, 0xaf, 0xde, 0xca, 0xca, 0xff};

/*!
 Constant to represent the service class UUID for Apple device.
 @note      From AISpec-seach for 0xFECACADEAFDECADEDEFACADE00000000
 */
static const UInt8 CSIAPProtocolAppleDeviceUUID[] = {0x00, 0x00, 0x00, 0x00, 0xde, 0xca, 0xfa, 0xde, 0xde, 0xca, 0xde, 0xaf, 0xde, 0xca, 0xca, 0xfe};

/*!
 @const     CSHIDProtocolUUID
 @abstract  constant to represent the service UUID for HID data.
 */
static const UInt8 CSHIDProtocolUUID[] = {0x11, 0x24};

/*
 The following ID is the HIDDescriptorList Attribute ID, used in SDP packets
 */
static const UInt8 CSHIDDescriptorListAttributeID[] = {0x02, 0x06};

/*!
 @const     CSSDPServiceAttributeList
 @abstract  constant to represent the sdp service attribute list.
 */
static const UInt8 CSSDPServiceAttributeList[] = {0x36, 0x00, 0x27, 0x09, 0x00, 0x01, 0x35, 0x11, 0x1c, 0x00, 0x00, 0x00, 0x00, 0xde, 0xca, 0xfa, 0xde, 0xde, 0xca, 0xde, 0xaf, 0xde, 0xca, 0xca, 0xff, 0x09, 0x00, 0x04, 0x35, 0x0c, 0x35, 0x03, 0x19, 0x01, 0x00, 0x35, 0x05, 0x19, 0x00, 0x03, 0x08, 0x02};

/*!
 @enumtype  CSBasebandMasks
 @enum      CSBasebandPayloadLengthSecondByteMask
            mask used to obtain the second baseband payload length byte.
 */
enum  {
    CSBasebandPayloadLengthSecondByteMask = 0x07F8,
};
typedef UInt8 CSBasebandMasks;

/*!
 @enumtype  CSBitMasks
 @enum      CSLeastSignificantBitMask
            represents a mask to isolate the least significant bit.
 @enum      CSTwoLeastSignificantBitsMask
            represents a mask to isolate the two least significant bits.
 @enum      CSThreeLeastSignificantBitsMask
            represents a mask to isolate the three least significant bits.
 @enum      CSFourLeastSignificantBitsMask
            represents a mask to isolate the four least significant bits.
 @enum      CSElevenLeastSignificantBitsMask
            represents a mask to isolate the eleven least significant bits.
 @enum      CSThreeMostSignificantBitsMask
            represents a mask to isolate the three most significant bits of a byte.
 @enum      CSLeastSignificantByteMask
            represents a mask to isolate the least significant byte.
 @enum      CSSecondLeastSignificatByteMask
            represents a mask to isolate the second least significant byte.
 @enum      CSThirdLeastSignificatByteMask
            represents a mask to isolate the third least significant byte.
 */
enum  {
    CSLeastSignificantBitMask = 0x01,
    CSTwoLeastSignificantBitsMask = 0x03,
    CSThreeLeastSignificantBitsMask = 0x07,
    CSFourLeastSignificantBitsMask = 0x0F,
    CSElevenLeastSignificantBitsMask = 0x7FF,
    CSThreeMostSignificantBitsMask = 0x00E0,
    CSLeastSignificantByteMask = 0x00FF,
    CSSecondLeastSignificatByteMask = 0xFF00,
    CSThirdLeastSignificatByteMask = 0xFF0000,
};
typedef UInt8 CSBitMasks;

/*!
 @enumtype  CSShiftIndexes
 @enum      CSOneBitShift
            index to represent that one bit will be shifted.
 @enum      CSTwoBitsShift
            index to represent that two bits will be shifted.
 @enum      CSThreeBitsShift
            index to represent that three bits will be shifted.
 @enum      CSFourBitsShift
            index to represent that four bits will be shifted.
 @enum      CSFiveBitsShift
            index to represent that five bits will be shifted.
 @enum      CSOneByteShift
            index to represent that one byte will be shifted.
 @enum      CSTwoBytesShift
            index to represent that two bytes will be shifted.
 @enum      CSThreeBytesShift
            index to represent that three bytes will be shifted.
 */
enum  {
    CSOneBitShift = 1,
    CSTwoBitsShift = 2,
    CSThreeBitsShift = 3,
    CSFourBitsShift = 4,
    CSFiveBitsShift = 5,
    CSOneByteShift = 8,
    CSTwoBytesShift = 16,
};
typedef UInt8 CSShiftIndexes;

