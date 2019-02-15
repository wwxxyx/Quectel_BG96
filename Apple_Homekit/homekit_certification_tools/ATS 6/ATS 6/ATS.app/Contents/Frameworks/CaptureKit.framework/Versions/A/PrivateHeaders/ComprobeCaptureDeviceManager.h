//
//  ComprobeCaptureDeviceManager.h
//  ATSMacApp
//
//  Created by Daniel Moura on 5/21/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@class ComprobeCaptureDevice;

/*!
 @const      ComprobeCaptureDeviceManagerAvailableComprobeDevicesKey
 @abstract   The string that represents property availableComprobeDevicesString.
 */
extern NSString *const ComprobeCaptureDeviceManagerAvailableComprobeDevicesKey;

@interface ComprobeCaptureDeviceManager : NSObject
/*!
 @property   availableComprobeDevices
 @abstract   The array that provides values to the Bluetooth comprobe pop up
 button.
 */
@property (nonatomic, readonly, copy) NSArray *availableDevices;

/*!
 @method     sharedManager
 @abstract   Implements singleton pattern and returns a single instance of ComprobeCaptureDeviceManager.
 */
+ (ComprobeCaptureDeviceManager *)sharedManager;

/*!
 @method     comprobeForSerialNumber:
 @abstract   Returns a Bluetooth Comprobe from the devices found with a specified serial number.
 */
- (ComprobeCaptureDevice *)comprobeForSerialNumber:(NSString *)serialNumber;

/*!
 @method     addComprobeWithSerialNumber:
 @abstract   Adds a new comprobe with the specified serial number;
 */
- (void)addComprobeWithSerialNumber:(NSString *)serialNumber;

/*!
 @method     removeComprobeWithSerialNumber:
 @abstract   Removes the comprobe with the related serial number;
 */
- (void)removeComprobeWithSerialNumber:(NSString *)serialNumber;

/*!
 @method     checkForComprobes:
 @abstract   Starts to check for bluetooth comprobe devices;
 @return     YES if the comprobe inquiry was successfully started, NO otherwise
 */
- (BOOL)checkForComprobes:(NSError **)error;

@end
