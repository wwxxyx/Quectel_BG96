//
//  Stenographer.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 4/25/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <CaptureKit/StenographerDelegate.h>
#import <CaptureKit/Rule.h>
#import <CaptureKit/RuleProtocols_Generated.h>
#import <CaptureKit/StenographModule.h>
#import <CaptureKit/CaptureEvent.h>
#import <CaptureKit/DeviceCaptureConfigurationTypes.h>

#import <CaptureKit/ATSEightPinStenograph.h>
#import <CaptureKit/iAP1Stenograph.h>
#import <CaptureKit/iAP2Stenograph.h>
#import <CaptureKit/AppStenograph.h>
#import <CaptureKit/ATSCarPlayStenograph.h>
#import <CaptureKit/IDBusStenograph.h>

@interface Stenographer : NSObject

/*!
 Initialize stenographer.
 */
- (instancetype)init;

/*!
 Initialize stenographer with delegate.

 @param delegate that is called to notify rule completion.
 */
- (instancetype)initWithDelegate:(id<StenographerDelegate>)delegate;

/*!
 @property  delegate
 @abstract  The stenographer delegate
 */
@property (nonatomic, assign) id<StenographerDelegate> delegate;

/*!
 @property  currentTime
 @abstract  The latest time, as set by increaseCurrentTimeTo:
 */
@property (nonatomic, readonly, assign) UInt64 currentTime;

/*!
 @property  currentEvent
 @abstract  The latest event seen
 */
@property (nonatomic, readonly, retain) CaptureEvent *currentEvent;

/*!
 @method    processEvent:
 @abstract  Handle an event as the new current event, passing to all modules
 */
- (void)processEvent:(CaptureEvent *)event;

/*!
 @method    increaseCurrentTimeTo:
 @abstract  Update the current time
 */
- (void)increaseCurrentTimeTo:(UInt64)delta;

/*!
 @method    completeRule:*
 @abstract  before a rule method or block exits, it must call one of these three methods first
 */
- (void)completeRule:(Rule *)rule;
- (void)completeRule:(Rule *)rule withError:(NSString *)errorString event:(CaptureEvent *)event;
- (void)completeRule:(Rule *)rule withError:(NSString *)errorString events:(CaptureEvent *)event, ... NS_REQUIRES_NIL_TERMINATION;
- (void)completeRule:(Rule *)rule withError:(NSString *)errorString eventsArray:(NSArray *)events;

/*!
 @method    scheduleSurveillanceBlock:forRule:
 @abstract  Schedules a surveillance block from within a rule
 */
- (void)scheduleSurveillanceBlock:(void (^)(void))block forRule:(Rule *)rule;

// Forwards to delegate
- (BOOL)areClaimSpecificRulesEnabled;
- (BOOL)isClaimEnabled:(NSString *)claimName;
- (BOOL)isAppleDeviceActingAsUSBDevice;

/*!
 @method    isEventClassAvailable:
 @abstract  Whether or not events of this class are available
 */
- (BOOL)isEventClassAvailable:(Class)aClass;

/*!
 @method        isTrafficOptional
 @abstract      Whether or not Accessory traffic (iAP, iAP2, future) is required in this configuration
 @discussion    This is set if traffic may be seen on this transport, but is not expected
                It can be used for rules which would otherwise expect traffic x seconds after
                a transport event (reset, power up), so that they don't fire if traffic isn't
                required.
 */
- (BOOL)isTrafficOptional;

/*!
 @method    deviceTransportType
 @abstract  The current dock transport type being used
 */
- (DeviceTransportType)deviceTransportType;

#pragma mark -
#pragma mark Modules

@property (nonatomic, readonly, strong) ATSEightPinStenograph *eightPin;
@property (nonatomic, readonly, strong) iAP1Stenograph *iAP1;
@property (nonatomic, readonly, strong) iAP2Stenograph *iAP2;
@property (nonatomic, readonly, strong) AppStenograph *app;
@property (nonatomic, readonly, strong) IDBusStenograph *idBus;

#pragma mark -
#pragma mark Simulation

/*!
 @method   simulateCertificationAssistantRunning
 @abstract This is a method to enable the rule tests to perform any back-door initialization that normally is provided by the CA workflow
 */
- (void)simulateCertificationAssistantRunning;

@end
