//
//  ATSBHardwareInfo.h
//  ATSBoxSupport
//
//  Created by Marin Balde on 11/1/10.
//  Copyright 2010 Apple. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <ATSBoxSupport/ATSBoxTypes.h>


@interface ATSBHardwareInfo : NSObject {

}

+ (ATFSource)atfSourceFromATFHardwareInfo:(ATFHardwareInfo)hardwareInfo;

@end
