//
//  CaptureConfigurationWindowController.h
//  ATSMacApp
//
//  Created by Cody Brimhall on 7/30/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class CaptureConfiguration;
@class FrontlineFrameworkInterface;

@protocol CaptureConfigurationWindowControllerDelegate;

/**
 An abstract window controller class that specifies behavior and user interface elements common to all types of capture configuration windows.
 
 This class is abstract and should not be instantiated directly.
 */
@interface CaptureConfigurationWindowController : NSWindowController <NSWindowDelegate>

/** The button that the user clicks to progress capture configuration. */
@property (nonatomic, assign) IBOutlet NSButton *nextButton;

/// Convenience property to find out whether or not Comprobe driver udpate is needed.
@property (nonatomic, readonly) BOOL isComprobeDriverUpdateNeeded;

/**
 An object that implements the CaptureConfigurationWindowControllerDelegate protocol and wishes to receive messages when the user either complets or cancels configuring a new capture.
 
 CaptureConfigurationWindowController must have a delegate.
 */
@property (nonatomic, weak) id<CaptureConfigurationWindowControllerDelegate> delegate;

/** The data model the user is configuring using the interface presented by the window controller. */
@property (nonatomic, strong) CaptureConfiguration *dataModel;

/** A flag indicating whether or not the user has indicated that a capture should start. */
@property (nonatomic, readonly, getter=isCaptureStarted) BOOL captureStarted;

/** The interface singleton that controls Bluetooth ComProbes. */
@property (nonatomic, readonly) FrontlineFrameworkInterface *frontlineFrameworkInterface;

/**
 Returns a new instance of CaptureConfigurationWindowController initialized with the given data model and delegate object.
 
 The delegate object cannot be nil. This is the designated initializer.
 
 @param dataModel The data model that should be configured by the interface presented by the receiver.
 @param delegate An object that implements the CaptureConfigurationWindowControllerDelegate protocol and wishes to receive a message when the user either completes or cancels configuring the data model.
 @return A new instance of CaptureConfigurationWindowController initialized with the given data model and delegate object.
 */
- (instancetype)initWithDataModel:(CaptureConfiguration *)dataModel delegate:(id<CaptureConfigurationWindowControllerDelegate>)delegate;

/**
 Action method optionally called by interface elements when the data model should be updated to reflect user input.
 
 The default implementation does nothing.
 
 @param sender The object sending the message.
 */
- (IBAction)updateDataModel:(id)sender;

/**
 Attempts to verify that the selected ATS and Beagle capture devices are attached and available.
 
 @return YES if the selected capture devices are available; NO otherwise.
 */
- (BOOL)validateCaptureDeviceSelectionsWithError:(NSError **)error;

/**
 Presents a modal alert sheet displaying the validation error information.
 
 @param validationError The error object describing why validation failed and what the user needs to do.
 */
- (void)presentValidationError:(NSError *)validationError;

/** Presents a modal alert sheet to inform the user that they need to wait for ComProbe device inquiry to finish before the capture can start. */
- (void)presentPerformingDeviceInquiryMessage;

/** Presents a modal alert displaying that no ComProbe is available for device inquiry. */
- (void)alertNoComprobeForDeviceInquiry;

/**
 Show an alert for the user to install the ComProbe driver if an update is necessary.
 
 @param completionHandler Called only if update is required and after the alert has been dismissed. returnCode parameter is the returncode from the alert
 */
- (void)showComprobeDriverUpdateNeededAlertCompletionHandler:(void (^)(NSInteger returnCode))completionHandler;

@end

/** Delegate protocol for notifying an interested object when the user has cancelled or completed configuring a data model. */
@protocol CaptureConfigurationWindowControllerDelegate <NSObject>

@required

/**
 Sent when the user has cancelled configuring the data model.
 
 This method is required.
 
 @param windowController The window controller whose data model the user cancelled configuring.
 */
- (void)captureConfigurationWindowControllerDidCancel:(CaptureConfigurationWindowController *)windowController;

/**
 Sent when the user has configured the data model.
 
 @param windowController The window controller whose data model the user completed configuring.
 @param dataModel The data model that the user configured.
 */
- (void)captureConfigurationWindowController:(CaptureConfigurationWindowController *)windowController didConfigureDataModel:(CaptureConfiguration *)dataModel;

@end
