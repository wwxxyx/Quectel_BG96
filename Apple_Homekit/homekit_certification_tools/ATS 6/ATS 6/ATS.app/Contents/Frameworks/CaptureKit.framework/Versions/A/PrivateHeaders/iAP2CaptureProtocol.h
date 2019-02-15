//
//  iAP2CaptureProtocol.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 4/30/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/ATSCaptureProtocol.h>

#import <CaptureKit/iAP2ControlSessionTrafficViewController.h>
#import <CaptureKit/iAP2LinkPacketsTrafficViewController.h>
#import <CaptureKit/iAP2FileTransferTrafficViewController.h>

#import <iAP2MessageKit/I2MRichParser.h>

@interface iAP2CaptureProtocol : ATSCaptureProtocol <I2MRichParserDataSource>

@property (nonatomic, readonly, strong) iAP2ControlSessionTrafficViewController *iAP2ControlSessionTrafficViewController;
@property (nonatomic, readonly, strong) iAP2FileTransferTrafficViewController *fileTransferTrafficViewController;
@property (nonatomic, readonly, strong) iAP2LinkPacketsTrafficViewController *linkPacketsTrafficViewController;

@property (nonatomic, readonly, strong) NSString *captureProtocolName;

/*!
 Initialize with transport and a link packet analyzer from that transport.
 
 @param transport Capture transport underlying the protocol.
 @param parentCaptureProtocol Capture protocol of which ever protocol that's creating this protocol.
        Can be nil, in case this protocol is bottom of the stack and is being created by the transport.
        Should be valid in case this protocol is carried over the parent protocol.
 @param linkPacketAnalyzer reference to LinkPacketOver*Transport*Analyzer; Used to process link packets.
 */
- (instancetype)initWithTransport:(CaptureTransport *)transport andParentCaptureProtocol:(ATSCaptureProtocol *)parentCaptureProtocol linkPacketAnalyzer:(id)linkPacketAnalyzer;

/*!
 Process an array of BSiAP2LinkPackets
 
 @param linkPackets Input Link packets to be processed.
 */
- (void)processLinkPackets:(NSArray *)linkPackets;

/*!
 Method to add sub protocols (that use the new Traffic Kit views) carried over iAP2. Call this method after adding iAP2 protocol to the transport.
 If we were to internally handle adding subprotocols, the EA traffic view would appear to the top of the iAP2 categories, but we want Control Session to be on the top.
 We should get that behavior for free when we transition to all Traffic Views in iAP2 to using the Traffic Kit.
 */
- (void)addSubProtocol;

@end
