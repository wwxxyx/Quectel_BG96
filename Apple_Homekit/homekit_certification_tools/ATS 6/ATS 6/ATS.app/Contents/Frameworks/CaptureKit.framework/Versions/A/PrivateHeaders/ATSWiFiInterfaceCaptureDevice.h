//
//  ATSWiFiInterfaceCaptureDevice.h
//  ATSMacApp
//
//  Created by Cody Brimhall on 2/11/15.
//  Copyright (c) 2015 Apple Inc. All rights reserved.
//

#import <CaptureKit/ATSWiFiCaptureDevice.h>

@class MobileDevice;

/** ATSWiFiInterfaceCaptureDevice is an ATSWiFiCaptureDevice subclass that sniffs traffic from a network interface. */
@interface ATSWiFiInterfaceCaptureDevice : ATSWiFiCaptureDevice

/** The name of the network interface to sniff. */
@property (readwrite, copy) NSString *interfaceName;

/** Underlining mobile device used */
@property (readonly) MobileDevice *mobileDevice;

/** Whether or not this mobile device has a hyphen ('-') in its udid; which means it'd be ECID based (starting fall 2018 Apple devices) */
@property (nonatomic, readonly) BOOL isMobileDeviceECIDBased;

/**
 Instantiate a capture device with the underlining mobile device and interfaceName
 Note:  You must set the interfaceName before you begin capturing
 
 @param mobileDevice The mobile device that is the underlining capture device
 @param The name of the network interface to sniff (Optional but you must set this before begin capture)
 */
- (instancetype)initWithMobileDevice:(MobileDevice *)mobileDevice interfaceName:(NSString *)interfaceName;

@end
