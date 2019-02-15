//
//  L2CAPFrameConfiguration.h
//  Header for Bluetooth L2CAP Signaling Configuration
//
//  Created by Edgard Lima on 08/13/18.
//  Copyright (c) 2018 Apple. All rights reserved.
//

#import <Foundation/Foundation.h>

// Minimal MTU size. See L2CAPConfigurationTypeMTU.
#define kL2CAPConfigurationMinimalAcluMtu 48

// Default MTU size. Assumed when MTU has not been informed. See L2CAPConfigurationTypeMTU.
#define kL2CAPConfigurationDefaultAcluMtu 672

// Default RetrasmissionAndFlowControlTxWindowSize size. Assumed when RetrasmissionAndFlowControlTxWindowSize has not been informed. See L2CAPConfigurationTypeRetransmissionAndFlowControl.
#define kL2CAPConfigurationDefaultRetrasmissionAndFlowControlTxWindowSize 0

// Default RetrasmissionAndFlowControlMaxTransmit size. Assumed when RetrasmissionAndFlowControlMaxTransmit has not been informed. See L2CAPConfigurationTypeRetransmissionAndFlowControl.
#define kL2CAPConfigurationDetaultRetrasmissionAndFlowControlMaxTransmit 1

// Default RetrasmissionAndFlowControlRetransmissionTimeout size. Assumed when RetrasmissionAndFlowControlRetransmissionTimeout has not been informed. See L2CAPConfigurationTypeRetransmissionAndFlowControl.
#define kL2CAPConfigurationDefaultRetrasmissionAndFlowControlRetransmissionTimeout 0

// Default RetrasmissionAndFlowControlMonitorTimeout size. Assumed when RetrasmissionAndFlowControlMonitorTimeout has not been informed. See L2CAPConfigurationTypeRetransmissionAndFlowControl.
#define kL2CAPConfigurationDefaultRetrasmissionAndFlowControlMonitorTimeout 0

// Default RetrasmissionAndFlowControlMaximumPDUPayloadSize size. Assumed when RetrasmissionAndFlowControlMaximumPDUPayloadSize has not been informed. See L2CAPConfigurationTypeRetransmissionAndFlowControl.
#define kL2CAPConfigurationDefaultRetrasmissionAndFlowControlMaximumPDUPayloadSize 0

// Default ExtendeFlowSpecificationIdentifier size. Assumed when ExtendeFlowSpecificationIdentifier has not been informed. See L2CAPConfigurationTypeExtendedFlowSpecification.
#define kL2CAPConfigurationDefaultExtendeFlowSpecificationIdentifier 0x01

typedef NS_ENUM(UInt16, L2CAPConfigurationType) {
    /// When there is no Configutarion Options. Used to request a response.
    L2CAPConfigurationNoType = 0x00,
    /** Maximum Transmission Unit (MTU). Maximum SDU (Service Data Unit) size for the channel.
        This is an informational parameter (not negotiated) and shall be not less than kMinimalAcluMtu.
        This value is asymmetric. The sender specifed the maximum MTU it can receive.
     */
    L2CAPConfigurationTypeMTU = 0x01,
    /// Asymmetric negotiable Flush Timeout used by the sender.
    L2CAPConfigurationTypeFlushTimeout = 0x02,
    /// Asymmetric negotiable QoS (Quality of Service) used by the sender.
    L2CAPConfigurationTypeQoS = 0x03,
    /// Retransmission and Flow Control parameters (assymetric, symetric, negotiable and non-negotiable parameters).
    L2CAPConfigurationTypeRetransmissionAndFlowControl = 0x04,
    /// Fcs - Frame Check Sequence
    L2CAPConfigurationTypeFcs = 0x05,
    /// Extended flow specification. Not-negotiable.
    L2CAPConfigurationTypeExtendedFlowSpecification = 0x06,
    /// Window Size
    L2CAPConfigurationTypeExtendedWindowSize = 0x07,
};

/// Flush Timeout in miliseconds
typedef NS_ENUM(UInt16, L2CAPConfigurationFlushTimeout) {
    /// No retransmission at baseband level. The minimal polling interval is 1.25 ms. So the minimal allowed flush timeout is 2 ms (0x0002).
    L2CAPConfigurationFlushTimeoutNoRetransmission = 0x0001,
    // 0x0002 to 0xFFFE Flush Timeout used by Baseband in miliseconds.
    /// Infinity amount of retransmissions. Reliable channel.
    L2CAPConfigurationFlushTimeoutReliable = 0xFFFF,
    /// Default value assumed when not negotiated. Same as L2CAPConfigurationFlushTimeoutReliable.
    L2CAPConfigurationFlushTimeoutDefault = L2CAPConfigurationFlushTimeoutReliable
};

