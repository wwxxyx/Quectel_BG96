//
//  BSiAP2LinkOverUARTAnalyzer.h
//  BusSniffer
//
//  Created by Joe Basu on 12/1/11.
//  Copyright (c) 2011 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <BusSniffer/BSiAPTypes.h>
#import <BusSniffer/BSiAP2LinkPacket.h>

@class BSiAP2LinkPacketizer;
@class BSiAP2MessageMaker;

@interface BSiAP2LinkOverUARTAnalyzer : NSObject {
	UInt64 _lastAvailableiPodTimestamp;
	UInt64 _lastAvailableAccessoryTimestamp;
	
	BSiAP2LinkPacketizer *_iPodiAP2LinkPacketizer;
	BSiAP2LinkPacketizer *_accessoryiAP2LinkPacketizer;

    UInt64 _currentiPodiAP2MessageTimestamp;
    UInt64 _currentAccessoryiAP2MessageTimestamp;
    
    BSiAP2MessageMaker *_iPodiAP2MessageMaker;
    BSiAP2MessageMaker *_accessoryiAP2MessageMaker;
}

- (void)reset;

- (id <BSiAP2LinkPacket>)processByte:(UInt8)byte atTimestamp:(UInt64)timestamp fromSource:(BSiAPSource)source;
- (id <BSiAP2LinkPacket>)processByte:(UInt8)byte fromSource:(BSiAPSource)source;
- (void)markAllLastAvailableTimestampsWithTimestamp:(UInt64)timestamp;

- (NSArray *)processData:(NSData *)data fromSource:(BSiAPSource)source;
- (NSArray *)processData:(NSData *)data atTimestamp:(UInt64)timestamp fromSource:(BSiAPSource)source;

- (NSArray *)processiAP2LinkPacket:(id <BSiAP2LinkPacket>)linkPacket;

@end
