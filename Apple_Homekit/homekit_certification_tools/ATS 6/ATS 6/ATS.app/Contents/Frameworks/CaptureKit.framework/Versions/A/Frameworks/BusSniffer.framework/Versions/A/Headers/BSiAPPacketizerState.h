//
//  BSiAPPacketizerState.h
//  BusSniffer
//
//  Created by Joe Basu on 11/17/09.
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <Foundation/Foundation.h>


enum {
	BSiAPPacketizerStateWaitingForSyncByte,
	BSiAPPacketizerStateWaitingForSOP,
	BSiAPPacketizerStateWaitingForLength,
	BSiAPPacketizerStateWaitingForLengthHi,
	BSiAPPacketizerStateWaitingForLengthLo,
	BSiAPPacketizerStateWaitingForPayload,
	BSiAPPacketizerStateWaitingForChecksum,
};

@interface BSiAPPacketizerState : NSObject {
	NSUInteger _recognizerState;
	NSMutableData *_currentPacketMutableData;
	UInt16 _currentPacketPayloadLength;
	UInt16 _currentPacketBytesRemaining;
}

@property (nonatomic) NSUInteger recognizerState;
@property (nonatomic, retain) NSMutableData *currentPacketMutableData;
@property (nonatomic) UInt16 currentPacketPayloadLength;
@property (nonatomic) UInt16 currentPacketBytesRemaining;

@end
