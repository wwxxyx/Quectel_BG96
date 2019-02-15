//
//  AirPlay2RuleGroup.h
//  ATSMacApp
//
//  Created by Vinod Madigeri on 7/24/18.
//  Copyright © 2018 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureRuleGroup.h>

@interface AirPlay2RuleGroup : CaptureRuleGroup

- (CaptureRuleResult *)runPTPAnnounceMessageNotSeen:(CaptureRule *)rule;
- (CaptureRuleResult *)runPTPAnnounceMessageGrandmasterPriority1Invalid:(CaptureRule *)rule;
- (CaptureRuleResult *)runPTPAnnounceMessageGrandmasterPriority2Invalid:(CaptureRule *)rule;
- (CaptureRuleResult *)runPTPAnnounceMessageGrandmasterClockQualityClockClassInvalid:(CaptureRule *)rule;
- (CaptureRuleResult *)runPTPAnnounceMessageGrandmasterClockQualityClockAccuracyInvalid:(CaptureRule *)rule;
- (CaptureRuleResult *)runPTPAnnounceMessageGrandmasterClockQualityOffsetScaledLogVarianceInvalid:(CaptureRule *)rule;

@end
