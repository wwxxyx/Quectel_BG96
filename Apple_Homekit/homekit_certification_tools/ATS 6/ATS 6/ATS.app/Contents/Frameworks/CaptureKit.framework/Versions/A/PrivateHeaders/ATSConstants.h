//
//  ATSConstants.h
//  ATSMacApp
//
//  Created by Zachary Church on 7/25/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class PhysicalBeagleUSBCaptureDevice;
@class CaptureDevice;
@class TimestampFormatter;

NSURL *ATSApplicationSupportDirectory(void);
char *strfob(char *str, size_t len);
NSString *NSStringWithFob(const char *fob);
NSURL *ATSServerURL(void);
NSString *CurrentTimestampUnits(void);
TimestampFormatter *CurrentTimestampFormatter(void);
bool IsUpdateNeededForPhysicalBeagle(PhysicalBeagleUSBCaptureDevice *beagle, UInt8 majorVersion, UInt8 minorVersion);
bool IsUpdateNeededForATSBox(CaptureDevice *captureDevice, UInt8 majorVersion, UInt8 minorVersion, UInt8 revisionVersion);
NSString *ATSBoxFirmwareUpdaterPath(void);
NSString *ATSLightningBoxFirmwareUpdaterPath(void);
NSString *ATSR3DFUBoxFirmwareUpdaterPath(void);
NSColor *AppleDeviceTrafficColor(void);
NSColor *AccessoryTrafficColor(void);
NSColor *appOriginatedTrafficColor(void);

/**
 The color is used for related entry highlighting. This is the user's "Highlight" color set in their System Preferences with an added alpha mask.

 @return The color to be used when referencing a related highlight
 */
NSColor *relatedHighlightColor(void);

extern NSString *const ATSEnvironmentProduction;
extern NSString *const ATSEnvironmentStaging;

extern NSString * const kSpecMapsFolder;

/* NSUserDefaults */

/**
 Defaults key for ATS Environment. "production" or "staging".
 */
extern NSString *const ATSEnvironmentKey;
extern NSString *const ATSServerKey;
extern NSString *const ATSShowWelcomeScreenKey;
extern NSString *const ATSAllowSavingPasswordsKey;
extern NSString *const ATSAllowSavingUsernameKey;
extern NSString *const ATSAppleDeviceTrafficColorKey;
extern NSString *const ATSAccessoryTrafficColorKey;
extern NSString *const ATSLogComProbeAPIActivityKey;
extern NSString *const ATSForceComProbeFirmwareUpdateKey;
extern NSString *const ATSNeverPromptFTDIUninstallerKey;
extern NSString *const ATSAcceptLicenseFlag;
extern NSString *const ATSCSVMostRecentlySelectedDirectoryPathKey;

/**
 The command-line flag when seen by ATS, it'll clear its defaults. Used by UI testing automation.
 */
extern NSString *const ATSResetDefaultsFlag;

// FIXME: <rdar://problem/23893942> Remove the "ATSShowInstallCommandLineToolMenuItemKey" user default after the Command Line tool is presentable to an ATS User
extern NSString *const ATSShowInstallCommandLineToolMenuItemKey;
extern NSString *const ATSSystemTimestampFormatKey;
extern NSString *const ATSTrafficFontKey;

/**
 Defaults key, when set to @YES, will disable all CarPlay ethernet level filtering.
 */
extern NSString *const ATSDisableCarPlayEthernetFilteringKey;

/**
 Defaults key, when set to @NO, will cause Advanced Capture Configuration to show an alert to the user everytime the "Include Streaming Media" checkbox is checked.
 */
extern NSString *const ATSHideIncludeStreamingMediaWarningKey;

/**
 Defaults key, when set to @YES, will log the build variant name (based on the preprocessor macro). Used to identify whether ATS Internal traget was built with internal views.
 */
extern NSString *const ATSShowBuildVariantKey;

extern const char fobATSShowQAUIKey[];
extern const char fobShowCACaptureDocumentsKey[];
extern const char fobValidateHelperToolKey[];
extern const char fobOverrideVersionIndex[];
extern const char fobiAP2MessageTestKey[];
extern const char fobEnableCaptureConfigurationLoggingKey[];
extern const char fobEnableBeagleUpdaterLoggingKey[];
extern const char fobAppleIDAuthenticationDebug[];
extern const char fobATSInternalUpdateServerIP[];
extern const char fobDisableInternalUpdaterKey[];
extern const char fobUSBPreserveStateAfterReset[];
extern const char fobLogCaptureConfiguration[];

typedef NS_ENUM(UInt8, ComProbeDriverUpdateState) {
    /** No update is currently required. User may proceed with using ComProbe. */
    ComProbeDriverUpdateStateNoUpdateRequired = 1,
    /** Driver update is required. Should prompt user to update. */
    ComProbeDriverUpdateStateUpdateRequired,
};

/*!
 @extern    kTrafficColorsDidChangeNotification
 @abstract  Notification that the traffic colors just changed
 */
extern NSString *kTrafficColorsDidChangeNotification;

// Method to register initial user defaults for ATS/CaptureKit
void registerUserDefaults(void);

/**
 The string used to identify internal build variants of ATS.
 */
extern NSString * const ATSBuildVariantInternal;

/**
 The string used to identify external build variants of ATS.
 */
extern NSString * const ATSBuildVariantExternal;

/**
 A string identifying the current build variant. Current variants are "internal" and "external".
 */
extern NSString * const ATSBuildVariantName;

/**
 Returns YES when the current target is an Apple-internal variant of ATS.

 @return YES if the current target is ATS Internal; NO otherwise.
 */
BOOL ATSIsInternalBuildVariant(void);

/**
 Current ATS version (read from the info plist).
 */
NSString *currentATSVersion(void);
