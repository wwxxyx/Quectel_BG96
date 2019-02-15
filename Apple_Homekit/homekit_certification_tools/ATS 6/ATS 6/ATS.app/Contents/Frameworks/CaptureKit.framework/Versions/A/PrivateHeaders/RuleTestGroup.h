//
//  RuleTestGroup.h
//  ATS
//
//  Created by Marin Balde on 8/11/10.
//  Copyright 2010 Apple Inc. All rights reserved.
//

#import <CaptureKit/RuleTests.h>
#import <CaptureKit/RuleTestProtocols_Generated.h>
#import <CaptureKit/iAP1PacketEvent.h>
#import <CaptureKit/EmulatedATFR3CaptureDevice.h>
#import <CaptureKit/EmulatedATSBoxCaptureDevice.h>
#import <CaptureKit/iAPConstants.h>
#import <CaptureKit/RuleConstants_Generated.h>
#import <CaptureKit/RuleError.h>
#import <CaptureKit/ATSRuleEngine.h>
#import <CaptureKit/iPodModelUtils.h>
#import <CaptureKit/ATFRetHardwareInfoEvent.h>
#import <CaptureKit/SelfCertClaimSelectConstants.h>

// All instance methods of the RuleTestGroup become instance methods of RuleTests.
// Likewise, the RuleTestGroup has access to all the instance variables and instance
// methods of RuleTests (i.e. self is the RuleTests object).

#define ImplementationForRuleTestGroup(ruleTestGroup)    \
    @interface RuleTests (ruleTestGroup) <ruleTestGroup> \
    @end                                                 \
    @implementation RuleTests (ruleTestGroup)

#define EndImplementation @end
