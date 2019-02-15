//
//  TimestampFormatter.h
//  ATS
//
//  Created by Joe Basu on 6/22/10.
//  Copyright © 2010 Apple Inc. All Rights Reserved.
//

#import <Foundation/Foundation.h>

@interface TimestampFormatter : NSFormatter

- (NSComparisonResult)compareTimestampString:(NSString *)firstTimestampString toTimestampString:(NSString *)secondTimestampString;
- (NSString *)stringForTimestamp:(UInt64)timestamp;
- (UInt64)timestampForString:(NSString *)str;
- (NSString *)descriptionForTimestamp:(UInt64)timestamp;

/**
 Class property to get a default timestamp format.
 */
@property (nonatomic, class, readonly) NSString *defaultTimestampFormat;

@end
