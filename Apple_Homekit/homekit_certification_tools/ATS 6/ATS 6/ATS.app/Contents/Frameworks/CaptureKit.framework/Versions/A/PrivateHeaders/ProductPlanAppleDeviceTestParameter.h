//
//  ProductPlanAppleDeviceTestParameter.h
//  ATSMacApp
//
//  Created by Kip Nicol on 7/21/11.
//  Copyright (c) 2011 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CaptureKit/ProductPlanTypes.h>

@class ProductPlanAppleDevice;

@interface ProductPlanAppleDeviceTestParameter : NSObject <NSCoding> {
    ProductPlanAppleDevice *_appleDevice;
    NSArray *_selfCertTests;
}

@property (nonatomic, readonly, retain) ProductPlanAppleDevice *appleDevice;
@property (nonatomic, readonly, retain) NSArray *selfCertTests;

- (id)initWithAppleDevice:(ProductPlanAppleDevice *)appleDevice
            selfCertTests:(NSArray *)selfCertTests;
- (id)initWithDictionary:(NSDictionary *)dict;

@end
