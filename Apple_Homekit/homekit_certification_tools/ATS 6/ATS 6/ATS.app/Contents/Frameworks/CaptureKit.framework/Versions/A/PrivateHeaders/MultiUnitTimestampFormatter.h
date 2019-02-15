//
//  MultiUnitTimestampFormatter.h
//  ATS
//
//  Created by Joe Basu on 6/21/10.
//  Copyright © 2010 Apple Inc. All Rights Reserved.
//

#import <CaptureKit/TimestampFormatter.h>

@interface MultiUnitTimestampFormatter : TimestampFormatter {
}

+ (MultiUnitTimestampFormatter *)sharedMultiUnitTimestampFormatter;

@end
