//
//  NSData+APTS.h
//  APTS
//
//  Created by Mark Hamlin on 10/17/12.
//  Copyright (c) 2012 Apple. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface NSData (APTS)

/*!
 @method    base64EncodedString
 @abstract  Encodes data into a base64 string
 */
- (NSString *)base64EncodedString;

/*!
 @method    sha256HexDigest
 @abstract  Returns the sha256 digest, hex encoded
 */
- (NSString *)sha256HexDigest;

/*!
 @method    md5HexDigest
 @abstract  Returns the md5 digest, hex encoded
 */
- (NSString *)md5HexDigest;

/*!
 @method    bestEffortASCIIStringDiscardingInvalidBytes
 @abstract  Returns an ascii NSString representation of the receiver, where bytes which couldn't be converted are left out entirely
 */
- (NSString *)bestEffortASCIIStringDiscardingInvalidBytes;

/*!
 @method    hexEncodedString
 @abstract  Returns the hex representation of the receiver
 */
// TODO: Remove if unused
- (NSString *)hexEncodedString;

/*!
 Returns the hex string with space between each byte representation of the receiver
 */
- (NSString *)hexEncodedStringWithSpaceBetweenEachByte;

/*!
 @method    dataWithHexString:
 @abstract  Attempts to convert the passed hex string to a data object (best effort). Takes any 0-9a-f character
 */
+ (NSData *)dataWithHexString:(NSString *)hexString;

@end
