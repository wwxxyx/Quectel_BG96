//
//  IDBPacketBulkDataPayloadTransferring.h
//  IDBusKit
//
//  Created by Vinod Madigeri on 6/15/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

/**
 Protocol grouping the attributes of a bulk data packet that is carrying a payload; of other protocols like HID, PD, EA, etc.
 */
@protocol IDBPacketBulkDataPayloadTransferring <NSObject>

/**
 IDBus packet sequence number
 */
@property (nonatomic, strong, readonly) NSNumber *sequence;

/**
 IDBus packet length
 */
@property (nonatomic, strong, readonly) NSNumber *length;

/**
 Protocol command of the protocols(HID, EA, PD, etc…) that this packet is carrying the payload of.
 */
@property (nonatomic, strong, readonly) NSNumber *protocolCommand;

/**
 Endpoint number
 */
@property (nonatomic, strong, readonly) NSNumber *endpoint;

/**
 Function name of the endpoint. Set by IDBusCaptureProtocol using its endpointToFunction map.
 */
@property (nonatomic, strong, readwrite) NSNumber *function;

/**
 Whether or not this IDBus bulk data payload is a partial payload.
 */
@property (nonatomic, readonly) BOOL isPartialPayload;

/**
 Bulk data payload carried over IDBus
 */
@property (nonatomic, strong, readonly) NSData *bulkDataPayload;

@end
