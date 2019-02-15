//
//  CaptureConfigurationVbusCurrentViewController.h
//  ATSMacApp
//
//  Created by Cody Brimhall on 8/9/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

/*!
 @header     CaptureConfigurationVbusCurrentViewController
 @abstract   This header file describes the interface for the view controller
             responsible for configuring a capture configuration data model's 
             Vbus-related properties.
 */

#import <CaptureKit/CaptureConfigurationAssistantViewController.h>

/*!
 @class      CaptureConfigurationVbusCurrentViewController
 @abstract   Concrete subclass of the abstract class 
             CaptureConfigurationAssistantViewController, with user interface 
             elements for specifying the amperage of the Vbus current provided 
             by the accessory.
 */
@interface CaptureConfigurationVbusCurrentViewController : CaptureConfigurationAssistantViewController

/*!
 @property   VbusCurrentMatrixLabel
 @abstract   The text field that labels the matrix of Vbus amperages the 
             accessory can provide to the Apple device.
 */
@property (nonatomic, assign) IBOutlet NSTextField *VbusCurrentMatrixLabel;

/*!
 @property   availableCurrentLevelNames
 @abstract   The list of names of the available Vbus current levels.
 */
@property (nonatomic, readonly) NSArray *availableCurrentLevels;

@end
