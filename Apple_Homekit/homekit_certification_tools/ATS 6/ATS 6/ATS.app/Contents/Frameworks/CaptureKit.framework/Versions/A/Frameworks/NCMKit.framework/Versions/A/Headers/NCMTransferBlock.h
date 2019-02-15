//
//  NCMTransferBlock.h
//  NCMKit
//
//  Created by Mark Hamlin on 5/17/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface NCMTransferBlock : NSObject

@property (nonatomic, assign) uint32_t dwSignature;
@property (nonatomic, assign) uint16_t wHeaderLength;
@property (nonatomic, assign) uint16_t wSequence;
@property (nonatomic, assign) uint32_t wBlockLength;
@property (nonatomic, assign) uint32_t fpIndex;
@property (nonatomic, assign) BOOL uses32BitFormat;

@property (nonatomic, retain) NSMutableArray *datagramPointerTables;

@end
