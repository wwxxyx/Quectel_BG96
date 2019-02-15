//
//  NCMDatagramPointerTable32.h
//  NCMKit
//
//  Created by Mark Hamlin on 5/17/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import "NCMDatagramPointerTable.h"

@interface NCMDatagramEntry32 : NSObject

@property (nonatomic, assign) uint32_t dwDatagramIndex;
@property (nonatomic, assign) uint32_t dwDatagramLength;

@property (nonatomic, strong) NSData *datagram;

@end

@interface NCMDatagramPointerTable32 : NCMDatagramPointerTable

@property (nonatomic, assign) uint32_t dwSignature;
@property (nonatomic, assign) uint16_t wLength;
@property (nonatomic, assign) uint16_t wReserved6;
@property (nonatomic, assign) uint32_t dwNextNdpIndex;
@property (nonatomic, assign) uint32_t dwReserved12;

@property (nonatomic, assign) BOOL hasCRC32;
@property (nonatomic, retain) NSMutableArray *datagramEntries;

@end

