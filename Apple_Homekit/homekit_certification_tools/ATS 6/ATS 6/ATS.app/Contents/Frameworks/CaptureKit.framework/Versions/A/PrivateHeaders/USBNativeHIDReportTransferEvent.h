//
//  USBNativeHIDReportTransferEvent.h
//  ATSMacApp
//
//  Created by Kip Nicol on 3/16/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/USBTransferEvent.h>

#define USB_NATIVE_HID_REPORT_TRANSFER_CLASSIFICATION(reportType, ...) CONSTRUCT_ACTIVATOR_WITH_EVENT(USBNativeHIDReportTransferEvent, @(reportType), ##__VA_ARGS__, YES)

/*!
 @class     USBNativeHIDReportTransferEvent
 @abstract  Represents a HID report transfer sent for the HID over Native USB feature
            in iAP2. Represents HID reports sent over both Control and Interrupt.
 */
@interface USBNativeHIDReportTransferEvent : USBTransferEvent

/*!
 @property  hidReportFieldValues
 @abstract  The BSUSBHIDReportFieldValues for the HID report.
 */
@property (nonatomic, readonly, copy) NSArray *hidReportFieldValues;

/*!
 @property  hidReportType
 @abstract  The HID report type of the field values.
 */
@property (nonatomic, readonly) BSUSBHIDReportType hidReportType;

/*!
 @method    +nativeHIDReportTransferEventWithTransfer:hidReportFieldValues:
 @abstract  Creates and returns a USBNativeHIDReportTransferEvent with the given params.
 @param     transfer
            The transfer object assocaited with the event.
 @param     hidReportFieldValues
            The HID report field values processed from the HID report data.
 */
+ (instancetype)nativeHIDReportTransferEventWithTransfer:(BSUSBTransfer *)transfer
                                    hidReportFieldValues:(NSArray *)hidReportFieldValues;

@end
