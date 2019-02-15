//
//  iAP2SessionIDStore+Protected.h
//  ATSMacApp
//
//  Created by Zachary Church on 8/2/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <CaptureKit/iAP2SessionIDStore.h>

@interface iAP2SessionIDStore()

@property (nonatomic) UInt8 controlSessionID;
@property (nonatomic) BOOL controlSessionIDValid;
@property (nonatomic) UInt8 controlSesisonVersion;

@property (nonatomic) UInt8 fileTransferSessionID;
@property (nonatomic) BOOL fileTransferSessionIDValid;

@property (nonatomic) UInt8 externalAccessoryProtocolSessionID;
@property (nonatomic) BOOL externalAccessoryProtocolSessionIDValid;

@end
