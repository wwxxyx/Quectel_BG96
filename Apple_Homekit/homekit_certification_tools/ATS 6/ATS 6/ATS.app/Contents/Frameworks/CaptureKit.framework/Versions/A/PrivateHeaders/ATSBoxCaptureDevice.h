//
//  ATSBoxCaptureDevice.h
//  ATS
//
//  Created by Joe Basu on 1/5/10.
//  Copyright © 2010 Apple Inc. All Rights Reserved.
//

#import <CaptureKit/CaptureDevice+Protected.h>
#import <ATSBoxSupport/ATSBoxSupport.h>

@class ATSBoxCaptureDevice;

@protocol ATSBoxCaptureDeviceDelegate <NSObject>

@required
- (NSArray *)atsBoxCaptureDevice:(ATSBoxCaptureDevice *)device sawATFData:(NSData *)atfData;
- (void)atsBoxCaptureDeviceDidBecomeInvalid:(ATSBoxCaptureDevice *)device;

@end

@interface ATSBoxCaptureDevice : CaptureDevice

@property (nonatomic, assign) id<ATSBoxCaptureDeviceDelegate> delegate;

@end
