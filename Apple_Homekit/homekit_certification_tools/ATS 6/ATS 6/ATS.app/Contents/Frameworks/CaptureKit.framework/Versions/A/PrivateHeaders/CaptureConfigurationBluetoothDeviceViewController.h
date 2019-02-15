//
//  CaptureConfigurationBluetoothDeviceViewController
//  ATSMacApp
//
//  Created by Gustavo E de Paula on 4/1/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureConfigurationAssistantViewController.h>

enum {
    BluetoothDeviceTypeAppleDevice,
    BluetoothDeviceTypeAccessory,
} typedef BluetoothDeviceType;


extern NSString *const CaptureConfigurationBluetoothDeviceInquiryButtonTitle;
extern NSString *const CaptureConfigurationBluetoothDeviceStopInquiryButtonTitle;

@interface CaptureConfigurationBluetoothDeviceViewController : CaptureConfigurationAssistantViewController

/*!
 @property   questionText
 @abstract   The question that describes the purpose of the view being presented.
 */
@property (nonatomic, copy) NSString *questionText;

/*!
 @method     initWithDataModel:andBluetoothDeviceType:
 @abstract   initializes a CaptureConfigurationBluetoothDeviceViewController view controller
 @param      dataModel
                The capture configuration data model configured by the presented view.
             bluetoothDeviceType
                The type of the bluetooth that will be configured by this view.
 @return     A CaptureConfigurationBluetoothDeviceViewController configured to present a view that configures a bluetooth device
 
 This is the designated initializer.
 */
- (id)initWithDataModel:(CaptureConfiguration *)dataModel andBluetoothDeviceType:(BluetoothDeviceType)bluetoothDeviceType;

/*!
 @method     viewControllerWithDataModel:
 @abstract   creates a CaptureConfigurationBluetoothDeviceViewController view controller
 @param      dataModel
 The capture configuration data model configured by the presented view.
 @return     A CaptureConfigurationBluetoothDeviceViewController configured to present a view that configures the bluetoothDevice
 */
+ (id)viewControllerWithDataModel:(CaptureConfiguration *)dataModel;

/*!
 @method     inquiryButtonAction:
 @abstract   starts or stops the bluetooth devices inquiry
 */
- (IBAction)inquiryButtonAction:(id)sender;

@end
