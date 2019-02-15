//
//  HIDCaptureProtocol.h
//  ATSMacApp
//
//  Created by Zachary Church on 3/14/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <CaptureKit/ATSCaptureProtocol.h>
#import <BusSniffer/BSUSBHIDTypes.h>
#import <BusSniffer/BSiAPTypes.h>

@class HIDReportDescriptorEvent;
@class HIDReportEvent;

@interface HIDCaptureProtocol : ATSCaptureProtocol

@property (nonatomic, readonly, strong) NSString *captureProtocolName;

/**
 "HID host" refers to the device that is the USB host when using HID over USB. This role accepts a report descriptor from the "HID device" and uses it to parse HID reports.
 
 "interfaceIdentifier" is any object that uniquely identifies a HID device. This is a transport-specific concept; AID uses endpoint numbers and CarPlay uses UUIDs to identify HID "interfaces"
 */

#pragma mark - HID Report descriptors
/**
 Indicates the HID host requests a Report Descriptor from the HID device.
 @param interfaceIdentifier an object that uniquely identifies a HID interface.
 @param timestamp
 */
- (void)processGetReportDescriptorForInterface:(id)interfaceIdentifier atTimestamp:(UInt64)timestamp;

/**
 Indicates the HID device provides a HID Report Descriptor to the HID host.
 @param interfaceIdentifier an object that uniquely identifies a HID interface.
 @param timestamp
 */
- (void)processReportDescriptorData:(NSData *)data forInterface:(id)interfaceIdentifier atTimestamp:(UInt64)timestamp;

#pragma mark - HID Reports
/**
 Use this version of processReportData when the HID report message includes a reportType somewhere (it's out-of-band, probably in a header or something).
 @param interfaceIdentifier an object that uniquely identifies a HID interface.
 @param timestamp
 */
- (void)processReportData:(NSData *)reportData ofType:(BSUSBHIDReportType)reportType forInterface:(id)interfaceIdentifier fromSource:(BSiAPSource)source atTimestamp:(UInt64)timestamp;

/**
 Indicates a GetHIDReport command happened.
 @param interfaceIdentifier an object that uniquely identifies a HID interface.
 @param timestamp
 */
- (void)processGetHIDReportOfType:(BSUSBHIDReportType)reportType forReportID:(UInt8)reportID forInterface:(id)interfaceIdentifier atTimestamp:(UInt64)timestamp;

/**
 Use this version of processReportData when the reportType is defined in the GetHIDReport request.
 @param interfaceIdentifier an object that uniquely identifies a HID interface.
 @param timestamp
 */
- (void)processReportData:(NSData *)reportData forInterface:(id)interfaceIdentifier fromSource:(BSiAPSource)source atTimestamp:(UInt64)timestamp;

/**
 Reset all protocol state
 */
- (void)reset;
@end
