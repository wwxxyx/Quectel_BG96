//
//  UDPFlowDataEvent.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 5/15/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureEvent+ReportAnalyzer.h>
#import <TrafficKit/TrafficKit.h>

@interface UDPFlowDataEvent : CaptureEvent

/*!
 @property  flow
 @abstract  The UDP flow which generated this event
 */
@property (nonatomic, retain) APUDPFlow *flow;

/*!
 @property  data
 @abstract  The UDP payload
 */
@property (nonatomic, retain) NSData *data;

/*!
 @property  senderIsEndpointZero
 @abstract  Whether or not the sender of this data is endpoint 0 of the flow
 */
@property (nonatomic, assign) BOOL senderIsEndpointZero;

@end
