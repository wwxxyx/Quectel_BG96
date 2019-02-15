//
//  IKImportSource.h
//  ATSImportKit
//
//  Created by Mark Hamlin on 5/10/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "IKSourcePacketizer.h"
#import "IKConstants.h"

@class IKImportPacket;

/*!
 @extern        IKImportSourceUnexpectedlyLostConnectionToATSNotificationName
 @discussion    Notification posted if the connection to ATS is unexpectedly lost
 */
extern NSString *IKImportSourceUnexpectedlyLostConnectionToATSNotificationName;

@interface IKImportSource : NSObject<IKSourcePacketizerDelegate>

/*!
 @method    initWithATSPath:transportMask:protocolMask:
 @abstract  Initialize with a path to "ATS Internal.app", and the transport/protocol mask
 */
- (id)initWithATSPath:(NSString *)atsPath transportMask:(IKImportTransport)transportMask protocolMask:(IKImportProtocol)protocolMask;

/*!
 @method    capturePacket:timestampNS:
 @abstract  Capture a packet to trace
 */
- (void)capturePacket:(IKImportPacket *)packet timestampNS:(UInt64)timestampNS;

/*!
 @method        saveTraceToPath:
 @abstract      Save the .ats trace to the given path. For example, @"/Users/myuser/Desktop/TheTrace.ats".
 @discussion    This method will expand tildes in the file path for you
 */
- (void)saveTraceToPath:(NSString *)path;

/*!
 @method    insertMessage:
 @abstract  Inserts a message into the trace
 */
- (void)insertMessage:(NSString *)message;

/*!
 @method    invalidate
 @abstract  Ensures all pending messages are sent and invalidates packetizer
 */
- (void)invalidate;

@end
