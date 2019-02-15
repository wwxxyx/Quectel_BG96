//
//  AccessoryTypeRuleClassification.h
//  ATSMacApp
//
//  Created by Carlos Frederico Azevedo on 4/10/15.
//  Copyright (c) 2015 Apple Inc. All rights reserved.
//

#import <CaptureKit/RuleListClassification.h>

#define WITH_ACCESSORY_TYPE(...) WITH_CLASSIFICATION(AccessoryTypeRuleClassification, RuleListClassificationTypeWhiteList, __VA_ARGS__ )
#define EXCLUDING_ACCESSORY_TYPE(...) WITH_CLASSIFICATION(AccessoryTypeRuleClassification, RuleListClassificationTypeBlackList, __VA_ARGS__ )

/**
 A classification describing the type of accessory which the rule should be executed for
 */
@interface AccessoryTypeRuleClassification : RuleListClassification

@end
