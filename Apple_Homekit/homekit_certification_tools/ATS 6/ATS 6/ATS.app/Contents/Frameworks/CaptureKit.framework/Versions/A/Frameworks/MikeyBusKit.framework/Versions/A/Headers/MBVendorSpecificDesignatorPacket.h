//
//  MBVendorSpecificDesignatorPacket.h
//  MikeyBusKit
//
//  Created by Vinod Madigeri on 10/4/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <MikeyBusKit/MBDesignator.h>

// The payload is expected to be in the below structure,
//  typedef struct
//  {
//      UInt8	length;     // length value = 3..152
//      UInt8   command;
//      UInt8	endpoint;
//      UInt8  *data;       // 0..149
//  };
// MikeyBus Vendor Specific Designator packet definition
// Note: This packet is transported in the MikeyBus Bulk packet payload.

extern NSString * const MBVendorSpecificCommandKey;
extern NSString * const MBVendorSpecificEndpointKey;
extern NSString * const MBVendorSpecificLengthKey;
extern NSString * const MBVendorSpecificiAPKey;
extern NSString * const MBVendorSpecificI2CKey;

@interface MBVendorSpecificDesignatorPacket : NSObject <MBDesignator>
@property (nonatomic, readonly) NSNumber *command;
@property (nonatomic, readonly) NSNumber *endpoint;
@property (nonatomic, readonly) NSData *bulkDataPayload;
@property (nonatomic, readonly) NSData *designatorData;
@property (nonatomic, readonly) uint8_t *bulkDataPayloadBytes;

@end
