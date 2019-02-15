//
//  ConnectorRuleClassification.h
//  ATSMacApp
//
//  Created by Natália Cabral on 9/24/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <CaptureKit/RuleListClassification.h>

#define WITH_CONNECTOR(...) WITH_CLASSIFICATION(ConnectorRuleClassification, RuleListClassificationTypeWhiteList, __VA_ARGS__ )
#define EXCLUDING_CONNECTOR(...) WITH_CLASSIFICATION(ConnectorRuleClassification, RuleListClassificationTypeBlackList, __VA_ARGS__ )

/*!
 @class     ConnectorRuleClassification
 @abstract  A classification describing a type of connector which the rule should be executed for
 */
@interface ConnectorRuleClassification : RuleListClassification

@end
