//
//  LKTErrors.h
//  LingoKit
//
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <Foundation/Foundation.h>

extern NSString * const LKTParserErrorDomain;

enum {
    LKTParserErrorNilValueError = 997,
    LKTParserErrorAllocationError = 998,
    LKTParserErrorUnknownError = 999,
    LKTParserErrorNotEnoughPacketBytes = 1000,
    LKTParserErrorTooManyPacketBytes = 1001,
    LKTParserErrorErrorOccurred = 1002,
    LKTParserErrorValueOutOfBounds = 1003,
    LKTParserErrorReservedValue = 1004,
    LKTParserErrorMissingSyncByte = 1005,
    LKTParserErrorMissingPacketStartByte = 1006,
    LKTParserErrorUnknownLingo = 1007,
    LKTParserErrorUnknownCommand = 1008,
    LKTParserErrorInvalidChecksum = 1009,
    LKTParserErrorReservedBit = 1010,
    LKTParserErrorInvalidSource = 1011,
    LKTParserErrorUnterminatedUTF8String = 1012,
    LKTParserErrorTransactionIDViolation = 1013,
    LKTParserErrorPrematurelyTerminatedUTF8String = 1014,
    LKTParserErrorUnknownToken = 1015,
    LKTParserErrorZeroTokenCount = 1016,
    LKTParserErrorRequiredBitNotSet = 1017,
    LKTParserErrorUTF8InvalidByteSequence = 1018,
    LKTParserErrorUTF8IncompleteByteSequence = 1019,
    LKTParserErrorUTF8UnknownValidationError = 1020,
    LKTParserErrorTokensHaveErrors = 1021,
};
