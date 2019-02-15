//
//  BSiAP2MessageMaker.h
//  BusSniffer
//
//  Created by Joe Basu on 4/12/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <BusSniffer/BSiAP2Message.h>

@class BSiAP2MessageMakerState;

@interface BSiAP2MessageMaker : NSObject {
    BSiAPTransport _transport;
    BSiAP2MessageMakerState *_state;
}

- (id)initWithTransport:(BSiAPTransport)transport;
- (NSData *)processByte:(UInt8)byte;
- (void)reset;
- (BOOL)partialPacketPresent;

@end
