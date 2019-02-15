//
//  MBDesignator.h
//  MikeyBusKit
//
//  Created by Vinod Madigeri on 10/7/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol MBDesignator <NSObject>

/**
 Command ID for this designator packet. Command value depends on what type of designator packet this is (Configuration, HID, VendorSpecific).
 */
@property (nonatomic, readonly) NSNumber *command;

/**
 Endpoint number for this designator packet. Endpoint value depends on what type of designator packet this is (Configuration, HID, VendorSpecific).
 */
@property (nonatomic, readonly) NSNumber *endpoint;

/**
 Command description for the command ID.
 */
@property (nonatomic, readonly) NSString *commandDescription;

/**
 Endpoint description for the endpoint number.
 */
@property (nonatomic, readonly) NSString *endpointDescription;

/**
 Bulk data payload that was used to initialize an instance of Designator packet.
 */
@property (nonatomic, readonly) NSData *bulkDataPayload;

/**
 Bulk data payload raw bytes that was used to initialize an instance of Designator packet.
 */
@property (nonatomic, readonly) uint8_t *bulkDataPayloadBytes;

/**
 Designator data is the payload in BulkData payload minus BulkData header.
 */
@property (nonatomic, readonly) NSData *designatorData;

/**
 Create an instance of designator packet with input bulk data payload.
 
 @param payload Bulk data payload that'll be used to initialize an instance of Designator packet.
 @param error Error is valid if there are any issues with the input bulk data payload packet length.
 */
+ (instancetype)designatorPacketWithBulkDataPayload:(NSData *)payload andError:(NSError **)error;

@end
