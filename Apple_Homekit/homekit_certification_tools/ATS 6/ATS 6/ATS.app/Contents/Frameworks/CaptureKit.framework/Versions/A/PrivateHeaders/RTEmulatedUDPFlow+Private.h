//
//  RTEmulatedUDPFlow+Private.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 5/22/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/RTEmulatedUDPFlow.h>
#import <CaptureKit/RuleTests.h>
#import <CaptureKit/RTNetworkTypes.h>

typedef struct {
    RTEthernetAddress mac;
    RTIPv6Address ipv6;
    uint16_t port;
} RTUDPEndpointInfo;

@interface RTEmulatedUDPFlow () {
    RTUDPEndpointInfo _client;
    RTUDPEndpointInfo _server;
}

@property (nonatomic, retain) RuleTests *ruleTests;
@property (nonatomic, assign) RTNetworkEmulationMode emulationMode;

@property (nonatomic, assign) BOOL hasSentAPayload;

// Convenience properties
@property (nonatomic, assign) BOOL appleDeviceIsServer;
@property (nonatomic, assign) BOOL accessoryIsServer;

/*!
 @method    initWithRuleTests:mode:client:server:
 @abstract  Initialize with a ruleTests
 */
- (id)initWithRuleTests:(RuleTests *)ruleTests mode:(RTNetworkEmulationMode)emulationMode client:(RTUDPEndpointInfo *)client server:(RTUDPEndpointInfo *)server;

- (NSArray *)emulateUDPPayload:(NSData *)payload source:(RTUDPEndpointInfo *)source dest:(RTUDPEndpointInfo *)dest;

@end
