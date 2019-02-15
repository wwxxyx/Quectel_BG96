//
//  NMEACoordinateMessage.h
//  ATSMacApp
//
//  Created by Natália Cabral on 11/18/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <CaptureKit/NMEAMessage.h>
#import <CaptureKit/KMLNodeFormattable.h>

/*!
    The KML format tags' keys
 */
extern NSString *const CaptureGeoDataKMLPlacemarkKey;
extern NSString *const CaptureGeoDataKMLNameKey;
extern NSString *const CaptureGeoDataKMLDescriptionKey;
extern NSString *const CaptureGeoDataKMLCoordinatesKey;
extern NSString *const CaptureGeoDataKMLPointKey;
extern NSString *const CaptureGeoDataKMLTimestampKey;
extern NSString *const CaptureGeoDataKMLWhenKey;
extern NSString *const CaptureGeoDataKMLStyleURLKey;
extern NSString *const CaptureGeoDataKMLHeadingKey;
extern NSString *const CaptureGeoDataKMLStyleKey;
extern NSString *const CaptureGeoDataKMLIconKey;
extern NSString *const CaptureGeoDataKMLIconStyleKey;
extern NSString *const CaptureGeoDataKMLHrefKey;
extern NSString *const CaptureGeoDataKMLStyleIdKey;

/*!
 Defines the directions of a coordinate
 
 @constant CoordinateDirectionNorth
 Constant to represent the coordinate direction is North
 @constant CoordinateDirectionSouth
 Constant to represent the coordinate direction is South
 @constant CoordinateDirectionWest
 Constant to represent the coordinate direction is West
 @constant CoordinateDirectionEast
 Constant to represent the coordinate direction is East
 */
typedef enum : char {
                   CoordinateDirectionNorth = 'N',
                   CoordinateDirectionSouth = 'S',
                   CoordinateDirectionWest = 'W',
                   CoordinateDirectionEast = 'E',
               } CoordinateDirection;

@interface NMEACoordinateMessage : NMEAMessage <KMLNodeFormattable>

/*!
 The latitude described in the NMEA sentece
 */
@property (nonatomic, strong) NSNumber *latitude;

/*!
 The longitude described in the NMEA sentece
 */
@property (nonatomic, strong) NSNumber *longitude;

/*!
 A computed property to return whether or not a GPS Fix is available. For GPGGA—(Fix Quality indicator != Invalid) and for GPRMC—(Mode indicator != Invalid Data)
 */
@property (nonatomic, readonly) BOOL isGPSFixAvailable;

@end
