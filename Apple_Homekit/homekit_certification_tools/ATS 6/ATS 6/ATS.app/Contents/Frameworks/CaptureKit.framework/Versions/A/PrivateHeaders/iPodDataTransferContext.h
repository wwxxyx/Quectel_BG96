//
//  iPodDataTransferContext.h
//  BBSniff
//
//  Created by Bob Burrough on 6/9/09.
//  Copyright 2009 Apple Inc.. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface iPodDataTransferContext : NSObject {

    UInt16 _sessionID;
    UInt16 _transactionID;
}

@property UInt16 sessionID;
@property UInt16 transactionID;

- (id)initWithTransactionID:(UInt16)transactionID andSessionID:(UInt16)sessionID;

@end
