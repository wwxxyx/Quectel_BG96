//
//  FrontlineFrameworkInterface.h
//  ATSMacApp
//
//  Created by Natália Cabral on 11/07/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <FTBluetoothDatasourceAPI/FTBluetoothDatasourceAPI.h>
#import <CaptureKit/PhysicalComprobeCaptureDevice.h>

/** The domain for errors generated while working with a ComProbe. */
extern NSString * const FrontlineFrameworkInterfaceErrorDomain;

/** An enumeration of errors encountered while working with a ComProbe. */
typedef NS_ENUM(NSInteger, FrontlineFrameworkInterfaceError) {
    /** A command couldn't run because device inquiry is running. */
    FrontlineFrameworkInterfaceErrorPerformingDeviceInquiry = 1,
};

@interface FrontlineFrameworkInterface : NSObject <FTBluetoothComProbeAttached, FTBluetoothComProbeDeviceInquiry, FTBluetoothComProbeDataCapture>

/** Property to represent if the bluetooth device inquiry is being performed. */
@property BOOL performingDeviceInquiry;

/*!
 A boolean that says if the registered comprobe is capturing or not
 */
@property (nonatomic, readonly, getter=isComprobeCapturing) BOOL comprobeCapturing;

/**
 Implements singleton pattern and returns a single instance of FrontlineFrameworkInterface.
 
 @return The shared FrontlineFrameworkInterface instance.
 */
+ (FrontlineFrameworkInterface *)sharedInterface;

/**
 Starts the bluetooth devices inquiry.
 
 @param error If device inquiry could not be started, on return this will contain an error object describing the failure. Pass nil if no error information is needed.
 @return YES if the inquiry was successfully started, NO otherwise.
 */
- (BOOL)startInquiryError:(NSError **)error;

/**
 Cancels the device inquiry.
 
 @param error If device inquiry could not be canceled, on return this will contain an error object describing the failure. Pass nil if no error information is needed.
 @return YES if the inquiry was successfully canceled, NO otherwise.
 */
- (BOOL)cancelInquiry:(NSError **)error;

/**
 Starts to check for bluetooth comprobe devices.
 
 @param error If the ComProbe check failed, on return this will contain an error object describing the failure. Pass nil if no error information is needed.
 @return YES if the comprobes activities were successfully started, NO otherwise.
 */
- (BOOL)checkForComprobes:(NSError **)error;

/**
 Updates the addresses of apple device and accessory devices.
 
 @param appleDeviceAddress The bdaddr of the Apple device, represented as a hex string.
 @param accessoryAddress The bdaddr of the accessory, represented as a hex string.
 @param linkKey The link key to use when sniffing, represented as a hex string.
 @param error On return, if an error occurred while updating the ComProbe settings, this will point to an error object describing the failure. Pass nil if no error information is needed.
 @return YES if the apple device address, accessory address and the link key were successfully updated, NO otherwise.
 */
- (BOOL)updateAppleDeviceAddress:(NSString *)appleDeviceAddress accessoryAddress:(NSString *)accessoryAddress linkKey:(NSString *)linkKey error:(NSError **)error;

/**
 Starts the bluetooth devices sniffing.
 
 @param error On return, if sniffing failed to start, this will point to an error object describing the failure. Pass nil if no error information is needed.
 @return YES if the comprobes sniffing was successfully started, NO otherwise.
 */
- (BOOL)startSniffingError:(NSError **)error;

/**
 Cancel the bluetooth devices sniffing.
 
 @param error On return, if sniffing failed to stop, this will point to an error object describing the failure. Pass nil if no error information is needed.
 @return     YES if the comprobes sniffing was successfully cancelled, NO otherwise.
 */
- (BOOL)cancelSniffing:(NSError **)error;

/** Stop all operations on ComProbe. */
- (void)stopComprobeActivities;

/*!
    Changes the currently selected ComProbe to the connected ComProbe with serial number equalt to the serial parameter.
    @param serial Serial number used to identify which ComProbe to set as selected.
    @return YES if selection change was succesful, NO if an error has occurred.
*/
- (BOOL)selectComProbeWithSerialString:(NSString *)serial error:(NSError **)error;

/**
 Check if the given link key is valid.
 
 @param linkKey The link key to use when sniffing, represented as a hex string.
 @param error On return, if the given link key is not valid, this will point to an error object describing the validation failure. Pass nil if no error information is needed.
 @return YES if link key is valid, NO otherwise.
 */
- (BOOL)isLinkKeyValid:(NSString *)linkKey error:(NSError **)error;

/**
 Identifies if an error is a firmware required error from this domain by comparing domain and error code.
 
 @param error The error to be verified.
 @return Yes if the error is a firmware required error combination and NO otherwise.
 */
+ (BOOL)isFirmwareError:(NSError *)error;

@end
