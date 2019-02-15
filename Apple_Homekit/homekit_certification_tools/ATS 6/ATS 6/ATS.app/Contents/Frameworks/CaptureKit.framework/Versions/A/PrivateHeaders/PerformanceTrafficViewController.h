//
//  PerformanceTrafficViewController.h
//  ATSMacApp
//
//  Created by Wesley McCloy on 7/23/18.
//  Copyright © 2018 Apple Inc. All rights reserved.
//

#import "ReportAnalyzerBridgeTrafficViewController.h"

@class ATSCaptureProtocol;

@interface PerformanceTrafficViewController : ReportAnalyzerBridgeTrafficViewController

/**
 Returns an instance of the Performance TVC.
 */
+ (instancetype)viewControllerWithCaptureProtocol:(ATSCaptureProtocol *)captureProtocol;

@end
