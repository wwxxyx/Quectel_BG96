//
//  CaptureDevice+Protected.h
//  ATSMacApp
//
//  Created by Vinod Madigeri on 3/7/18.
//  Copyright © 2018 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureDevice.h>

@interface CaptureDevice ()

/**
 Instantiate a capture device with input serial number and device path.
 
 @param serialNumber Serial number of the serial capture device.
 @param devicePath Device path of the serial capture device.
 */
+ (instancetype)deviceWithSerialNumber:(NSString *)serialNumber;

/**
 Instantiate a capture device with input serial number and device path.
 
 @param serialNumber Serial number of the serial capture device.
 @param devicePath Device path of the serial capture device.
 */
- (instancetype)initWithSerialNumber:(NSString *)serialNumber;

/**
 Method to connect the capture device.
 
 @param usingiAP2 Whether or not this capture device is configured to expect iAP2.
 @param error Error is valid if there was an issue connecting the capture device.
 
 @return Status on whether or not the connection was successful.
 */
- (BOOL)connectUsingiAP2:(BOOL)usingiAP2 error:(NSError **)error;

/**
 Method to disconnect the capture device.
 
 @param error Error is valid if there was an issue disconnecting the capture device.
 
 @return Status on whether or not the disconnection was successful.
 */
- (BOOL)disconnectAndReturnError:(NSError **)error;

/**
 Method to start reading bytes from the capture device.
 */
- (void)startReading;

@end
