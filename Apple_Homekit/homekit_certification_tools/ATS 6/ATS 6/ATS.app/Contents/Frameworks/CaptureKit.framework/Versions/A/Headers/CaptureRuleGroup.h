//
//  CaptureRuleGroup.h
//  ATSMacApp
//
//  Created by Sergio Sette on 10/28/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CaptureKit/EventListener.h>
#import <CaptureKit/CaptureRuleResult.h>
#import <CaptureKit/CaptureRule.h>
#import <CaptureKit/DeviceCaptureConfigurationTypes.h>
#import <CaptureKit/ProtocolRuleClassification.h>
#import <CaptureKit/ConnectorRuleClassification.h>
#import <CaptureKit/TransportRuleClassification.h>
#import <CaptureKit/FeatureRuleClassification.h>
#import <CaptureKit/AccessoryTypeRuleClassification.h>
#import <CaptureKit/SpecReferenceConstants.h>

@class CaptureConfigurationDescription;
@class CaptureRuleStenograph;
@protocol CaptureRuleGroupEventListener;

/*!
    Macros to define a new rule with the specified CaptureRuleInformation, selector and classifications
 */
#define RULE(ruleName, appBuildVersion, description, ruleSpecLabel, ruleSpecFilePredicate, ruleErrorMessage, ruleErrorLevel, ruleSelector, ...) \
    [[CaptureRule alloc] initWithName:ruleName \
    buildVersion:appBuildVersion \
    ruleDescription:description \
    specLabel:ruleSpecLabel \
    specFilePredicate:ruleSpecFilePredicate \
    errorMessage:ruleErrorMessage \
    errorLevel:ruleErrorLevel \
    selector:ruleSelector \
    classifications:@[ __VA_ARGS__ ]]

/*!
 Macro to define a new rule (in alt format) with the hardcoded spec section and spec file name along with specified CaptureRuleInformation, selector and classifications
 */
#define RULE_ALT(ruleName, appBuildVersion, description, ruleSpecSection, ruleSpecFileName, ruleErrorMessage, ruleErrorLevel, ruleSelector, ...) \
    [[CaptureRule alloc] initWithName:ruleName \
    buildVersion:appBuildVersion \
    ruleDescription:description \
    specSection:ruleSpecSection \
    specFileName:ruleSpecFileName \
    errorMessage:ruleErrorMessage \
    errorLevel:ruleErrorLevel \
    selector:ruleSelector \
    classifications:@[ __VA_ARGS__ ]]

/*!
    Macro to define the rules error localizable message
 */
#define LocalizedRuleErrorMessageComment(key, comment) \
    NSLocalizedStringFromTableInBundle((key), (@"RulesErrorMessagesLocalizable"), ([NSBundle bundleForClass: [CaptureRuleGroup class]]), (comment))

#define LocalizedRuleErrorMessage(key) \
    LocalizedRuleErrorMessageComment((key), @"")

@class Stenographer;

@interface CaptureRuleGroup : NSObject <EventListener>

/*!
    The capture configuration selected by the user for this session
 */
@property (nonatomic, strong, readwrite) CaptureConfigurationDescription *configuration;

/*!
    The stenograph responsible for handling the events
 */
@property (nonatomic, strong) CaptureRuleStenograph *stenograph;

/*!
 The stenographer(Containing old way of writing stenographs) reference to be able to share a single instance of stenograph modules per capture transport.
 */
@property (nonatomic, strong) Stenographer *stenographer;

/*!
 The list of rules that belong to this rule group. Subclasses should override this class method to provide a list of rules.
 */
@property (class, readonly) NSArray<CaptureRule *> *ruleList;

/*!
    Classes that inherit from CaptureRuleGroup must overwrite this method with the Event subclasses they can process.
    @return An NSSet of Event subclasses which can be processed by the processEvent method.
 */
- (NSSet *)registeredEventsForRuleGroup;

/*!
    Initialize a CaptureRuleGroup with the specified configuration and stenograph
    @param configuration   the specified capture configuration
    @param stenographer Stenographer(Containing old way of writing stenographs) reference to be able to share a single instance of stenograph modules per capture transport.
    @param stenograph      the specified stenograph
    @return A new CaptureRuleGroup
 */
- (instancetype)initWithConfiguration:(CaptureConfigurationDescription *)configuration
                         stenographer:(Stenographer *)stenographer
                           stenograph:(CaptureRuleStenograph *)stenograph;

/*!
    Initialize a CaptureRuleGroup with the specified configuration. This method should be overriden by the subclasses
    @param configuration the specified capture configuration
    @param stenographer Stenographer(Containing old way of writing stenographs) reference to be able to share a single instance of stenograph modules per capture transport.
    @return A new CaptureRuleGroup
 */
- (instancetype)initWithConfiguration:(CaptureConfigurationDescription *)configuration stenographer:(Stenographer *)stenographer;

/*!
    Add the specified listener to receive the CaptureRuleResult
    @param listener     listener to be added
 */
- (void)addListener:(id<CaptureRuleGroupEventListener>)listener;

/*!
    The name of the rule group. This method should be overriden by the subclasses
    @return the rule group name
 */
+ (NSString *)name;

@end
