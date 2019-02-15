//
//  CarPlayControlTrafficViewController.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 5/16/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/ReportAnalyzerBridgeTrafficViewController.h>

@interface CarPlayControlTrafficViewController : ReportAnalyzerBridgeTrafficViewController

/*!
 @method    rtspTrafficViewController
 @abstract  A new traffic view controller
 */
+ (CarPlayControlTrafficViewController *)rtspTrafficViewController;

@end
