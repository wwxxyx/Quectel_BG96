//
//  APUDPFlow.h
//  TrafficKit
//
//  Created by Mark Hamlin on 7/11/12.
//  Copyright (c) 2012 Apple. All rights reserved.
//

#import <Foundation/Foundation.h>

@class APUDPPacket;
@class APUDPFlowTable;
@class APUDPFlow;

/**
 Delegate protocol to notifiy UDP payloads.
 */
@protocol APUDPDataStreamDelegate <NSObject>

@required

/**
 Delegate implementing this method will receive UDP payload.
 
 @param flow UDP Flow instance
 @param packet UDP packet seen over the UDP flow
 @param payload UDP data observed over the UDP flow
 @param sourceEndpoint Source endpoint string
 */
- (void)udpFlow:(APUDPFlow *)flow didRecieveUDPPacket:(APUDPPacket *)packet withPayload:(NSData *)payload sourceEndpoint:(NSString *)sourceEndpoint;

@end

/*!
 @class     APUDPFlow
 @abstract  Handles UDP stream reconstruction
 */
@interface APUDPFlow : NSObject

/*!
 Identifier string for this UDP flow
 */
@property (nonatomic, readonly) NSString *identifier;

/*!
 @property      packetClass
 @abstract      Class of packets in this flow
 @discussion    Only packets from this class are allowed into the flow -- other instances with this ip/port combo will be dropped. If an observer wishes to not filter by packet type, then it can nillify this upon getting the flow creation notification
 */
@property (nonatomic, strong) Class packetClass;

/*!
 @property  endpoint0Address
 @abstract  IP address of the first endpoint
 */
@property (nonatomic, strong) NSString *endpoint0Address;

/*!
 @property  endpoint0Port
 @abstract  Port of the first endpoint
 */
@property (nonatomic, assign) unsigned short endpoint0Port;

/*!
 @property  endpoint1Address
 @abstract  IP address of the second endpoint
 */
@property (nonatomic, strong) NSString *endpoint1Address;

/*!
 @property  endpoint1Port
 @abstract  Port of the second endpoint
 */
@property (nonatomic, assign) unsigned short endpoint1Port;

/*!
 @property  endpoint0
 @abstract  The 0th endpoint identifier
 */
@property (readonly, nonatomic, strong) NSString *endpoint0;

/*!
 @property  endpoint1
 @abstract  The 1st endpoint identifier
 */
@property (readonly, nonatomic, strong) NSString *endpoint1;

/*!
 Stream delegate to receive UDP payloads observed over this flow
 */
@property (nonatomic, weak) id<APUDPDataStreamDelegate> streamDelegate;

/*!
 Whether or not this UDP flow is carrying mDNS packets.
 */
@property (nonatomic, readonly) BOOL isCarryingmDNSPackets;

#pragma mark -
#pragma mark Parse Submission (APUDPFlowTable only)

/*!
 @method        endpointWithAddress:port:
 @abstract      Returns a standardized representation of an endpoint
 @discussion    This currently returns address:port, but it would be best to not rely on that
 */
+ (NSString *)endpointWithAddress:(NSString *)address port:(unsigned short)port;

/*!
 Initializes instance with first and second endpoints
 
 @param     identifier Identifier string for this UDP flow
 @param     endpoint0 First endpoint
 @param     endpoint1 Second endpoint
 @param     table UDP Flow table
 
 @return    Initialized instance
 */
- (instancetype)initWithFlowIdentifier:(NSString *)identifier endpoint0:(NSString *)endpoint0 other:(NSString *)endpoint1 table:(APUDPFlowTable *)table;

/*!
 @method    processPacket:
 @abstract  Handles state changes and data submission to the flow
 @param     packet
            The packet
 @param     sourceIsZero
            Whether or not the source of this packet is the 0th endpoint
 @param     error
            Error string assigned here upon error (mainly stream corruption)
 */
- (void)processPacket:(id)packet sourceIsZero:(BOOL)sourceIsZero error:(NSString **)error;

/*!
 @method    isAlive
 @abstract  Is the flow still alive?
 @return    Whether or not this flow should be kept in the flow table
 */
- (BOOL)isAlive;

/*!
 @method    isPacketMemberOfFlow:
 @abstract  Whether or not the passed packet appears to belong in this flow
 */
- (BOOL)isPacketMemberOfFlow:(APUDPPacket *)packet;

@end
