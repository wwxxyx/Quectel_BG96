//
//  BSiAP2LinkOverUSBBulkPipeAnalyzer.h
//  BusSniffer
//
//  Created by Joe Basu on 3/12/12.
//  Copyright © 2012 Apple Inc. All Rights Reserved.
//

#import <Foundation/Foundation.h>
#import <BusSniffer/BSUSBTransfer.h>
#import <BusSniffer/BSUSBTransaction.h>
#import <BusSniffer/BSUSBBulkPipeiAP2LinkPacket.h>

@class BSUSBAnalyzer;
@class BSiAP2LinkOverUSBBulkPipeAnalyzerTransferState;
@class BSiAP2LinkPacketizer;
@class BSiAP2MessageMaker;

@interface BSiAP2LinkOverUSBBulkPipeAnalyzer : NSObject {
	BSUSBAnalyzer *_usbAnalyzer;

	BSiAP2LinkOverUSBBulkPipeAnalyzerTransferState *_bulkInTransferState;
	BSiAP2LinkOverUSBBulkPipeAnalyzerTransferState *_bulkOutTransferState;

	BSiAP2LinkPacketizer *_iPodiAP2LinkPacketizer;
	BSiAP2LinkPacketizer *_accessoryiAP2LinkPacketizer;
	
    UInt64 _currentiPodiAP2MessageTimestamp;
    UInt64 _currentAccessoryiAP2MessageTimestamp;
    
    BSiAP2MessageMaker *_iPodiAP2MessageMaker;
    BSiAP2MessageMaker *_accessoryiAP2MessageMaker;
}

+ (id)iAPOverUSBBulkPipeAnalyzerWithUSBAnalyzer:(BSUSBAnalyzer *)usbAnalyzer;

- (id)initWithUSBAnalyzer:(BSUSBAnalyzer *)usbAnalyzer;

- (id <BSUSBTransfer>)processBulkTransaction:(id <BSUSBTransaction>)transaction
								   direction:(BSUSBEndpointDirection)direction;

- (NSArray *)processBulkTransfer:(id <BSUSBTransfer>)transfer
										 direction:(BSUSBTransferDirection)direction;

- (id <BSUSBTransfer>)processInterruptInTransaction:(id <BSUSBTransaction>)transaction;

- (NSArray *)processiAP2LinkPacket:(id <BSiAP2LinkPacket>)linkPacket;

@end
