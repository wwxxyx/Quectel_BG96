//
//  SensorSample.h
//  ATSMacApp
//
//  Created by Natália Cabral on 11/17/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef enum : UInt8 {
                   SensorSampleComponentTimeOffsetIndex,
                   SensorSampleComponentXAxisSampleIndex,
                   SensorSampleComponentYAxisSampleIndex,
                   SensorSampleComponentZAxisSampleIndex,
                   SensorSampleComponentCount,
               } SensorSampleComponentIndex;

@interface SensorSample : NSObject

@property (nonatomic, strong) NSNumber *timeOffset;
@property (nonatomic, strong) NSNumber *xAxisSample;
@property (nonatomic, strong) NSNumber *yAxisSample;
@property (nonatomic, strong) NSNumber *zAxisSample;

+ (NSArray *)requiredFieldIndexes;

@end
