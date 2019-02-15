//
//  TokenFillediAP1PacketEvent.h
//  ATSMacApp
//
//  Created by Joe Basu on 7/14/11.
//  Copyright © 2011 Apple Inc. All Rights Reserved.
//

#import <CaptureKit/iAP1PacketEvent.h>

@interface TokenFillediAP1PacketEvent : iAP1PacketEvent {
    NSArray *_validTokens;
}

+ (id)tokenFillediAP1PacketEventWithiAP1PacketEvent:(iAP1PacketEvent *)iAP1PacketEvent;
- (id)initWithiAP1PacketEvent:(iAP1PacketEvent *)iAP1PacketEvent;

@property (nonatomic, copy) NSArray *validTokens;

@end
