//
//  RuleTests+Network.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 5/21/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/RuleTests.h>

#import <CaptureKit/RTNetworkTypes.h>
#import <CaptureKit/RTEmulatedTCPConnection.h>
#import <CaptureKit/RTEmulatedUDPFlow.h>

@interface RuleTests (Network)

/*!
 @method    encapsulatedEthernetPayload:type:source:dest:
 @abstract  Formats encapsulated payload using the ethertype from source to dest
 */
+ (NSData *)encapsulatedEthernetPayload:(NSData *)payload type:(RTEthernetType)etherType source:(RTEthernetAddress)source dest:(RTEthernetAddress)dest;

/*!
 @method    encapsulatedIPv6Payload:type:source:dest:
 @abstract  Formats encapsulated IPv6 frame using the passed payload type (eg. RTIPTypeTCP)
 */
+ (NSData *)encapsulatedIPv6Payload:(NSData *)payload type:(RTIPType)type source:(RTIPv6Address)source dest:(RTIPv6Address)dest;

/*!
 @method    encapsulatedUDPPayload:source:dest:
 @abstract  Formats a UDP packet using the payload data, source and dest ports
 */
+ (NSData *)encapsulatedUDPPayload:(NSData *)payload source:(uint16_t)source dest:(uint16_t)dest;

/*!
 @method    encapsulatedTCPPayload:source:dest:ack:seq:flags:
 @abstract  Formats a TCP packiet using the payload, source, dest port and flags
 */
+ (NSData *)encapsulatedTCPPayload:(NSData *)payload source:(uint16_t)source dest:(uint16_t)dest ack:(uint32_t)ack seq:(uint32_t)seq flags:(RTTCPFlags)flags;

#pragma mark -
#pragma mark TCP Connection Helpers

/*!
 @method    emulatedTCPConnectionOverNCM
 @abstract  Returns an emulated TCP connection instance using NCM
 */
- (RTEmulatedTCPConnection *)emulatedTCPConnectionOverNCM;

#pragma mark -
#pragma mark UDP Flow Helpers

/*!
 @method    emulatedUDPFlowOverNCMWithSourceMAC:sourceIPv6:sourcePort:destMac:destIPv6:destPort:
 @abstract  Returns an emulated UDP flow instance using NCM
 */
- (RTEmulatedUDPFlow *)emulatedUDPFlowOverNCMWithSourceMAC:(RTEthernetAddress)sourceMAC sourceIPv6:(RTIPv6Address)sourceIP sourcePort:(uint16_t)sourcePort
                                                   destMAC:(RTEthernetAddress)destMAC
                                                  destIPv6:(RTIPv6Address)destIP
                                                  destPort:(uint16_t)destPort;

/*!
 @method    emulatedUDPFlowOverNCMWithAppleDevicePort:accessoryPort:
 @abstract  Returns an emulated UDP flow between the default Apple device and Accessory endpoints
 */
- (RTEmulatedUDPFlow *)emulatedUDPFlowOverNCMWithAppleDevicePort:(uint16_t)applePort accessoryPort:(uint16_t)accessoryPort;

@end
