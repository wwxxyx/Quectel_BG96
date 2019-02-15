//
//  PTPMessageEvent.h
//  ATSMacApp
//
//  Created by Vinod Madigeri on 4/29/18.
//  Copyright © 2018 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureEvent.h>
#import <BusSniffer/BSiAPTypes.h>
#import <PTPKit/PTPKit.h>

@class UDPFlowDataEvent;

@interface PTPMessageEvent : CaptureEvent

/** Instance of PTP Event Message */
- (instancetype)initWithUDPDataEvent:(UDPFlowDataEvent *)udpEvent
                    messageClassType:(PTPMessageClassType)messageClassType
                              source:(BSiAPSource)source;

/** The source of the PTP message */
@property (nonatomic, readonly) BSiAPSource source;

/** PTP message */
@property (nonatomic, readonly) PTPMessage ptpMessage;

/** PTP error */
@property (nonatomic, readonly) NSArray<NSError *> *ptpParseErrors;

/** UDP flow data event with PTP message */
@property (nonatomic, readonly) UDPFlowDataEvent *udpDataEventWithPTP;

/** PTP message class type */
@property (nonatomic, readonly) PTPMessageClassType messageClassType;

/** String description for the message class type */
@property (nonatomic, readonly) NSString *messageClassTypeString;

/** String description for the source IP address */
@property (nonatomic, readonly) NSString *sourceIPAddress;

/** String description for the destination IP address */
@property (nonatomic, readonly) NSString *destinationIPAddress;

@end
