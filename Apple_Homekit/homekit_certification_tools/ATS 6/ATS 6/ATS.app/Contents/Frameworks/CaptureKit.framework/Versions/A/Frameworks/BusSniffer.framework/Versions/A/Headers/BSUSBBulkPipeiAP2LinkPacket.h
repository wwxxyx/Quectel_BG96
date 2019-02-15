//
//  BSUSBBulkPipeiAP2LinkPacket.h
//  BusSniffer
//
//  Created by Joe Basu on 3/13/12.
//  Copyright © 2012 Apple Inc. All Rights Reserved.
//

#import <Foundation/Foundation.h>
#import <BusSniffer/BSiAP2LinkPacket.h>
#import <BusSniffer/BSUSBTransfer.h>


@protocol BSUSBBulkPipeiAP2LinkPacket <BSiAP2LinkPacket>

@property (nonatomic, readonly, retain) NSArray *transfers;

@end

@interface BSUSBBulkPipeiAP2LinkPacket : BSiAP2LinkPacket <BSUSBBulkPipeiAP2LinkPacket> {
	NSArray *_transfers;
}

+ (id)usbBulkiAP2LinkPacketWithTimestamp:(UInt64)timestamp
                                 source:(BSiAPSource)source
                                   data:(NSData *)data
                              transfers:(NSArray *)transfers;

- (id)initWithTimestamp:(UInt64)timestamp
				 source:(BSiAPSource)source
				   data:(NSData *)data
			  transfers:(NSArray *)transfers;

@end
