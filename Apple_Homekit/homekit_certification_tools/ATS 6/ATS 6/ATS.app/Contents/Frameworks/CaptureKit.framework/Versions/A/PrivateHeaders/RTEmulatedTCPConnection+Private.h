//
//  RTEmulatedTCPConnection+Private.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 5/22/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/RTEmulatedTCPConnection.h>
#import <CaptureKit/RuleTests.h>
#import <CaptureKit/RTNetworkTypes.h>

typedef struct {
    RTEthernetAddress mac;
    RTIPv6Address ipv6;
    uint16_t port;

    uint32_t seq;
    uint32_t ack;
} RTTCPEndpointInfo;

@interface RTEmulatedTCPConnection () {
    RTTCPEndpointInfo _client;
    RTTCPEndpointInfo _server;
}

@property (nonatomic, retain) RuleTests *ruleTests;
@property (nonatomic, assign) RTNetworkEmulationMode emulationMode;

@property (nonatomic, assign) BOOL connectionActive;

@property (nonatomic, assign) BOOL clientSentFIN;
@property (nonatomic, assign) BOOL serverSentFIN;

// Convenience properties
@property (nonatomic, assign) BOOL appleDeviceIsServer;
@property (nonatomic, assign) BOOL accessoryIsServer;

/*!
 @method    initWithRuleTests:mode:
 @abstract  Initialize with a ruleTests
 */
- (id)initWithRuleTests:(RuleTests *)ruleTests mode:(RTNetworkEmulationMode)emulationMode;

- (NSArray *)emulateServerTCPPayload:(NSData *)payload flags:(RTTCPFlags)flags;
- (NSArray *)emulateClientTCPPayload:(NSData *)payload flags:(RTTCPFlags)flags;
- (NSArray *)emulateTCPPayload:(NSData *)payload source:(RTTCPEndpointInfo *)source dest:(RTTCPEndpointInfo *)dest flags:(RTTCPFlags)flags;

@end
