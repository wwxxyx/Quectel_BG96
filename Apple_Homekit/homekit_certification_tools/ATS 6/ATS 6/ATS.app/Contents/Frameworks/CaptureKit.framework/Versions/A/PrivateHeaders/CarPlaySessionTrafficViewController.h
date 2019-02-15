//
//  CarPlaySessionTrafficViewController.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 6/6/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/ReportAnalyzerBridgeTrafficViewController.h>

@interface CarPlaySessionTrafficViewController : ReportAnalyzerBridgeTrafficViewController

/*!
 @method    sessionTrafficViewController
 @abstract  A new traffic view controller subscribed to session events
 */
+ (CarPlaySessionTrafficViewController *)sessionTrafficViewController;

@end