typedef NS_ENUM(UInt8, L2CAPConfigurationQoSServiceType) {
    /// All others values shall be ignored. No outgoing traffic is being sent.
    L2CAPConfigurationQoSServiceTypeNoTraffic = 0x00,
    L2CAPConfigurationQoSServiceTypeBestEffort = 0x01,
    /// Default value when no configuration is sent
    L2CAPConfigurationQoSServiceTypeDefault = L2CAPConfigurationQoSServiceTypeBestEffort,
    L2CAPConfigurationQoSServiceTypeGuaranteed = 0x02,
    // 0x03 - 0xFF - Reserved.
};

/// Average data rate in bytes/seconds.
typedef NS_ENUM(UInt32, L2CAPConfigurationQoSTokenRate) {
    /// Do not care. This is not allowed when L2CAPConfigurationQoSServiceTypeGuaranteed is specified.
    L2CAPConfigurationQoSTokenRateNotspecified = 0x00000000,
    /// Default value when no configuration is sent
    L2CAPConfigurationQoSTokenRateDefault = L2CAPConfigurationQoSTokenRateNotspecified,
    /// Wildcard representing the maximum token rate available. For Best Effort, means an much Bandwidth as possible. For Guaranteed, means the maximum bandwidth available at the time of the request.
    L2CAPConfigurationQoSTokenRateMaximumAvailable = 0xFFFFFFFF,
    // 0x02 - 0xFFFFFFFE - Average data rate in bytes/seconds.
};

/// bucket size in bytes
typedef NS_ENUM(UInt32, L2CAPConfigurationQoSTokenBucketSize) {
    /// No Bucket is needed. This is not allowed when L2CAPConfigurationQoSServiceTypeGuaranteed is specified.
    L2CAPConfigurationQoSTokenBucketSizeNotNeeded = 0x00000000,
    /// Default value when no configuration is sent
    L2CAPConfigurationQoSTokenBucketSizeDefault = L2CAPConfigurationQoSTokenBucketSizeNotNeeded,
    /// Wildcard representing the maximum token rate available. For Best Effort, means as big as possible. For Guaranteed, means the maximum L2CAP SDU size.
    L2CAPConfigurationQoSTokenBucketSizeMaximumAvailable = 0xFFFFFFFF,
    // 0x02 - 0xFFFFFFFE - Bucket size in bytes.
};

/// peak bandwidth in bytes per second.
typedef NS_ENUM(UInt32, L2CAPConfigurationQoSPeakBandwidth) {
    /// Don't care. This is not allowed when L2CAPConfigurationQoSServiceTypeGuaranteed is specified.
    L2CAPConfigurationQoSPeakBandwidthDontCare = 0x00000000,
    /// Default value when no configuration is sent
    L2CAPConfigurationQoSPeakBandwidthDefault = L2CAPConfigurationQoSPeakBandwidthDontCare,
    // 0x00000001 - 0xFFFFFFFF - peak bandwidth in bytes per second.
};

/// maximum acceptable delay in microseconds.
typedef NS_ENUM(UInt32, L2CAPConfigurationQoSLatency) {
    /// Don't care. This is not allowed when L2CAPConfigurationQoSServiceTypeGuaranteed is specified.
    L2CAPConfigurationQoSLatencyDontCare = 0xFFFFFFFF,
    /// Default value when no configuration is sent
    L2CAPConfigurationQoSLatencyDefault = L2CAPConfigurationQoSLatencyDontCare,
    // 0x00000001 - 0xFFFFFFFE - maximum acceptable delay in microseconds.
};

/// maximum acceptable delay variation in microseconds.
typedef NS_ENUM(UInt32, L2CAPConfigurationQoSDelayVariation) {
    /// Don't care.
    L2CAPConfigurationQoSDelayVariationDontCare = 0xFFFFFFFF,
    /// Default value when no configuration is sent
    L2CAPConfigurationQoSDelayVariationDefault = L2CAPConfigurationQoSDelayVariationDontCare,
    // 0x00000001 - 0xFFFFFFFE - maximum acceptable delay variation in microseconds.
};

