//
//  BluetoothDevice.h
//  ATSMacApp
//
//  Created by Daniel Moura on 5/21/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface BluetoothDevice : NSObject <NSCoding>

/*!
 @property   name
 @abstract   Property that represents the name of a bluetooth device.
 */
@property (nonatomic, copy, readonly) NSString *name;

/*!
 @property   address
 @abstract   Property that represents the BD_ADDR for the bluetooth device.
 */
@property (nonatomic, copy, readonly) NSString *address;

/*!
 @method    initWithAddress:name:
 @abstract  Initializes a BluetoothDevice with the given parameters.
 @param     address
            The bluetooth device's bluetooth address.
 @param     name
            The bluetooth device's name.
 */
- (instancetype)initWithAddress:(NSString *)address name:(NSString *)name;

/*!
 @method    bluetoothDeviceWithAddress:name:
 @abstract  Returns a BluetoothDevice initialized to the given parameters.
 @param     address
            The bluetooth device's bluetooth address.
 @param     name
            The bluetooth device's name.
 */
+ (instancetype)bluetoothDeviceWithAddress:(NSString *)address name:(NSString *)name;

/*!
 @method    formattedMacAddress
 @abstract  Returns an IEEE 802 formatted MAC address (00:11:22:33:44:55) derived
            from the Frontline framework MAC address (0x001122334455).
 */
- (NSString *)formattedMacAddress;

@end
