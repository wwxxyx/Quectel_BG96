//
//  AccessoryDataTransferContext.h
//  ATSMacApp
//
//  Created by Marin Balde on 11/30/10.
//  Copyright 2010 Apple Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class iAP1PacketEvent;

@interface AccessoryDataTransferContext : NSObject {
    NSMutableDictionary *_sessions;
}

- (void)reset;
- (void)processAccessoryDataTransferPacket:(iAP1PacketEvent *)packet;
- (void)processAccessoryDataTransferACKPacket:(iAP1PacketEvent *)packet;
- (void)processCloseDataSessionPacket:(iAP1PacketEvent *)packet;

- (NSArray *)failedAccessoryDataTransferPacketsForSessionID:(UInt16)sessionID;

@end
