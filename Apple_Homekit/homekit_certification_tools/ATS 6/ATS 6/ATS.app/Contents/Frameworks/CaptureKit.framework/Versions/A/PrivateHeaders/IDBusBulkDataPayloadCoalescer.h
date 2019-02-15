//
//  IDBusBulkDataPayloadCoalescer.h
//  ATSMacApp
//
//  Created by Vinod Madigeri on 6/22/17.
//  Copyright © 2017 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <IDBusKit/IDBusKit.h>


/**
 Interface to build a logical bulk data payload—keep track of and coalesce all the partial bulk data payloads over each IDBus commands for Apple device and accessory.
 */
@interface IDBusBulkDataPayloadCoalescer : NSObject

/**
 Method that tracks all the bulk data packets (with or without partial payloads) until there is a complete payload. Also, takes care of retransmissions of partial payloads.
 
 @param bulkDataPacket bulk data packets (with or without a partial payload)
 
 @return Is a valid data buffer when the input payload produces a complete payload after coalescing all the partial payloads. If the input bulk data packet contains a complete payload, the same is returned without buffering.
 */
- (NSData *)processBulkDataPacket:(IDBPacket *)packet;

@end
