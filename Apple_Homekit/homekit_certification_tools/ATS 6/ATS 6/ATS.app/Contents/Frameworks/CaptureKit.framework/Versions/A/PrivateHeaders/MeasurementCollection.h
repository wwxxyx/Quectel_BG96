//
//  MeasurementCollection.h
//  iAPCapture
//
//  Created by David Silver on 7/19/10.
//  Copyright 2010 Apple Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>
@class Measurement;

@protocol MeasurementCollectionDelegate;

@interface MeasurementCollection : NSObject {
    NSMutableArray *_measurements;

    double _sumOfValues;
    double _minValue;
    double _maxValue;
    double _avgValue;
}

@property (readonly) NSArray *measurements;
@property (readonly) UInt64 minTimestamp;
@property (readonly) UInt64 maxTimestamp;
@property (readonly) double minTimestampInSeconds;
@property (readonly) double maxTimestampInSeconds;
@property (readonly) double minValue;
@property (readonly) double maxValue;
@property (readonly) double avgValue;

- (void)addMeasurement:(Measurement *)measurement;
- (void)removeAllMeasurements;

- (BOOL)hasMeasurements;                 //Returns YES if at the collection has at least one measurement
- (Measurement *)lastMeasurement;        //Returns the most recently added measurement
- (NSNumber *)valueForTime:(double)time; //Returns the value at the given time. If there is no data for that point, interpolates a value from
                                         //the surrounding points
@end
