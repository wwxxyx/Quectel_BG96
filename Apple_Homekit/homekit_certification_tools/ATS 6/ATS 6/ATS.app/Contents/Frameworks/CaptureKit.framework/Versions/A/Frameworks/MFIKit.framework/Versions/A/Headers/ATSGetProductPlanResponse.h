//
//  ATSGetProductPlanResponse.h
//  MFIKit
//
//  Created by Carlos Frederico Azevedo on 8/27/15.
//  Copyright (c) 2015 Apple Inc. All rights reserved.
//

#import <MFIKit/MFIKit.h>

extern NSString *const MFIIs30PinKey;
extern NSString *const MFIIsLightningKey;
extern NSString *const MFISupportediAPProtocolsKey;

@interface ATSGetProductPlanResponse : ATSResponse

@property (nonatomic, readonly) BOOL isLightningConnector;

@property (nonatomic, readonly) BOOL is30PinConnector;

@property (nonatomic, readonly) NSNumber *supportediAPProtocols;

+ (instancetype)getProductPlanResponseWithDictionary:(NSDictionary *)dictionary;


@end
