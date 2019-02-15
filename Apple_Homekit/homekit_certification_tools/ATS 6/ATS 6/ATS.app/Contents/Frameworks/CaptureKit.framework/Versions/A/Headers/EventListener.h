//
//  EventListener.h
//  ATSMacApp
//
//  Created by Sergio Sette on 10/28/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

@class CaptureEvent;

@protocol EventListener

/*!
    Processes the event
    @param event    The event to be processed
 */
- (void)processEvent:(CaptureEvent *)event;

/*!
    Updates the current time
    @param timestamp The new value of time
 */
- (void)increaseCurrentTimeTo:(UInt64)timestamp;

/*!
    A Set of Event derived classes for which processEvent may be called.
    If the Event is not of a registered class it will be ignored.
    @note IMPORTANT! CaptureRuleGroup subclasses must not implement this method. They must use ruleGroupRegisteredEvents instead!
 */
- (NSSet *)registeredEvents;

@end
