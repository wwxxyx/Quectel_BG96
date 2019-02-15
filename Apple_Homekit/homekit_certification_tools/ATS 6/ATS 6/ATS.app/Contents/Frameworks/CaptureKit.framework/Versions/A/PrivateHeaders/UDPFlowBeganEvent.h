//
//  UDPFlowBeganEvent.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 5/15/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureEvent+ReportAnalyzer.h>
#import <TrafficKit/TrafficKit.h>

@interface UDPFlowBeganEvent : CaptureEvent

/*!
 The UDP flow which generated this event
 */
@property (nonatomic, retain) APUDPFlow *flow;

/*!
 The description of the UDP flow
 */
@property (nonatomic, copy) NSString *flowDescription;

@end
