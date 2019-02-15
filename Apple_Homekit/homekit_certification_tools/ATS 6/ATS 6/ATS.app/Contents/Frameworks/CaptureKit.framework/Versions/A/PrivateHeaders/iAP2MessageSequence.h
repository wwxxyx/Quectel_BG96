//
//  iAP2MessageSequence.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 7/30/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <CaptureKit/iAP2MessageEvent.h>

typedef int iAP2MessageSequenceState;

/*!
 @class     iAP2MessageSequence
 @abstract  Abstract representation of a required sequence of messages (state machine)
 */
@interface iAP2MessageSequence : NSObject

/*!
 @property  state
 @abstract  Current sequence state
 */
@property (readonly, nonatomic, assign) iAP2MessageSequenceState state;

/*!
 @method    messageSequence
 @abstract  Creates a new, autoreleased message sequence object
 */
+ (instancetype)messageSequence;

/*!
 @method    moveToState:
 @abstract  Moves to the given state
 */
- (void)moveToState:(iAP2MessageSequenceState)state;

#pragma mark -
#pragma mark Subclassers

/*!
 @method        processEvent:
 @abstract      Process a message event
 @param         event
                Event to process
 @discussion    Subclassers must override this
 */
- (void)processEvent:(iAP2MessageEvent *)event;

/*!
 @method        currentStateName
 @abstract      Returns the human readable string of the current state
 @discussion    Subclassers must override this
 */
- (NSString *)currentStateName;

/*!
 @method        isAccepting
 @abstract      Whether or not the current state is an acceptable (stable) state
 @discussion    This is optional. The default is NO. This can be used to determine if an acceptable state has been reached, such as for identification if the process has either cancelled or succeeded
 */
- (BOOL)isAccepting;

/*!
 @method        reset
 @abstract      Resets to initial state before any events were processed
 @discussion    The superclass implementation just moves to state 0. Subclassers may override to do more.
 */
- (void)reset;

@end
