//
//  CaptureConfigurationBeagleViewController.h
//  ATSMacApp
//
//  Created by Cody Brimhall on 8/27/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

/*!
 @header     CaptureConfigurationAssistantViewController
 @abstract   This header file describes the interface for the view controller 
             that manages configuring a capture configuration data model's 
             Beagle capture device properties.
 */

#import <CaptureKit/CaptureConfigurationAssistantViewController.h>

@class BeagleUSBCaptureDevice;

/*!
 @class      CaptureConfigurationBeagleViewController
 @abstract   Concrete subclass of the abstract class
             CaptureConfigurationAssistantViewController, with user interface
             elements for selecting the Beagle USB protocol analyzer to use in 
             the capture.
 */
@interface CaptureConfigurationBeagleViewController : CaptureConfigurationAssistantViewController

/*!
 @property   beagleArrayController
 @abstract   The array controller that manages the list of available Beagles.
 */
@property (assign) IBOutlet NSArrayController *beagleArrayController;

/*!
 @property   beaglePlaceholderPopUp
 @abstract   The pop up button displayed in place of the Beagle selection pop up 
             button when there are no Beagles available.
 */
@property (assign) IBOutlet NSPopUpButton *beaglePlaceholderPopUp;

/*!
 @property   selectedCaptureDevice
 @abstract   The selected Beagle USB protocol analyzer.
 */
@property (nonatomic, readonly) BeagleUSBCaptureDevice *selectedCaptureDevice;

/*!
 @method     initWithDataModel:
 @abstract   Returns a view controller configured to present a view that
             configures some set of the given capture configuration data model's
             properties.
 @param      dataModel
             The capture configuration data model configured by the presented
             view.
 @param      dataModel
             The capture configuration data model configured by the presented view.
 @return     A view controller configured to present a view that configures some
             set of the data model's properties.
 @discussion By default the view controller is always configurable for 8-pin.
             This is the designated initializer.
 */
- (id)initWithDataModel:(CaptureConfiguration *)dataModel;

/*!
 @method     viewControllerWithDataModel:
 @abstract   Returns a view controller configured to present a view that
             configures some set of the given capture configuration data model's
             properties.
 @param      dataModel
             The capture configuration data model configured by the presented
             view.
 @param      dataModel
             The capture configuration data model configured by the presented view.
 @return     A view controller configured to present a view that configures some
             set of the data model's properties.
 @discussion By default the view controller is always configurable for 8-pin.
 */
+ (id)viewControllerWithDataModel:(CaptureConfiguration *)dataModel;

@end
