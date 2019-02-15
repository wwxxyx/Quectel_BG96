//
//  SecondsTimestampFormatter.h
//  ATS
//
//  Created by Joe Basu on 4/27/10.
//  Copyright © 2010 Apple Inc. All Rights Reserved.
//

#import <CaptureKit/TimestampFormatter.h>

@interface SecondsTimestampFormatter : TimestampFormatter

+ (SecondsTimestampFormatter *)sharedSecondsTimestampFormatter;

- (double)doubleForTimestamp:(uint64_t)timestamp;

@end
