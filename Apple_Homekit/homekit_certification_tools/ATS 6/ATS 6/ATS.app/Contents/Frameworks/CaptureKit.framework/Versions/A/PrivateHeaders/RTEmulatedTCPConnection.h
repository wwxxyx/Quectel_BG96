//
//  RTEmulatedTCPConnection.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 5/22/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <CaptureKit/TCPFlowBeganEvent.h>
#import <CaptureKit/TCPFlowDataEvent.h>
#import <CaptureKit/TCPFlowEndedEvent.h>

#import <CaptureKit/RTNetworkTypes.h>

@interface RTEmulatedTCPConnection : NSObject

/*!
 @method    emulOpenConnectionFromSourceMAC:sourceIPv6:sourcePort:destMAC:destIPv6:destPort:
 @abstract  Emulates a TCP connection opening with the source mac
 */
- (TCPFlowBeganEvent *)emulOpenConnectionFromSourceMAC:(RTEthernetAddress)sourceMAC sourceIPv6:(RTIPv6Address)sourceIP sourcePort:(uint16_t)sourcePort
                                               destMAC:(RTEthernetAddress)destMAC
                                              destIPv6:(RTIPv6Address)destIP
                                              destPort:(uint16_t)destPort;

/*!
 @method    emulServerDataReturningAll:
 @abstract  Emulates a TCP packet and ack for the data, from the server
 */
- (NSArray *)emulServerDataReturningAll:(NSData *)data;

/*!
 @method    emulServerData:
 @abstract  Emulates a TCP packet and ack for the data, from the server
 */
- (TCPFlowDataEvent *)emulServerData:(NSData *)data;

/*!
 @method    emulClientDataReturningAll:
 @abstract  Emulates a TCP packet and ack for the data, from the client, returns all events
 */
- (NSArray *)emulClientDataReturningAll:(NSData *)data;

/*!
 @method    emulClientData:
 @abstract  Emulates a TCP packet and ack for the data, from the client
 */
- (TCPFlowDataEvent *)emulClientData:(NSData *)data;

/*!
 @method    emulServerCloseConnection
 @abstract  Emulates the server closing their endpoint of the connection
 */
- (NSArray *)emulServerCloseConnection;

/*!
 @method    emulClientCloseConnection
 @abstract  Emulates the client closing their endpoint of the connection
 */
- (NSArray *)emulClientCloseConnection;

/*!
 @method    emulCloseConnection
 @abstract  Emulates closing the connection, initiated by the client
 */
- (TCPFlowEndedEvent *)emulCloseConnection;

#pragma mark -
#pragma mark Convenience

/*!
 @method    emulOpenConnectionFromAppleDevicePort:toAccessoryPort:
 @abstract  Convenience open connection emulation where the Apple device is the client, and Accessory is server
 */
- (TCPFlowBeganEvent *)emulOpenConnectionFromAppleDevicePort:(uint16_t)appleDevicePort toAccessoryPort:(uint16_t)accessoryPort;

/*!
 @method    emulOpenConnectionFromAccessoryPort:toAppleDevicePort:
 @abstract  Convenience open connection emulation where the Apple device is the client, and Accessory is server
 */
- (TCPFlowBeganEvent *)emulOpenConnectionFromAccessoryPort:(uint16_t)accessoryPort toAppleDevicePort:(uint16_t)appleDevicePort;

/*!
 @method    emulAppleDeviceData:
 @abstract  Emulates a TCP packet and ack for the data, from the Apple device
 */
- (TCPFlowDataEvent *)emulAppleDeviceData:(NSData *)data;

/*!
 @method    emulAccessoryData:
 @abstract  Emulates a TCP packet and ack for the data, from the accessory
 */
- (TCPFlowDataEvent *)emulAccessoryData:(NSData *)data;

/*!
 @method    emulAppleDeviceCloseConnection
 @abstract  Emulates the Apple device initiating the connection closure
 */
- (NSArray *)emulAppleDeviceCloseConnection;

/*!
 @method    emulAccessoryCloseConnection
 @abstract  Emulates the Accessory initiating the connection closure
 */
- (NSArray *)emulAccessoryCloseConnection;

@end
