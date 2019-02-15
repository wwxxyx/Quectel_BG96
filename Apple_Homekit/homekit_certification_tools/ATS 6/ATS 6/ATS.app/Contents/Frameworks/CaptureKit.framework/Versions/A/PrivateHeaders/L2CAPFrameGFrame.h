//
//  L2CAPFrameGFrame.h
//  Header for Bluetooth L2CAP GFrame Support
//
//  Created by Edgard Lima on 08/13/18.
//  Copyright (c) 2018 Apple. All rights reserved.
//

// WARNING: (edgard.lima)
// This is GFrame implementation for GFrame in Basic L2CAP Mode (Bluetooth specification v1.1)
// For the other modes, it may change:
// 1- Flow Control Mode
// 2- Retransmission Mode
// 3- Enhanced Retransmissin Mode
// 4- Streaming Mode
// 5- LE Credit Based Flow Control Mode

#import <Foundation/Foundation.h>
#import "L2CAPFrame.h"
#import <CaptureKit/L2CAPTypes.h>

@interface L2CAPFrameGFrame : L2CAPFrame

/// do not call this method directly. call [L2CAPFrame buildL2CAPFrameFromFrameData] instead. It will instantiate the correct L2CAPFrame instance.
- (instancetype)initWithChannelAndFrameData:(NSNumber*)channelID frameData:(NSData *)frameData errors:(NSMutableArray<NSError *> *)errors;

/// Protocol/Service Multiplexer. See L2CAPPsm.
@property (readonly) NSNumber *psm;

@end
