//
//  PTPParser.h
//  PTPKit
//
//  Created by Vinod Madigeri on 5/29/18.
//  Copyright © 2018 Apple. All rights reserved.
//

#import <Foundation/Foundation.h>

extern NSString * const PTPErrorDomain;

/** PTP parse errors */
typedef NS_ENUM(NSUInteger, PTPErrorType) {
    PTPErrorTypeInvalidHeaderLength,
    PTPErrorTypeInvalidPacketLength,
    PTPErrorTypeUsingReserved,
    PTPErrorTypeUnknownError,
};

/** PTP Message type */
typedef NS_ENUM(UInt8, PTPMessageType) {
    /** Sync Event Message */
    PTPMessageTypeSync = 0x00,
    /** Delay_Req Event Message */
    PTPMessageTypeDelayReq = 0x01,
    /** Pdelay_Req Event Message */
    PTPMessageTypePdelayReq = 0x02,
    /** Pdelay_Resp Event Message */
    PTPMessageTypePdelayResp = 0x03,
    /** Reserved */
    PTPMessageTypeReserved4 = 0x04,
    /** Reserved */
    PTPMessageTypeReserved5 = 0x05,
    /** Reserved */
    PTPMessageTypeReserved6 = 0x06,
    /** Reserved */
    PTPMessageTypeReserved7 = 0x07,
    /** Follow_Up General Message */
    PTPMessageTypeFollowUp = 0x08,
    /** Delay_Resp General Message */
    PTPMessageTypeDelayResp = 0x09,
    /** Pdelay_Resp_Follow_Up General Message */
    PTPMessageTypePdelayRespFollowUp = 0x0A,
    /** Announce General Message */
    PTPMessageTypeAnnounce = 0x0B,
    /** Signaling General Message */
    PTPMessageTypeSignaling = 0x0C,
    /** Management General Message */
    PTPMessageTypeManagement = 0x0D,
    /** Reserved */
    PTPMessageTypeReservedE = 0x0E,
    /** Reserved */
    PTPMessageTypeReservedF = 0x0F,
    
    // Not from spec
    
    /** Unknown */
    PTPMessageTypeUnknown = 0xFF,
};

/** PTP Control field enumuration */
typedef NS_ENUM(UInt8, PTPControlField) {
    PTPControlFieldSync = 0x00,
    PTPControlFieldDelayReq = 0x01,
    PTPControlFieldFollowUp = 0x02,
    PTPControlFieldDelayResp = 0x03,
    PTPControlFieldManagement = 0x04,
    PTPControlFieldAllOthers = 0x05,
    // PTPControlFieldReserved = 0x06-0xFF,
};

/** PTP clock accuracy enumuration */
typedef NS_ENUM(UInt8, PTPClockAccuracy) {
    PTPClockAccuracyWithin25ns = 0x20,
    PTPClockAccuracyWithin100ns = 0x21,
    PTPClockAccuracyWithin250ns = 0x22,
    PTPClockAccuracyWithin1us = 0x23,
    PTPClockAccuracyWithin2dot5us = 0x24,
    PTPClockAccuracyWithin10us = 0x25,
    PTPClockAccuracyWithin25us = 0x26,
    PTPClockAccuracyWithin100us = 0x27,
    PTPClockAccuracyWithin250us = 0x28,
    PTPClockAccuracyWithin1ms = 0x29,
    PTPClockAccuracyWithin2dot5ms = 0x2A,
    PTPClockAccuracyWithin10ms = 0x2B,
    PTPClockAccuracyWithin25ms = 0x2D,
    PTPClockAccuracyWithin100ms = 0x2C,
    PTPClockAccuracyWithin250ms = 0x2F,
    PTPClockAccuracyWithin1s = 0x30,
    PTPClockAccuracyWithin10s = 0x31,
    PTPClockAccuracyUnknown = 0xFE,
};

/** PTP Time Source enumuration */
typedef NS_ENUM(UInt8, PTPTimeSource) {
    PTPTimeSourceAtomicClock = 0x10,
    PTPTimeSourceGPS = 0x20,
    PTPTimeSourceTerrestrialRadio = 0x30,
    PTPTimeSourcePTP = 0x40,
    PTPTimeSourceNTP = 0x50,
    PTPTimeSourceHandSet = 0x60,
    PTPTimeSourceOther = 0x90,
    PTPTimeSourceInternalOscillator = 0xA0,
};

