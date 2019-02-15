//
//  FeatureRuleClassification.h
//  ATSMacApp
//
//  Created by Sergio Sette on 10/10/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <CaptureKit/RuleListClassification.h>

#define WITH_FEATURE(...) WITH_CLASSIFICATION(FeatureRuleClassification, RuleListClassificationTypeWhiteList, __VA_ARGS__ )
#define EXCLUDING_FEATURE(...) WITH_CLASSIFICATION(FeatureRuleClassification, RuleListClassificationTypeBlackList, __VA_ARGS__ )


/*!
 @class     FeatureRuleClassification
 @abstract  A classification describing a type of feature which the rule should be executed for
 */
@interface FeatureRuleClassification : RuleListClassification
@end
