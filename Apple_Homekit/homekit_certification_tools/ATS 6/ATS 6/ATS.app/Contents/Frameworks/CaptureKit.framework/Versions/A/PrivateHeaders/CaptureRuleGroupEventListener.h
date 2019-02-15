//
//  CaptureRuleGroupEventListener.h
//  ATSMacApp
//
//  Created by Sergio Sette on 10/28/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

@class CaptureRuleResult;

@protocol CaptureRuleGroupEventListener

/*!
    Notify that the rule group was executed and returned the specified result
    @param  finishedRuleResults the rule execution results array
 */
- (void)ruleGroupFinishedWithResult:(NSArray<CaptureRuleResult *> *)finishedRuleResults;

@end
