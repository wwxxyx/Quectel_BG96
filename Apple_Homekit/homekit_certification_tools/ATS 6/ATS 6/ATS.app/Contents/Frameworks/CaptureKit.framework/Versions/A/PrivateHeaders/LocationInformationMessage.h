//
//  LocationInformationMessage.h
//  ATSMacApp
//
//  Created by Natália Cabral on 11/17/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "InformationMessage.h"

@interface LocationInformationMessage : InformationMessage

/*!
    The list of NMEA sentence
 */
@property (nonatomic, strong) NSArray *nmeaMessages;

/*!
 Initializes a new LocationInformationMessage with the specified messages and timestamp
    @param nmeaMessages     the list of NMEA sentence
    @param timestamp        timestamp when the LocationInformation message was received
    @return a new LocationInformationMessage
 */
- (id)initWithNMEAMessagse:(NSArray *)nmeaMessages timestamp:(UInt64)timestamp;

@end
