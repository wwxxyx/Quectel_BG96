//
//  USBCSVCaptureImporter.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 5/10/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/ATSCaptureImporter.h>

@interface USBCSVCaptureImporter : ATSCaptureImporter

/*!
 @property  appleDeviceActingAsUSBDevice
 @abstract  Whether or not the Apple device's role is the USB device
 */
@property (nonatomic, assign) BOOL appleDeviceActingAsUSBDevice;

/*!
 @property  isiAP2
 @abstract  Whether or not the trace uses iAP2
 */
@property (nonatomic, assign) BOOL isiAP2;

/*!
 @property  usesCarPlay
 @abstract  Whether or not the trace uses CarPlay
 */
@property (nonatomic, assign) BOOL usesCarPlay;

/*!
 @method    initWithFileURL:
 @abstract  Initialize with the text file at the passed URL
 */
- (id)initWithFileURL:(NSURL *)url;

@end
