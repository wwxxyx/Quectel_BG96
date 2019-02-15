//
//  BSErrorDomain.h
//  BusSniffer
//
//  Created by Kip Nicol on 3/4/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

/** The error domain string. */
extern NSString * const BSUSBHIDReportDescriptorErrorDomain;

/** Defines error codes for errors found while parsing HID report descriptors. */
typedef NS_ENUM(NSInteger, BSUSBHIDReportDescriptorError) {
    BSUSBHIDReportDescriptorErrorInvalidItem = 1,
    BSUSBHIDReportDescriptorErrorReportIDAfterInputOutputFeatureItem,
    BSUSBHIDReportDescriptorErrorReportCountUsageCountNotEqual,
    BSUSBHIDReportDescriptorErrorArrayLogicalMinMaxRangeLengthUsageCountNotEqual,
    BSUSBHIDReportDescriptorErrorPaddingNotConstant,
    BSUSBHIDReportDescriptorErrorReportIDDataGreaterThanOneByte,
    BSUSBHIDReportDescriptorErrorReportIDReserved,
    BSUSBHIDReportDescriptorErrorLogicalMinGreaterThanLogicalMax,
    BSUSBHIDReportDescriptorErrorUsagePageDataGreaterThanTwoBytes,
    BSUSBHIDReportDescriptorErrorUsageMinMaxExtendedMismatch,
    BSUSBHIDReportDescriptorErrorUsageMinMaxUsagePageMismatch,
    BSUSBHIDReportDescriptorErrorUsageMinGreaterThanUsageMax,
    BSUSBHIDReportDescriptorErrorPopNoCorrespondingPush,
    BSUSBHIDReportDescriptorErrorReportFieldSpanGreaterThanFourBytes,
    BSUSBHIDReportDescriptorErrorTopLevelCollectionNotDefined,
    BSUSBHIDReportDescriptorErrorTopLevelCollectionNotApplication,
    BSUSBHIDReportDescriptorErrorCollectionNoUsage,
    BSUSBHIDReportDescriptorErrorReportSpanGreaterThanOneTopLevelCollection,
    BSUSBHIDReportDescriptorErrorMissingRequiredItems,
    BSUSBHIDReportDescriptorUsagePageUndefined,
    BSUSBHIDReportDescriptorErrorUsageIDUndefined,
    BSUSBHIDReportDescriptorReportNotByteAligned,
};

/** The error domain string. */
extern NSString * const BSUSBHIDReportErrorDomain;

/** Defines error codes for errors found while parsing HID reports. */
typedef NS_ENUM(NSInteger, BSUSBHIDReportError) {
    BSUSBHIDReportErrorUndefinedHIDReport = 1,
    BSUSBHIDReportErrorInvalidDataLength,
    BSUSBHIDReportErrorDescriptorUndefined,
    BSUSBHIDReportErrorInvalidReportID,
};

/** The error domain string for device descriptor errors. */
extern NSString * const BSUSBDeviceDescriptorErrorDomain;

/** Defines error codes for errors found while parsing USB device descriptors. */
typedef NS_ENUM(NSInteger, BSUSBDeviceDescriptorError) {
    BSUSBDeviceDescriptorErrorInvalidLength = 1,
};

extern NSString * const BSPowerDeliveryErrorDomain;

typedef NS_ENUM(NSInteger, BSPowerDeliveryError) {
    BSPowerDeliveryErrorInvalidPowerDataObject = 1,
    BSPowerDeliveryErrorInvalidRequestDataObject,
    BSPowerDeliveryErrorUndefinedCapabilities,
};

/** The error domain string. */
extern NSString * const BSExternalAccessoryErrorDomain;

/** Defines error codes for errors found for EA packets. */
typedef NS_ENUM(NSUInteger, BSExternalAccessoryError) {
    BSExternalAccessoryErrorInvalidPacket = 1,
    BSExternalAccessoryErrorExpectedPayload = 2,
};
