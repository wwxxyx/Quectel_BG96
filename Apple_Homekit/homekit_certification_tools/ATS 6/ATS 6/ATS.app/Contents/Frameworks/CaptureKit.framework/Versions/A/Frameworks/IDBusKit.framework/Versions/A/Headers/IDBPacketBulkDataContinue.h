//
//  IDBPacketBulkDataContinue.h
//  IDBusKit
//
//  Created by Zachary Church on 8/18/15.
//  Copyright (c) 2015 Apple Inc. All rights reserved.
//

#import <IDBusKit/IDBusKit.h>

@interface IDBPacketBulkDataContinue : IDBPacket

@property (readonly) NSNumber *sequence;

@end
