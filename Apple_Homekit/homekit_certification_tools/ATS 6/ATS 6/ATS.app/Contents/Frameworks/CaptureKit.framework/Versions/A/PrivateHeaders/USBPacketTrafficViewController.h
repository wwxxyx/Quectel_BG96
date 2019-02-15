//
//  USBPacketTrafficViewController.h
//  ATSMacApp
//
//  Created by Bob Burrough on 12/12/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <CaptureKit/ReportAnalyzerBridgeTrafficViewController.h>

@interface USBPacketTrafficViewController : ReportAnalyzerBridgeTrafficViewController

/**
 Create an instance of the TVC.
 */
+ (instancetype)viewController;

@end
