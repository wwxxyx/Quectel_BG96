//
//  CaptureConfigurationAdvancedWindowController.h
//  ATSMacApp
//
//  Created by Cody Brimhall on 7/29/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <CaptureKit/CaptureConfigurationWindowController.h>

@class CaptureConfiguration;
@class BeagleUSBCaptureDeviceManager;
@class CaptureConfigurationAdvancedWiredViewController;
@class CaptureConfigurationAdvancedWirelessViewController;

extern NSString *const CaptureConfigurationAdvancedWindowControllerWiredTabTag;
extern NSString *const CaptureConfigurationAdvancedWindowControllerWirelessTabTag;

/** Concrete subclass of the abstract class CaptureConfigurationWindowController, with user interface elements for presenting the entire set of options a user has when configuring a capture configuration data model. */
@interface CaptureConfigurationAdvancedWindowController : CaptureConfigurationWindowController <NSTabViewDelegate>

/** The view controller that handles the events related to ATS capture configuration tab. */
@property (nonatomic, assign) IBOutlet CaptureConfigurationAdvancedWiredViewController *wiredTabViewController;

/** The view controller that handles the events related to bluetooth capture configuration tab. */
@property (nonatomic, assign) IBOutlet CaptureConfigurationAdvancedWirelessViewController *wirelessTabViewController;

/** The tab view that contains the Bluetooth and ATS Box capture configurations views. */
@property (nonatomic, assign) IBOutlet NSTabView *captureDeviceTabView;

/** The object that maintains the list of available Beagle capture devices. */
@property (nonatomic, readonly) BeagleUSBCaptureDeviceManager *beagleDeviceManager;

@end
