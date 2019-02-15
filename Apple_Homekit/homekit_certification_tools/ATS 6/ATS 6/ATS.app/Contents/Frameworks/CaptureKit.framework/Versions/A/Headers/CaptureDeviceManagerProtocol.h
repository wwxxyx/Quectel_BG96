//
//  CaptureDeviceManagerProtocol.h
//  ATSMacApp
//
//  Created by Lawrence Kwak on 4/30/18.
//  Copyright © 2018 Apple Inc. All rights reserved.
//

#ifndef CaptureDeviceManagerProtocol_h
#define CaptureDeviceManagerProtocol_h

#import <CaptureKit/CaptureDevice.h>

@protocol CaptureDeviceManagerProtocol <NSObject>

/*!
 An array of available devices that are not currently connected/capturing
 button.
 */
@property (nonatomic, readonly) NSMutableArray<CaptureDevice*> *availableDevices;

/*!
 An array of all attached devices regardless of their connected/capturing state
 button.
 */
@property (nonatomic, readonly) NSMutableArray<CaptureDevice*> *attachedDevices;


/**
 Returns if this CaptureDeviceManager is ready.
 
 @param errors An array of errors of reasons why it is not ready.
 */
- (BOOL)isReadyForCapture:(NSMutableArray<NSError*> *)errors;


@end

#endif /* CaptureDeviceManagerProtocol_h */
