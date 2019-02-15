//
//  MobileDevicesManager.h
//  ATSMacApp
//
//  These classes wrap the MobileDevice.framework into a more user friendly objective c classes
//
//  Created by Lawrence Kwak on 4/26/18.
//  Copyright © 2018 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <MobileDevice/MobileDevice.h>

// Mobile Device class
@interface MobileDevice : NSObject

/* For more details on property values see the Mobile Device framework documentation */

/*!
  Unique Device ID
 */
@property NSString* udid;

/*!
 Device Name
 */
@property NSString* deviceName;

/*!
 OS Build Version
 */
@property NSString* buildVersion;

/*!
 Product Type
 */
@property NSString* productType;

/*!
 Product Version
 */
@property NSString* productVersion;

/*!
 Device Class
 */
@property NSString* deviceClass;

/*!
 Device is paired (trusted with host mac)
 */
@property BOOL trusted;

@end

@class MobileDevicesManager;

// MobileDeviceManagerDelegate protocol
@protocol MobileDeviceManagerDelegate <NSObject>

@required

/**
 Is called when a Mobile Device does an action such as attach/detach
 
 @param mobileDeviceManager Mobile Device Manager
 @param action The action (defined in Mobile device framework)
 @param deviceUDID The mobile device Unique ID
 @param device (Optional) The mobile device object involved in action, will be nil on detach
 
 */
- (void)mobileDeviceManager:(MobileDevicesManager *)mobileDeviceManager performedAction:(AMDeviceAction)action forDeviceUDID:(NSString *)deviceUDID mobileDevice:(MobileDevice *)device;

@end

// MobileDevicesManager class
@interface MobileDevicesManager : NSObject

/*!
 Array of all connected MobileDevices
 */
@property NSMutableArray<MobileDevice*> *connectedDevices;

/*!
 Delegate to notify of MobileDevice action's
 */
@property id<MobileDeviceManagerDelegate> delegate;

@end
