//
//  NetworkTrafficViewController.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 6/21/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/ReportAnalyzerBridgeTrafficViewController.h>

@interface NetworkTrafficViewController : ReportAnalyzerBridgeTrafficViewController

/*!
 @method    networkTrafficViewController
 @abstract  A new traffic view controller subscribed to network events
 */
+ (NetworkTrafficViewController *)networkTrafficViewController;

@end
