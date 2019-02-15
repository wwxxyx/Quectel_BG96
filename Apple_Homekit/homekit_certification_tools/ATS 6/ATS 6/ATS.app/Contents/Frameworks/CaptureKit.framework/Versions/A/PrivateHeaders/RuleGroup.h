//
//  RuleGroup.h
//  ATS
//
//  Created by Marin Balde on 7/20/10.
//  Copyright 2010 Apple Inc. All rights reserved.
//

#import <CaptureKit/Stenographer.h>
#import <CaptureKit/iAPConstants.h>
#import <CaptureKit/RuleProtocols_Generated.h>
#import <CaptureKit/RuleConstants_Generated.h>
#import <CaptureKit/Rule.h>
#import <CaptureKit/iAP1PacketEvent.h>
#import <CaptureKit/ATFRetHardwareInfoEvent.h>
#import <CaptureKit/ATFEvent.h>
#import <CaptureKit/SelfCertClaimSelectConstants.h>
#import <CaptureKit/iPodModelUtils.h>
#import <CaptureKit/USBResetEvent.h>
#import <CaptureKit/TokenFillediAP1PacketEvent.h>
#import <CaptureKit/TransportRuleClassification.h>
#import <CaptureKit/ProtocolRuleClassification.h>
#import <CaptureKit/ConnectorRuleClassification.h>
#import <CaptureKit/FeatureRuleClassification.h>
#import <CaptureKit/iAP2MessageEvent+Protected.h>

// All instance methods of the RuleGroup become instance methods of Stenographer.
// Likewise, the RuleGroup has access to all the instance variables and instance
// methods of Stengrapher (i.e. self is the Stenographer object).

#define ImplementationForRuleGroup(ruleGroup)       \
    @interface Stenographer (ruleGroup) <ruleGroup> \
    @end                                            \
    @implementation Stenographer (ruleGroup)

#define EndImplementation @end

#define DEFINE_RULE(name, ...)                   \
    +(NSArray *)fetch##name##RuleClassifications \
    {                                            \
        return @[ __VA_ARGS__ ];                 \
    }                                            \
    -(void)run##name##Rule : (Rule *)rule
