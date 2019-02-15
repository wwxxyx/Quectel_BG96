//
//  ODSFPContext.h
//  BBSniff
//
//  Created by Bob Burrough on 6/9/09.
//  Copyright 2009 Apple Inc.. All rights reserved.
//

#import <Cocoa/Cocoa.h>

//used to map SessionID to the transaction ID of the OpendDataSessionProtocol that created it
@interface ODSFPContext : NSObject {
    UInt16 _sessionID;
    UInt16 _transactionID;
    BOOL _isOpen;
    BOOL _devACKed;
    UInt8 _devACKStatus;
    UInt16 _closeDataSessionTransactionID;
    BOOL _accessoryACKedClosed;
}

@property UInt16 sessionID;
@property UInt16 transactionID;
@property BOOL isOpen;
@property BOOL devACKed;
@property UInt8 devACKStatus;
@property (nonatomic) UInt16 closeDataSessionTransactionID;
@property (nonatomic) BOOL accessoryACKedClosed;

- (id)initWithTransactionID:(UInt16)transactionID andSessionID:(UInt16)sessionID;

@end
