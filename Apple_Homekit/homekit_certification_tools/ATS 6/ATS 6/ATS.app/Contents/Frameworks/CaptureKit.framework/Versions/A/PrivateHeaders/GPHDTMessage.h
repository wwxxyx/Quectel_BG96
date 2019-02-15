//
//  GPHDTMessage.h
//  ATSMacApp
//
//  Created by Natália Cabral on 17/11/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <CaptureKit/NMEAMessage.h>

@interface GPHDTMessage : NMEAMessage

typedef enum : UInt8 {
    GPHDTComponentHeadingIndex = 1,
    GPHDTComponentDegreesIndex,
    GPHDTComponentCount
} GPHDTComponentIndex;


/*!
 Fields of NMEA 0183 GPHDT message
 */
@property (nonatomic, strong) NSNumber *heading;

@end
