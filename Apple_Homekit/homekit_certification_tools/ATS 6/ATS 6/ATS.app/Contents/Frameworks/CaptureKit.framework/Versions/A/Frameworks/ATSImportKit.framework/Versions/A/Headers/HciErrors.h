//
//  HciErrors.h
//  Header file for HCI Importing Errors
//
//  Created by Edgard Lima on 09/12/18.
//  Copyright (c) 2018 Apple. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <ATSImportKit/ATSImportLogger.h>

typedef NS_ENUM(UInt32, HciError) {
    HciErrorWriteToFileFailed                    =     1 | ATS_LOG_LEVEL_ERROR,
    HciErrorReadFromFileFailed                   =     2 | ATS_LOG_LEVEL_ERROR,
    HciErrorReadFailedWithTooSmallFile           =     3 | ATS_LOG_LEVEL_FATAL,
    HciErrorReadFailedWithEmptyFile              =     4 | ATS_LOG_LEVEL_FATAL,
    HciErrorReadFailedWithNoMagicString          =     5 | ATS_LOG_LEVEL_FATAL,
    HciErrorReadFailedWithUnexpetedVersion       =     6 | ATS_LOG_LEVEL_FATAL,
    HciErrorReadFailedWithNotEnoughDataForRecord =     7 | ATS_LOG_LEVEL_ERROR,
    HciErrorRecordTypeIgnored                    =     8 | ATS_LOG_LEVEL_TRACE,
    HciErrorRecordTypeUnknown                    =     9 | ATS_LOG_LEVEL_ERROR,

    HciErrorAclFrameSmallerThanHeader            =   100 | ATS_LOG_LEVEL_ERROR,
    HciErrorAclPayloadNotEnoughBytes             =   101 | ATS_LOG_LEVEL_ERROR,
    HciErrorAclPayloadTooMuchBytes               =   102 | ATS_LOG_LEVEL_WARNING,
    HciErrorAclUnknownPacketBoundary             =   103 | ATS_LOG_LEVEL_ERROR,
    
    HciErrorL2capFrameSmallerThanHeader          =   200 | ATS_LOG_LEVEL_ERROR,
    HciErrorL2capContinuationWithoutStart        =   201 | ATS_LOG_LEVEL_WARNING,

    HciErrorRfcommFrameSmallerThanHeader         =   300 | ATS_LOG_LEVEL_ERROR,
    HciErrorRfcommPayloadNotEnoughBytes          =   301 | ATS_LOG_LEVEL_ERROR,
    HciErrorRfcommPayloadTooMuchBytes            =   302 | ATS_LOG_LEVEL_WARNING,
    HciErrorRfcommFcsCheckFailed                 =   303 | ATS_LOG_LEVEL_ERROR,
    
    HciErrorImportLogCountSummary                =  1000 | ATS_LOG_LEVEL_INFO,
    HciErrorImportRecordsSummary                 =  1001 | ATS_LOG_LEVEL_INFO,
    
    HciErrorBtsnoopReservedDataLink              = 10000 | ATS_LOG_LEVEL_FATAL,
    HciErrorBtsnoopDataLinkParseNotImplemented   = 10001 | ATS_LOG_LEVEL_FATAL,
};

extern NSString * const kHciErrorDomain;

@interface HciErrorHelper : NSObject

#pragma mark - File or Record errors

+ (NSError *)errorWriteToFileFailed:(NSException*)exception;

+ (NSError *)errorReadFromFileFailed:(NSException*)exception;

+ (NSError *)errorReadFailedWithTooSmallFile:(size_t)expectedBytes availableBytes:(size_t)availableBytes;

+ (NSError *)errorReadFailedWithEmptyFile;

+ (NSError *)errorReadFailedWithNoMagicString:(NSString*)magicString;

+ (NSError *)errorReadFailedWithUnexpectedVersion:(NSInteger)unexpectedVersion;

+ (NSError *)errorReadFailedWithNotEnoughDataForRecord:(size_t)expectedBytes availableBytes:(size_t)availableBytes;

+ (NSError *)errorRecordTypeIgnored:(UInt32)recordTypeIgnored;

+ (NSError *)errorRecordTypeUnknown:(UInt32)recordTypeUnknown;

#pragma mark - Hci ACL Log

+ (NSError *)errorAclFrameSmallerThanHeader:(size_t)expectedBytes availableBytes:(size_t)availableBytes;

+ (NSError *)errorAclPayloadNotEnoughBytes:(size_t)expectedBytes availableBytes:(size_t)availableBytes;

+ (NSError *)errorAclPayloadTooMuchBytes:(size_t)expectedBytes availableBytes:(size_t)availableBytes;

+ (NSError *)errorAclUnknownPacketBoundary:(UInt8)packetBoundary;

#pragma mark - L2CAP Log

+ (NSError *)errorL2capFrameSmallerThanHeader:(size_t)expectedBytes availableBytes:(size_t)availableBytes;

+ (NSError *)errorL2capContinuationWithoutStart;

#pragma mark - Rfcomm Log

+ (NSError *)errorRfcommFrameSmallerThanHeader:(size_t)expectedBytes availableBytes:(size_t)availableBytes;

+ (NSError *)errorRfcommPayloadNotEnoughBytes:(size_t)expectedBytes availableBytes:(size_t)availableBytes;

+ (NSError *)errorRfcommPayloadTooMuchBytes:(size_t)expectedBytes availableBytes:(size_t)availableBytes;

+ (NSError *)errorRfcommFcsCheckFailed:(UInt8)fcs;

#pragma mark - File or Record Info

+ (NSError *)errorImportCountSummary:(NSDictionary<NSNumber*,NSNumber*>*)summary;

+ (NSError *)errorImportRecordsSummary:(NSUInteger)recordsCount;

#pragma mark - BtSnoop log

+ (NSError *)errorBtsnoopReservedDataLink:(NSUInteger)dataLink;

+ (NSError *)errorBtsnoopDataLinkParseNotImplemented:(NSUInteger)dataLink;

@end
