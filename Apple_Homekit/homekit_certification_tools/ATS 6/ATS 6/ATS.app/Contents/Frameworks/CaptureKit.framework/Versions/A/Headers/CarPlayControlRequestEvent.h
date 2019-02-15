//
//  CarPlayControlRequestEvent.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 6/6/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/CarPlaySessionEvent.h>

@import TrafficKit.APHTTPBPlistContainer;

@interface CarPlayControlRequestEvent : CarPlaySessionEvent <APHTTPBPlistReportAnalyzerDataSource>

/*!
 @property  container
 @abstract  The container for the control request
 @see       CarPlayMessages.h
 */
@property (nonatomic, retain) APHTTPBPlistContainer* container;

@end
