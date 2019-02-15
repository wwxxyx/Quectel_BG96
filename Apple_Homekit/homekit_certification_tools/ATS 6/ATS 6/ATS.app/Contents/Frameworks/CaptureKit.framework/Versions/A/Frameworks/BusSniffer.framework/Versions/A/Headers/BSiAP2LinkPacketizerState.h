//
//  BSiAP2LinkPacketizerState.h
//  BusSniffer
//
//  Created by Joe Basu on 1/9/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

enum {
	BSiAP2LinkPacketizerStateWaitingForSOPHiByte,
    BSiAP2LinkPacketizerStateWaitingForSOPLoByte,
    BSiAP2LinkPacketizerStateWaitingForPacketLengthHiByte,
    BSiAP2LinkPacketizerStateWaitingForPacketLengthLoByte,
    BSiAP2LinkPacketizerStateWaitingForRestOfPacket,
    BSiAP2LinkPacketizerStateWaitingForByteSequence,
};

@interface BSiAP2LinkPacketizerState : NSObject {
	NSUInteger _recognizerState;
	NSMutableData *_packetMutableData;
	UInt16 _packetLength;
	UInt16 _packetBytesRemaining;
}

@property (nonatomic) NSUInteger recognizerState;
@property (nonatomic, retain) NSMutableData *packetMutableData;
@property (nonatomic) UInt16 packetLength;
@property (nonatomic) UInt16 packetBytesRemaining;

@end
