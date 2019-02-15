//
//  BSiAPOverUARTAnalyzer.h
//  BusSniffer
//
//  Created by Joe Basu on 5/10/09.
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <Foundation/Foundation.h>
#import <BusSniffer/BSiAPTypes.h>
#import <BusSniffer/BSiAPPacket.h>

@class BSiAPPacketizer;
@class BSiAPTransactionIDAnalyzer;

@interface BSiAPOverUARTAnalyzer : NSObject {
	UInt64 _lastAvailableiPodTimestamp;
	UInt64 _lastAvailableAccessoryTimestamp;
	
	BSiAPPacketizer *_iPodiAPPacketizer;
	BSiAPPacketizer *_accessoryiAPPacketizer;

	BSiAPTransactionIDAnalyzer *_iAPTransactionIDAnalyzer;
}

- (void)reset;

- (id <BSiAPPacket>)processByte:(UInt8)byte atTimestamp:(UInt64)timestamp fromSource:(BSiAPSource)source;
- (id <BSiAPPacket>)processByte:(UInt8)byte fromSource:(BSiAPSource)source;
- (void)markAllLastAvailableTimestampsWithTimestamp:(UInt64)timestamp;

- (NSArray *)processData:(NSData *)data fromSource:(BSiAPSource)source;

@end
