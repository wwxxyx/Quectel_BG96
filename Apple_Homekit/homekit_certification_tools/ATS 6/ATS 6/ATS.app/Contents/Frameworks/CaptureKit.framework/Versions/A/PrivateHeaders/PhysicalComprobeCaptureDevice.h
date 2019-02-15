//
//  PhysicalComprobeCaptureDevice.h
//  ATSMacApp
//
//  Created by Natália Cabral on 19/06/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CaptureKit/ComprobeCaptureDevice.h>

/** PhysicalComprobeCaptureDevice instances provide information about attached ComProbes and mediate  */
@interface PhysicalComprobeCaptureDevice : ComprobeCaptureDevice

/** The version of the firmware installed on chip A. */
@property (retain) NSNumber *chipAFirmwareVersion;

/** The version of the firmware installed on chip B. */
@property (retain) NSNumber *chipBFirmwareVersion;

/** The version of the firmware installed on chip C. */
@property (retain) NSNumber *chipCFirmwareVersion;

/** The version of the firmware installed on chip D. */
@property (retain) NSNumber *chipDFirmwareVersion;

/** A placeholder for the non-existent device path. Capture configuration expects this property to be present for physical capture devices. */
@property (readonly) NSString *devicePath;

/**
 Called when a new Bluetooth frame is received.
 
 @param frame The new Bluetooth frame.
 @param timestamp The frame's timestamp after it has been normalized for display.
 */
- (void)readFrame:(FTDataFrame *)frame atTimestamp:(UInt64)timestamp;

/**
 Notification from the FFI that the comprobe has finished sniffing.
 
 @param error If there was an error, this object is set.
 */
- (void)comprobeFinishedSniffingWithError:(NSError *)error;

/**
 Notification from the FFI that the comprobe encountered an error.
 
 @param error An error object describing the problem.
 */
- (void)comprobeEncounteredError:(NSError *)error;

/** Notifies that the inquiry has finished. */
- (void)inquiryComplete;

@end
