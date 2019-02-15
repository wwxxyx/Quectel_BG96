//
//  MikeyBusSuperFrameTrafficViewController.h
//  ATSMacApp
//
//  Created by Vinod Madigeri on 11/2/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import "ReportAnalyzerBridgeTrafficViewController.h"

@class ATSCaptureProtocol;

@interface MikeyBusSuperFrameTrafficViewController : ReportAnalyzerBridgeTrafficViewController

/**
 Returns an instance of MikeyBus Super Frame Traffic View Controller.
 */
+ (instancetype)viewControllerWithCaptureProtocol:(ATSCaptureProtocol *)captureProtocol;

@end
