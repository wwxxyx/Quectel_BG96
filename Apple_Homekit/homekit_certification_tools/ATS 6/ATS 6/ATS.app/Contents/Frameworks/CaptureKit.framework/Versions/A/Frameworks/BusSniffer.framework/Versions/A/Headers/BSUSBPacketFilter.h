//
//  BSUSBPacketFilter.h
//  BusSniffer
//
//  Created by Joe Basu on 8/13/09.
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <Foundation/Foundation.h>
#import <BusSniffer/BSUSBPacket.h>

@interface BSUSBPacketFilter : NSObject {
    BOOL _allowedPacketAddresses[256];
	NSMutableArray *_allowedPacketAddressesEndpoints;
	BOOL _filterOpen;
}

@property (nonatomic, readonly) BOOL filterOpen;

- (void)reset;
- (BOOL)allowedPacket:(id <BSUSBPacket>)packet;
- (void)allowPacketsFromAddress:(UInt8)address endpoint:(UInt8)endpoint direction:(BSUSBEndpointDirection)direction;
- (BOOL)packetsAllowedFromAddress:(UInt8)address endpoint:(UInt8)endpoint direction:(BSUSBEndpointDirection)direction;
- (void)prohibitPacketsFromAddress:(UInt8)address endpoint:(UInt8)endpoint direction:(BSUSBEndpointDirection)direction;
- (BOOL)packetsAllowedFromAddress:(UInt8)address;

@end