typedef NS_ENUM(UInt8, L2CAPConfigurationRetrasmissionAndFlowControlMode) {
    /// Basic L2CAP, if passed all other parameter shall be ignored
    L2CAPConfigurationRetrasmissionAndFlowControlModeBasicL2CAP = 0x00,
    L2CAPConfigurationRetrasmissionAndFlowControlModeDefault = L2CAPConfigurationRetrasmissionAndFlowControlModeBasicL2CAP,
    /// Retransmission Mode. Used only for backwards compatibility is peer doesn't support L2CAPConfigurationRetrasmissionAndFlowControlModeEnhancedRetransmission
    L2CAPConfigurationRetrasmissionAndFlowControlModeRetransmission = 0x01,
    /// Flow Control Mode. Used only for backwards compatibility is peer doesn't support L2CAPConfigurationRetrasmissionAndFlowControlModeStreaming
    L2CAPConfigurationRetrasmissionAndFlowControlModeFlowControl = 0x02,
    /// Enhanced Retransmission. Should be enable if L2CAPConfigurationFlushTimeoutReliable has been requested. Shall only be sent if the peer entity has already reported to support this Extended Feature Mask in Information Response Configuration.
    L2CAPConfigurationRetrasmissionAndFlowControlModeEnhancedRetransmission = 0x03,
    L2CAPConfigurationRetrasmissionAndFlowControlModeStreaming = 0x04,
    // 0x05 - 0xFF - Reserved for future use
};

/** Fcs (Frame Check Sequence) the will be included in S/I-Frames that are sent.
    Not negotiable.
 */
typedef NS_ENUM(UInt8, L2CAPConfigurationFcs) {
    /// No Fcs. Shall only be used if sent in both directions. Otherwise default must be adopted.
    L2CAPConfigurationFcsTypeNone = 0x00,
    /// 16-bits Fcs.
    L2CAPConfigurationFcsType16 = 0x01,
    /// Default value when no configuration is sent
    L2CAPConfigurationFcsTypeDefault = L2CAPConfigurationFcsType16,
    // 0x02 - 0xFF - reserved
};

typedef NS_ENUM(UInt8, L2CAPConfigurationExtendedFlowSpecificationServiceType) {
    /// All others values shall be ignored. No outgoing traffic is being sent.
    L2CAPConfigurationExtendedFlowSpecificationServiceTypeNoTraffic = 0x00,
    L2CAPConfigurationExtendedFlowSpecificationServiceTypeBestEffort = 0x01,
    /// Default value when no configuration is sent
    L2CAPConfigurationExtendedFlowSpecificationServiceTypeDefault = L2CAPConfigurationExtendedFlowSpecificationServiceTypeBestEffort,
    L2CAPConfigurationExtendedFlowSpecificationServiceTypeGuaranteed = 0x02,
    // 0x03 - 0xFF - Reserved.
};

typedef NS_ENUM(UInt16, L2CAPConfigurationExtendedFlowSpecificationMaximumSduSize) {
    L2CAPConfigurationExtendeFlowSpecificationMaximumSduSizeMaxBandwidth = 0xFFFF,
    L2CAPConfigurationExtendeFlowSpecificationMaximumSduSizeDefault = L2CAPConfigurationExtendeFlowSpecificationMaximumSduSizeMaxBandwidth
};

typedef NS_ENUM(UInt32, L2CAPConfigurationExtendedFlowSpecificationSduInterArrivalTime) {
    L2CAPConfigurationExtendeFlowSpecificationSduInterArrivalMaxBandwidth = 0xFFFFFFFF,
    L2CAPConfigurationExtendeFlowSpecificationSduInterArrivalDefault = L2CAPConfigurationExtendeFlowSpecificationSduInterArrivalMaxBandwidth
};

typedef NS_ENUM(UInt32, L2CAPConfigurationExtendedFlowSpecificationAccessLatency) {
    L2CAPConfigurationExtendeFlowSpecificationAccessLatencyNotKnown = 0xFFFFFFFF,
    L2CAPConfigurationExtendeFlowSpecificationAccessLatencyDefault = L2CAPConfigurationExtendeFlowSpecificationAccessLatencyNotKnown
};

typedef NS_ENUM(UInt32, L2CAPConfigurationExtendedFlowSpecificationFlushTimout) {
    L2CAPConfigurationExtendeFlowSpecificationFlushTimeoutNoRetransmission = 0x00000000,
    L2CAPConfigurationExtendeFlowSpecificationFlushTimeoutDoNotDiscard = 0xFFFFFFFF,
    L2CAPConfigurationExtendeFlowSpecificationFlushTimeoutDefault = L2CAPConfigurationExtendeFlowSpecificationFlushTimeoutDoNotDiscard
};

typedef NS_ENUM(UInt16, L2CAPConfigurationExtendedWindowSizeMax) {
    /// Valid for Streaming Mode. Invalid for Enhanced Retransmission Mode.
    L2CAPConfigurationExtendeWindowSizeMaxNotSet = 0x0000,
    L2CAPConfigurationExtendeWindowSizeMaxDefault = L2CAPConfigurationExtendeWindowSizeMaxNotSet
    // 0x0001 - 0x3FFF - Valid for Enhanced Retransmission Mode. Invalid for Streaming Mode.
    // 0x4000 - 0xFFFF - Reserved.
};

@interface L2CAPConfiguration : NSObject

