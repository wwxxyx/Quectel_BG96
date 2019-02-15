//
//  BSiAP2LinkPacketizer.h
//  BusSniffer
//
//  Created by Joe Basu on 1/9/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <BusSniffer/BSiAPTypes.h>

@class BSiAP2LinkPacketizerState;

@interface BSiAP2LinkPacketizer : NSObject {
	BSiAPTransport _transport;
	BSiAP2LinkPacketizerState *_state;
}

/**
 Returns whether or not there's a partial packet being coalesced.
 */
@property (readonly) BOOL isPartialPacketPresent;

+ (id)packetizerWithTransport:(BSiAPTransport)transport;
- (id)initWithTransport:(BSiAPTransport)transport;
- (void)reset;
- (NSData *)processByte:(UInt8)byte;
- (NSData *)partialPacketData;

@end
