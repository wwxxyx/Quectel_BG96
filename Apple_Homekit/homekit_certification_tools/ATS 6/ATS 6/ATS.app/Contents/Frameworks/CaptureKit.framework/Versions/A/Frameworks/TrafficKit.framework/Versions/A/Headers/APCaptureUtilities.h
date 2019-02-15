//
//  APCaptureUtilities.h
//  TrafficKit
//
//  Created by Mark Hamlin on 7/9/12.
//  Copyright (c) 2012 Apple. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <netinet/in.h>
#import <netinet6/in6.h>

@interface APCaptureUtilities : NSObject

/*!
 @method    stringFromSockaddr
 @abstract  Converts a sockaddr (in or in6) to a human readable string
 @param     addr
            The sockaddr
 @return    Address string, eg. xxx.xxx.xxx.xxx
 */
+ (NSString *)stringFromSockaddr:(const struct sockaddr *)addr;

/*!
 @method    sourceAddressFromPacket:
 @abstract  Searches the protocol hierarchy for the first IP packet (ipv4 or ipv6) and returns the source address
 */
+ (NSString *)sourceAddressFromPacket:(id)packet;

/*!
 @method    destAddressFromPacket:
 @abstract  Searches the protocol hierarchy for the first IP packet (ipv4 or ipv6) and returns the dest address
 */
+ (NSString *)destAddressFromPacket:(id)packet;

@end
