//
//  UDPFlowEndedEvent.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 5/15/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureEvent+ReportAnalyzer.h>
#import <TrafficKit/TrafficKit.h>

@interface UDPFlowEndedEvent : CaptureEvent

/*!
 @property  flow
 @abstract  The TCP flow which generated this event
 */
@property (nonatomic, retain) APUDPFlow *flow;

@end
