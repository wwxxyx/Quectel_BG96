//
//  RuleError.h
//  ATS
//
//  Created by Marin Balde on 7/15/10.
//  Copyright 2010 Apple Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <CaptureKit/CaptureRuleResult.h>
#import <CaptureKit/CaptureEvent.h>

// error domain
extern NSString *const RuleErrorDomain;

// ATSRuleErrorCodes are equivalent to the ATSRule IDs from RuleConstants.h,
// therefore no separate enum for error codes is defined here

@class Rule;

@interface RuleError : NSError

+ (instancetype)errorWithCaptureRuleResult:(CaptureRuleResult *)captureRuleResult;
+ (instancetype)errorWithRule:(Rule *)rule description:(NSString *)errorString events:(NSArray<CaptureEvent *> *)events forceErrorLevel:(NSUInteger)errorLevel;
+ (instancetype)errorWithRule:(Rule *)rule description:(NSString *)errorString events:(NSArray<CaptureEvent *> *)events;

@property (readonly) NSUInteger ruleID; // same as code
@property (readonly) NSString *errorString; // same as localizedDescription
@property (readonly) NSUInteger errorLevel;
@property (readonly) NSString *ruleName;
@property (readonly) NSArray<CaptureEvent *> *events;
@property (readonly) BOOL productPlanRule;
@property (readonly) NSString *specFilePredicate;
@property (readonly) NSString *specReferenceLabel;
@property (readonly) NSNumber *specPageNumber;
@property (readonly) NSString *specSectionName;

// Used in Rule tests
@property (nonatomic) BOOL asserted;

@end
