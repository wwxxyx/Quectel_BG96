//
//  iAP1CaptureProtocol.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 4/30/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/ATSCaptureProtocol.h>

#import <CaptureKit/iAPPacketTrafficViewController.h>

@interface iAP1CaptureProtocol : ATSCaptureProtocol

@property (nonatomic, readonly, retain) iAPPacketTrafficViewController *packetTrafficViewController;

@property (nonatomic, readonly, strong) NSString *captureProtocolName;

/*!
 @method    processiAPPacket:
 @abstract  Process a BS iAPPacket
 */
- (void)processiAPPacket:(id<BSiAPPacket>)iAPPacket;

@end
