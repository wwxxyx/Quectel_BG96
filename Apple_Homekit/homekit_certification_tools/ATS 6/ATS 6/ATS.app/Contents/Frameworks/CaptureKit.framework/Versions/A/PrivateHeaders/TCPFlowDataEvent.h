//
//  TCPFlowDataEvent.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 5/15/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureEvent+ReportAnalyzer.h>

#import <TrafficKit/TrafficKit.h>

@interface TCPFlowDataEvent : CaptureEvent

/*!
 @property  flow
 @abstract  The TCP flow which generated this event
 */
@property (nonatomic, retain) APTCPFlow *flow;

/*!
 @property  data
 @abstract  The chunk of the TCP stream
 */
@property (nonatomic, retain) NSData *data;

/*!
 @property  senderIsEndpointZero
 @abstract  Whether or not the sender of this data is endpoint 0 of the flow
 */
@property (nonatomic, assign) BOOL senderIsEndpointZero;

@end
