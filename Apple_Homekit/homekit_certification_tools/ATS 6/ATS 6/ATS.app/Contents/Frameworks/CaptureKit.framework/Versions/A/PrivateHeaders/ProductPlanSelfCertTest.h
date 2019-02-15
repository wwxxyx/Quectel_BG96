//
//  ProductPlanSelfCertTest.h
//  ATSMacApp
//
//  Created by Kip Nicol on 8/4/11.
//  Copyright (c) 2011 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CaptureKit/ProductPlanType.h>
#import <CaptureKit/ProductPlanAppleDevice.h>

@class AccessoryDataModel;

@interface ProductPlanSelfCertTest : ProductPlanType {
}

@property (nonatomic, readonly) SelfCertTestType type;
@property (nonatomic, readonly) NSString *name;

+ (id)selfCertTestWithSelfCertTestType:(SelfCertTestType)type;
- (id)initWithSelfCertTestType:(SelfCertTestType)type;
- (BOOL)supportsAccessoryDataModel:(AccessoryDataModel *)dataModel;
- (BOOL)supportsAccessoryDataModel:(AccessoryDataModel *)dataModel appleDevice:(ProductPlanAppleDevice *)appleDevice;
- (NSArray *)testTypeWithParents;

@end
