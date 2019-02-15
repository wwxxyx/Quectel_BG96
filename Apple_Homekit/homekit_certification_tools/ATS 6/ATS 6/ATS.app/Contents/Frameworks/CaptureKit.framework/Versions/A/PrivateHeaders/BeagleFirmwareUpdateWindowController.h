//
//  BeagleFirmwareUpdateWindowController.h
//  ATSMacApp
//
//  Created by Kip Nicol on 4/2/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class PhysicalBeagleUSBCaptureDevice;

/*!
 @class     BeagleFirmwareUpdateWindowController
 @abstract  Window Controller which handles the Beagle update process.
 */
@interface BeagleFirmwareUpdateWindowController : NSWindowController

/*!
 @method     presentModalSheetForWindow:
 @abstract   Presents the firmware update sheet modally for the given window.
 @param      window
             The window that the modal sheet should be attached to.
 @param      beagleToUpdate
             The beagle object that the update will be performed on.
 @param      completionHandler
             The Block object to call after the sheet has been dismissed. The BeagleFirmwareUpdateWindowController
             object is passed to the Block to allow access to it.
 */
- (void)presentModalSheetForWindow:(NSWindow *)parentWindow withBeagleToUpdate:(PhysicalBeagleUSBCaptureDevice *)beagleToUpdate completionHandler:(void (^)(BeagleFirmwareUpdateWindowController *))completionHandler;

/*!
 @method     dismissModalSheet
 @abstract   Dismisses the firmware update sheet.
 */
- (void)dismissModalSheet;

@end
