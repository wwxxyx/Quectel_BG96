//
//  EthernetCaptureProtocol.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 5/14/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/ATSCaptureProtocol.h>
#import <CaptureKit/CaptureEvent+ReportAnalyzer.h>
#import <CaptureKit/ATSCarPlayCaptureProtocol.h>

@interface EthernetCaptureProtocol : ATSCaptureProtocol <ATSCarPlayCaptureProtocolDelegate>

@property (nonatomic, retain) ATSCarPlayCaptureProtocol *carplayCaptureProtocol;

@property (nonatomic, readonly, strong) NSString *captureProtocolName;

/**
 Time manager instance used by NetworkCapture, TCPFlowTable and UDPFlowTable.
 */
@property (nonatomic, strong) APTime *timeManager;

/**
 NetworkCaptur instance responsible for making APPackets with the ethernet frames seen during traffic sniffing.
 */
@property (nonatomic, strong) APNetworkCapture *networkCapture;

/**
 TCP flow table to keep track of when a new TCP connection starts and ends.
 */
@property (nonatomic, strong) APTCPFlowTable *tcpFlowTable;

/**
 UDP flow table to keep track of when a new UDP connection starts and ends.
 */
@property (nonatomic, strong) APUDPFlowTable *udpFlowTable;

/**
 Flow table to receive Bonjour Service Instances from.
 */
@property (nonatomic, strong) APmDNSFlowTable *mDNSFlowTable;

/**
 Whether or not this Ethernet Capture Protocol belongs to wireless CarPlay session.
 */
@property (nonatomic, readonly) BOOL isWirelessCarPlay;

/*!
 @method    processEthernetFrameData:atTimestamp:
 @abstract  Process an ethernet frame given its raw bytes
 @param     relevant Whether or not this frame is relevant and should be recorded to file (assigned to)
 */
- (void)processEthernetFrameData:(NSData *)ethernetFrame atTimestamp:(UInt64)timestamp relevant:(BOOL *)relevant;

/*!
 @method    deliverEvent:
 @abstract  Assign event dependencies and send to transport for final delivery
 */
- (void)deliverEvent:(CaptureEvent *)event;

/*!
 @method    resetTransport
 @abstract  Resets the transport and any analyzers
 */
- (void)resetTransport;

@end
