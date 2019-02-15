//
//  RuleEngine.h
//  ATS
//
//  Created by Marin Balde on 7/12/10.
//  Copyright 2010 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CaptureKit/StenographerDelegate.h>
#import <CaptureKit/RuleEngineDelegate.h>
#import <CaptureKit/DeviceCaptureConfigurationTypes.h>
#import <CaptureKit/CaptureRuleGroupEventListener.h>
#import <CaptureKit/CaptureConfigurationDescription.h>
#import <CaptureKit/Stenographer.h>

@class Rule;
@class RuleError;
@class iAP1PacketEvent;
@class ATSRuleEngine;
@class CaptureEvent;

enum {
    RuleEngineModeMethods,
    RuleEngineModeBlocks,
};
typedef UInt8 RuleEngineMode;

@interface ATSRuleEngine : NSObject <StenographerDelegate, CaptureRuleGroupEventListener> {
    // all public instance methods use this lock to guarantee
    // that they all execute sequentially when called from any thread
    // in practice, they should only be called from a single thread, usually the main thread
    NSRecursiveLock *_engineLock;

    NSUInteger *_resolutionStates; // array of RuleResolutionStates for each Rule

    NSMutableArray *_runQueue;

    NSMutableArray *_surveillanceBlockQueues;          // array of arrays
    NSMutableArray *_currentSurveillanceBlockSubqueue; // the current subqueue
    NSMutableArray *_newSurveillanceBlockSubqueue;     // the new subqueue

    NSMutableArray *_errorQueue;
    NSMutableArray *_captureErrorQueue;

    RuleEngineMode _mode;

    UInt64 _timestamp;
}

@property (nonatomic, weak) id<RuleEngineDelegate> delegate;

@property (nonatomic, readonly) Stenographer *stenographer;

/*!
    The capture configuration selected by the user
 */
@property (nonatomic, strong) CaptureConfigurationDescription *captureConfiguration;

@property (nonatomic) UInt64 timestamp;

/*!
 A set of event classes which this rule engine expects to see.
 */
@property (nonatomic, readonly, strong) NSArray *whitelistedEvents;

// Class Methods
// returns YES if the graph was successfully loaded
+ (BOOL)loadGraph;
+ (Rule *)ruleNodeForRuleID:(NSUInteger)ruleID;
+ (NSArray *)allRules;

// Instance Methods

/**
 Create an instance of Rule Engine with input capture configuration, available events and stenographer.
 
 @param captureConfiguration This is required to know the transport type the rule engine is associated with.
 @param whitelistedEvents List of all the events that this rule engine should care about. If there are event types that rule engine receives which are not part of the whitelistedEvents, it will Assert.
 @param stenographer Stenographer instance that the rule engine needs to be the delegate of.
 
 @note Rule engine is always the delegate of the Stenographer. The only reason rule engine does not create stenographer is because previously some modules inside the stenographer were required by other some capture protocols before rule engine was created.
 */
- (instancetype)initWithCaptureConfiguration:(CaptureConfigurationDescription *)captureConfiguration whitelistedEvents:(NSArray *)whitelistedEvents stenographer:(Stenographer *)stenographer;

- (void)reset;
- (void)processEvent:(CaptureEvent *)event;
- (void)increaseCurrentTimeTo:(UInt64)timestamp;

/*!
 @method   simulateCertificationAssistantRunning
 @abstract This is a method to enable the rule tests to perform any back-door initialization that normally is provided by the CA workflow
 */
- (void)simulateCertificationAssistantRunning;

/*!
 @method    isEligibleRuleID:
 @abstract  Whether or not we are configured to run this rule
 */
- (BOOL)isEligibleRuleID:(NSUInteger)ruleID;

@end
