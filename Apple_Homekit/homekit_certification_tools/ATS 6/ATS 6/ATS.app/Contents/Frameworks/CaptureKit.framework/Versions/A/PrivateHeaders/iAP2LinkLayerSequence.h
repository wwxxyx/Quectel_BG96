//
//  iAP2LinkLayerSequence.h
//  ATSMacApp
//
//  Created by Natália Cabral on 06/08/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CaptureKit/iAP2LinkPacketEvent.h>

typedef int iAP2LinkOperationState;

@interface iAP2LinkLayerSequence : NSObject

/*!
    Current operation state
 */
@property (readonly, nonatomic, assign) iAP2LinkOperationState state;

/*!
    Creates a new, autoreleased iAP2 link operation object
 */
+ (instancetype)iAP2LinkOperation;

/*!
    Moves to the given state
 */
- (void)moveToState:(iAP2LinkOperationState)state;

#pragma mark -
#pragma mark Subclassers

/*!
    Process an iAP2 link packet event
    @param         event
    Event to process
    @discussion    Subclassers must override this
 */
- (void)processEvent:(iAP2LinkPacketEvent *)event;

/*!
    Resets to initial state before any events were processed
    @discussion    The superclass implementation just moves to state 0. Subclassers may override to do more.
 */
- (void)reset;

@end
