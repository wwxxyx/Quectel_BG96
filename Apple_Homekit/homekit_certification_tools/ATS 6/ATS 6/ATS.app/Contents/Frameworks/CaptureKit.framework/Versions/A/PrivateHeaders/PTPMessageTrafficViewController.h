//
//  PTPMessageTrafficViewController.h
//  ATSMacApp
//
//  Created by Vinod Madigeri on 4/30/18.
//  Copyright © 2018 Apple Inc. All rights reserved.
//

#import <CaptureKit/ReportAnalyzerBridgeTrafficViewController.h>

@interface PTPMessageTrafficViewController : ReportAnalyzerBridgeTrafficViewController

/**
 Instantiate PTP Messages traffic view controller.
 */
+ (instancetype)ptpMessageTrafficViewController;

@end
