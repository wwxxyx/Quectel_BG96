//
//  CaptureConfigurationAssistantWindowController.h
//  ATSMacApp
//
//  Created by Cody Brimhall on 7/23/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

/*!
 @header     CaptureConfigurationAssistantWindowController
 @discussion This header file describes the interface for the capture 
             configuration window controller, which presents capture document 
             configuration options through a series of views.
 */

#import <Cocoa/Cocoa.h>
#import <CaptureKit/CaptureConfigurationWindowController.h>
#import <CaptureKit/CaptureConfigurationTypes.h>

@class CaptureConfigurationAssistantViewController;

/*!
 @class      CaptureConfigurationAssistantWindowController
 @abstract   Concrete subclass of the abstract class
             CaptureConfigurationWindowController, with user interface elements 
             for presenting a series of views through which the user configures 
             a capture configuration data model.
 */
@interface CaptureConfigurationAssistantWindowController : CaptureConfigurationWindowController

/*!
 @property   viewTitleTextField
 @abstract   The text field containing the current configuration view's title.
 */
@property (nonatomic, assign) IBOutlet NSTextField *viewTitleTextField;

/*!
 @property   contentBox
 @abstract   The box that contains and presents the current configuration view.
 */
@property (nonatomic, assign) IBOutlet NSBox *contentBox;

/*!
 @property   previousButton
 @abstract   The button that returns to the previous configuration view 
             presented in the content box.
 */
@property (nonatomic, assign) IBOutlet NSButton *previousButton;

/*!
 @property   advancedOptionsButton
 @abstract   The button that switches the user from the capture configuration 
             assistant to the advanced capture configuration window.
 */
@property (nonatomic, assign) IBOutlet NSButton *advancedOptionsButton;

/*!
 @method     previousButtonPressed:
 @abstract   Action method called when the user presses the Go Back button.
 @param      sender
             The object sending the message.
 */
- (IBAction)previousButtonPressed:(id)sender;

/*!
 @method     startCaptureButtonPressed:
 @abstract   Action method called when the user clicks on the Start Capture
             button.
 @param      sender
             The object sending the message.
 @discussion The default implementation sets the captureStarted flag to YES and
             sends captureConfigurationWindowController:didConfigureDataModel:
             to the delegate object.
 */
- (IBAction)startCaptureButtonPressed:(id)sender;

/*!
 @property   currentState
 @abstract   The current state of the capture configuration. The state
 corresponds to the view being presented in the content box.
 */
@property (nonatomic) CaptureConfigurationState currentState;

/*!
 Calls the same functionalities as the windowDidLoad method. Needs to be called when the current state was chagned outside of
 a retreat or advance view.
 */
- (void) updateCurrentView;

@end
