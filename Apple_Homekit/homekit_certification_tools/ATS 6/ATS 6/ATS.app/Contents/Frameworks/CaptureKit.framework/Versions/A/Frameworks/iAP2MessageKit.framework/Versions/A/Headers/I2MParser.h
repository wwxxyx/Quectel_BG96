//
//  I2MParser.h
//  iAP2MessageKit
//
//  Created by Joe Basu on 5/17/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <iAP2MessageKit/I2MMessage.h>

extern NSString * const I2MParserErrorDomain;

typedef enum : NSInteger {
    I2MParserErrorEmptyData,
    I2MParserErrorTooFewMessageBytes,
    I2MParserErrorFirstByteNotStartByte,
    I2MParserErrorSecondByteNotStartByte,
    I2MParserErrorMessageLengthMismatch,
    I2MParserErrorTooFewParameterBytes,
    I2MParserErrorInvalidParameterLength,
    I2MParserErrorNoMessageDefinition,
    I2MParserErrorIncorrectSource,
    I2MParserErrorParameterCountsWrong,
    I2MParserErrorUnknownEnumValue,
    I2MParserErrorObjectValue,
    I2MParserErrorUnterminatedString,
    I2MParserErrorEmptyString,
} I2MParserError;

/*!
 @class     I2MParser
 @abstract  An abstract class for message parsers
 */

@interface I2MParser : NSObject

/*!
 @method -messageFromData:source:error:
 @abstract Attempts to parse the supplied data into an I2MMessage structure
 @param data The raw message data
 @param source The source of the data (I2MSourceAccessory, I2MSourceAppleDevice)
 @param error An out parameter for storing the NSError of parsing error
 @return The resulting message object or nil on error
 */
- (I2MMessage *)messageFromData:(NSData *)data
                         source:(I2MSource)source
                          error:(NSError **)error;

@end
