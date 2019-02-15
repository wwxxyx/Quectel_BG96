//
//  IAPDLogCaptureImporter.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 5/10/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/ATSCaptureImporter.h>

@interface IAPDLogCaptureImporter : ATSCaptureImporter

/*!
 @method    transportString
 @abstract  The transport string for the target transport
 */
@property (nonatomic, retain) NSString *transportString;

/*!
 @method    isiAP2
 @abstract  Whether or not the transport we're looking for uses iAP2
 */
@property (nonatomic, assign) BOOL isiAP2;

/*!
 @property  appleDeviceActingAsUSBDevice
 @abstract  If USB, is the Apple device acting as a USB device?
 */
@property (nonatomic, assign) BOOL appleDeviceActingAsUSBDevice;

/*!
 @method    initWithLog:
 @abstract  Initialize with the URL of a log
 @return    self on success, nil if log could not be opened
 */
- (id)initWithLog:(NSURL *)url;

/*!
 @method    iAPStreams
 @abstract  Return dictionary of iAP1 packet streams with the key being the transport for each
 */
- (NSDictionary *)iAPStreams;

/*!
 @method    iAP2LinkStreams
 @abstract  Return dictionary of iAP2 Link packet streams with the key being the transport for each
 */
- (NSDictionary *)iAP2LinkStreams;

/*!
 @method    stopLoading
 @abstract  Sets a flag to stop the loading of streams if that's happening on another thread
 */
- (void)stopLoading;

@end
