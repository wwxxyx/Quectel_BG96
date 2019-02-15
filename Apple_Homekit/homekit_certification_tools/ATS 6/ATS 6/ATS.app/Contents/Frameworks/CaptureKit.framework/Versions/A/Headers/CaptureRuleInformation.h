//
//  CaptureRuleInformation.h
//  ATSMacApp
//
//  Created by Sergio Sette on 10/29/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface CaptureRuleInformation : NSObject

/*!
 @enumtype  CaptureRuleErrorLevel
 @enum      CaptureRuleLevelError
            Code to represent that the result should be interpreted as an error in case of rule failure
 @enum      CaptureRuleLevelWarning
            Code to represent that the result should be interpreted as a warning in case of rule failure
 @enum      CaptureRuleLevelInfo
            Code to represent that the result should be interpreted as an info in case of rule failure
 */
typedef enum {
    CaptureRuleLevelError = 0,
    CaptureRuleLevelWarning = 1,
    CaptureRuleLevelInfo = 2,
} CaptureRuleErrorLevel;

/*!
    The description of what the rule validates
 */
@property (nonatomic, strong, readwrite) NSString *captureRuleDescription;

/*!
    The rule spec reference
 */
@property (nonatomic, strong, readwrite) NSString *specReferenceLabel;

/*!
 The format string for the rule spec file
 */
@property (nonatomic, strong, readwrite) NSString *specFilePredicate;

/*!
    The message to be displayed if the rule fails
 */
@property (nonatomic, strong, readwrite) NSString *errorMessage;

/*!
    The rule name
 */
@property (nonatomic, strong, readwrite) NSString *name;

/*!
    The app build version when the rule was first shipped
 */
@property (nonatomic, strong, readwrite) NSString *buildVersion;

/*!
    The level of the result if the rule fails
 */
@property (nonatomic, readwrite) CaptureRuleErrorLevel errorLevel;

/*!
    Initializes a new CaptureRuleInformation with the specified name, description, specRef, error message and level
    @param ruleName                 rule name
    @param buildVersion             app build version when the rule was shipped
    @param captureRuleDescription   rule description
    @param specEntry                rule spec reference object
    @param errorMessage             message to be presented when the rule fails
    @param errorLevel               level of the rule result when the rule fails
 */
- (instancetype)initWithName:(NSString *)ruleName
                buildVersion:(NSString *)buildVersion
             ruleDescription:(NSString *)captureRuleDescription
               specReference:(NSString *)referenceLabel
           specFilePredicate:(NSString *)specFilePredicate
                errorMessage:(NSString *)errorMessage
                  errorLevel:(CaptureRuleErrorLevel)errorLevel;
/*!
    Returns a copy of the CaptureRuleInformation object
 */
- (CaptureRuleInformation *)copy;

@end
