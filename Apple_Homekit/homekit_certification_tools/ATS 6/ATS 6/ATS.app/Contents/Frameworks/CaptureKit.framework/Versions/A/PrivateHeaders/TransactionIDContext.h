//
//  TransactionIDContext.h
//  ATSMacApp
//
//  Created by Kip Nicol on 10/12/10.
//  Copyright 2010 Apple Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface TransactionIDContext : NSObject {
    UInt8 *_tidSeenBuffer;
}

- (BOOL)wasTransactionIDSeen:(UInt16)transactionID;
- (void)setSeen:(BOOL)seen forTransactionID:(UInt16)transactionID resetOnRollover:(BOOL)rollover;
- (void)resetTransactionIDBuffers;

@end
