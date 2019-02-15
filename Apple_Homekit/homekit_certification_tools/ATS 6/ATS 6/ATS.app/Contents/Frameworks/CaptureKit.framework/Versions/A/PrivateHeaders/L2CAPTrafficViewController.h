//
//  L2CAPViewController.h
//  ATSMacApp
//
//  Created by Douglas Frari on 09/08/18.
//  Copyright © 2018 Apple Inc. All rights reserved.
//

#import <CaptureKit/ReportAnalyzerBridgeTrafficViewController.h>

@interface L2CAPTrafficViewController : ReportAnalyzerBridgeTrafficViewController

/**
 Instantiate Bluetooth (L2CAP) traffic view controller.
 */
+ (instancetype)viewController;

@end
