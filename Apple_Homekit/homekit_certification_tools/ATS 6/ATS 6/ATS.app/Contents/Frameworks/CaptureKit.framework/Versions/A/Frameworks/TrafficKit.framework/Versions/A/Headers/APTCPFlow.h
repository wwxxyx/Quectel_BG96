//
//  APTCPFlow.h
//  APTS
//
//  Created by Mark Hamlin on 6/26/12.
//  Copyright (c) 2012 Apple. All rights reserved.
//

#import <Foundation/Foundation.h>

@class APTCPFlowTable;
@class APTCPPacket;
@class APChaChaKeyPacket;
@class APTCPFlow;

/**
 Delegate protocol to notifiy decrypted TCP payloads and missing TCP data lengths.
 */
@protocol APTCPDataStreamDelegate <NSObject>

@required

/**
 Delegate implementing this method will receive decrypted TCP data.
 
 @param flow TCP Flow instance
 @param packet TCP packet seen over the TCP flow
 @param decryptedPayload TCP data (decrypted) observed over the TCP flow
 @param sourceEndpoint Source endpoint string
 */
- (void)tcpFlow:(APTCPFlow *)flow didRecieveTCPPacket:(APTCPPacket *)packet withDecryptedPayload:(NSData *)decryptedPayload sourceEndpoint:(NSString *)sourceEndpoint;

/**
 Delegate implementing this method will receive information about missing data.
 
 @param flow TCP Flow instance
 @param missingDataLength Missing TCP data length
 @param sequenceNumber Sequence number of the TCP packet that's incomplete
 @param sourceEndpoint Source endpoint string
 */
- (void)tcpFlow:(APTCPFlow *)flow foundMissingDataOfLength:(NSNumber *)missingDataLength forPacketWithSequenceNumber:(NSNumber *)sequenceNumber sourceEndpoint:(NSString *)sourceEndpoint;

@end

/**
 Enum representing the two end points of the TCP flow.
 */
typedef NS_ENUM(NSUInteger, APTCPFlowEndpointIndex) {
    /** Endpoint 0. */
    APTCPFlowEndpointIndexEndpoint0 = 0,
    /** Endpoint 1. */
    APTCPFlowEndpointIndexEndpoint1 = 1,
    /** Endpoint NotFound. */
    APTCPFlowEndpointIndexNotFound = NSNotFound,
};

/*!
 @class     APTCPFlow
 @abstract  Handles TCP stream reconstruction
 */
@interface APTCPFlow : NSObject

/*!
 Unique identifier string for this TCP flow
 */
@property (nonatomic, readonly) NSString *identifier;

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
 @property  streamName
 @abstract  A human-readable name for the stream (ex. "Ctrl", "Events")
 */
@property (nonatomic, strong) NSString *streamName;

/*!
 Boolean property to know whether or not the flow is still alive.
 */
@property (nonatomic, readonly) BOOL isAlive;

/**
 Stream delegate to receive decrypted TCP payloads observed over this flow
 */
@property (nonatomic, weak) id<APTCPDataStreamDelegate> streamDelegate;

#pragma mark -
#pragma mark Parse Submission (APTCPFlowTable only)

/*!
 @method        endpointWithAddress:port:
 @abstract      Returns a standardized representation of an endpoint
 @discussion    This currently returns address:port, but it would be best to not rely on that
 */
+ (NSString *)endpointWithAddress:(NSString *)address port:(unsigned short)port;

/*!
 Initializes instance with first and second endpoints
 
 @param     identifier  Identifier string for this TCP flow
 @param     endpoint0 First endpoint
 @param     endpoint1 Second endpoint
 @param 	table TCP flow table
 
 @return    Initializes instance
 */
- (instancetype)initWithIdentifier:(NSString *)identifier endpoint0:(NSString *)endpoint0 endpoint1:(NSString *)endpoint1 table:(APTCPFlowTable *)table;

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
- (void)processPacket:(APTCPPacket *)packet sourceIsZero:(BOOL)sourceIsZero error:(NSString **)error;

/*!
 Method to receive and process ChaChaKey packet.
 
 @param chachaKeyPacket ChaChaKeyPacket with the key necessary for decrypting CarPlay traffic
 */
- (void)processChaChaKeyPacket:(APChaChaKeyPacket *)chachaKeyPacket;

/*!
 @method    isPacketMemberOfFlow:
 @abstract  Whether or not the passed packet appears to belong in this flow
 */
- (BOOL)isPacketMemberOfFlow:(APTCPPacket *)packet;

/*!
 @method   indexOfMemberOnPort:
 @abstract Returns the index (0, 1) of the flow member on the given port, or NSNotFound if neither member is using the port.
 */
- (NSUInteger)indexOfMemberOnPort:(unsigned short)port;

@end
