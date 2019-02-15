//
//  NCMDatagramPointerTable.h
//  NCMKit
//
//  Created by Mark Hamlin on 5/17/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import "NCMDatagramPointerTable.h"

@interface NCMDatagramEntry16 : NSObject

@property (nonatomic, assign) uint16_t wDatagramIndex;
@property (nonatomic, assign) uint16_t wDatagramLength;

@property (nonatomic, strong) NSData *datagram;

@end

@interface NCMDatagramPointerTable16 : NCMDatagramPointerTable

@property (nonatomic, assign) uint32_t dwSignature;
@property (nonatomic, assign) uint16_t wLength;
@property (nonatomic, assign) uint16_t wNextFpIndex;

@property (nonatomic, assign) BOOL hasCRC32;
@property (nonatomic, retain) NSMutableArray *datagramEntries;

@end
