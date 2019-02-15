//
//  PowerDeliveryTrafficViewController.h
//  ATSMacApp
//
//  Created by Zachary Church on 3/28/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <CaptureKit/ReportAnalyzerBridgeTrafficViewController.h>

@class ATSCaptureProtocol;

@interface PowerDeliveryTrafficViewController : ReportAnalyzerBridgeTrafficViewController

+ (instancetype)viewControllerForCaptureProtocol:(ATSCaptureProtocol *)captureProtocol;

@end
