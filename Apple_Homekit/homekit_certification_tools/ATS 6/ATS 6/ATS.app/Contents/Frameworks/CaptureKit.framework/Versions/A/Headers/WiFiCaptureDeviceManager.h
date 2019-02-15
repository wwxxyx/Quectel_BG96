//
//  WiFiCaptureDeviceManager.h
//  ATSMacApp
//
//  Created by Lawrence Kwak on 4/30/18.
//  Copyright © 2018 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#import <CaptureKit/CaptureDeviceManagerProtocol.h>

@class ATSWiFiInterfaceCaptureDevice;

@interface WiFiCaptureDeviceManager : NSObject<CaptureDeviceManagerProtocol>

/*!
 Implements singleton pattern and returns a single instance of WiFiCaptureDeviceManager.
 */
+ (WiFiCaptureDeviceManager *)sharedManager;

/**
 Property that checks whether or not rvictl is installed in the macOS
 */
@property (nonatomic, readonly) BOOL isRVICTLInstalled;

/**
 Method to prompt users to install MobileDeviceDevelopment.pkg that in turn installs rvictl that's required for Wi-Fi captures.

 @param window Window (if not nil) to show the alert sheet on.
 @param wifiCaptureDevice The capture device that is being used to attempt to start capture on; Can be nil; used to check if the capture device is ECID based on Sierra to throw a different alert.
 @param completionHandler Completion handler that is invoked with the the alert is dismissed.
 */
- (void)showRVICTLInstallNeededSheetInWindow:(NSWindow *)window forCaptureDevice:(ATSWiFiInterfaceCaptureDevice *)wifiCaptureDevice completionHandler:(void (^)(void))completionHandler;
    
@end
