/*
 *  FIDTokenCollection.h
 *  BBSniff
 *
 *  Created by Marin Balde on 3/6/10.
 *  Copyright 2010 Apple Inc. All rights reserved.
 *
 */

#import <Foundation/Foundation.h>

@class LKTToken;
@class FIDTokenContext;

@interface FIDTokenCollection : NSObject {
    NSMutableDictionary *_collection;
}

// Returns the matching FIDTokenContext for a given token
- (FIDTokenContext *)matchingFIDTokenContextForToken:(LKTToken *)token;

// call this from SetFIDTokenValues
- (void)addToken:(LKTToken *)token;
// call this from AckFIDTokenValues
- (void)updateToken:(LKTToken *)token setACKStatus:(UInt8)ackStatus;

// count the number of tokens that have been sent after receiving a NACK No Retry
- (NSUInteger)countTokensSentAfterNoRetry;
// same as above but reset the sentAfterNoRetry value of each FIDTokenContext in the collection
- (NSUInteger)countTokensSentAfterNoRetryAndResetCount:(BOOL)reset;

// reset the sentAfterNoRetry value of each FIDTokenContext in the collection
- (void)resetAllSentAfterNoRetryValues;
// remove all items from the collection
- (void)removeAllItems;

@end
