//
//  Measurement.h
//  iAPCapture
//
//  Created by David Silver on 7/19/10.
//  Copyright 2010 Apple Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface Measurement : NSObject {
    UInt64 _timestamp;
    double _value;
    BOOL _isReal; //Indicates whether this measurement is real or if it was a fake one generated to help the graphs display properly
}

@property (readonly) UInt64 timestamp;
@property (readonly) double value;
@property (readonly) BOOL isReal;

+ (id)measurementWithValue:(double)value timestamp:(UInt64)timestamp;
- (id)initWithValue:(double)value timestamp:(UInt64)timestamp;

+ (id)measurementWithValue:(double)value timestamp:(UInt64)timestamp isReal:(BOOL)isReal;
- (id)initWithValue:(double)value timestamp:(UInt64)timestamp isReal:(BOOL)isReal;

- (double)timestampInSeconds;
- (NSPoint)pointValue;
- (NSValue *)wrappedPointValue;
@end
