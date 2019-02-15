//
//  PreferencesWindowController.h
//  ATS
//
//  Created by Joe Basu on 11/11/09.
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <Cocoa/Cocoa.h>

@class SelfCertWindowController;
@class iAPTrafficViewController;

extern NSString * const SystemTimestampFormatChangedNotificationName;

@interface PreferencesWindowController : NSWindowController {
    NSToolbar *_toolbar;
    NSView *_generalPreferencesView;
    NSView *_trafficPreferencesView;
    NSView *_bluetoothDevicesPreferencesView;
    NSString *_displayedItemIdentifier;
    NSButton *_suppressErrorsAndWarningsCheckbox;
    NSButton *_displaySyncByteCheckbox;
    NSColorWell *_appleDeviceColorWell;
    NSColorWell *_accColorWell;
    NSView *_trafficPreviewView;
    iAPTrafficViewController *_trafficPreviewController;
    SelfCertWindowController *_selfCertWindowController;
}

@property (nonatomic, retain) IBOutlet NSToolbar *toolbar;
@property (nonatomic, retain) IBOutlet NSView *trafficPreferencesView;
@property (nonatomic, retain) IBOutlet NSView *bluetoothDevicesPreferencesView;
@property (nonatomic, retain) IBOutlet NSButton *suppressErrorsAndWarningsCheckbox;
@property (nonatomic, retain) IBOutlet NSButton *suppressIDBusWakeBreakCheckbox;
@property (nonatomic, retain) IBOutlet NSButton *displaySyncByteCheckbox;
@property (nonatomic, retain) IBOutlet NSColorWell *appleDeviceColorWell;
@property (nonatomic, retain) IBOutlet NSColorWell *accColorWell;
@property (nonatomic, retain) NSColor *appleDeviceTrafficColor;
@property (nonatomic, retain) NSColor *accessoryTrafficColor;
@property (nonatomic, copy) NSString *displayedItemIdentifier;
@property (nonatomic, retain) IBOutlet NSView *trafficPreviewView;
@property (nonatomic, retain) iAPTrafficViewController *trafficPreviewController;
@property (nonatomic, retain) IBOutlet SelfCertWindowController *selfCertWindowController;
@property (nonatomic, retain) IBOutlet NSArrayController *bluetoothDevicesArrayController;
@property (nonatomic, retain) IBOutlet NSArrayController *bluetoothCaptureConfigurationArrayController;
@property (nonatomic, retain) NSViewController *specSelectionViewController;

- (void)showPreferences:(id)sender;
- (IBAction)toolbarItemSelected:(id)sender;
- (void)changeTimestampUnits:(id)sender;
- (IBAction)suppressErrorsAndWarningsToggled:(id)sender;
-(void)showSpecSelectionView;

/*!
 @method    suppressIDBusWakeBreakToggled:
 @abstract  Invoked whenever the suppressIDBusWakeBreak setting is changed
 */
- (IBAction)suppressIDBusWakeBreakToggled:(id)sender;

- (IBAction)openFontPanel:(id)sender;
- (IBAction)defaultFont:(id)sender;
- (void)setAppleDeviceTrafficColor:(NSColor *)color;
- (void)setAccessoryTrafficColor:(NSColor *)color;
- (IBAction)removeBluetoothDevice:(id)sender;
- (IBAction)removeBluetoothCaptureConfiguration:(id)sender;

@end
