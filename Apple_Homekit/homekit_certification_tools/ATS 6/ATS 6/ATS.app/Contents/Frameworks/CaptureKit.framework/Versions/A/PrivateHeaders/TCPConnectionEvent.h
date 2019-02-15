//
//  TCPConnectionEvent.h
//  ATSMacApp
//
//  Created by Vinod Madigeri on 10/16/17.
//  Copyright © 2017 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureEvent.h>

@class APTCPFlow;

typedef NS_ENUM(NSUInteger, TCPConnectionStatus) {
    /**
     A new TCP stream is opened.
     */
    TCPConnectionStatusOpened,
    /**
     A TCP stream is closed.
     */
    TCPConnectionStatusClosed,
};

@interface TCPConnectionEvent : CaptureEvent

/**
 Instantiates an event to describe a non-CarPlay TCP flow.
 
 @param timestamp Timestamp of when the TCP connection started or finished.
 @param isSuitableForRuleEngine Whether or not this is suitable for rule engine.
 @param tcpFlow TCP flow associated with this event.
 @param connectionStatus Whether this TCP connection opened or closed.
 
 @return An instance of TCPConnectionEvent.
 */
- (instancetype)initWithTimestamp:(UInt64)timestamp
            suitableForRuleEngine:(BOOL)isSuitableForRuleEngine
                          tcpFlow:(APTCPFlow *)tcpFlow
                 connectionStatus:(TCPConnectionStatus)connectionStatus;

/**
 The TCP flow associated with the event.
 */
@property (nonatomic, readonly) APTCPFlow *tcpFlow;

/**
 Whether this TCP connection started or ended.
 */
@property (nonatomic, readonly) TCPConnectionStatus connectionStatus;

@end
