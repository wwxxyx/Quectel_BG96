//
//  CaptureRule.h
//  ATSMacApp
//
//  Created by Sergio Sette on 10/28/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CaptureKit/CaptureRuleInformation.h>

@class CaptureRuleResult;
@class CaptureEvent;

@interface CaptureRule : NSObject

/*!
    The information regarding the rule
 */
@property (nonatomic, strong, readonly) CaptureRuleInformation *ruleInformation;

/*!
    The list of classifications that constrains the rule eligibility
 */
@property (nonatomic, strong, readonly) NSArray *classifications;

/*!
    The selector to the rule run method
 */
@property (nonatomic, readonly) SEL selector;

/*!
    Array containing and events that the rule may need to use for comparisons
 */
@property (nonatomic, strong) NSMutableArray<CaptureEvent *> *eventArray;

/*!
    Construct a new CaptureRule with the specified CaptureRuleInformation, selector and classifications
    @param ruleName                 rule name
    @param buildVersion             app build version whe the rule was first shipped
    @param captureRuleDescription   rule description
    @param specLabel                rule spec Label
    @param specFilePredicate        spec file predicate
    @param errorMessage             message to be presented when the rule fails
    @param errorLevel               level of the rule result when the rule fails
    @param selector                 selector to the rule run method
    @param classifications          list of classifications that constrains the rule eligibility
    @return a new CaptureRule
*/
- (instancetype)initWithName:(NSString *)ruleName
                buildVersion:(NSString *)buildVersion
             ruleDescription:(NSString *)captureRuleDescription
                   specLabel:(NSString *)specLabel
           specFilePredicate:(NSString *)specFilePredicate
                errorMessage:(NSString *)errorMessage
                  errorLevel:(CaptureRuleErrorLevel)errorLevel
                    selector:(SEL)selector
             classifications:(NSArray *)classifications;

/**
 Method to initialize a new rule with a lot of necessary input parameters.
 
 @param ruleName                 rule name
 @param buildVersion             app build version whe the rule was first shipped
 @param captureRuleDescription   rule description
 @param specSection              rule spec section string
 @param specFileName             spec file name
 @param errorMessage             message to be presented when the rule fails
 @param errorLevel               level of the rule result when the rule fails
 @param selector                 selector to the rule run method
 @param classifications          list of classifications that constrains the rule eligibility
 
 @return a new CaptureRule

 @Note we need this secondary init method to be able to make sure from RULE_ALT macro (which is also necessary to be able to make rules without SpecKit spec references).
 */
- (instancetype)initWithName:(NSString *)ruleName
                buildVersion:(NSString *)buildVersion
             ruleDescription:(NSString *)captureRuleDescription
                 specSection:(NSString *)specSection
                specFileName:(NSString *)specFileName
                errorMessage:(NSString *)errorMessage
                  errorLevel:(CaptureRuleErrorLevel)errorLevel
                    selector:(SEL)selector
             classifications:(NSArray *)classifications;

/*!
    Returns a new object with the same contents of the rule
    @return the new object that is a copy
 */
- (CaptureRule *)copy;

@end
