//
//  PASCDMessage.h
//  ATSMacApp
//
//  Created by Natália Cabral on 17/11/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <CaptureKit/NMEAMessage.h>
#import <CaptureKit/PASCDSensorSample.h>

extern const UInt8 PASCDMessageMaxSampleCount;
extern const UInt8 PASCDMessageMinSampleCount;

typedef enum : char {
                   SensorTypeCombinedLeftRight = 'C',
               } SensorType;

typedef enum : char {
                   TransmissionStateUnknown = 'U',
                   TransmissionStatePark = 'P',
                   TransmissionStateReverse = 'R',
                   TransmissionStateDrivingForward = 'D',
                   TransmissionStateNeutral = 'N',
               } TransmissionState;

typedef enum : UInt8 {
                   PASCDComponentTimestampIndex = 1,
                   PASCDComponentSensorTypeIndex,
                   PASCDComponentTransmissionStateIndex,
                   PASCDComponentSlipDetectIndex,
                   PASCDComponentSampleCountIndex,
                   PASCDComponentFirstSensorSampleIndex,
                   PASCDComponentCount = PASCDComponentFirstSensorSampleIndex
               } PASCDComponentIndex;

@interface PASCDMessage : NMEAMessage

/*!
    Fields of custom NMEA 0183 PASCD message
 */
@property (nonatomic, strong) NSNumber *pascdTimestamp;
@property (nonatomic, strong) NSNumber *sensorType;
@property (nonatomic, strong) NSNumber *transmissionState;
@property (nonatomic, strong) NSNumber *slipDetect;
@property (nonatomic, strong) NSNumber *sampleCount;
@property (nonatomic, strong) NSArray *sensorSampleList;

@end
