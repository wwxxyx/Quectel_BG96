//
//  CaptureRuleResult.h
//  ATSMacApp
//
//  Created by Sergio Sette on 10/28/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CaptureKit/CaptureRuleInformation.h>

typedef NS_ENUM(NSUInteger, CaptureRuleStatus) {
    /// Code to represent that the rule failed
    CaptureRuleStatusFinishedWithError,
    /// Code to represent that the rule finished with no errors
    CaptureRuleStatusFinishedWithSuccess,
    /// Code to represent that the rule has not reached its conclusion yet
    CaptureRuleStatusStillRunning,
};

@interface CaptureRuleResult : NSObject

/*!
    The status of the rule result
 */
@property CaptureRuleStatus status;


/*!
    The CaptureRuleInformation of the rule that generated this result
 */
@property (nonatomic, strong) CaptureRuleInformation *ruleInformation;

/*!
    The events that caused the rule failure
 */
@property (nonatomic, strong) NSArray *events;

/*!
    The message to be displayed if the rule failed
 */
@property (nonatomic, strong) NSString *errorMessage;

/*!
    Constructs a new CaptureRuleResult with the specified CaptureRuleInformation, CaptureRuleStatus, error message and events
    @param rule     the rule information
    @param status   the rule result status
    @param message  the message to be displayed if the rule failed
    @param events   the events that caused the rule failure
    @return a new CaptureRuleResult
 */
+ (CaptureRuleResult *)captureRuleResultWithRuleInformation:(CaptureRuleInformation *)ruleInformation
                                                  andStatus:(CaptureRuleStatus)status
                                                 andMessage:(NSString *)message
                                                  andEvents:(NSArray *)events;

/*!
    Constructs a new CaptureRuleResult of Success with the specified CaptureRuleInformation
    @param rule     the rule information
    @return a new CaptureRuleResult
 */
+ (CaptureRuleResult *)captureSuccessRuleResultWithRuleInformation:(CaptureRuleInformation *)ruleInformation;

/*!
 Constructs a new CaptureRuleResult of StillRunning with the specified CaptureRuleInformation
 @param rule     the rule information
 @return a new CaptureRuleResult
 */
+ (CaptureRuleResult *)captureStillRunningRuleResultWithRuleInformation:(CaptureRuleInformation *)ruleInformation;

/*!
 Whether or not this result represents a reportable error string that needs to be shown in traffic view.
 */
@property (nonatomic, readonly) BOOL hasReportableResult;

/*!
 Whether or not this result has reached to its conclusion.
 */
@property (nonatomic, readonly) BOOL isFinished;

@end
