//
//  BluetoothCaptureConfigurationManager.h
//  ATSMacApp
//
//  Created by Natália Cabral on 07/01/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CaptureKit/BluetoothCaptureConfiguration.h>

@class BluetoothDevice;

/*!
 @const      ComprobeCaptureDeviceAvailableBluetoothDevicesKey
 @abstract   The string that represents property availableBluetoothDevices.
 */
extern NSString *const ComprobeCaptureDeviceAvailableBluetoothDevicesKey;

/*!
 @const      ComprobeCaptureDeviceBluetoothCaptureConfigurationsKey
 @abstract   The string that represents property bluetoothCaptureConfigurations.
 */
extern NSString *const ComprobeCaptureDeviceBluetoothCaptureConfigurationsKey;

@interface BluetoothCaptureConfigurationManager : NSObject

/*!
 @property  availableBluetoothDevices
 @abstract  The array of bluetooth devices that are currently available for capture.
 */
@property (nonatomic, copy, readonly) NSArray *availableBluetoothDevices;

/*!
 @property   bluetoothCaptureConfigurations
 @abstract   Property to represent the array of bluetooth capture configuration.
 */
@property (nonatomic, copy) NSArray *bluetoothCaptureConfigurations;

/*!
 @method     sharedManager
 @abstract   Implements singleton pattern and returns a single instance of BluetoothCaptureConfigurationManager.
 */
+ (BluetoothCaptureConfigurationManager *)sharedManager;

/*!
 @method     addBluetoothDevice:
 @abstract   Adds a bluetooth device to the list of devices available from this manager
 @discuss    If the device address already exists, the device associated with that address is replaced.
 */
- (void)addBluetoothDevice:(BluetoothDevice *)bluetoothDevice;

/*!
 @method     removeBluetoothDeviceWithAddress:
 @abstract   Removes a bluetooth device with the given address from the list of devices available from this manager
 */
- (void)removeBluetoothDeviceWithAddress:(NSString *)address;

/*!
 @method     bluetoothDeviceForAddress:
 @abstract   Returns the bluetooth device with the given address.
 */
- (BluetoothDevice *)bluetoothDeviceForAddress:(NSString *)address;

/*!
 @method     addBluetoothCaptureConfiguration:
 @abstract   Adds a new bluetooh capture configurution to the list. 
 */
- (void)addBluetoothCaptureConfiguration:(BluetoothCaptureConfiguration *)newBluetoothCaptureConfiguration;

/*!
 @method     configurationForAppleDeviceAddress:accessoryAddress:protocolMask:
 @abstract   Returns a BluetoothCaptureConfiguration from the saved configurations list with a specified apple device, accessory and protocol.
 */
- (BluetoothCaptureConfiguration *)configurationForAppleDeviceAddress:(NSString *)appleDeviceAddress accessoryAddress:(NSString *)accessoryAddress protocolMask:(CaptureConfigurationProtocolMask)prototolMask;

/*!
 @method     configurationForAppleDeviceAddress:accessoryAddress:protocolMask:
 @abstract   Returns a BluetoothCaptureConfiguration from the saved configurations list with a specified apple device and accessory.
 */
- (BluetoothCaptureConfiguration *)configurationForAppleDeviceAddress:(NSString *)appleDeviceAddress accessoryAddress:(NSString *)accessoryAddress;

@end
