//
//  PAGCD.h
//  ATSMacApp
//
//  Created by Natália Cabral on 17/11/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <CaptureKit/NMEAMessage.h>
#import <CaptureKit/SensorSample.h>

extern const UInt8 PAGCDMessageMaxSampleCount;
extern const UInt8 PAGCDMessageMinSampleCount;

typedef enum : UInt8 {
                   PAGCDComponentTimestampIndex = 1,
                   PAGCDComponentSampleCountIndex,
                   PAGCDComponentFirstSensorSampleIndex,
                   PAGCDComponentCount = PAGCDComponentFirstSensorSampleIndex,
               } PAGCDComponentIndex;

@interface PAGCDMessage : NMEAMessage

/*!
 Fields of custom NMEA 0183 PAGCD message
 */
@property (nonatomic, strong) NSNumber *timeStamp;
@property (nonatomic, strong) NSNumber *sampleCount;
@property (nonatomic, strong) NSArray *sensorSampleList;

@end
