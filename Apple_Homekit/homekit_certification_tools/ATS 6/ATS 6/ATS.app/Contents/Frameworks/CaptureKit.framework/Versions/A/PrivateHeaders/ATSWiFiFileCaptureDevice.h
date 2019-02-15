//
//  ATSWiFiFileCaptureDevice.h
//  ATSMacApp
//
//  Created by Cody Brimhall on 2/11/15.
//  Copyright (c) 2015 Apple Inc. All rights reserved.
//

#import <CaptureKit/ATSWiFiCaptureDevice.h>

/** ATSWiFiFileCaptureDevice is an ATSWiFiCaptureDevice subclass that sniffs traffic from a packet capture file. */
@interface ATSWiFiFileCaptureDevice : ATSWiFiCaptureDevice

/** The URL for the file to sniff. */
@property (readonly) NSURL *fileURL;

/**
 Initializes and returns a new capture device configured to sniff the file at the given URL.
 
 @param fileURL The URL of the packet capture file to sniff.
 @return The new capture device object.
 */
- (instancetype)initWithFileURL:(NSURL *)fileURL;

@end
