//
//  CaptureRuleStenograph.h
//  ATSMacApp
//
//  Created by Sergio Sette on 10/29/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CaptureKit/EventListener.h>

@interface CaptureRuleStenograph : NSObject

/*!
    The last event processed by the stenograph
 */
@property (nonatomic, strong) CaptureEvent *currentEvent;

/*!
 Timestamp of the most recent CaptureEvent seen over the transport.
 */
@property UInt64 currentTimestamp;

/*!
    Resets all the variables stored by the stenograph
 */
- (void)reset;

/*!
 Event classes which provoke a reset of the stenographer
 @return An NSSet of Event subclasses which provoke a reset of the stenographer.
 */
- (NSSet *)registeredEventsForReset;

/*!
 Classes that inherit from CaptureRuleStenograph must overwrite this method with the Event subclasses they can process.
 @return An NSSet of Event subclasses which can be processed by the processEvent method.
 */
- (NSSet *)registeredEventsForStenograph;

/*!
    Increases the current time value and calls clearCurrentEvent
 */
- (void)increaseCurrentTimeTo:(UInt64)timestamp;

/*!
 Processes the event
 @param event    The event to be processed
 */
- (void)processEvent:(CaptureEvent *)event;

@end
