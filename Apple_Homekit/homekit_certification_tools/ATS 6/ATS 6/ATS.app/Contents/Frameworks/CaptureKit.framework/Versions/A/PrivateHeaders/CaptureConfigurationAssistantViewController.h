//
//  CaptureConfigurationAssistantViewController.h
//  ATSMacApp
//
//  Created by Cody Brimhall on 7/24/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

/*!
 @header     CaptureConfigurationAssistantViewController
 @discussion This header file describes the interface for an abstract view 
             controller class whose subclasses present views for the Capture 
             Assistant content box.
 */

#import <Cocoa/Cocoa.h>
#import <CaptureKit/CaptureConfigurationManager.h>
#import <CaptureKit/CaptureConfigurationAssistantWindowController.h>

@class CaptureConfiguration;
@class CaptureConfigurationAssistantWindowController;

/*!
 @class      CaptureConfigurationAssistantViewController
 @abstract   An abstract view controller class that specifies the behavior and 
             user interface elements common to all view controllers that present 
             their views in the capture configuration content box.
 */
@interface CaptureConfigurationAssistantViewController : NSViewController

/*!
 @property  windowController
 @abstract  The window controller for the view controller.
 */
@property (nonatomic, assign) NSWindowController *windowController;

/*!
 @property   descriptionTextField
 @abstract   The text field at the top of the view being presented, whose string
             value describes the purpose of the view.
 */
@property (nonatomic, assign) IBOutlet NSTextField *descriptionTextField;

/*!
 @property   descriptionText
 @abstract   The text that describes the purpose of the view being presented.
 */
@property (nonatomic, copy) NSString *descriptionText;

/*!
 @property   dataModel
 @abstract   The capture configuration data model that the view being presented
             configures.
 */
@property (nonatomic, retain) CaptureConfiguration *dataModel;

/*!
 @method     viewControllerWithDataModel:
 @abstract   Returns a view controller configured to present a view that 
             configures some set of the given capture configuration assistant 
             data model's properties.
 @param      dataModel
             The capture configuration assistant data model configured by the 
             presented view.
 @return     A view controller configured to present a view that configures some 
             set of the data model's properties.
 */
+ (id)viewControllerWithDataModel:(CaptureConfiguration *)dataModel;

/*!
 @method     initWithDataModel:nibName:
 @abstract   Returns a view controller configured to present a view that
             configures some set of the given capture configuration data model's
             properties.
 @param      dataModel
             The capture configuration data model configured by the presented
             view.
 @param      nibName
             The name of nib file associated with the receiver.
 @return     A view controller configured to present a view that configures some
             set of the data model's properties.
 @discussion This is the designated initializer.
 */
+ (id)viewControllerWithDataModel:(CaptureConfiguration *)dataModel nibName:(NSString *)nibName;

/*!
 @method     initWithDataModel:
 @abstract   Returns a view controller configured to present a view that 
             configures some set of the given capture configuration data model's
             properties.
 @param      dataModel
             The capture configuration data model configured by the presented 
             view.
 @return     A view controller configured to present a view that configures some 
             set of the data model's properties.
 @discussion Window controllers created with this initializer are configured to 
             use their class name as the name of their associated nib file.
 */
- (id)initWithDataModel:(CaptureConfiguration *)dataModel;

/*!
 @method     initWithDataModel:nibName:
 @abstract   Returns a view controller configured to present a view that 
             configures some set of the given capture configuration data model's
             properties.
 @param      dataModel
             The capture configuration data model configured by the presented 
             view.
 @param      nibName
             The name of nib file associated with the receiver. Pass nil to use 
             the receiver's class name as the name of its nib file.
 @return     A view controller configured to present a view that configures some 
             set of the data model's properties.
 @discussion This is the designated initializer.
 */
- (id)initWithDataModel:(CaptureConfiguration *)dataModel nibName:(NSString *)nibName;

/*!
 @method     prepareView
 @abstract   Called before the view will be presented in order to give the view 
             a chance to update itself.
 @discussion The default implementation does nothing.
 */
- (void)prepareView;

/*!
 @method     validateConfiguration:
 @abstract   Returns a flag indicating whether or not the receiver considers
             its data model configuration to be valid within the scope of the
             properties the view controller cares about. If an error pointer is
             provided, upon returning it will point to an error object
             describing the validation error in detail.
 @param      error
             An error pointer that will be pointed at an error object on return 
             if there the view controller configuration validation fails. Pass 
             in nil if no error object is required.
 @return     YES if the view controller's configuration is valid within the 
             scope of the properties the view controller cares about; NO 
             otherwise.
 @discussion The receiver may return YES even though the data model itself is in 
             an invalid state. For example, the transport configuration view 
             controller will return YES if no transports are selected, because
             that is a valid transport configuration; however, the data model 
             itself fail to validate if it is also configured to capture iAP
             traffic using an ATS capture device.
 
             The default implementaiton always returns YES.
 */
- (BOOL)validateConfiguration:(NSError **)error;

/*!
 @method     shouldAdvanceToNextView
 @abstract   Returns a flag indicating whether or not the receiver wants the
             view to advance.
 @discussion The receiver may return YES if actions need to take place before
             the view may be advanced such as updating firmware on a capture device.
 
             The default implementaiton always returns YES.
 */
- (BOOL)shouldAdvanceToNextView;

/*!
 @method     freezeCaptureDeviceSelections
 @abstract   Called when the receiver should freeze in the selections of any
             capture device selection controls.
 @discussion Capture device selection freezes are typically needed when a new
             capture is going to start while the receiver is still in use.
             Available capture devices can potentially become unavailable, which
             changes the selection of the capture device controls unexpectedly.
 
             The default implementation does nothing.
 */
- (void)freezeCaptureDeviceSelections;

/*!
 @method     unfreezeCaptureDeviceSelections
 @abstract   Called when the receiver should unfreeze the selections of any
             capture device selection controls.
 @discussion Capture device selection freezes are typically needed when a new
             capture is going to start while the receiver is still in use.
             Available capture devices can potentially become unavailable, which
             changes the selection of the capture device controls unexpectedly.
 
             The default implementation does nothing.
 */
- (void)unfreezeCaptureDeviceSelections;

/*!
 @method     updateDataModel:
 @abstract   Action method called when the capture configuration data model 
             needs to be update in response to a change in the user interface, 
             generally as the result of user input.
 @param      sender
             The object sending the message.
 @discussion The default implementation does nothing.
 */
- (IBAction)updateDataModel:(id)sender;

@end
