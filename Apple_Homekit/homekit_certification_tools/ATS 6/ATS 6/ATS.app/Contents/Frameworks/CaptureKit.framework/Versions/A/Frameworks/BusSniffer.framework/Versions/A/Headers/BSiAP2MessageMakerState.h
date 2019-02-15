//
//  BSiAP2MessageMakerState.h
//  BusSniffer
//
//  Created by Joe Basu on 4/12/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

enum {
	BSiAP2MessageMakerStateWaitingForSOMHiByte,
    BSiAP2MessageMakerStateWaitingForSOMLoByte,
    BSiAP2MessageMakerStateWaitingForMessageLengthHiByte,
    BSiAP2MessageMakerStateWaitingForMessageLengthLoByte,
    BSiAP2MessageMakerStateWaitingForRestOfPacket,
};

@interface BSiAP2MessageMakerState : NSObject {
    UInt64 _timestamp;
	NSUInteger _recognizerState;
	NSMutableData *_packetMutableData;
	UInt16 _packetLength;
	UInt16 _packetBytesRemaining;
}

@property (nonatomic) UInt64 timestamp;
@property (nonatomic) NSUInteger recognizerState;
@property (nonatomic, retain) NSMutableData *packetMutableData;
@property (nonatomic) UInt16 packetLength;
@property (nonatomic) UInt16 packetBytesRemaining;

@end
