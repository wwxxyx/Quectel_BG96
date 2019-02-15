//
//  NanosecondsTimestampFormatter.h
//  ATS
//
//  Created by Joe Basu on 7/7/10.
//  Copyright © 2010 Apple Inc. All Rights Reserved.
//

#import <CaptureKit/TimestampFormatter.h>

@interface NanosecondsTimestampFormatter : TimestampFormatter

+ (NanosecondsTimestampFormatter *)sharedNanosecondsTimestampFormatter;

@end
