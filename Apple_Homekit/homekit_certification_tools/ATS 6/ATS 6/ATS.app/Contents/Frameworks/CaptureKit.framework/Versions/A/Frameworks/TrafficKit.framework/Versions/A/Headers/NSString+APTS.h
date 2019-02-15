//
//  NSString+APTS.h
//  APTS
//
//  Created by Mark Hamlin on 9/7/12.
//  Copyright (c) 2012 Apple. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface NSString (APTS)

/*!
 @method        airPlayServiceNameMACAddress
 @abstract      Returns the MAC address portion of an AirPlay service name
 @discussion    MACADDRESS@DeviceName, this will return @"MACADDRESS"
 @return        MAC address, or nil on failure
 */
- (NSString *)airPlayServiceNameMACAddress;

/*!
 @method        airPlayServiceNameDeviceName
 @abstract      Returns the Device Name portion of an AirPlay service name
 @discussion    MACADDRESS@DeviceName, this will return @"DeviceName"
 @return        Device Name, or nil on failure
 */
- (NSString *)airPlayServiceNameDeviceName;

/*!
 @method    stringWithUUID
 @abstract  Create a UUID
 @return    UUID, from CF
 */
+ (NSString *)stringWithUUID;

/*!
 @method    stringWithDuration:
 @abstract  Converts a duration in seconds to a human readable string, "x h x m x s"
 */
+ (NSString *)stringWithDuration:(double)durationInSeconds;
+ (NSString *)stringWithDuration:(double)durationInSeconds includeMilliseconds:(BOOL)includeMilliseconds includeMicroseconds:(BOOL)includeMicroseconds;

/*!
 @method    stringWithElapsedTime:
 @abstract  Converts an elapsed time to a human readable format as hours:minutes:seconds
 */
+ (NSString *)stringWithElapsedTime:(double)timeInSeconds;

/*!
 @method    sha256HexDigest
 @abstract  Generates the sha256 digest of the receiver (as utf-8), and returns the hex representation
 */
- (NSString *)sha256HexDigest;

/*!
 @method    stringWithHumanReadableBytesSize:
 @abstract  Converts a number of bytes (eg. 1024) into a human readable string (eg. 1KB)
 */
+ (NSString *)stringWithHumanReadableBytesSize:(NSUInteger)numBytes;

@end

@interface NSMutableAttributedString (APTS)

/*!
 @method    appendFormat:
 @abstract  Appends the format string
 */
- (void)appendFormat:(NSString *)format, ... __attribute__((deprecated)); // FUTURE: Update APTS to use TKAttributedString instead

/*!
 @method    appendColor:format:
 @abstract  Appends the format string with the given color
 */
- (void)appendColor:(NSColor *)color format:(NSString *)format, ... __attribute__((deprecated)); // FUTURE: Update APTS to use TKAttributedString instead

/*!
 @method    escapeStringFromFormat:
 @abstract  Escapes a string in the style suitable to be passed to formattedStringWithFormat:
 */
+ (NSString *)escapeStringFromFormat:(NSString *)format, ... __attribute__((deprecated)); // FUTURE: Update APTS to use TKAttributedString instead

/*!
 @method    formattedStringWithFormat:
 @abstract  Parsers format string and generates an NSMutabelAttributedString
 */
+ (NSMutableAttributedString *)formattedStringWithFormat:(NSString *)format, ... __attribute__((deprecated)); // FUTURE: Update APTS to use TKAttributedString instead

// Convenience macros for +escapeStringWithFormat and +formattedStringWithFormat
// Example usage: AP_FMT(@"Hello, <red>%@</red>", AP_ESC(@"</red> Not red! HA! <red>")); // Joke's on you
// This line above will create the black text "Hello, ", followed by the red text "</red> Not red! HA! <red>", to give an example of escaping
// user input. As you can see, with the AP_ESC wrapper, "Not Red! HA!" would not be red. However, the tags were escaped.
//
//  Available tags:
//      <red> : Make enclosed text red
//      </red>: Remove the color attribute
//
//      <green> : Make enclosed text green
//      </green>: Remove the color attribute
//
//      <blue> : Make enclosed text blue
//      </blue>: Remove the color attribute
//
//      <yellow> : Make enclosed text yellow
//      </yellow>: Remove the color attribute
//
//      <orange> : Make enclosed text orange
//      </orange>: Remove the color attribute
//
//      <gray> : Make enclosed text gray
//      </gray>: Remove the color attribute
//
//      <under> : Make the enclose text underscored
//      </under>: Remove the underscore attribute
//
//      <strike>: Make the enclosed text strikethrough
//      </strike>: Remove the strikethrough attribute
//
#define AP_ESC(s, ...) [NSMutableAttributedString escapeStringFromFormat:s, ##__VA_ARGS__]
#define AP_FMT(s, ...) [NSMutableAttributedString formattedStringWithFormat:s, ##__VA_ARGS__]

@end
