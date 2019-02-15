//
//  GPRMCMessage.h
//  ATSMacApp
//
//  Created by Sergio Sette on 11/12/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <CaptureKit/NMEACoordinateMessage.h>
#import <Foundation/Foundation.h>
#import <CoreLocation/CoreLocation.h>

typedef enum : UInt8 {
                   GPRMCMessageModeIndicatorAutonomous = 'A',
                   GPRMCMessageModeIndicatorDifferential = 'D',
                   GPRMCMessageModeIndicatorApproximation = 'E',
                   GPRMCMessageModeIndicatorInvalid = 'N',
               } GPRMCMessageModeIndicator;

typedef enum : UInt8 {
                   GPRMCComponentTimeIndex = 1,
                   GPRMCComponentValidityIndex,
                   GPRMCComponentLatitudeIndex,
                   GPRMCComponentLatitudeDirectionIndex,
                   GPRMCComponentLongitudeIndex,
                   GPRMCComponentLongitudeDirectionIndex,
                   GPRMCComponentSpeedIndexIndex,
                   GPRMCComponentTrueCourseIndex,
                   GPRMCComponentDateIndex,
                   GPRMCComponentVariationIndex,
                   GPRMCComponentVariationDirectionIndex,
                   GPRMCComponentModeIndicator,
                   GPRMCComponentCount,
               } GPRMCComponentIndex;

@interface GPRMCMessage : NMEACoordinateMessage

/*!
 Fields of NMEA 0183 GPRMC message
 */
@property (nonatomic, strong) NSDate *time;
@property (nonatomic, strong) NSDate *date;
@property (nonatomic, strong) NSNumber *validity;
@property (nonatomic, strong) NSNumber *speed;
@property (nonatomic, strong) NSNumber *trueCourse;
@property (nonatomic, strong) NSNumber *variation;
@property (nonatomic, strong) NSNumber *modeIndicator;
@property (nonatomic, readonly) BOOL isGPSFixAvailable;

@end
