//
//  BSVPortiAPPacket.h
//  BusSniffer
//
//  Created by Joe Basu on 1/15/10.
//  Copyright © 2010 Apple Inc. All Rights Reserved.
//

#import <BusSniffer/BSiAPPacket.h>


@protocol BSVPortiAPPacket <BSiAPPacket>

@property (nonatomic, readonly, retain) NSArray *sourceiAPPackets;

@end


@interface BSVPortiAPPacket : BSiAPPacket <BSVPortiAPPacket> {
	NSArray *_sourceiAPPackets;
}

+ (id)vPortiAPPacketWithTimestamp:(UInt64)timestamp
						   source:(BSiAPSource)source
						transport:(BSiAPTransport)transport
							 data:(NSData *)data
				 hasTransactionID:(BOOL)hasTransactionID
				 sourceiAPPackets:(NSArray *)sourceiAPPackets;

- (id)initWithTimestamp:(UInt64)timestamp
				 source:(BSiAPSource)source
			  transport:(BSiAPTransport)transport
				   data:(NSData *)data
	   hasTransactionID:(BOOL)hasTransactionID
	   sourceiAPPackets:(NSArray *)sourceiAPPackets;

@end
