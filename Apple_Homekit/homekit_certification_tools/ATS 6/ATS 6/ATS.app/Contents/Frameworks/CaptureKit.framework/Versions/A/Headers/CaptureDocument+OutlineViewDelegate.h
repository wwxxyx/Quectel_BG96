//
//  CaptureDocument+OutlineViewDelegate.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 5/7/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureDocument.h>

@class TrafficViewController;

@interface CaptureDocument (OutlineViewDelegate)

/**
 Make the passed traffic view controller active and visible
 
 param trafficViewController Traffic view controller to switch to.
 */
- (void)switchToTrafficViewController:(TrafficViewController *)trafficViewController;

/**
 Handle outline view selection change
 */
- (void)refreshOutlineView;

@end
