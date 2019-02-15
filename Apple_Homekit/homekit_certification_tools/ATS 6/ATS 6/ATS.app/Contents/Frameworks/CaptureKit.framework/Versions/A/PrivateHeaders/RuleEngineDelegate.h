//
//  RuleEngineDelegate.h
//  ATSMacApp
//
//  Created by Marin Balde on 9/20/10.
//  Copyright 2010 Apple Inc. All rights reserved.
//

@class ATSRuleEngine;
@class ProductPlan;
@protocol EventChanneling;

@protocol RuleEngineDelegate
@required
- (void)ruleEngine:(ATSRuleEngine *)ruleEngine firedErrors:(NSArray *)errors atTimestamp:(UInt64)timestamp;
- (BOOL)ruleEngineAreClaimSpecificRulesEnabled:(ATSRuleEngine *)ruleEngine;
- (BOOL)ruleEngine:(ATSRuleEngine *)ruleEngine isClaimEnabled:(NSString *)claimName;
- (BOOL)appleDeviceActingAsUSBDevice;

/*!
 @method        productPlan
 @abstract      The current product plan
 */
- (ProductPlan *)productPlan;

/*!
 @method        ruleEngine:flushATSAppEventsBeforeTimestamp:
 @abstract      Request by the rule engine to flush any ATS app event timestamps up to but not including the given timestamp
 @param         ruleEngine
                Rule engine requesting the flush
 @param         timestamp
                Timestamp to flush up until
 @discussion    This gives CaptureDocument a chance to deliver any ATS app events for the given timestamp to this rule engine before moving on
 */
- (void)ruleEngine:(ATSRuleEngine *)ruleEngine flushATSAppEventsBeforeTimestamp:(UInt64)timestamp;

/*!
 @method        isTrafficOptional
 @abstract      Whether or not traffic (iAP) is optional on this transport
 @discussion    This was previously known as "Hardware-Only", but that term
                is implies no traffic is expected. Really it should mean
                that traffic is optional. We'll still validate it, but certain
                rules that ensure its existance (say, after power up) won't run
 */
- (BOOL)isTrafficOptional;

@end
