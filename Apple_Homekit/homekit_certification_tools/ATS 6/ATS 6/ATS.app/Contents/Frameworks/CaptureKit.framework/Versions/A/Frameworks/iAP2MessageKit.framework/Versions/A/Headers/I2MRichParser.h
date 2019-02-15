//
//  I2MRichParser.h
//  iAP2MessageKit
//
//  Created by Joe Basu on 5/16/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <iAP2MessageKit/I2MParser.h>
#import <iAP2MessageKit/I2MMessage.h>

@class I2MRichParser;

/** I2MRichParserDataSource is a data source protocol for the iAP2 message parser. Data sources provide buffers to be parsed into message data structures. */
@protocol I2MRichParserDataSource <NSObject>

/**
 dataSource should return YES if it wants to provide an aux objectValue for the current message. If NO is returned, auxiliaryObjectValueForParser:parameter:message:context:errorFound:errorString: will not be called.
 
 @param parser The I2MRichParser calling the method.
 @param message The message object to be parsed.
 @param context Arbitrary data that is passed to the parser in messageFromData:source:context:error
 @return YES if an auxiliary object is available; NO otherwise.
 */
- (BOOL)auxiliaryObjectValuesAvailableForParser:(I2MRichParser *)parser message:(I2MMessage *)message context:(void *)context;

/**
 If the data source returns has an available object value for the current parameter, it may return that value. The object will be set to the I2MParameter's objectValue property.
 
 @param parser The I2MRichParser calling the method.
 @param parameter The current parameter to set the object value for.
 @param message The message object to be parsed.
 @param context Arbitrary data that is passed to the parser in messageFromData:source:context:error
 @param errorFound If an error occurred while parsing the parameter's data, set this to YES. This will fail the I2MMessage's parsing.
 @param errorString If an error occured while parsing the parameter's data, set this to a string describing why the parsing failed. This will get set to the I2MMessage's error object. errorFound must also be set.
 @return The auxiliary object, or nil if none is available.
 */
- (id)auxiliaryObjectValueForParser:(I2MRichParser *)parser parameter:(I2MParameter *)parameter message:(I2MMessage *)message context:(void *)context errorFound:(BOOL *)errorFound errorString:(NSString **)errorString;

@end

/** A message parser that uses message definition to mark parameters with types and names, perform error checks, and supports group parameters. */
@interface I2MRichParser : I2MParser

/** Data source object conforming to I2MRichParserDataSource. Will get sent the messages in the I2MRichParserDataSource protocol. */
@property (nonatomic, weak) id<I2MRichParserDataSource> dataSource;

/**
 Creates and returns a parser using the provided array of message definitions.
 
 @param messageDefinitions An array of I2MMessageDefinitions.
 @return The newly created and initialized message parser object.
 */
+ (instancetype)parserWithMessageDefinitions:(NSArray *)messageDefinitions;

/**
 Initializes a parser.
 
 @param messageDefinitions An array of I2MMessageDefinitions.
 @return The initialized message parser object.
 */
- (instancetype)initWithMessageDefinitions:(NSArray *)messageDefinitions;

/*!
 Parses the given data object and returns an iAP2 message if possible.
 
 @param data The data to parse into a message.
 @param source The source of the data.
 @param context Arbitrary data that will get passed to the I2MRichParserDataSource dataSource's methods.
 @param error Contains the error object if there was a problem parsing the message.
 @param errorIsRecoverable A pointer to a bool that will be set to YES if an error is encountered but the message can still be delivered to the rule engine.
 @return The parsed iAP2 message, or nil if the data buffer didn't contain enough data for an entire message.
 */
- (I2MMessage *)messageFromData:(NSData *)data source:(I2MSource)source context:(void *)context error:(NSError **)error errorIsRecoverable:(BOOL *)errorIsRecoverable;

@end
