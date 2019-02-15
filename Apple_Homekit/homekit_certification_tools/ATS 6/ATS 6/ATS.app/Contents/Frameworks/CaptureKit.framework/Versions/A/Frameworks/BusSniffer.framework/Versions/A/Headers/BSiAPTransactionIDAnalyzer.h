//
//  BSiAPTransactionIDAnalyzer.h
//  BusSniffer
//
//  Created by Joe Basu on 8/29/09.
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <Foundation/Foundation.h>
#import <BusSniffer/BSiAPPacket.h>

@interface BSiAPTransactionIDAnalyzer : NSObject {
	BOOL _transactionIDsPresent;
}

@property (nonatomic, readonly) BOOL transactionIDsPresent;

- (void)reset;
- (BOOL)processiAPPacket:(id <BSiAPPacket>)iAPPacket transactionIDStateChanged:(BOOL *)stateChanged;

@end
