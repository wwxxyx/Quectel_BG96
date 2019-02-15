//
//  BSUSBiAPPacket.h
//  BusSniffer
//
//  Created by Joe Basu on 6/13/09.
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <Foundation/Foundation.h>
#import <BusSniffer/BSiAPPacket.h>

@protocol BSUSBHIDiAPPacket <BSiAPPacket>

@property (nonatomic, readonly, copy) NSArray *hidReports;

@end

@interface BSUSBHIDiAPPacket : BSiAPPacket <BSUSBHIDiAPPacket> {
	NSArray *_hidReports;
}

+ (id)usbHIDiAPPacketWithTimestamp:(UInt64)timestamp
							source:(BSiAPSource)source
							  data:(NSData *)data
				  hasTransactionID:(BOOL)hasTransactionID
						hidReports:(NSArray *)hidReports;

- (id)initWithTimestamp:(UInt64)timestamp
				 source:(BSiAPSource)source
				   data:(NSData *)data
	   hasTransactionID:(BOOL)hasTransactionID
			 hidReports:(NSArray *)hidReports;
@end
