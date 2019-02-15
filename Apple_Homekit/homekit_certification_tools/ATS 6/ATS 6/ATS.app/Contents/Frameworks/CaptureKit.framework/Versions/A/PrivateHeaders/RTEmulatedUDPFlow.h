//
//  RTEmulatedUDPFlow.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 5/22/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <CaptureKit/UDPFlowBeganEvent.h>
#import <CaptureKit/UDPFlowDataEvent.h>

#import <CaptureKit/RTNetworkTypes.h>

@interface RTEmulatedUDPFlow : NSObject

/*!
 @method    emulServerData:
 @abstract  Emulates a UDP packet from the server
 */
- (UDPFlowDataEvent *)emulServerData:(NSData *)data;

/*!
 @method    emulClientData:
 @abstract  Emulates a UDP packet from the client
 */
- (UDPFlowDataEvent *)emulClientData:(NSData *)data;

#pragma mark -
#pragma mark Convenience

/*!
 @method        emulAppleDeviceData:
 @abstract      Emulates a UDP packet from the Apple device
 @discussion    This may only be used if the default Apple device and Accessory endpoints are in use
 */
- (UDPFlowDataEvent *)emulAppleDeviceData:(NSData *)data;

/*!
 @method        emulAccessoryData:
 @abstract      Emulates a UDP packet from the Accessory
 @discussion    This may only be used if the default Apple device and Accessory endpoints are in use
 */
- (UDPFlowDataEvent *)emulAccessoryData:(NSData *)data;

@end
