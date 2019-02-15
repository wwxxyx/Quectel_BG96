//
//  BSExternalAccessoryPacket.h
//  BusSniffer
//
//  Created by Vinod Madigeri on 4/29/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface BSExternalAccessoryPacket : NSObject

/**
 The payload for this External Accessory packet.
 */
@property (nonatomic, strong, readonly) NSData *payload;

/**
 In case EA is over iAP2, there is a session identifier associated with it. identifier is nil otherwise.
 */
@property (nonatomic, strong, readonly) NSNumber *identifier;

/**
 In case EA is over iAP2, there is a session identifier associated with it. @"--" otherwise.
 */
@property (nonatomic, strong, readonly) NSString *identifierDisplayValue;

/**
 Packet error, if any.
 */
@property (nonatomic, strong, readonly) NSArray <NSError *> *errors;

/**
 Factory method to get packet created from payload.
 
 @param payload Payload for this External Accessory packet.
 @param identifier External Accessory session identifier. In case EA is over iAP2, there is a session identifier associated with it. identifier is nil otherwise.
 @param errors Errors already associated with this packet, determined by the lower protocol layers like iAP2 or IDBus
 */
+ (instancetype)packetWithData:(NSData *)payload withIdentifier:(NSNumber *)identifier andPacketErrors:(NSArray <NSError *> *)errors;

@end
