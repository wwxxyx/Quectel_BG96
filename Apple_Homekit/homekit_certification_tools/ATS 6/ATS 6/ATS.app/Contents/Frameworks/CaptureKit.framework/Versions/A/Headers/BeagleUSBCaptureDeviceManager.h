//
//  BeagleUSBCaptureDeviceManager.h
//  ATS
//
//  Created by Joe Basu on 2/23/09.
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <Foundation/Foundation.h>
#import <CaptureKit/CaptureDeviceManagerProtocol.h>


@interface BeagleUSBCaptureDeviceManager : NSObject<CaptureDeviceManagerProtocol>

/**
 List of available Beagle devices currently plugged-in.
 
 @note The capture devices are added/removed from the array if the Beagle was disconnected from the Mac or if the Beagle was used to capture. The array is an instance of BeagleUSBCaptureDevice which is a subclass of CaptureDevice.
 */
@property (nonatomic, readonly) NSMutableArray<CaptureDevice *> *availableDevices;

+ (BeagleUSBCaptureDeviceManager *)sharedManager;

@end
