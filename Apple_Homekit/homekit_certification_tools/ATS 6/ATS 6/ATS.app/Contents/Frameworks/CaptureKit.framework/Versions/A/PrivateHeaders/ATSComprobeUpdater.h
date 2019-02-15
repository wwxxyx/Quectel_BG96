//
//  ATSComprobeUpdater.h
//  ATSMacApp
//
//  Created by Zachary Church on 7/27/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CaptureKit/ATSConstants.h>

@interface ATSComprobeUpdater : NSObject

/** Whether ATSMacAppDelegate should refresh its comprobe driver check the next time comprobeDriverNeedsUpdate is called. */
@property (nonatomic, assign) BOOL comprobeDriverCheckNeedsUpdate;

+ (instancetype)sharedInstance;

/** Returns the state of the comprobe driver update process. */
- (ComProbeDriverUpdateState)comprobeDriverUpdateState;

- (void)checkComProbeDriver;

@end
