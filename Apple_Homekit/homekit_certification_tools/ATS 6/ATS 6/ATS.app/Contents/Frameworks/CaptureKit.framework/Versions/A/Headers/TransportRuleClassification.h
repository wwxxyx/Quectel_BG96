//
//  TransportRuleClassification.h
//  ATSMacApp
//
//  Created by Sergio Sette on 9/22/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <CaptureKit/RuleListClassification.h>

#define WITH_TRANSPORT(...) WITH_CLASSIFICATION(TransportRuleClassification, RuleListClassificationTypeWhiteList, __VA_ARGS__ )
#define EXCLUDING_TRANSPORT(...) WITH_CLASSIFICATION(TransportRuleClassification, RuleListClassificationTypeBlackList, __VA_ARGS__ )

/*!
 @class     TransportRuleClassification
 @abstract  A classification describing a type of transport which the rule should be executed for
 */
@interface TransportRuleClassification : RuleListClassification

@end
