//
//  iAP2SessionIDStore.h
//  ATSMacApp
//
//  Created by Joe Basu on 8/6/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface iAP2SessionIDStore : NSObject

@property (nonatomic, readonly) UInt8 controlSessionID;
@property (nonatomic, readonly) BOOL controlSessionIDValid;
@property (nonatomic, readonly) UInt8 controlSesisonVersion;

@property (nonatomic, readonly) UInt8 fileTransferSessionID;
@property (nonatomic, readonly) BOOL fileTransferSessionIDValid;

@property (nonatomic, readonly) UInt8 externalAccessoryProtocolSessionID;
@property (nonatomic, readonly) BOOL externalAccessoryProtocolSessionIDValid;

@end
