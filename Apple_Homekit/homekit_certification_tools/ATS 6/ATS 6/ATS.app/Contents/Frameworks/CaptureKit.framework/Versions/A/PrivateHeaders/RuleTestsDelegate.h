//
//  RuleTestsDelegate.h
//  ATSMacApp
//
//  Created by Marin Balde on 9/24/10.
//  Copyright 2010 Apple Inc. All rights reserved.
//

#import <CaptureKit/ProductPlanTypes.h>

@class ATSAppEvent;
@class ProductPlan;

@protocol RuleTestsDelegate

- (void)insertMessageForCurrentlyRunningTest:(NSString *)message;

@end

@protocol RuleTestsDocumentDelegate

@property (nonatomic, readwrite, assign) BOOL allowCommandsWithReservedErrors;

- (void)simulateCertificationAssistantRunning;
- (void)simulateCertificationAssistantNotRunning;
- (void)forceClaim:(NSString *)claimName;
- (void)clearAllForcedClaims;

/*!
 @method    resetForcedProductPlan
 @abstract  Resets and clears the forced product plan
 */
- (void)resetForcedProductPlan;

/*!
 @method    forcedProductPlanInfo
 @abstract  Access to the forced product plan (info) so that features/etc may be added
 */
- (NSMutableDictionary *)forcedProductPlanInfo;

/*!
 @method    insertATSAppEvent:
 @abstract  Delivers the out of band event to all relevant traffic views and rule engines
 @see       CaptureDocument -insertATSAppEvent
 */
- (void)insertATSAppEvent:(ATSAppEvent *)event;

/*!
 @method    ruleTestsAccumulatedEvents
 @abstract  Returns an array of all accumulated events from the rule tests
 */
- (NSArray *)ruleTestsAccumulatedEvents;

/*!
 @method    supportsiAP2
 @abstract  Whether or not iAP2 is supported
 */
- (BOOL)supportsiAP2;

/*!
 @method    supportsiAP1
 @abstract  Whether or not iAP1 is supported
 */
- (BOOL)supportsiAP1;

@end
