//
//  GPGSVSatelliteInformation.h
//  ATSMacApp
//
//  Created by Natália Cabral on 17/11/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface GPGSVSatelliteInformation : NSObject

@property (nonatomic, strong) NSNumber *satellitePRNNumber;
@property (nonatomic, strong) NSNumber *elevation;
@property (nonatomic, strong) NSNumber *azimuth;
@property (nonatomic, strong) NSNumber *snr;

@end
