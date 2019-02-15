//
//  ATSBoxCaptureDeviceManager.h
//  ATS
//
//  Created by Joe Basu on 1/5/10.
//  Copyright © 2010 Apple Inc. All Rights Reserved.
//

#import <Foundation/Foundation.h>

@class IORegistryDeviceNotifier;

@interface ATSBoxCaptureDeviceManager : NSObject {
    NSMutableArray *_attachedDevices;
    NSPredicate *_connectedPredicate;

    IORegistryDeviceNotifier *_serialDeviceNotifier;
}

@property (nonatomic, copy, readonly) NSArray *availableDevices;

+ (ATSBoxCaptureDeviceManager *)sharedManager;

@end
