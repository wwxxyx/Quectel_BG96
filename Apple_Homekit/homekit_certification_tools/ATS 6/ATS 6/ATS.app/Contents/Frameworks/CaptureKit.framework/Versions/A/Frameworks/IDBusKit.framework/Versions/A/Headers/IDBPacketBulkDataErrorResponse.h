//
//  IDBPacketBulkDataErrorResponse.h
//  IDBusKit
//
//  Created by Zachary Church on 7/13/15.
//  Copyright (c) 2015 Apple Inc. All rights reserved.
//

#import <IDBusKit/IDBusKit.h>

@interface IDBPacketBulkDataErrorResponse : IDBPacket

@property (readonly) NSNumber *errorCode;
@property (readonly) NSNumber *endpoint;
@property (readonly) NSNumber *sequenceNumber;
@property (readonly) NSData *bulkDataPayload;

@end