/** PTP Message class type */
typedef NS_ENUM(NSUInteger, PTPMessageClassType) {
    /** PTP Event class type */
    PTPMessageClassTypeEventClass,
    /** PTP Message class type */
    PTPMessageClassTypeGeneralClass,
};

typedef NS_ENUM(NSInteger, PTPMessageErrorCode) {
    /**  */
    PTPMessageErrorUnpackingUnimpplemented = -1,
};

#pragma pack(1) // Tightly pack
// As defined in `5.3 Data types and on-the-wire formats in a PTP system` chapter of IEEE PTP1588v2-2008 spec
#define CLOCK_IDENTITY_LENGTH 8
typedef char ClockIdentity[CLOCK_IDENTITY_LENGTH];

typedef struct {
    ClockIdentity clockIdentity;
    uint16_t portNumber;
} PortIdentity;

typedef struct {
    uint32_t lsb;
    uint16_t msb;
} UInteger48;

typedef struct {
    UInteger48 secondsField;
    uint32_t nanosecondsField;
} PTPTimestamp;

typedef struct {
    uint8_t clockClass;
    uint8_t clockAccuracy;
    uint16_t offsetScaledLogVariance;
} ClockQuality;

// As defined in `13. PTP message formats` chapter of IEEE PTP1588v2-2008 PTP spec
typedef struct {
    unsigned char transportSpecific;
    PTPMessageType messageType;
    unsigned char reserved0;
    unsigned char versionPTP;
    uint16_t messageLength;
    uint8_t domainNumber;
    char reserved1;
    uint16_t flagField;
    int64_t correctionField;
    uint32_t reserved2;
    PortIdentity sourcePortIdentity;
    uint16_t sequenceId;
    uint8_t controlField;
    int8_t logMessageInterval;
} PTPHeader;

typedef struct {
    PTPTimestamp originTimestamp;
} Sync;

typedef struct {
    PTPTimestamp originTimestamp;
} DelayReq;

typedef struct {
    PTPTimestamp receiveTimestamp;
    PortIdentity requestingPortIdentity;
} DelayResp;

typedef struct {
    PTPTimestamp preciseOriginTimestamp;
} FollowUp;

typedef struct {
    PTPTimestamp originTimestamp;
    unsigned char reserved[10];
} PdelayReq;

typedef struct {
    PTPTimestamp requestReceiptTimestamp;
    PortIdentity requestingPortIdentity;
} PdelayResp;

typedef struct {
    PTPTimestamp responseOriginTimestamp;
    PortIdentity requestingPortIdentity;
} PdelayRespFollowUp;

typedef struct {
    PTPTimestamp originTimestamp;
    int16_t currentUtcOffset;
    uint8_t reserved;
    uint8_t grandmasterPriority1;
    ClockQuality grandmasterClockQuality;
    uint8_t grandmasterPriority2;
    ClockIdentity grandmasterIdentity;
    int16_t stepsRemoved;
    uint8_t timeSource;
} Announce;

typedef struct {
    PTPHeader header;
    union {
        Sync syn;
        Announce announce;
        DelayReq delayReq;
        DelayResp delayResp;
        FollowUp followUp;
        PdelayReq pDelayReq;
        PdelayResp pDelayResp;
        PdelayRespFollowUp pDelayRespFollowUp;
    } msg;
} PTPMessage;

#pragma pack() // Reset packing to default

/**
 Method to get string description of the PTP message type
 */
NSString *stringForPTPMessageType(PTPMessageType type);

/**
 Method to get string description of the PTP class type
 */
NSString *stringForPTPMessageClassType(PTPMessageClassType classType);

/**
 Method to get string description of the PTP error type
 */
NSString *stringForPTPErrorType(PTPErrorType ptpError);

/**
 Method to get string description of the PTP Control Field
 */
NSString *stringForPTPControlField(PTPControlField controlField);

/**
 Method to get string description of the PTP Clock Accuracy
 */
NSString *stringForPTPClockAccuracy(UInt8 clockAccuracy);

/**
 Method to get string description of the Time Source
 */
NSString *stringForPTPTimeSource(UInt8 timeSource);

/**
 Method to peek into unparsed data to find the PTP message type
 */
PTPMessageType peekPTPMessageType(NSData *data);

/**
 Method to parse PTP Message from an UDP payload. Returns an array of errors
 
 @note If the return value is YES and there are errors, then the errors should be treated as warning
 */
BOOL unpackPTP(NSData *data, PTPMessage *ptpMessage, NSMutableArray<NSError *> *errors);
