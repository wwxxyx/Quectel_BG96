//
//  CarPlayControlResponseEvent.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 6/6/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/CarPlaySessionEvent.h>
@class APHTTPBPlistContainer;

@interface CarPlayControlResponseEvent : CarPlaySessionEvent

/*!
 @property  container
 @abstract  The container for the control request
 @see       CarPlayMessages.h
 */
@property (nonatomic, retain) APHTTPBPlistContainer* container;

@end
