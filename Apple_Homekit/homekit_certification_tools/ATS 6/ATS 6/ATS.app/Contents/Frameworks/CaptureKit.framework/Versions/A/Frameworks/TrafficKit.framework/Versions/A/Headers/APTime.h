//
//  APTime.h
//  TrafficKit
//
//  Created by Mark Hamlin on 5/15/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@class APTimer;
@interface APTime : NSObject

/*!
 @method    updateCurrentTime:
 @abstract  Updates the current time to "now"
 */
- (void)updateCurrentTime:(double)now;

/*!
 @method    currentTime
 @abstract  Access the current time
 */
- (double)currentTime;

/*!
 @method        driveTimeUsingRealTimeWithTick:
 @abstract      Sets up an NSTimer to drive the capture timer using the provided tick interval
 @discussion    To support offline captures from reloaded capture sources, we need to be able to drive the current time
                in a way that's not at all related to clock time. If your application needs realtime capture, you MUST
                call this method once to setup a driver. Otherwise, it's your responsibility to call +updateCurrentTime:
 */
- (void)driveTimeUsingRealTimeWithTick:(double)tickIntervalInSeconds;

/*!
 @method    scheduledTimerWithTimeInterval:target:selector:repeats:
 @abstract  Similar to NSTimer's version, schedules a timer.
 */
- (APTimer *)scheduledTimerWithTimeInterval:(NSTimeInterval)ti target:(id)aTarget selector:(SEL)aSelector repeats:(BOOL)yesOrNo;

/*!
 @method    reset
 @abstract  Reset the time manager. This is only required if using real-time driving
 */
- (void)reset;

@end

@interface APTimer : NSObject

/*!
 @method    invalidate
 @abstract  Invalidates the timer
 */
- (void)invalidate;

@end
