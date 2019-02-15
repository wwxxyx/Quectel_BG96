//
//  StenographModule.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 8/27/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@class CaptureEvent;
@class EightPinHardwareEvent;
@class Stenographer;

@interface StenographModule : NSObject

/*!
 @property  stenographer
 @abstract  The stenographer which owns this module
 */
@property (nonatomic, assign) Stenographer *stenographer;

/*!
 @method    stenograph
 @abstract  Default constructor (alloc+init+autorelease)
 */
+ (instancetype)stenograph;

/*!
 @method        processEvent:
 @abstract      Process given event. Must be called on same thread as other accessors
 @discussion    Subclassers may either override this, or add a convenience method to the root class for other modules to take advantage of
 */
- (void)processEvent:(CaptureEvent *)event;

/*!
 @method        registerEventProcessor:event:
 @abstract      Registers a selector to be invoked every time an event of the given class is encountered
 @param         processor
                Selector to be invoked
 @param         event
                Event class to trigger invocation of the selector
 */
- (void)registerEventProcessor:(SEL)processor event:(Class)event;

/*!
 Returns a set of event classes that this stenograph module has registered for processEvent.
 */
- (NSSet *)registeredEventsForStenograph;

#pragma mark -
#pragma mark Reset (Subclassers)

/*!
 @method    hardwareReset
 @abstract  Called by superclass whenever there is an accessory disconnect
 */
- (void)hardwareReset;

/*!
 @method    clearAllCurrentEvents
 @abstract  Clears all current events
 */
- (void)clearAllCurrentEvents;

#pragma mark -
#pragma mark Simulation

/*!
 @method   simulateCertificationAssistantRunning
 @abstract This is a method to enable the rule tests to perform any back-door initialization that normally is provided by the CA workflow
 */
- (void)simulateCertificationAssistantRunning;

@end
