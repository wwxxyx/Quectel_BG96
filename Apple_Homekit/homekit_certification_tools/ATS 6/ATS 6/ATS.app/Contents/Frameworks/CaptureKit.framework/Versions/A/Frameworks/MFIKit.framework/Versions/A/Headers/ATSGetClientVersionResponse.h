//
//  ATSGetClientVersionResponse.h
//  ATSMacApp
//
//  Created by Carlos Frederico Azevedo on 8/8/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ATSResponse.h"

@interface ATSGetClientVersionResponse : ATSResponse

@property (nonatomic, readonly) NSString *packageMd5;
@property (nonatomic, readonly) NSString *url;
@property (nonatomic, readonly) NSString *versionIndex;
@property (nonatomic, readonly) NSString *versionString;

+ (ATSGetClientVersionResponse *)getClientResponseWithDictionary:(NSDictionary *)dictionary;

@end
