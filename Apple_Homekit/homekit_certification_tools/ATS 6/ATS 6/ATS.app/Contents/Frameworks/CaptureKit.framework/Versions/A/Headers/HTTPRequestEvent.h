//
//  HTTPRequestEvent.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 5/16/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureEvent.h>
#import <BusSniffer/BusSniffer.h>

@class APHTTPRequest;
@class CarPlayControlRequestEvent;

@interface HTTPRequestEvent : CaptureEvent

/*!
 @property  source
 @abstract  The source of the request
 */
@property (nonatomic, assign) BSiAPSource source;

/*!
 @property  request
 @abstract  The HTTP request object
 */
@property (nonatomic, retain) APHTTPRequest *request;

/*!
 @property  controlRequestEvent
 @abstract  The control request event, if any
 */
@property (nonatomic, retain) CarPlayControlRequestEvent *controlRequestEvent;

@end
