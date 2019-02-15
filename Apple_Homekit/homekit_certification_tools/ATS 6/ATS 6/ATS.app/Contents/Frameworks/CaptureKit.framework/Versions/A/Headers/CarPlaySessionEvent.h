//
//  CarPlaySessionEvent.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 6/6/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureEvent.h>
@import BusSniffer.BSiAPTypes;

@interface CarPlaySessionEvent : CaptureEvent

/*!
 @property  errorDescription
 @abstract  Error description for the event
 */
@property (nonatomic, retain) NSString *errorDescription;

/*!
 @property  source
 @abstract  The source of the request
 */
@property (nonatomic, assign) BSiAPSource source;

@end
