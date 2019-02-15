//
//  PerformanceCaptureTransport.h
//  ATSMacApp
//
//  Created by Wesley McCloy on 7/23/18.
//  Copyright © 2018 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureTransport.h>
@class PerformanceEventConfig;

@interface PerformanceCaptureTransport : CaptureTransport

/**
 Convenience initializer
 
 @param document The CaptureDocument that this transport is running on
 
 @return The initialized PerformanceCaptureTransport
 */
- (instancetype)initWithDocument:(CaptureDocument *)document;

/**
 Start the stream of PerformanceEvents with the given settings
 */
- (void)startWithConfig:(PerformanceEventConfig *)config numberOfRows:(NSUInteger)rows;

@end
