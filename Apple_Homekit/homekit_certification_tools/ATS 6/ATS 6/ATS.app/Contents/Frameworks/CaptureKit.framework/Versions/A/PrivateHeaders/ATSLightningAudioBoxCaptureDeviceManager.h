//
//  ATSLightningAudioBoxCaptureDeviceManager.h
//  ATSMacApp
//
//  Created by Vinod Madigeri on 10/16/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <CaptureKit/ATSSerialCaptureDeviceManager.h>

@interface ATSLightningAudioBoxCaptureDeviceManager : ATSSerialCaptureDeviceManager

/**
 Get a singleton instance of ATS Lightning Box capture device manager.
 */
+ (instancetype)sharedManager;

@end
