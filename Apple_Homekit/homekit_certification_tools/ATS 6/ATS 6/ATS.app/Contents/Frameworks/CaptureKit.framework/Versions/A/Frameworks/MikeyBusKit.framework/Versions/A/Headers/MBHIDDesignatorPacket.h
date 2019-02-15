//
//  MBHIDDesignatorPacket.h
//  MikeyBusKit
//
//  Created by Vinod Madigeri on 10/7/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <MikeyBusKit/MBDesignator.h>

@interface MBHIDDesignatorPacket : NSObject <MBDesignator>

@property (nonatomic, readonly) NSNumber *command;
@property (nonatomic, readonly) NSData *bulkDataPayload;
@property (nonatomic, readonly) NSData *designatorData;
@property (nonatomic, readonly) uint8_t *bulkDataPayloadBytes;

@end
