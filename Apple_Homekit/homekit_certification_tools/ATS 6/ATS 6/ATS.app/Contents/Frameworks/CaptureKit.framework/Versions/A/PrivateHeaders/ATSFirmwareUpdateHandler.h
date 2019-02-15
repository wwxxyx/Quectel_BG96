//
//  ATSFirmwareUpdateHandler.h
//  ATSMacApp
//
//  Created by Kip Nicol on 2/8/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>

/** Domain for errors encountered while attempting to update a ComProbe's firmware. */
extern NSString * const ATSFirmwareUpdateErrorDomain;

/** An enumeration of possible firmware updater states. */
typedef NS_ENUM(UInt8, ATSFirmwareUpdateStatus) {
    /** The firmware updater app launch succeeded. */
    ATSFirmwareUpdateStatusAppLaunchSucceeded = 0,
    
    /** The firmware updater app launch failed. */
    ATSFirmwareUpdateStatusAppLaunchFailed,
    
    /** The user canceled the update. */
    ATSFirmwareUpdateStatusAppLaunchCanceled,
};

/** ATSFirmwareUpdateHandler manages the process of launching the ComProbe firmware update app. */
@interface ATSFirmwareUpdateHandler : NSObject

/** The name of the ComProbe being updated. */
@property (readonly, copy) NSString *comProbeName;

/** The URL of the firmware image to be written to the ComProbe. */
@property (readonly) NSURL *imageURL;

/** The URL of the app that will perform the firmware update. */
@property (readonly) NSURL *appURL;

/**
 Returns the dictionary of default ComProbe configuration details. The dictionary is loaded from disk once and then cached for subsequent calls.
 
 @return The default configuration info dictionary.
 */
+ (NSDictionary *)comProbeInfo;

/**
 Returns a firmware update handler configured for a ComProbe with the given PID.
 
 @param pid The USB PID for the ComProbe to be updated.
 @return A new firmware update handler.
 */
+ (instancetype)firmwareUpdateHandlerForComProbeWithPID:(NSInteger)pid;

/**
 Initializes and returns a firmware update handler configured with the given ComProbe name and firmware image URL.
 
 @param name The display name of the ComProbe to be updated.
 @param imageURL The URL for the new firmware image.
 @param appURL The URL for the app that will perform the firmware update.
 @return A new firmware update handler.
 */
- (instancetype)initWithComProbeName:(NSString *)name imageURL:(NSURL *)imageURL appURL:(NSURL *)appURL;

/**
 Begins a window-modal sheet that prompts the user to update the firmware.
 
 @param window The window to begin the sheet on.
 @param completionHandler Block called after the user has selected an update choice (Update, or Cancel). If an error occurred, the completion handler is passed an error object describing the problem.
 */
- (void)beginUpdateSheetModelForWindow:(NSWindow *)window completionHandler:(void (^)(NSError *))completionHandler;

@end
