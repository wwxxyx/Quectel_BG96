//
//  NetworkPacketEvent.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 5/15/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureEvent+ReportAnalyzer.h>

#import <TrafficKit/TrafficKit.h>

@interface NetworkPacketEvent : CaptureEvent

/*!
 @property  packet
 @abstract  The captured packet
 */
@property (nonatomic, readonly, retain) APPacket *packet;

/*!
 @method    networkPacketWithPacket:timestamp:
 @abstract  Creates a NetworkPacketEvent using the timestamp and passed packet
 */
+ (NetworkPacketEvent *)networkPacketWithPacket:(APPacket *)packet timestamp:(UInt64)timestamp;

/*!
 @method    reportAnalyzerEntry
 @abstract  Generates and returns a report analyzer entry for this event
 */
- (APReportAnalyzerEntry *)reportAnalyzerEntry;

@end
