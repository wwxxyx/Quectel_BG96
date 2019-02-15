//
//  ATSResponse.h
//  ATSMacApp
//
//  Created by Carlos Frederico Azevedo on 8/8/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
@class ATSErrorResponse;
@class ATSUploadFileResponse;
@class ATSAuthenticateResponse;
@class ATSSubmitCertificationResponse;
@class ATSGetClientVersionResponse;
@class ATSGetProductPlanListResponse;

extern const NSString *ATSResponseErrorMessageKey;

@interface ATSResponse : NSObject

@property (nonatomic, retain) NSDictionary *dictionary;
@property (nonatomic, assign) NSInteger successStatusCode;

+ (ATSResponse *)ATSResponseFromJSONResponse:(NSDictionary *)jsonResponse;

+ (ATSResponse *)ATSResponseFromError:(NSError *)error;

+ (NSString *)identifier;

@end
