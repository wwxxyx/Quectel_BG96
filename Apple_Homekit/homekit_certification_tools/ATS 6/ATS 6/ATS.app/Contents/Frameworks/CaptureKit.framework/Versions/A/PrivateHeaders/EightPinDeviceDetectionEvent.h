//
//  EightPinDeviceDetectionEvent.h
//  ATSMacApp
//
//  Created by Cody Brimhall on 10/16/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

/*!
 @header     EightPinDeviceDetectionEvent
 @abstract   This file describes the interface for the EightPinHardwareEvent 
             that represents a device detection event.
 */

#import <CaptureKit/EightPinHardwareEvent.h>
#import <CaptureKit/ProductPlanTypes.h>
#import <ATSR3BoxSupport/ATSR3BoxSupport.h>

/*!
 @class      EightPinDeviceDetectionEvent
 @abstract   Encapsulates information about a ReportDeviceDetection ATF sent by 
             the Lightning Box.
 */
@interface EightPinDeviceDetectionEvent : EightPinHardwareEvent

/*!
 @property   deviceType
 @abstract   The Apple device type reported by the Lightning Box.
 */
@property (nonatomic) AppleDeviceType deviceType;

/*!
 @property   deviceDetectionStatus
 @abstract   The device detection status code reported by the Lightning Box.
 */
@property (nonatomic) ATFR3DeviceDetectionStatus deviceDetectionStatus;

@end
