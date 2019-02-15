//
//  ATSSubmitCertificationResponse.h
//  ATSMacApp
//
//  Created by Carlos Frederico Azevedo on 8/8/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ATSResponse.h"

@interface ATSSubmitCertificationResponse : ATSResponse

@property (nonatomic, readonly) NSString *message;
@property (nonatomic, readonly) NSString *receipt;

+ (ATSSubmitCertificationResponse *)submitCertificationResponseWithDictionary:(NSDictionary *)dictionary;

@end
