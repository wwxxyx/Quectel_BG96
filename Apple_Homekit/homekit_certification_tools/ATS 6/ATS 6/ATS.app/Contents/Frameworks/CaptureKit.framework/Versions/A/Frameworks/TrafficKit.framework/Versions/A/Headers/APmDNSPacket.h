//
//  APmDNSPacket.h
//  TrafficKit
//
//  Created by Mark Hamlin on 5/30/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

@class APReportAnalyzerEntry;
#import <TrafficKit/APPacket.h>

typedef NS_ENUM(uint16_t, APDNSRecordType) {
    APDNSARecord = 0x0001,
    APDNSPTRRecord = 0x000c,
    APDNSTXTRecord = 0x0010,
    APDNSSRVRecord = 0x0021,
    APDNSAAAARecord = 0x001c,
    APDNSNSECRecord = 0x002f,
    APDNSOPTRecord = 0x0029,
    APDNSALLRecord = 0x00FF,
};

@interface APDNSResource : NSObject
- (APReportAnalyzerEntry *)reportAnalyzerEntry;
@end

@interface APDNSSRVResource : APDNSResource
@property (nonatomic, assign) uint16_t priority;
@property (nonatomic, assign) uint16_t weight;
@property (nonatomic, assign) uint16_t port;
@property (nonatomic, strong) NSString *hostName;
@end

@interface APDNSTXTResource : APDNSResource
@property (nonatomic, strong) NSDictionary *txt; // key = string, value = UTF8 string or NSData

/** Whether or not txt record contains a features flag table */
@property (nonatomic, readonly) BOOL isFeaturesFlagPresent;

/** 64 bit features flag parsed from the features string sent in txt record */
@property (nonatomic, readonly) UInt64 featuresFlag;

/** srcvers */
@property (nonatomic, readonly) double srcvers;

@end

@interface APDNSPTRResource : APDNSResource
@property (nonatomic, strong) NSString *canonicalName;
@end

@interface APDNSIPv4AddressResource : APDNSResource
@property (nonatomic, strong) NSString *ipv4Address;
@end

@interface APDNSIPv6AddressResource : APDNSResource
@property (nonatomic, strong) NSString *ipv6Address;
@end

/*!
 Protocol implemented by mDNS question and answer records.
 */
@protocol APmDNSQuerying <NSObject>

@property (nonatomic, strong) NSString *recordName;
@property (nonatomic, assign) APDNSRecordType queryRecordType;

@end

@interface APmDNSQuestion : NSObject <APmDNSQuerying>
@property (nonatomic, strong) NSString *recordName;
@property (nonatomic, assign) APDNSRecordType queryRecordType;
@property (nonatomic, assign) uint16_t questionClass;
@end

@interface APmDNSRecord : NSObject <APmDNSQuerying>
@property (nonatomic, strong) NSString *recordName;
@property (nonatomic, assign) APDNSRecordType queryRecordType;
@property (nonatomic, assign) uint16_t recordClass;
@property (nonatomic, assign) uint32_t ttl;
@property (nonatomic, assign) uint16_t resourceDataLength;
@property (nonatomic, strong) NSData *resourceData;
@property (nonatomic, assign) NSInteger resourceDataOffsetInMessage;

@property (nonatomic, strong) APDNSResource *resource; // Parsed resource, if any
@end

@interface APmDNSPacket : APPacket

@property (nonatomic, assign) uint16_t transactionID;
@property (nonatomic, assign) uint16_t flags;
@property (nonatomic, assign) uint16_t questionRRC;
@property (nonatomic, assign) uint16_t answerRRC;
@property (nonatomic, assign) uint16_t authorityRRC;
@property (nonatomic, assign) uint16_t additionalRRC;

// parsed flags
@property (nonatomic, assign) BOOL isNameServiceRequest;
@property (nonatomic, assign) BOOL isNameServiceResponse;
@property (nonatomic, assign) uint8_t operationCode;
@property (nonatomic, assign) BOOL isAuthoritativeAnswer;
@property (nonatomic, assign) BOOL hasTruncation;
@property (nonatomic, assign) BOOL recursionDesired;
@property (nonatomic, assign) BOOL recursionAvailable;
@property (nonatomic, assign) uint8_t reserved;
@property (nonatomic, assign) uint8_t returnCode;

/*!
 @property  questionRecords
 @abstract  Array of APmDNSQuestion objects
 */
@property (nonatomic, strong) NSArray <APmDNSQuestion *> *questionRecords;

/*!
 @property  answerRecords
 @abstract  Array of APmDNSRecord objects
 */
@property (nonatomic, strong) NSArray <APmDNSRecord *> *answerRecords;

/*!
 @property  authoritativeAnswerRecords
 @abstract  Array of APmDNSRecord objects
 */
@property (nonatomic, strong) NSArray <APmDNSRecord *> *authoritativeAnswerRecords;

/*!
 @property  additionalRecords
 @abstract  Array of APmDNSRecord objects
 */
@property (nonatomic, strong) NSArray <APmDNSRecord *> *additionalRecords;

/*!
 @method    nameOfRecordType:
 @abstract  Returns the name of the record type if known, or @"unknown" if not
 */
+ (NSString *)nameOfRecordType:(APDNSRecordType)recordType;

@end
