//
//  MikeyBusBulkDataTrafficViewController.h
//  ATSMacApp
//
//  Created by Vinod Madigeri on 10/28/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import "ReportAnalyzerBridgeTrafficViewController.h"

@class ATSCaptureProtocol;

@interface MikeyBusBulkDataTrafficViewController : ReportAnalyzerBridgeTrafficViewController

/**
 Returns an instance of MikeyBus Bulk Data Traffic View Controller.
 */
+ (instancetype)viewControllerWithCaptureProtocol:(ATSCaptureProtocol *)captureProtocol;

@end
