//
//  BSiAPOverUSBBulkPipeAnalyzer.h
//  BusSniffer
//
//  Created by Joe Basu on 6/24/09.
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <Foundation/Foundation.h>
#import <BusSniffer/BSUSBTransfer.h>
#import <BusSniffer/BSUSBTransaction.h>
#import <BusSniffer/BSUSBBulkPipeiAPPacket.h>

@class BSUSBAnalyzer;
@class BSiAPOverUSBBulkPipeAnalyzerTransferState;
@class BSiAPPacketizer;
@class BSiAPTransactionIDAnalyzer;

@interface BSiAPOverUSBBulkPipeAnalyzer : NSObject {
	BSUSBAnalyzer *_usbAnalyzer;

	BSiAPOverUSBBulkPipeAnalyzerTransferState *_bulkInTransferState;
	BSiAPOverUSBBulkPipeAnalyzerTransferState *_bulkOutTransferState;

	BSiAPPacketizer *_iPodiAPPacketizer;
	BSiAPPacketizer *_accessoryiAPPacketizer;
	
	BSiAPTransactionIDAnalyzer *_iAPTransactionIDAnalyzer;

}

+ (id)iAPOverUSBBulkPipeAnalyzerWithUSBAnalyzer:(BSUSBAnalyzer *)usbAnalyzer;

- (id)initWithUSBAnalyzer:(BSUSBAnalyzer *)usbAnalyzer;

- (id <BSUSBTransfer>)processBulkTransaction:(id <BSUSBTransaction>)transaction
								   direction:(BSUSBEndpointDirection)direction;

- (NSArray *)processBulkTransfer:(id <BSUSBTransfer>)transfer
										 direction:(BSUSBTransferDirection)direction;

- (id <BSUSBTransfer>)processInterruptInTransaction:(id <BSUSBTransaction>)transaction;

@end
