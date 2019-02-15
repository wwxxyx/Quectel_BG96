//
//  PAACDMessage.h
//  ATSMacApp
//
//  Created by Natália Cabral on 17/11/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <CaptureKit/NMEAMessage.h>
#import <CaptureKit/SensorSample.h>

extern const UInt8 PAACDMessageMaxSampleCount;
extern const UInt8 PAACDMessageMinSampleCount;

typedef enum : UInt8 {
                   PAACDComponentTimestampIndex = 1,
                   PAACDComponentGValueIndex,
                   PAACDComponentSampleCountIndex,
                   PAACDComponentFirstSensorSampleIndex,
                   PAACDComponentCount = PAACDComponentFirstSensorSampleIndex,
               } PAACDComponentIndex;

@interface PAACDMessage : NMEAMessage

/*!
 Fields of custom NMEA 0183 PAACD message
 */
@property (nonatomic, strong) NSNumber *timeStamp;
@property (nonatomic, strong) NSNumber *gValue;
@property (nonatomic, strong) NSNumber *sampleCount;
@property (nonatomic, strong) NSArray *sensorSampleList;

@end
