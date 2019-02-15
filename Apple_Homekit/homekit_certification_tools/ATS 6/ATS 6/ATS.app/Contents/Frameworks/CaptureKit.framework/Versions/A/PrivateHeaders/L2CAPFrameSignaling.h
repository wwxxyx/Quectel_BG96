//
//  L2CAPFrameSignaling.h
//  Header for Bluetooth L2CAP Signaling
//
//  Created by Edgard Lima on 08/13/18.
//  Copyright (c) 2018 Apple. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CaptureKit/L2CAPFrame.h>
#import <CaptureKit/L2CAPTypes.h>
#import <CaptureKit/L2CAPConfiguration.h>

typedef NS_ENUM(UInt8, L2CAPSignalingCode) {
    L2CAPSignalingCodeReserved = 0x00,
    /// Sent in response to a command with a unknown command code. "Core_V3.0 + HS.pdf" pg 1220.
    L2CAPSignalingCodeCommandReject = 0x01,  // Data (Optional)
    /// Sent to create a L2CAP channel beteween two devices. "Core_V3.0 + HS.pdf" pg 1221.
    L2CAPSignalingCodeConnectionRequest = 0x02,
    /// Shall be sent in response to Connection Request. "Core_V3.0 + HS.pdf" pg 1223.
    L2CAPSignalingCodeConnectionResponse = 0x03,
    /// Sent to establish an initial Logical Link transmission contract beteween two L2CAP entities. "Core_V3.0 + HS.pdf" pg 1224.
    L2CAPSignalingCodeConfigureRequest = 0x04,  // See configuration options Data (Optional)
    /// Sent in response to Configure Request. "Core_V3.0 + HS.pdf" pg 1226.
    L2CAPSignalingCodeConfigureResponse = 0x05,
    /// Required to terminate a L2CAP channel. "Core_V3.0 + HS.pdf" pg 1228.
    L2CAPSignalingCodeDisconnectionRequest = 0x06,
    /// Sent in response to a disconnection request. "Core_V3.0 + HS.pdf" pg 1229.
    L2CAPSignalingCodeDisconnectionResponse = 0x07,
    /// Sent to request a response. "Core_V3.0 + HS.pdf" pg 1230.
    L2CAPSignalingCodeEchoRequest = 0x08,  // Data (Optional)
    /// Sent in response to a Echo Request. "Core_V3.0 + HS.pdf" pg 1230.
    L2CAPSignalingCodeEchoResponse = 0x09,  // Data (Optional)
    /// Sent to request implementation specific information. "Core_V3.0 + HS.pdf" pg 1230.
    L2CAPSignalingCodeInformationRequest = 0x0A,
    /// Sent in response to Information Requesnt. "Core_V3.0 + HS.pdf" pg 1231.
    L2CAPSignalingCodeInformationResponse = 0x0B,  // Data (Optional)
    /// Sent to create a L2CAP channel beteween two devices. "Core_V3.0 + HS.pdf" pg 1234.
    L2CAPSignalingCodeCreateChannelRequest = 0x0C,
    /// Sent in response to Create Channel Request. "Core_V3.0 + HS.pdf" pg 1235.
    L2CAPSignalingCodeCreateChannelResponse = 0x0D,
    /// Sent to move an existing L2CAP channel from a physical link on on Controller to another Controller. "Core_V3.0 + HS.pdf" pg 1236.
    L2CAPSignalingCodeMoveChannelRequest = 0x0E,
    /// Sent in response to Move Channel Request. "Core_V3.0 + HS.pdf" pg 1237.
    L2CAPSignalingCodeMoveChannelResponse = 0x0F,
    /// Sent to confirm a pending Move Channel Request. "Core_V3.0 + HS.pdf" pg 1238.
    L2CAPSignalingCodeMoveChannelConfirmationRequest = 0x10,
    /// Sent in response to Move Channel Confirmation Request. "Core_V3.0 + HS.pdf" pg 1239.
    L2CAPSignalingCodeMoveChannelConfirmationResponse = 0x11,
    /// It is not in Bluetooth specification 3.0
    L2CAPSignalingCodeConnectionParameterUpdateRequest = 0x12,
    /// It is not in Bluetooth specification 3.0
    L2CAPSignalingCodeConnectionParameterUpdateResponse = 0x13,
    /// It is not in Bluetooth specification 3.0
    L2CAPSignalingCodeLECreditBasedConnectionRequest = 0x14,
    /// It is not in Bluetooth specification 3.0
    L2CAPSignalingCodeLECreditBasedConnectionResponse = 0x15,
    /// It is not in Bluetooth specification 3.0
    L2CAPSignalingCodeLEFlowControlCredit = 0x16,
    // 0x17 - 0xFF - Reserved
};

typedef NS_ENUM(UInt16, L2CAPSignalingCommandRejectReason) {
    /// Command not Understood. Data is 0 length
    L2CAPSignalingCommandRejectReasonUnderstood = 0x0000,
    /// Signaling MTU Excedded. Data is 2 lenght, with the MTU the sender of this packet accepts.
    L2CAPSignalingCommandRejectReasonMTUExcedded = 0x0001,
    /// Typical channel is invalid because doesn't exist. Data is 2 bytes for local endpoint and more 2 bytes for remote channel endpoint.
    L2CAPSignalingCommandRejectReasonInvalidCID = 0x0002,
    // 0x0003 - 0xFFFF - Reserved
};

