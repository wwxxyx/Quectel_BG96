//
//  TUSBTransportDataModel.h
//  ATSMacApp
//
//  Created by Carlos Frederico Azevedo on 4/14/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CaptureKit/TransportDataModel.h>
#import <CaptureKit/CaptureConfigurationTypes.h>

@class BeagleUSBCaptureDevice;
@class CaptureConfiguration;

/*!
 @class      USBTransportDataModel
 @abstract   This class is a concrete instance of the TransportDataModel abstract class.
             A USB transport also requires an instance of a BeagleUSBCaptureDevice so it can be
             used for testing.
 */
@interface USBTransportDataModel : TransportDataModel {
    BOOL _isUSBHost;
}

/*!
 @property  isUSBHost
 @abstract  Flag indicating whether this transport is AppleDeviceIsUSBHost(YES) or 
            AppleDeviceIsUSBDevice(NO).
 */
@property (nonatomic) BOOL isUSBHost;

@end
