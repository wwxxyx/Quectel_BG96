//
//  L2CAPFrameBFrame.h
//  Header for Bluetooth L2CAP BFrame Support
//
//  Created by Edgard Lima on 08/13/18.
//  Copyright (c) 2018 Apple. All rights reserved.
//

// WARNING: (edgard.lima)
// This is BFrame implementation for BFrame in Basic L2CAP Mode (Bluetooth specification v1.1)
// For the other modes, it may change:
// 1- Flow Control Mode
// 2- Retransmission Mode
// 3- Enhanced Retransmissin Mode
// 4- Streaming Mode
// 5- LE Credit Based Flow Control Mode

#import <Foundation/Foundation.h>
#import "L2CAPFrame.h"

@interface L2CAPFrameBFrame : L2CAPFrame

/// do not call this method directly. call [L2CAPFrame buildL2CAPFrameFromFrameData] instead. It will instantiate the correct L2CAP Frame instance.
- (instancetype)initWithChannelAndFrameData:(NSNumber*)channelID frameData:(NSData *)frameData errors:(NSMutableArray<NSError *> *)errors;

@end
