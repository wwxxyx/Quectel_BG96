//
//  ATSExternalAccessoryTrafficViewController.h
//  ATSMacApp
//
//  Created by Vinod Madigeri on 4/28/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <CaptureKit/ReportAnalyzerBridgeTrafficViewController.h>

@class ATSCaptureProtocol;

@interface ATSExternalAccessoryTrafficViewController : ReportAnalyzerBridgeTrafficViewController

/**
 Factory method to get a EA Traffic view controller.
 
 @param The protocol associated with this TVC.
 @param externalAccessoryEventChannel Event channel for EA events.
 
 @return An instance of EA TVC.
 */
+ (instancetype)viewControllerWithCaptureProtocol:(ATSCaptureProtocol *)captureProtocol withEventChannel:(id<EventChanneling>)externalAccessoryEventChannel;

@end
