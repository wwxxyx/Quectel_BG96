//
//  EthernetCaptureProtocol+Processing.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 5/16/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/EthernetCaptureProtocol.h>

#import <TrafficKit/TrafficKit.h>

#define kUnknownPortValue 0xFFFFFFFF

#import <CaptureKit/TCPFlowBeganEvent.h>
#import <CaptureKit/TCPFlowEndedEvent.h>
#import <CaptureKit/TCPFlowDataEvent.h>
#import <CaptureKit/UDPFlowBeganEvent.h>
#import <CaptureKit/UDPFlowEndedEvent.h>
#import <CaptureKit/UDPFlowDataEvent.h>
#import <CaptureKit/HTTPRequestEvent.h>
#import <CaptureKit/HTTPResponseEvent.h>
#import <CaptureKit/HTTPIncompleteMessageEvent.h>
#import <CaptureKit/CarPlayMessages.h>

extern const NSUInteger kPTPEventMessageUDPPort;
extern const NSUInteger kPTPGeneralMessageUDPPort;


@interface EthernetCaptureProtocol ()

@property (nonatomic) UInt64 currentTimestamp;

// CarPlay Session (Controller control pipe)
/**
 RTSP Controller control flow; Direction, device to accessory; iOS control
 */
@property (nonatomic, strong) NSMutableSet<APHTTPStreamAnalyzer *> *rtspControllerControlAnalyzers;

/**
 CarPlay Session (Accessory control pipe)
 */
@property (nonatomic) NSUInteger rtspAccessoryControlPort; // Dynamic

/**
 Apple devices supporting CarPlay will advertise a Bonjour service (of a server type _carplay-ctrl._tcp) to the accessory. This HTTP stream analyzer parses HTTP protocol over a TCP connection that is used to initiate a CarPlay session with an Apple device.
 <rdar://problem/25422767> [ER] Show CarPlay Control Connect message in the CarPlay Session traffic view
 */
@property (nonatomic, strong) APHTTPStreamAnalyzer *appleDeviceCarPlayCtrlAnalyzer;

/**
 RTSP Accessory control flow; Direction, accessory to device; Accessory control
 */
@property (nonatomic, strong) APHTTPStreamAnalyzer *rtspAccessoryControlAnalyzer;

// Main audio
@property (nonatomic) NSUInteger rtpAccessoryMainAudioPort;   // Dynamic
@property (nonatomic) NSUInteger rtpAppleDeviceMainAudioPort; // Dynamic
@property (nonatomic, strong) APUDPFlow *rtpAccessoryReceivingMainAudioFlow;
@property (nonatomic, strong) APUDPFlow *rtpAppleDeviceReceivingMainAudioFlow;

// Alt audio
@property (nonatomic) NSUInteger rtpAccessoryAltAudioPort; // Dynamic
@property (nonatomic, strong) APUDPFlow *rtpAccessoryAltAudioFlow;

// Screen
@property (nonatomic) NSUInteger rtpAccessoryScreenPort; // Static <rdar://problem/15204420> RTP Screen negotiation is missing/not documented
@property (nonatomic, strong) APTCPFlow *rtpAccessoryScreenFlow;

// Main High audio port (CarPlay wireless)
/**
 RTP Main High audio port identified over CarPlay wireless.
 */
@property (nonatomic) NSUInteger rtpAccessoryMainHighAudioPort;

/**
 Main High audio flow for CarPlay wireless.
 */
@property (nonatomic) APUDPFlow *rtpAccessoryMainHighAudioFlow;

/**
 IP address of the Apple device that is involved in the CarPlay session.
 */
@property (nonatomic) NSMutableSet<NSString *> *appleDeviceAddresses;

/**
 IP address of the CarPlay accessory that is involved in the CarPlay session.
 */
@property (nonatomic) NSArray<NSString *> *accessoryAddresses;

/**
 PTP Event Message Flows
 */
@property (nonatomic, strong) NSMutableSet<APUDPFlow *> *ptpEventMessageFlows;

/**
 PTP General Message Flows
 */
@property (nonatomic, strong) NSMutableSet<APUDPFlow *> *ptpGeneralMessageFlows;

@end

@interface EthernetCaptureProtocol (Processing) <APHTTPStreamAnalyzerDelegate, APTCPFlowTableConnectionDelegate, APUDPFlowTableConnectionDelegate, APTCPDataStreamDelegate, APUDPDataStreamDelegate, APmDSNBonjourServiceDelegate>

/**
 Whether or not the media streams (if seen) in the CarPlay session be processed and saved to the trace.
 */
@property (nonatomic, readonly) BOOL shouldIncludeStreamingMedia;

/*!
 @method    resetParser
 @abstract  Reset all parser state
 */
- (void)resetParser;

@end
