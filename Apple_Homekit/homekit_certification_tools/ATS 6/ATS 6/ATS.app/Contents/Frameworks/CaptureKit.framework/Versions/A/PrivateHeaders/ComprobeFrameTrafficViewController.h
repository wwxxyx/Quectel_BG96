//
//  ComprobeFrameTrafficViewController.h
//  ATSMacApp
//
//  Created by Douglas Frari on 17/07/18.
//  Copyright © 2018 Apple Inc. All rights reserved.
//

#import <CaptureKit/ReportAnalyzerBridgeTrafficViewController.h>

@interface ComprobeFrameTrafficViewController : ReportAnalyzerBridgeTrafficViewController

/**
 * Instantiate Bluetooth (Baseband/ComProbe) traffic view controller.
 */
+ (instancetype)viewController;

@end
