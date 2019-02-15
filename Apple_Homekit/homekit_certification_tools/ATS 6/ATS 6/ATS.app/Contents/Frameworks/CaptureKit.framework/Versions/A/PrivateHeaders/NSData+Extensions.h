//
//  NSData+Extensions.h
//  BTS
//
//  Created by Marin Balde on 5/5/11.
//  Copyright 2011 Apple. All rights reserved.
//

#import <Foundation/Foundation.h>

#define NSDataExtensionsEndOfBytes 0x5500

@interface NSData (NSData_Extensions)

/*!
 @method    dataWithBytes:
 @abstract  Returns an NSData from a va_list of bytes.
 @param     firstByte
            The first byte in the va_list.
 @param     ...
            The subsequent bytes, followed by @c NSDataExtensionsEndOfBytes.
 */
+ (NSData *)dataWithBytes:(UInt)firstByte, ...;

- (NSString *)md5Checksum;

+ (NSString *)md5ChecksumForFileAtPath:(NSString *)filePath;

/*!
 @method    enumerateDataUsingBlock:maxSubdataLength:
 @abstract  Enumerates over the data using the passed block, providing subdata no longer than maxSubdataLength
 */
- (void)enumerateDataUsingBlock:(void (^)(NSData *subdata, NSRange range, BOOL *stop))block maxSubdataLength:(NSUInteger)maxLength;

@end
