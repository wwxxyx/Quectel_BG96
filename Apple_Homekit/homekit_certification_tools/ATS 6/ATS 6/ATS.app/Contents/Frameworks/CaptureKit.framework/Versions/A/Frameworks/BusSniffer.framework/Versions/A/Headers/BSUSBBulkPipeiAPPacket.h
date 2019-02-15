//
//  BSUSBBulkPipeiAPPacket.h
//  BusSniffer
//
//  Created by Joe Basu on 6/24/09.
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <Foundation/Foundation.h>
#import <BusSniffer/BSiAPPacket.h>
#import <BusSniffer/BSUSBTransfer.h>


@protocol BSUSBBulkPipeiAPPacket <BSiAPPacket>

@property (nonatomic, readonly, retain) NSArray *transfers;

@end

@interface BSUSBBulkPipeiAPPacket : BSiAPPacket <BSUSBBulkPipeiAPPacket> {
	NSArray *_transfers;
}

+ (id)usbBulkiAPPacketWithTimestamp:(UInt64)timestamp
							 source:(BSiAPSource)source
							   data:(NSData *)data
				   hasTransactionID:(BOOL)hasTransactionID
						  transfers:(NSArray *)transfers;


- (id)initWithTimestamp:(UInt64)timestamp
				 source:(BSiAPSource)source
				   data:(NSData *)data
	   hasTransactionID:(BOOL)hasTransactionID
			  transfers:(NSArray *)transfers;

@end
