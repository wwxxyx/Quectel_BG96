//
//  CATestEvent.h
//  ATSMacApp
//
//  Created by Rafael Lucena on 8/10/15.
//  Copyright (c) 2015 Apple Inc. All rights reserved.
//

#import <CaptureKit/ATSAppEvent.h>

@interface CATestEvent : ATSAppEvent

/** An array of enum values for the test types that the event is associated with. */
@property (readonly, nonatomic, copy) NSArray<NSNumber *> *testTypes;

/**
 Initializes a new test event and associates it with the given list of test type enum values.
 
 @param testTypes An array of `NSNumber` instances encapsulating the enum values for the test types that the event is associated with.
 @return The new test event.
 */
- (instancetype)initWithTestTypes:(NSArray<NSNumber *> *)testTypes;

@end
