//
//  MBConfigurationDesignatorPacket.h
//  MikeyBusKit
//
//  Created by Vinod Madigeri on 10/5/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <MikeyBusKit/MBDesignator.h>

// The payload is expected to be in the below structure,
//  typedef struct
//  {
//      UInt8	length;     // length value = 3..152
//      UInt8   command;
//      UInt8  *data;       // 0..150
//  };
// Note: This packet is transported in the MikeyBus Bulk packet payload.

@interface MBConfigurationDesignatorPacket: NSObject <MBDesignator>

@property (nonatomic, readonly) NSData *bulkDataPayload;
@property (nonatomic, readonly) NSData *designatorData;
@property (nonatomic, readonly) NSNumber *command;
@property (nonatomic, readonly) uint8_t *bulkDataPayloadBytes;

/**
 Parsed Key Value pairs for this configuration designator packet.
 */
@property (nonatomic, readonly) NSDictionary <NSString *, NSString *> *parsedPayloadFields;

/**
 Method to initialize configuration designator packet.
 
 @param payload Bulk Data Payload.
 
 @param error Error is valid if there was one parsing the bulk data payload.
 */
- (instancetype)initWithBulkDataPayload:(NSData *)payload andError:(NSError **)error;

@end
