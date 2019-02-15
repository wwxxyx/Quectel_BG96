//
//  CaptureDevice.h
//  ATS
//
//  Created by Joe Basu on 2/23/09.
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <Foundation/Foundation.h>

@interface CaptureDevice : NSObject

/**
 Capture device name.
 */
@property (nonatomic, readonly, copy) NSString *name;

/**
 Capture device serial number.
 */
@property (nonatomic, readonly, copy) NSString *serialNumber;

/**
 Boolean to track whether or not the capture device is being used to capture data.
 */
@property (nonatomic, readwrite) BOOL connected;

@end
