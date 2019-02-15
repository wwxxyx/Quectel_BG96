//
//  MD5Checksum.h
//
//  Created by Bob Burrough on 2/26/09.
//  Copyright 2009 Apple Inc.. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface MD5Checksum : NSObject <NSCoding> {
    NSData *checksum;
}

+ (id)MD5ChecksumWithFile:(NSString *)path;
- (id)initWithData:(NSData *)data;
- (id)initWithCoder:(NSCoder *)decoder;
- (void)encodeWithCoder:(NSCoder *)encoder;
- (NSString *)string;

@end
