//
//  ComprobeFrame.h
//  ComprobeSupport
//
//  Created by Natália Cabral on 10/09/13.
//  Copyright (c) 2013 Apple. All rights reserved.
//  Refactored by Edgard Lima on 08/07/18.
//  Copyright (c) 2018 Apple. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <ComprobeSupport/BasebandDefs.h>

@class FTDataFrame;

/*!
 @enum  ComprobeFrameStatus
 @const See the BPA 100 User Manual v4 sec 5.4 for details on
 the status constants.
 */
typedef NS_ENUM(UInt8, ComprobeFrameStatus) {
    /// A good packet has a packet header with a valid header error check (HEC) and a packet payload with a valid CRC.
    ComprobeFrameStatusSuccess = 0,
    /// If the header has a valid HEC, but the Logical Transport (LT) address in the packet header is not the one we got form the FHS packet for the connection we are sniffing, the packet status is MemberError.
    ComprobeFrameStatusMemberError = 3,
    /// If the HEC fails, the packet status is HeaderError and the firmware does not send the actual packet payload, it only sends a sniffer header containing the packet status denoting the error.
    ComprobeFrameStatusHeaderError = 4,
    /// The packet was received with a valid HEC, but the payload length in the payload header exceeds the maximum length for the packet type, it is marked as the LengthError in the PacketStatus field.
    ComprobeFrameStatusLengthError = 5,
    /// If the header has valid HEC, but the CRC is not valid, the packet status is Payload error, which is decoded as CRC error.
    ComprobeFrameStatusPayloadError = 6,
    /// The packet was received with a valid HEC, but the packet type is not one of those expected by the firmware, it is marked with “UnsupportedPacket” status.
    ComprobeFrameStatusUnsupportedPacket = 9,
};

typedef NS_ENUM(NSInteger, ComprobeFrameSource) {
    /// Frame transmited by Apple Device (e.g. iPhone)
    ComprobeFrameSourceAppleDevice,
    /// Frame transmited by Accessory (e.g. Car Play)
    ComprobeFrameSourceAccessory,
    /// Unknown source.
    ComprobeFrameSourceUnknown,
};

typedef NS_ENUM(UInt8, ComprobePacketTypeResolved) {
    ComprobePacketTypeResolvedNull,
    ComprobePacketTypeResolvedPoll,
    ComprobePacketTypeResolvedFHS,
    ComprobePacketTypeResolvedDM1,
    ComprobePacketTypeResolvedDM3,
    ComprobePacketTypeResolvedDM5,
    ComprobePacketTypeResolvedUndefined,
    ComprobePacketTypeResolvedReserved,
    ComprobePacketTypeResolvedHV1,
    ComprobePacketTypeResolvedHV2,
    ComprobePacketTypeResolvedHV3,
    ComprobePacketTypeResolvedDV,
    ComprobePacketTypeResolvedEV3,
    ComprobePacketTypeResolved2EV3,
    ComprobePacketTypeResolved3EV3,
    ComprobePacketTypeResolvedEV4,
    ComprobePacketTypeResolvedEV5,
    ComprobePacketTypeResolved2EV5,
    ComprobePacketTypeResolved3EV5,
    ComprobePacketTypeResolvedDH1,
    ComprobePacketTypeResolved2DH1,
    ComprobePacketTypeResolved3DH1,
    ComprobePacketTypeResolvedDH3,
    ComprobePacketTypeResolved2DH3,
    ComprobePacketTypeResolved3DH3,
    ComprobePacketTypeResolvedDH5,
    ComprobePacketTypeResolved2DH5,
    ComprobePacketTypeResolved3DH5,
    ComprobePacketTypeResolvedAUX1,
};

@interface ComprobeFrame : NSObject

/// Frontline file frame that enclosed this comprobe frame.
@property (nonatomic, strong) FTDataFrame *ftDataFrame;

/// Raw Data of this Comprobe frame.
@property (nonatomic, readonly) NSData *frameData;

/// Timestamp. TODO (edgard.lime): Timestamp in nano seconds.
@property (nonatomic, readonly, assign) UInt64 timestamp;

/// If this captured baseband frame has been sent by the Apple Device or by the Accessory.
@property (nonatomic, readonly, assign) ComprobeFrameSource source;

/// The length of the frame's header. A 8-bits unsigned integer, ranging from 0 to 255.
@property (readonly) NSNumber * snifferHeaderLength;

/*!
 The payload data carried by the frame, i.e. data after frame header.
 
 @note In the case of a ComprobeFrameErrorTypeFrameDataDiscarded error, the payload will still be returned. The caller has to decided if it will be treated as an error and discard the payload and was a warning and still use the returned payload.
 */
@property (readonly) NSData *payloadData;

/// The value of the comprobe's bluetooth clock. A 28-bits unsigned integer.
@property (readonly) NSNumber *bluetoothClock;

/// The status of the frame. A 4-bits unsigned integer. See ComprobeFrameStatus.
@property (readonly) NSNumber *packetStatus;

/// The Active Member address (Logical Transport Address) of the frame. A 3-bits unsigned integer ranging from 0 to 7.
@property (readonly) NSNumber *amAddress;

/// The raw integer type of the frame.  A 16-bits unsigned integer, ranging from 0 to 15. See also packetTypeResolved.
@property (readonly) NSNumber *packetType;

