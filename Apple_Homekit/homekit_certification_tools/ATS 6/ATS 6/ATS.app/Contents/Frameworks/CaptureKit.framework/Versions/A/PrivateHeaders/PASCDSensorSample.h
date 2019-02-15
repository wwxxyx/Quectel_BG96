//
//  PASCDSensorSample.h
//  ATSMacApp
//
//  Created by Natália Cabral on 11/17/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef enum : UInt8 {
                   PASCDSensorSampleComponentTimeOffsetIndex,
                   PASCDSensorSampleComponentSpeedIndex,
                   PASCDSensorSampleComponentCount,
               } PASCDSensorSampleComponentIndex;

@interface PASCDSensorSample : NSObject

@property (nonatomic, strong) NSNumber *timeOffset;
@property (nonatomic, strong) NSNumber *speed;

+ (NSArray *)requiredFieldIndexes;

@end
