//
//  CaptureKitManager.h
//  ATSMacApp
//
//  Created by Vinod Madigeri on 3/1/18.
//  Copyright © 2018 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface CaptureKitManager : NSObject

/**
 Method to initialize the Capture device managers and register the required defaults (and potentially any other setup that CaptureKit needs before being ready to be used).
 
 @note This method internally wraps all the initialization inside of dispatch_once so the code only executes once.
 */
+ (void)setup;

@end
