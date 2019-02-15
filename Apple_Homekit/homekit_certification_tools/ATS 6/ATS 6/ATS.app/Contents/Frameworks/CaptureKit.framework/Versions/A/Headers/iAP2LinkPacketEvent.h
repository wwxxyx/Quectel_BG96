//
//  iAP2LinkPacketEvent.h
//  ATSMacApp
//
//  Created by Joe Basu on 1/10/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <CaptureKit/iAP2Event.h>
#import <CaptureKit/CaptureKit.h>

@class BSiAP2LinkPacket;

@interface iAP2LinkPacketEvent : iAP2Event<ReportAnalyzerDisplayProtocol>

/*!
 Static initializer that creates the link packet event.
 @param packet represents the BSiAP2LinkPacket object.
 @return instancetype of iAP2LinkPacketEvent object.
 */
+ (instancetype)iAP2LinkPacketEventWithBSiAP2LinkPacket:(BSiAP2LinkPacket *)packet;

/*!
 Initializes the link packet event with its packet.
 @param packet represents the BSiAP2LinkPacket object.
 @return instancetype of iAP2LinkPacketEvent object.
 */
- (instancetype)initWithBSiAP2LinkPacket:(BSiAP2LinkPacket *)packet;

/*!
 Returns whether the packet is considered valid or not.
 @return YES if the packet is considered valid, NO otherwise. 
 */
- (BOOL)isValidPacket;

@property (nonatomic, retain) BSiAP2LinkPacket *packet;

@end
