//
//  NCMKit.h
//  NCMKit
//
//  Created by Mark Hamlin on 5/17/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <NCMKit/NCMAnalyzer.h>
#import <NCMKit/NCMTypes.h>
#import <NCMKit/NCMTransferBlock.h>
#import <NCMKit/NCMDatagramPointerTable.h>
#import <NCMKit/NCMDatagramPointerTable16.h>
#import <NCMKit/NCMDatagramPointerTable32.h>

#define NCMLocalizedStringForKey(key, comment) NSLocalizedStringFromTableInBundle(key, nil, [NSBundle bundleForClass:[NCMAnalyzer class]], comment)