typedef NS_ENUM(UInt16, L2CAPSignalingConnectionResponseResult) {
    /// Connection Successful
    L2CAPSignalingConnectionResponseResultSuccessful = 0x0000,
    /// Connection Pending
    L2CAPSignalingConnectionResponseResultPending = 0x0001,
    /// Connection Refused - PSM not supported
    L2CAPSignalingConnectionResponseResultRefusedPsmNotSupported = 0x0002,
    /// Connection Refused - Security Block
    L2CAPSignalingConnectionResponseResultRefusedSecurityBlock = 0x0003,
    /// Connection Refused - No Resources Available
    L2CAPSignalingConnectionResponseResultRefusedNoResources = 0x0004,
    // 0x0005 is reserved
    /// Connection Refused - Invalid Source CID
    L2CAPSignalingConnectionResponseResultRefusedInvalidSource = 0x0006,
    /// Connection Refused - Source CID already allocated
    L2CAPSignalingConnectionResponseResultRefusedSourceAlreadyAllocated = 0x0007,
    // 0x0008 to 0xFFFF are Reserved.
};

typedef NS_ENUM(UInt16, L2CAPSignalingConnectionResponseStatus) {
    /// No further information available
    L2CAPSignalingConnectionResponseStatusNoInfo = 0x0000,
    /// Authentication pending
    L2CAPSignalingConnectionResponseStatusAuthenticationPending = 0x0001,
    /// Authorization pending
    L2CAPSignalingConnectionResponseStatusAuthorizationPending = 0x0002,
    // 0x0003 - 0xFFFF are reserved
};

typedef NS_ENUM(UInt16, L2CAPSignalingConfigurationResponseResult) {
    /// Success
    L2CAPSignalingConfigurationResponseResultSuccess = 0x0000,
    /// Failure – unacceptable parameters
    L2CAPSignalingConfigurationResponseResultUnacceptableParameter = 0x0001,
    /// Failure – rejected (no reason provided)
    L2CAPSignalingConfigurationResponseResultRejected = 0x0002,
    /// Failure – unknown options
    L2CAPSignalingConfigurationResponseResultUnknownOption = 0x0003,
    /// Pending
    L2CAPSignalingConfigurationResponseResultPending = 0x0004,
    /// Failure - flow spec rejected
    L2CAPSignalingConfigurationResponseResultFlowSpecRejected = 0x0005,
    //  0x0006 - 0xFFFF
};

@interface L2CAPFrameSignaling : L2CAPFrame

/// do not call this method directly. call [L2CAP buildL2CAPFrameFromFrameData] instead.
- (instancetype)initWithChannelAndFrameData:(NSNumber*)channelID frameData:(NSData *)frameData errors:(NSMutableArray<NSError *> *)errors;

/*!
 Returns L2CAP Signaling Code. This method shall only be called if channelID is L2CAPChannelIDSignaling. Please check the possible returned values in L2CAPSignalingCode
 
 @param errors If there are parsing errors, it be added to errors array paramenter. Pass nil if you don't care. If errors points to nil and there is an error, it will be allocated internally. To check if there is a parsing error, pass a non-null errors pointing to nil and after return check if it still points to nil.
 @seealso L2CAPSignalingCode
 @see channelID
 @see signalingIdentifier
 @return return the Signaling Code.
 */
/// L2CAPSignalingCode
@property (readonly) NSNumber *code;

/*!
 Returns L2CAP Signaling Identifier. This method shall only be called if channelID is L2CAPChannelIDSignaling.
 
 @param errors If there are parsing errors, it be added to errors array paramenter. Pass nil if you don't care. If errors points to nil and there is an error, it will be allocated internally. To check if there is a parsing error, pass a non-null errors pointing to nil and after return check if it still points to nil.
 @seealso L2CAPSignalingCode
 @see channelID
 @see signalingIdentifier
 @return return the Signaling Code.
 */

/// UInt8
@property (readonly) NSNumber *identifier;

/// L2CAPSignalingCommandRejectReason
@property (readonly) NSNumber *commandRejectReason;

/// L2CAPPsm
@property (readonly) NSNumber *connectionRequestPsm;

/// L2CAPChannelId
@property (readonly) NSNumber *connectionRequestSourceCID;

/// L2CAPChannelId
@property (readonly) NSNumber *connectionResponseDestinationCID;

/// L2CAPChannelId
@property (readonly) NSNumber *connectionResponseSourceCID;

/// L2CAPSignalingConnectionResponseResult
@property (readonly) NSNumber *connectionResponseResult;

/// L2CAPSignalingConnectionResponseStatus
@property (readonly) NSNumber *connectionResponseStatus;

/// L2CAPChannelId
@property (readonly) NSNumber *disconnectionResponseDestinationCID;

/// L2CAPChannelId
@property (readonly) NSNumber *disconnectionResponseSourceCID;

/// L2CAPChannelId
@property (readonly) NSNumber *configurationRequestDestinationCID;

/// UInt16
@property (readonly) NSNumber *configurationRequestFlags;

/// L2CAPChannelId
@property (readonly) NSNumber *configurationResponseSourceCID;

/// UInt16
@property (readonly) NSNumber *configurationResponseFlags;

/// L2CAPSignalingConfigurationResponseResult
@property (readonly) NSNumber *configurationResponseResult;

/// The configuration sent in L2CAPSignalingCodeConfigureRequest and L2CAPSignalingCodeConfigureResponse
@property (readonly) L2CAPConfiguration *configuration;

@end
