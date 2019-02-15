//
//  ATSErrorResponse.h
//  ATSMacApp
//
//  Created by Carlos Frederico Azevedo on 8/8/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ATSResponse.h"

@interface ATSErrorResponse : ATSResponse

@property (nonatomic, readonly) NSString *message;
@property (nonatomic, strong) NSString *statusCodeMessage;
@property (nonatomic, strong) NSString *statusCode;



+ (ATSErrorResponse *)errorResponseWithDictionary:(NSDictionary *)dictionary;

+ (ATSErrorResponse *)errorResponse;



@end
