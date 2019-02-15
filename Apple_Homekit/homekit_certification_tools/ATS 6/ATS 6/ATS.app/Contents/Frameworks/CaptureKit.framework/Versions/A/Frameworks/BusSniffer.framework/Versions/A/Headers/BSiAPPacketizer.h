//
//  BSiAPPacketizer.h
//  BusSniffer
//
//  Created by Joe Basu on 11/17/09.
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <Foundation/Foundation.h>
#import <BusSniffer/BSiAPTypes.h>

@class BSiAPPacketizerState;

@interface BSiAPPacketizer : NSObject {
	BSiAPTransport _transport;
	BSiAPPacketizerState *_state;
}

+ (id)iAPPacketizerWithTransport:(BSiAPTransport)transport;
- (id)initWithTransport:(BSiAPTransport)transport;
- (void)reset;
- (NSData *)processByte:(UInt8)byte;
- (BOOL)partialPacketPresent;
- (NSData *)partialPacketData;

@end
