//
//  PerformanceEvent.h
//  ATSMacApp
//
//  Created by Wesley McCloy on 7/23/18.
//  Copyright © 2018 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureKit.h>
#import <CaptureKit/PerformanceEventConfig.h>

@interface PerformanceEvent : CaptureEvent

@property (nonatomic, strong) NSMutableArray *relatedEvents;
@property NSUInteger indexNum;

/**
 Intialize a Performance event with the given settings
 */
- (instancetype)initWithConfig:(PerformanceEventConfig *)config atTimestamp:(UInt64)timestamp;

@end
