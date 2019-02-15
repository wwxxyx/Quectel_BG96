//
//  NMEAUtil.h
//  ATSMacApp
//
//  Created by Sergio Sette on 11/18/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CaptureKit/NMEAMessage.h>

@interface NMEAUtil : NSObject

/*!
 Converts a NMEA data to NSDate
 
 @param dateStamp    the NMEA dateStamp
 @return the converted NSDate
 */
+ (NSDate *)dateFromNMEADateStamp:(NSString *)dateStamp;

/*!
 Validates bounds of date between 01/01/15 and 31/12/45 (dd/mm/yy)
 
 @param date The NSDate to be validated
 @return A boolean indicating if the param is between 01/01/15 and 31/12/45
 */
+ (BOOL)validateDateStampBoundsWithDate:(NSDate *)date;

/*!
 Converts a NMEA time to NSDate
 
 @param timeStamp    the NMEA timeStamp
 @return the converted NSDate
 */
+ (NSDate *)timeFromNMEATimeStamp:(NSString *)timeStamp;

/*!
 Converts a NMEA latitude description to a NSNumber
 
 @param nmeaLatitude    the NMEA latitude
 @return the converted NSNumber
 */
+ (NSNumber *)latitudeFromNMEALatitude:(NSString *)nmeaLatitude;

/*!
 Converts a NMEA latitude with no direction to a NSNumber latitude with direction
 If an invalid direction is provided, nil is returned
 
 @param direction    the NMEA direction
 @param latitude    the NMEA latitude
 @return the converted NSNumber or nil if the direction is invalid
 */
+ (NSNumber *)latitudeByAddingDirection:(NSString *)direction latitude:(NSNumber *)latitude;

/*!
 Converts a NMEA latitude description to a NSNumber
 
 @param nmeaLongitude    the NMEA latitude
 @return the converted NSNumber
 */
+ (NSNumber *)latitudeFromNMEALatitude:(NSString *)nmeaLatitude latitudeDirection:(NSString*)latitudeDirection;

/*!
 Converts a NMEA longitude description to a NSNumber
 
 @param nmeaLongitude    the NMEA longitude
 @return the converted NSNumber
 */
+ (NSNumber *)longitudeFromNMEALongitude:(NSString *)nmeaLongitude longitudeDirection:(NSString*)longitudeDirection;

/*!
 Converts a NMEA longitude description to a NSNumber
 
 @param nmeaLongitude    the NMEA longitude
 @return the converted NSNumber
 */
+ (NSNumber *)longitudeFromNMEALongitude:(NSString *)nmeaLongitude;

/*!
 Converts a NMEA longitude with no direction to a NSNumber longitude with direction
 If an invalid direction is provided, nil is returned
 
 @param direction    the NMEA direction
 @param latitude    the NMEA longitude
 @return the converted NSNumber or nil if the direction is invalid
 */
+ (NSNumber *)longitudeByAddingDirection:(NSString *)direction longitude:(NSNumber *)longitude;

/*!
 Converts a NSDate to time description
 
 @param date    NSDate with time information
 @return the time description
 */
+ (NSString *)timeDescriptionFromDate:(NSDate *)date;

/*!
 Converts a NSDate to date and time description
 
 @param date    NSDate with date and time information
 @return the datetime description
 */
+ (NSString *)dateDescriptionFromDate:(NSDate *)date time:(NSDate *)time;

/*!
 Converts a NSDate to date and time description in KML format
 
 @param date    NSDate with date and time information
 @return the datetime description in KML format
 */
+ (NSString *)kmlDateDescriptionFromDate:(NSDate *)date time:(NSDate *)time;

/*!
 Calculate the NMEA message checksum
 
 @param message    the NMEA sentence
 @return the sentence checksum
 */
+ (UInt8)calculateChecksum:(NSString *)message;

@end
