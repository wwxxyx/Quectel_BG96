//
//  APChaChaKeyPacket.h
//  TrafficKit
//
//  Created by Zachary Church on 2/18/15.
//  Copyright (c) 2015 Apple Inc. All rights reserved.
//

#import <TrafficKit/TrafficKit.h>

@interface APChaChaKeyPacket : APPacket

/*!
 @property  streamType
 @abstract A human-readable description of the stream with which these keys should be associated. CarPlay, for example, has "Ctrl" and "Events" streams.
 */
@property (strong) NSString *streamType;

/*!
 @property  readKey
 @abstract Data sent from the TCP server should be decrypted with this key
 */
@property (strong) NSData *readKey;

/*!
 @property  writeKey
 @abstract Data sent from the TCP client should be decrypted with this key
 */
@property (strong) NSData *writeKey;

@end
