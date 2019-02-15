//
//  ATSSmartConnectorBoxCaptureDeviceManager.h
//  ATSMacApp
//
//  Created by Vinod Madigeri on 8/9/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <CaptureKit/ATFR3CaptureDeviceManager.h>

@interface ATSSmartConnectorBoxCaptureDeviceManager : ATFR3CaptureDeviceManager

/**
 Get a singleton instance of ATS Smart Connector Box capture device manager.
 */
+ (instancetype)sharedManager;

@end
