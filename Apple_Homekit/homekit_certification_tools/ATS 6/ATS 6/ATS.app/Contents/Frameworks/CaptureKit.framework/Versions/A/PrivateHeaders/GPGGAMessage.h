//
//  GPGGAMessage.h
//  ATSMacApp
//
//  Created by Natália Cabral on 12/11/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <CaptureKit/NMEACoordinateMessage.h>
#import <CoreLocation/CoreLocation.h>

typedef enum : UInt8 {
                   FixQualityInvalid = 0,
                   FixQualityGPSFix = 1,
                   FixQualityDGPSFix = 2,
                   FixQualityPPSFix = 3,
                   FixQualityRTKinematic = 4,
                   FixQualityFloatRTK = 5,
                   FixQualityEstimated = 6,
                   FixQualityManualInputMode = 7,
                   FixQualitySimulationMode = 8,
               } FixQuality;

typedef enum : UInt8 {
    GPGGAComponentTimeIndex = 1,
    GPGGAComponentLatitudeIndex,
    GPGGAComponentLatitudeDirectionIndex,
    GPGGAComponentLongitudeIndex,
    GPGGAComponentLongitudeDirectionIndex,
    GPGGAComponentFixQualityIndex,
    GPGGAComponentNumberOfSatellitiesIndex,
    GPGGAComponentHDOPIndex,
    GPGGAComponentAltitudeIndex,
    GPGGAComponentAltitudeUnitIndex,
    GPGGAComponentGeoidalSeparationIndex,
    GPGGAComponentGeoidalSeparationUnitIndex,
    GPGGAComponentDGPSAgeIndex,
    GPGGAComponentDGPSReferenceStationIDIndex,
    GPGGAComponentCount,
} GPGGAComponentIndex;

@interface GPGGAMessage : NMEACoordinateMessage

/*!
    Fields of NMEA 0183 GPGGA message
 */
@property (nonatomic, strong) NSDate *time;
@property (nonatomic, strong) NSNumber *fixQuality;
@property (nonatomic, strong) NSNumber *numberOfSatellities;
@property (nonatomic, strong) NSNumber *horizontalDilutionOfPrecision;
@property (nonatomic, strong) NSNumber *altitude;
@property (nonatomic, strong) NSNumber *geoidalSeparation;
@property (nonatomic, strong) NSNumber *dgpsAge;
@property (nonatomic, strong) NSNumber *dgpsReferenceStationId;
@property (nonatomic, readonly) BOOL isGPSFixAvailable;
@end
