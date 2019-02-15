//
//  GPGSVMessage.h
//  ATSMacApp
//
//  Created by Natália Cabral on 17/11/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <CaptureKit/NMEAMessage.h>
#import <CaptureKit/GPGSVSatelliteInformation.h>

@interface GPGSVMessage : NMEAMessage

extern const UInt8 GPGSVMaxNumberofSatelliteForSentence;

typedef enum : UInt8 {
                   GPGSVComponentTotalNumberOfMessagesIndex = 1,
                   GPGSVComponentMessageNumberIndex,
                   GPGSVComponentTotalNumberofSatellitesIndex,
                   GPGSVComponentFirstSatelliteInfoIndex,
                   GPGSVComponentCount = GPGSVComponentFirstSatelliteInfoIndex,
               } GPGSVComponentIndex;

typedef enum : UInt8 {
                   GPGSVSatelliteInfoPRNNumberIndex = 0,
                   GPGSVSatelliteInfoElevationIndex,
                   GPGSVSatelliteInfoAzimuthIndex,
                   GPGSVSatelliteInfoSNRIndex,
                   GPGSVSatelliteInfoComponentCount,
               } GPGSVatelliteInfoIndex;

/*!
    Fields of NMEA 0183 GPGSV message
 */
@property (nonatomic, strong) NSNumber *totalNumberOfMessages;
@property (nonatomic, strong) NSNumber *messageNumber;
@property (nonatomic, strong) NSNumber *totalNumberOfSatellites;
@property (nonatomic, strong) NSArray *satellitesInfoList;

@end
