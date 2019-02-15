//
//  BSiAPVPortAnalyzer.h
//  BusSniffer
//
//  Created by Joe Basu on 1/15/10.
//  Copyright © 2010 Apple Inc. All Rights Reserved.
//

#import <Foundation/Foundation.h>
#import <BusSniffer/BSiAPTypes.h>
#import <BusSniffer/BSiAPPacket.h>
#import <BusSniffer/BSiAPVPort.h>

@class BSiAPPacketizer;
@class BSiAPTransactionIDAnalyzer;


@interface BSiAPVPortAnalyzer : NSObject {
	id <BSiAPVPort> _vPort;
	
	NSMutableArray *_currentiPodSourcePackets;
	NSMutableArray *_currentAccessorySourcePackets;

	BSiAPPacketizer *_iPodiAPPacketizer;
	BSiAPPacketizer *_accessoryiAPPacketizer;
	
	BSiAPTransactionIDAnalyzer *_iAPTransactionIDAnalyzer;
}

- (id)initWithVPort:(id <BSiAPVPort>)vPort;
- (void)reset;

- (NSArray *)processData:(NSData *)data fromiAPPacket:(id <BSiAPPacket>)iAPPacket;

@end
