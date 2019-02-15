//
//  CaptureConfigurationComprobeViewController
//  ATSMacApp
//
//  Created by Gustavo E de Paula on 4/1/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureConfigurationAssistantViewController.h>

@interface CaptureConfigurationCaptureDeviceViewController : CaptureConfigurationAssistantViewController

/*!
 initializes a CaptureConfigurationComprobeViewController view controller.
 @param      dataModel
             The capture configuration data model configured by the presented view.
 @return     A CaptureConfigurationComprobeViewController configured to present a view that configures the bluetooth Comprobe
 
 This is the designated initializer.
 */
- (instancetype)initWithDataModel:(CaptureConfiguration *)dataModel;

/*!
 creates to initialize a CaptureConfigurationComprobeViewController view controller
 @param      dataModel
             The capture configuration data model configured by the presented view.
 @return     A CaptureConfigurationComprobeViewController configured to present a view that configures the bluetooth Comprobe
 */
+ (instancetype)viewControllerWithDataModel:(CaptureConfiguration *)dataModel;

@end
