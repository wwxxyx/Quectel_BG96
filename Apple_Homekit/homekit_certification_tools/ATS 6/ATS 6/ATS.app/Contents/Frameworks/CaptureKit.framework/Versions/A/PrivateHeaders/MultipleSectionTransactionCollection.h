//
//  MultipleSectionTransactionCollection.h
//  ATSMacApp
//
//  Created by Kip Nicol on 9/28/10.
//  Copyright 2010 Apple Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class MultipleSectionTransactionContext;
@class iAP1PacketEvent;

@interface MultipleSectionTransactionCollection : NSObject {
    NSMutableDictionary *_multipleSectionTransactionContexts; // Dictionary of contexts keyed by their transaction IDs
}

// Adds a new multiple section packet to the correct context and returns the
// corresponding mst context
- (MultipleSectionTransactionContext *)addPacket:(iAP1PacketEvent *)packet;

// Returns the context with the given transaction ID
- (MultipleSectionTransactionContext *)multipleSectionTransactionContextForTransactionID:(UInt16)transID;

// Removes all transactions which have finished
- (void)removeAllFinishedTransactions;

// Removes all transactions
- (void)removeAllTransactions;

@end

@interface MultipleSectionTransactionContext : NSObject {
    NSMutableArray *_sectionPackets; // Array of packets in a multisection transaction
}

@property (nonatomic, readonly, retain) NSArray *sectionPackets;

- (id)initWithFirstPacket:(iAP1PacketEvent *)packet;

// Adds a new section index to the context
- (void)addSectionPacket:(iAP1PacketEvent *)packet;

// The first section index is valid if the index is 0
- (BOOL)isFirstSectionIndexValid;

// The last section index is consistent if the first lastSection equals the last lastSection
- (BOOL)isLastSectionIndexConsistent;

// The last section index is valid if the second to last index + 1 equals the last index (must be in order)
- (BOOL)isLastSectionIndexValid;

// Returns whether the transaction has completed
- (BOOL)isFinished;

@end