/**
 Initializes L2CAPConfiguration from raw data.
 
 @param data Raw data containing L2CAP Configuration to be parsed. See also property data.
 @param errors As L2CAPConfiguration is initialized and data is parsed, consistency checking are performed. A inconsistency found will be saved in errors.
 
 @return Initialized L2CAPConfiguration object.
 */
- (instancetype)initWithData:(NSData *)data errors:(NSMutableArray<NSError *> *)errors;

/// BOOL (True is Required. False is just a Hint)
@property (readonly) NSNumber *required;

/// L2CAPConfigurationType. The required bit is removed from the returned value. To know if it is required or not use the required property.
@property (readonly) NSNumber *type;

/// UInt16
@property (readonly) NSNumber *mtu;

/// L2CAPConfigurationFlushTimeout
@property (readonly) NSNumber *flushTimeout;

/// L2CAPConfigurationQoSServiceType
@property (readonly) NSNumber *qosServiceType;

/// L2CAPConfigurationQoSTokenRate. Averate data rate (Bytes/sec) with application transmits data (excluding L2CAP protocol overahead). See L2CAPConfigurationTokenRate.
@property (readonly) NSNumber *qosTokenRate;

/// L2CAPConfigurationQoSTokenBucketSize
@property (readonly) NSNumber *qosTokenBucketSize;

/// L2CAPConfigurationQoSPeakBandwidth
@property (readonly) NSNumber *qosPeakBandwidth;

/// L2CAPConfigurationQoSLatency
@property (readonly) NSNumber *qosLatency;

/// L2CAPConfigurationQoSDelayVariation
@property (readonly) NSNumber *qosDelayVariation;

/// L2CAPConfigurationRetrasmissionAndFlowControlMode
@property (readonly) NSNumber *retransmissionAndFlowControlMode;

/** Transmssion Window. 1 to 32 for Flow Control and Transmission Mode. 1 to 63 in Enhanced Retransmission Mode.
    In FlowControl and Transmission Mode, the transmiting device can send as many PDUs fit withing the window.
    In Enhanced Retransmission Mode, the transmiting device can send as many I-Frames without acknowledge.
    In is just informational and not negotiated.
    In L2CAPConfigurationRetrasmissionAndFlowControlModeStreaming this value shall be set to zero.
    UInt8.
 */
@property (readonly) NSNumber *retransmissionAndFlowControlTxWindowSize;

/** Number of transmissions of a single I-Frame that L2CAP is allow to try in Retransmissin Mode and Enhanced Retransmission Mode
    When packet is lost after MaxTransmit time, a Disconnect Request shall be sent.
    In Enhanced Retransmission, is the maximum number of I-Frames and P-Frames with P-bit set 1.
    UInt8.
 */
@property (readonly) NSNumber *retransmissionAndFlowControlMaxTransmit;

/** retransmission timeout in miliseconds.
    If the acknowledge of a I-Frame is not received in RetransmissionTimeout miliseconds, it will be restransmited.
    UInt16.
 */
@property (readonly) NSNumber *retransmissionAndFlowControlRetransmissionTimeout;

/** In Retransmission mode, is the value of miliseconds S-Frames to be transmited in Return channel when no frames are received in Forward channel.
    UInt16.
 */
@property (readonly) NSNumber *retransmissionAndFlowControlMonitorTimeout;

/// UInt16
@property (readonly) NSNumber *retransmissionAndFlowControlMaximumPDUPayloadSize;

/** Fcs the will be included in S/I-Frames that are sent.
    L2CAPConfigurationFcs.
 */
@property (readonly) NSNumber *fcsType;

/// UInt8
@property (readonly) NSNumber *extendedFlowSpecificationIdentifier;

/// L2CAPConfigurationExtendedFlowSpecificationServiceType
@property (readonly) NSNumber *extendedFlowSpecificationServiceType;

/// Rate at which application can deliver to L2CAP for transmission. L2CAPConfigurationExtendedFlowSpecificationMaximumSduSize.
@property (readonly) NSNumber *extendedFlowSpecificationMaximumSduSize;

/// Rate at which application can deliver to L2CAP for transmission. L2CAPConfigurationExtendedFlowSpecificationSduInterArrivalTime.
@property (readonly) NSNumber *extendedFlowSpecificationSduInterArrivalTime;

/// L2CAPConfigurationExtendedFlowSpecificationAccessLatency
@property (readonly) NSNumber *extendedFlowSpecificationAccessLatency;

/// L2CAPConfigurationExtendedFlowSpecificationFlushTimout
@property (readonly) NSNumber *extendedFlowSpecificationFlushTimeout;

/// Not-negotiable. L2CAPConfigurationExtendedWindowSizeMax
@property (readonly) NSNumber *extendedExtendedWindowSizeMax;

/// Just returns the raw data it has been initialized with initWithData. See initWithData.
@property (readonly) NSData *data;

@end
