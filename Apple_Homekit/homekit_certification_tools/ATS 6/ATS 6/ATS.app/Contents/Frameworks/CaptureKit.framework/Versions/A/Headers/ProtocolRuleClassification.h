//
//  ProtocolRuleClassification.h
//  ATSMacApp
//
//  Created by Natália Cabral on 9/24/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <CaptureKit/RuleListClassification.h>

#define WITH_PROTOCOL(...) WITH_CLASSIFICATION(ProtocolRuleClassification, RuleListClassificationTypeWhiteList, __VA_ARGS__ )
#define EXCLUDING_PROTOCOL(...) WITH_CLASSIFICATION(ProtocolRuleClassification, RuleListClassificationTypeBlackList, __VA_ARGS__ )

/*!
 @class     ProtocolRuleClassification
 @abstract  A classification describing a type of protocol which the rule should be executed for
 */
@interface ProtocolRuleClassification : RuleListClassification

@end
