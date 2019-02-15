//
//  ATSWiFiCaptureDevice.h
//  ATSMacApp
//
//  Created by Cody Brimhall on 2/9/15.
//  Copyright (c) 2015 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureDevice+Protected.h>
#import <CaptureKit/ATSSniffOperation.h>

@class ATSWiFiCaptureDevice;

/** The placeholder string to use as the capture device serial number. */
extern NSString *const ATSWiFiCaptureDeviceSerialNumberPlaceholder;

/** ATSWiFiCaptureDevice delegates are notified of new data as it comes in from the capture hardware. */
@protocol ATSWiFiCaptureDeviceDelegate <NSObject>

/**
 Informs the delegate that the capture device received the given data.
 
 This method is required.
 
 @param captureDevice The capture device that received the data.
 @param timestamp The timestamp in seconds of the sniffed data, in nanoseconds.
 @param data The sniffed data.
 */
- (void)WiFiCaptureDevice:(ATSWiFiCaptureDevice *)captureDevice didSniffData:(NSData *)data atTimestamp:(UInt64)timestamp;

- (void)WiFiCaptureDeviceDidBecomeInvalid:(ATSWiFiCaptureDevice *)captureDevice;

@end

/** ATSWiFiCaptureDevice instances manage the process of sniffing traffic from a WiFi card. */
@interface ATSWiFiCaptureDevice : CaptureDevice <ATSSniffOperationDelegate>

/** The queue that runs the sniff operation. */
@property NSOperationQueue *operationQueue;

/** The capture device delegate. */
@property (weak) id<ATSWiFiCaptureDeviceDelegate> delegate;

/**
 Initializes and returns a new capture device configured to sniff the file at the given URL.
 
 @param fileURL The URL of the packet capture file to sniff.
 @return The new capture device object.
 */
+ (instancetype)captureDeviceForFileURL:(NSURL *)fileURL;

- (void)configureAndAddSniffOperation:(ATSSniffOperation *)sniffOperation;

@end
