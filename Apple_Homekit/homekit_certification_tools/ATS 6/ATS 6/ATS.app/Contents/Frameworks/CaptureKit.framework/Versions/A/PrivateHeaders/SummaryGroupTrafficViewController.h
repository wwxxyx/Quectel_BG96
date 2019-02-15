//
//  SummaryGroupTrafficViewController.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 8/13/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <CaptureKit/TrafficViewController.h>

@class SummarySection;

@interface SummaryGroupTrafficViewController : TrafficViewController

/*!
 @method    initWithGroupName:
 @abstract  Initialize with a group name, such as "Hardware"
 */
- (id)initWithGroupName:(NSString *)name;

/*!
 @method    addSection:
 @abstract  Adds this section to the bottom of the section display list
 @param     section
            The section to add
 */
- (void)addSection:(SummarySection *)section;

/*!
 @method        reportEventWithName:
 @abstract      Reports an event triggered with the given name
 @discussion    All sections in this group will receive -handleEventWithName:
 */
- (void)reportEventWithName:(NSString *)name;

@end
