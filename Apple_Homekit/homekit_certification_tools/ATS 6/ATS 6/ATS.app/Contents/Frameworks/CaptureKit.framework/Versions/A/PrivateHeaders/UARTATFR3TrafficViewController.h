//
//  UARTATFR3TrafficViewController.h
//  ATSMacApp
//
//  Created by Marin Balde on 4/11/12.
//  Copyright © 2012 Apple Inc. All Rights Reserved.
//

#import <CaptureKit/TrafficViewController.h>

@interface UARTATFR3TrafficViewController : TrafficViewController

/**
 The channel for errors that we want to only show up in this TVC. E.g. "ATF R3 Frames are arriving
 out of order"
 */
@property (nonatomic, strong, readonly) id<EventChanneling> errorEventChannel;

@end
