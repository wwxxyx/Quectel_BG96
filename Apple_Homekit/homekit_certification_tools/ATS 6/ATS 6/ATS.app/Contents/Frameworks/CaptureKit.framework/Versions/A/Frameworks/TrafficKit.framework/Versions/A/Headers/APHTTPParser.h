//
//  APHTTPParser.h
//  APTS
//
//  Created by Mark Hamlin on 9/28/12.
//  Copyright (c) 2012 Apple. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface APHTTPParser : NSObject

/*!
 @method        nextLineFromBytes:length:
 @abstract      Fetches the next line from the bytes
 @discussion    Returns nil if the full line terminator (\r\n or \n) wasn't seen
                The return will include the terminating characters
 */
+ (NSString *)nextLineFromBytes:(const uint8_t *)bytes length:(NSUInteger)length;

/*!
 @method    parseHTTPFrameFrom:payload:initialLine:
 @abstract  Parses the next HTTP frame from the buffer and pops the frame from the start of it.
 @param     payload
            The parsed payload
 @param     initialLine
            The first line parsed. This will always have count of 3
 */
+ (NSDictionary *)parseHTTPFrameFrom:(NSMutableData *)buffer payload:(NSData **)payload initialLine:(NSArray **)initialLine;

/*!
 @method    parseHTTPHeader:
 @abstract  Parses a raw HTTP header blob, provided with the trailing \r\n
 @return    An ordered array of key-value tupes, ie. @[@[@"Content-Length", @"applicatoin/json"], @["X-Some-Key", @"SomeValue"]]
 */
+ (NSArray *)parseHTTPHeader:(NSData *)httpHeader;

@end
