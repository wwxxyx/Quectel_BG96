//
//  ATSTestProductPlan.h
//  ATSMacApp
//
//  Created by Zachary Church on 11/9/15.
//  Copyright © 2015 Apple Inc. All rights reserved.
//

#import <SelfCertKitDesktop/SelfCertKitDesktop.h>

extern NSString *const ATSProductPlanTestMode;


@interface ATSTestProductPlan : SKProductPlan

/**
 @property index
 @abstract This index is used for when there are multiple test mode runs; they'll be named "Test mode 1", "Test mode 2", etc.
 */
@property (nonatomic) NSUInteger index;

+ (instancetype)productPlan;

@end
