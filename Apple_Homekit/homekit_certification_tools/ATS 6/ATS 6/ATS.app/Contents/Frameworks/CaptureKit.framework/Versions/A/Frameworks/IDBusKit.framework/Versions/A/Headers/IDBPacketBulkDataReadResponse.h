//
//  IDBPacketBulkDataReadResponse.h
//  IDBusKit
//
//  Created by Zachary Church on 7/20/15.
//  Copyright (c) 2015 Apple Inc. All rights reserved.
//

#import <IDBusKit/IDBusKit.h>
#import "IDBPacketBulkDataPayloadTransferring.h"

@interface IDBPacketBulkDataReadResponse : IDBPacket <IDBPacketBulkDataPayloadTransferring>

@property (nonatomic, strong, readwrite) NSNumber *function;

@end
