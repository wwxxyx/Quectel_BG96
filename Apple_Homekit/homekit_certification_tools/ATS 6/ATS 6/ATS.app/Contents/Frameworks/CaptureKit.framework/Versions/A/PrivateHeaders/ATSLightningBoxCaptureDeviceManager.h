//
//  ATSLightningBoxCaptureDeviceManager.h
//  ATSMacApp
//
//  Created by Vinod Madigeri on 8/10/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <CaptureKit/ATFR3CaptureDeviceManager.h>

@interface ATSLightningBoxCaptureDeviceManager : ATFR3CaptureDeviceManager

/**
 Get a singleton instance of ATS Lightning Box capture device manager.
 */
+ (instancetype)sharedManager;

@end
