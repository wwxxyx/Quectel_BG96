//
//  IKSourcePacketizer.h
//  ATSImportKit
//
//  Created by Mark Hamlin on 5/13/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@class IKSourcePacketizer;
@protocol IKSourcePacketizerDelegate

/*!
 @method    packetizer:receivedMessage:frameData:
 @abstract  Delegate method called back from a background thread to notify a message was read, including the raw frame data
 */
- (void)packetizer:(IKSourcePacketizer *)packetizer receivedMessage:(NSDictionary *)message frameData:(NSData *)frameData;

/*!
 @method    packetizerLostConnection:
 @abstract  Notification that the packetizer has lost one of its connections, from a background thread
 */
- (void)packetizerLostConnection:(IKSourcePacketizer *)packetizer;

@end

@interface IKSourcePacketizer : NSObject

/*!
 @method    initWithReceiveHandle:sendHandle:delegate:
 @abstract  Initialize with 2 pipes for bidirectional communication
 */
- (id)initWithReceiveHandle:(NSFileHandle *)receiveHandle sendHandle:(NSFileHandle *)sendHandle delegate:(id<IKSourcePacketizerDelegate>)delegate;

/*!
 @method    sendMessage:
 @abstract  Send a message to the other endpoint
 */
- (void)sendMessage:(NSDictionary *)message;

/*!
 @method    invalidate
 @abstract  Shut down threads and pipes
 */
- (void)invalidate;

@end
