//
//  NSString+ATSAdditions.h
//  ATS
//
//  Created by Joe Basu on 8/20/09.
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <Foundation/Foundation.h>

@interface NSString (ATSAdditions)

/*!
 Returns a copy of the receiver where the first letter has been capitalized.
 
 @return The capitalized version of the receiver.
 */
- (NSString *)firstCharacterCapitalizedString;

/*!
 Returns a copy of the receiver where the first letter is lower-case.
 
 @return The lower-case version of the receiver.
 */
- (NSString *)firstCharacterLowercaseString;

/*!
 Returns a flag indicating whether or not the receiver contains the given string as a substring.
 
 @param  string The substring to search the receiver for.
 @return YES if the receiver contains the substring; NO otherwise.
 */
- (BOOL)containsString:(NSString *)string;

/*!
 Creates and returns a UUID.
 
 @return UUID, from CFUUID.
 */
+ (NSString *)stringWithUUID;

/*!
 Returns the number of occurrences of substring in receiver.
 
 @param  substring The substring to search the receiver for.
 @return The number of instances of the substring.
 */
- (NSUInteger)countOccurrencesOfSubstring:(NSString *)substring;

/*!
 Returns the description in current units for the passed timestamp.
 
 @param  timestamp The timestamp for which to return a description.
 @return The timestamp description string.
 */
+ (NSString *)descriptionForTimestamp:(UInt64)timestamp;

/*!
 Returns the string in current units for the passed timestamp.
 
 @param  timestamp The timestamp for which to return a string.
 @return The string for the given timestamp.
 */
+ (NSString *)stringForTimestamp:(UInt64)timestamp;

/*!
 Returns an NSData object representing the byte form of the string's hex representation.
 
 @return The data representation of the receiver.
 */
- (NSData *)dataFromHexidecimalRepresentation;

/**
 Returns a copy of the receiver with all characters from the given set removed.
 
 @return The filtered string.
 */
- (NSString *)stringByRemovingCharactersInSet:(NSCharacterSet *)characterSet;

/**
 Returns the original word if 1 is passed, or the pluralized word if anything else is passed.
 
 @return The properly pluralized word.
 */
- (NSString *)pluralizeWithCount:(NSUInteger)number;


@end
