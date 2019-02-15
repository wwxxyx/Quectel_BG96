//
//  IORegistryDeviceNotifier.h
//
//  Created by Joe Basu on 11/18/08.
//  Modernized into ARC and delegate pattern by Wesley McCloy on 09/28/18.
//  Copyright © 2018 Apple Inc. All Rights Reserved.
//

#import <Foundation/Foundation.h>

@class IORegistryDeviceNotifier;

@protocol IORegistryDeviceNotifierDelegate <NSObject>

/**
 Delegate callback for when devices have been discovered

 @param registry The registry instance that discovered the device
 @param addedDevices The devices that have been added, this is a collection of device properties
 */
- (void)ioRegistry:(IORegistryDeviceNotifier *)registry devicesAdded:(NSArray<NSDictionary *> *)addedDevices;

/**
 Delegate callback for when devices have been removed
 
 @param registry The registry instance that observed the device having been removed
 @param removedDevices The devices that have been removed, this is a collection of device properties
 */
- (void)ioRegistry:(IORegistryDeviceNotifier *)registry devicesRemoved:(NSArray<NSDictionary *> *)removedDevices;

@end

@interface IORegistryDeviceNotifier : NSObject

/**
 The delegate that receives notifications about added or removed devices
 */
@property (nonatomic, weak) id<IORegistryDeviceNotifierDelegate> delegate;

/**
 Initialize a new IORegistryDeviceNotifier and start observing for devices that meet the supplied criteria

 @param deviceName The name of the device
 @param deviceClass The class of the device
 @param productID The product ID of the device
 @param vendorID The vendor ID of the device
 @param delegate The delegate that receives the notifications
 @return The IORegistryDeviceNotifier instance
 */
+ (instancetype)notifierForDevicesNamed:(NSString *)deviceName
                            deviceClass:(NSString *)deviceClass
                              productID:(NSNumber *)productID
                               vendorID:(NSNumber *)vendorID
                               delegate:(id<IORegistryDeviceNotifierDelegate>)delegate;

// TODO: do we need this
/**
 Unsubscribes the notifier
 */
- (void)invalidate;

@end
