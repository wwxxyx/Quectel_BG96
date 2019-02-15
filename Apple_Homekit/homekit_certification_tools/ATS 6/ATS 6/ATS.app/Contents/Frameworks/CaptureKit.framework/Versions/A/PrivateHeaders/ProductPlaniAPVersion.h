//
//  ProductPlaniAPVersion.h
//  ATSMacApp
//
//  Created by Kip Nicol on 8/13/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <CaptureKit/ProductPlanType.h>

@interface ProductPlaniAPVersion : ProductPlanType

@property (nonatomic, readonly, assign) iAPVersionType type;

+ (id)iapVersionWithiAPVersionType:(iAPVersionType)type;
- (id)initWithiAPVersionType:(iAPVersionType)type;

@end
