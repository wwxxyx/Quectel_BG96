//
//  NCMAnalyzer.h
//  NCMKit
//
//  Created by Mark Hamlin on 5/17/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "NCMTypes.h"

@class NCMAnalyzer;
@class NCMTransferBlock;
@protocol NCMAnalyzerDelegate

/*!
 @method    ncmAnalyzer:processEthernetFrame:
 @abstract  Callback for analyzer when an ethernet frame has been parsed
 */
- (void)ncmAnalyzer:(NCMAnalyzer *)analyzer processEthernetFrame:(NSData *)ethernetFrame source:(NCMSource)source;

@end

@interface NCMAnalyzer : NSObject

/*!
 @property  delegate
 @abstract  The analyzer's delegate
 */
@property (nonatomic, weak) id<NCMAnalyzerDelegate> delegate;

/*!
 @method    analyzerWithDelegate:
 @abstract  Create a new analyzer
 */
+ (instancetype)analyzerWithDelegate:(id<NCMAnalyzerDelegate>)delegate;

/*!
 @method        processNCMTransfer:maxPacketSize:maxNTBSize:source:localizedErrorDescription:
 @abstract      Process an NCM transfer from the passed source
 @return        NCM transfer block data, if a full NCM frame has been parsed. Otherwise nil
 @discussion    Errors can be detected when this function returns nil AND localizedErrorDescription is set
 */
- (NSData *)processNCMTransfer:(NSData *)transfer maxPacketSize:(UInt16)maxPacketSize maxNTBSize:(UInt32)maxNTBSize source:(NCMSource)source localizedErrorDescription:(NSString **)localizedErrorDescription;

/*!
 @method        transferBlockFromNCMTransferBlockData:localizedErrorDescription:
 @abstract      Generates an NCMTransferBlock instance from the passed data
 @discussion    Allocating these objects is slow, so this should be performed on-demand when the user looks at it
 */
+ (NCMTransferBlock *)transferBlockFromNCMTransferBlockData:(NSData *)transfer localizedErrorDescription:(NSString **)localizedErrorDescription;

@end
