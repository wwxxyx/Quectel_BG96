//
//  USBDeviceRequestHIDReportDescriptorEvent.h
//  ATSMacApp
//
//  Created by Kip Nicol on 1/15/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/USBTransferEvent.h>
#import <BusSniffer/BusSniffer.h>

/*!
 @class     USBNativeHIDReportDescriptorTransferEvent
 @abstract  Represents a HID report descriptor transfer sent for the HID over Native USB feature
            in iAP2.
 */
@interface USBNativeHIDReportDescriptorTransferEvent : USBTransferEvent

/*!
 @property  hidReportDescriptor
 @abstract  The USB HID report descriptor.
*/
@property (nonatomic, readonly, retain) BSUSBHIDReportDescriptor *hidReportDescriptor;

/*!
 @method    +deviceRequestHIDReportDescriptorEventWithTimestamp:usbDeviceRequestInfo:usbHIDReportDescriptor:
 @abstract  Returns the event object initialized with the given values.
 @param     transfer
            The transfer object.
 @param     hidReportDescriptor
            The BSUSBHIDReportDescriptor object for the HID report descriptor.
*/
+ (instancetype)nativeHIDReportDescriptorTransferEventWithTransfer:(BSUSBTransfer *)transfer
                                               hidReportDescriptor:(BSUSBHIDReportDescriptor *)hidReportDescriptor;

/*!
 @method    -initWithTimestamp:usbDeviceRequestInfo:usbHIDReportDescriptor:
 @abstract  Initializes and returns the event object with the given values.
 @param     transfer
            The transfer object.
 @param     hidReportDescriptor
            The BSUSBHIDReportDescriptor object for the HID report descriptor.
 */
- (instancetype)initWithTransfer:(BSUSBTransfer *)transfer
             hidReportDescriptor:(BSUSBHIDReportDescriptor *)hidReportDescriptor;

@end
