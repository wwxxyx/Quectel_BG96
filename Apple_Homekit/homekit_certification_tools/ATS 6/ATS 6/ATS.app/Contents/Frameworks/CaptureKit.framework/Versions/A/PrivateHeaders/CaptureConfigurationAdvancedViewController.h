//
//  CaptureConfigurationBluetoothViewController.h
//  ATSMacApp
//
//  Created by Natália Cabral on 08/01/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <CaptureKit/CaptureConfiguration.h>

@class CaptureConfigurationAdvancedWindowController;

@interface CaptureConfigurationAdvancedViewController : NSViewController

/*!
 @property  windowController
 @abstract  The window controller for the view controller. Set in IB.
 */
@property (nonatomic, assign) IBOutlet CaptureConfigurationAdvancedWindowController *windowController;

/**
 A capture configuration data model that reflects the selections in the view controller's view.
 */
@property (nonatomic, readonly) CaptureConfiguration *configuredDataModel;

/*!
 Called before the advanced window controller starts a capture to give the view controller a chance to prevent the start.
 @discussion   Subclasses should override and return NO when capture should not start.
 */
- (BOOL)shouldStartCaptureForConfiguration:(CaptureConfiguration *)configuration;

/*!
 Method to present a sheet in the Advanced Capture Configuration window when user checks the "Include Streaming Media" checkbox
 
 @param checkBoxState current CheckBox state
 @param actionHandler block to run when Exclude or Include button is pressed or if the checbox is unchecked. Can be nil.
 */
- (void)presentIncludeStreamingMediaAlert:(NSButton *)checkBox
        actionHandler:(void (^)(BOOL shouldIncludeStreamingMedia))actionHandler;

/**
 Method to save the current data model before closing or switching the tab view.
 */
- (void)saveCurrentDataModel;

@end
