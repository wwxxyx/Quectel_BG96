//
//  TrafficAttribute.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 7/14/11.
//  Copyright 2011 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureEvent.h>

@interface TrafficAttribute : NSObject {
    UInt64 _timestamp;
    BOOL _aboveTimestamp;
}
@property (nonatomic, assign) UInt64 timestamp;
// If the event should go above other events with the same timestamp
@property (nonatomic, assign) BOOL aboveTimestamp; // Default: NO

- (id)initWithTimestamp:(UInt64)aTimestamp;

- (id)initWithInfo:(NSDictionary *)info;
- (NSMutableDictionary *)exportedInfo;

// Allocates a TrafficAttribute from info; Will allocate the correct subclass
+ (TrafficAttribute *)AttributeWithInfo:(NSDictionary *)info;

@end
