//
//  CaptureConfigurationBluetoothPairingViewController
//  ATSMacApp
//
//  Created by Gustavo E de Paula on 4/1/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureConfigurationAssistantViewController.h>

@interface CaptureConfigurationBluetoothPairingViewController : CaptureConfigurationAssistantViewController <NSTextFieldDelegate>

/*!
 @method     initWithDataModel:
 @abstract   initializes a CaptureConfigurationBluetoothPairingViewController view controller
 @param      dataModel
 The capture configuration data model configured by the presented view.
 @return     A CaptureConfigurationBluetoothPairingViewController configured to present a view that configures the pairing mode
 
 This is the designated initializer.
 */
- (id)initWithDataModel:(CaptureConfiguration *)dataModel;

/*!
 @method     viewControllerWithDataModel:
 @abstract   creates a CaptureConfigurationBluetoothPairingViewController view controller
 @param      dataModel
 The capture configuration data model configured by the presented view.
 @return     A CaptureConfigurationBluetoothPairingViewController configured to present a view that configures the pairing mode
 */
+ (id)viewControllerWithDataModel:(CaptureConfiguration *)dataModel;

@end