/// The type of the frame as in ComprobePacketTypeResolved. A 8-bits unsigned integer. Resolved based on the channelType. If an error os set  ComprobePacketTypeResolvedUnknown will be returned. If it is  ComprobePacketTypeResolvedReserved you may want to raise an error. See also packetType.
@property (readonly) NSNumber *packetTypeResolved;

/**
 The flow control flag at Baseband layer. A 1-bit unsigned integer.
 
 @seealso kBasebandLinkFlowGo
 @seealso kBasebandLinkFlowStop.
 */
@property (readonly) NSNumber *linkFlow;

/// The arqn (Automatic Repeat Request) of the frame. A 1-bit unsigned integer.
@property (readonly) NSNumber *arqn;

/// The seqn of the frame. For each new Baseband frame sent by a peer this value will alternate beteween 0 and 1. A 1-bit unsigned integer.
@property (readonly) NSNumber *seqn;

/// The channel type of the frame. Logical Link ID (LLID). A 2-bits unsigned integer. See BasebandChannelType.
@property (readonly) NSNumber *channelType;

/// The flow control flag at L2CAP layer.  A 1-bit unsigned integer. See kBasebandL2CAPFlowGo and kBasebandL2CAPFlowStop.
@property (readonly) NSNumber *l2capFlow;

/// The length of the Comprobe Payload. i.e. The payloadLength reported by packetLength field in Comprobe header. It should be totalFrameLength - headerLength but may not. A 16-bis unsigned integer.
@property (readonly) NSNumber *packetLength;

/// The test mode of the comprobe. A 1-bit unsigned integer.
@property (readonly) NSNumber *testMode;

/// The timestamp taken from the frame's header. A 32-bis unsigned integer. TODO (edgard.lima): Time since when?
@property (readonly) NSNumber *frameHeaderTimestamp;

/// bytFrequencyOffset. A 8-bits unsigned integer. TODO (edgard.lima) What is bytFrequencyOffset?
@property (readonly) NSNumber *bytFrequencyOffset;

/// bytSequenceNumber. A 8-bits unsigned integer. TODO (edgard.lima) What is bytSequenceNumber?
@property (readonly) NSNumber *bytSequenceNumber;

/// bytInfoByte. A 8-bits unsigned integer. TODO (edgard.lima) What is bytInfoByte?
@property (readonly) NSNumber *bytInfoByte;

/// The decryption status of the frame. A 1-bit unsigned integer.
@property (readonly) NSNumber *decryptionStatus;

/// The role switch info of the frame. A 1-bit unsigned integer.
@property (readonly) NSNumber *roleSwitchInfo;

/// The strengt of the signal. A 4-bits unsigned integer. TODO (edgard.lima): How to convert it to DBM.
@property (readonly) NSNumber *signalStrength;

/// bytRFU1. A 8-bits unsigned integer. TODO (edgard.lima) What is bytRFU1?
@property (readonly) NSNumber *bytRFU1;

/// Returns if it is a L2CAP frame. i.e. channelType is BasebandChannelTypeBasebandLogicalLinkContinuation or is BasebandChannelTypeBasebandLogicalLinkStartOrNoFragmentation.
@property (readonly) BOOL isL2cap;

/// Returns if it is L2CAP No Fragmentation, i.e. channelType is BasebandChannelTypeBasebandLogicalLinkStartOrNoFragmentation and the L2CAP payload fits into it.
@property (readonly) BOOL isL2capNoFragmentation;

/// If it is L2CAP. Returns the payload length. For L2CAP Continuation, there is no header and everything is payload.
@property (readonly) UInt16 l2capPayloadLength;

/// The index number to use in the frame description. 
@property (nonatomic) NSUInteger dataFrameIndex;

/*!
 Inits a ComprobeFrame with the specified data, timestamp and source properties.
 
 @param data with header and payload.
 
 @param source if the packet is sent form Apple device to a Accessory or the other direction around.
 @param timestamp Absolute timestamp in nano-seconds, since the Epoch (1/1/1970) of the packet.
 
 @return     A new ComprobeFrame with the specified data, timestamp and source properties.
 */
- (instancetype)initWithData:(NSData *)data source:(ComprobeFrameSource)source timestamp:(UInt64)timestamp;

/**
 Emulates a ComprobeFrame from payload Data and few mininal fields of the header.
 
 @param payloadData Usually the payload of a comprobre frame, i.e. the frame withou the header.
 @param headerPayloadLength the lenght of the payloadData reported by the header. Usually it will be the same as payloadData.lenght.
 @param seqn Baseband seqn.
 @param channelType, if it is L2CAP (no frag/start/cont) or LMP.
 @param source if the packet is sent form Apple device to a Accessory or the other direction around.
 @param timestamp Absolute timestamp in nano-seconds, since the Epoch (1/1/1970) of the packet.
 
 @return  A new ComprobeFrame with the specified payloadData, timestamp and source properties.
 */
- (instancetype)initWithPayloadData:(NSData *)payloadData headerPayloadLength:(UInt16)headerPayloadLength seqn:(uint8_t)seqn channelType:(BasebandChannelType)channelType source:(ComprobeFrameSource)source timestamp:(UInt64)timestamp;

/// Errors if any. See error ComprobeFrameErrorTypeFrameDataDiscarded that might be returned. You may actually want to ignore such error or treat it as warning.
@property (nonatomic, strong) NSArray<NSError *> *errors;

@end
