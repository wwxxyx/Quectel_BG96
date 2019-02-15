//
//  BSHIDReport.h
//  BusSniffer
//
//  Created by Zachary Church on 3/16/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BSUSBHIDTypes.h"
#import "BSUSBHIDReportDescriptor.h"

@class BSUSBHIDReportFieldValue;

@interface BSHIDReport : NSObject

@property (nonatomic, readonly, strong) NSNumber *reportID;
@property (nonatomic, readonly, copy) NSData *data;
@property (nonatomic, readonly, copy) BSUSBHIDReportDescriptor *descriptor;
@property (nonatomic, readonly) UInt64 timestamp;
@property (nonatomic, readonly) BSUSBHIDReportType reportType;
@property (nonatomic, readonly) BSUSBHIDReportStatus status;
@property (nonatomic, readonly, copy) NSArray<BSUSBHIDReportFieldValue *> *fields;
@property (nonatomic, readonly, strong) NSError *error;

/**
 @property descriptorName
 Each transport has a unique way of identifying HID interfaces. This is a user-facing string that identifies the HID report descriptor that should be associated with this HID report.
 */
@property (nonatomic, readonly, copy) NSString *descriptorName;

+ (instancetype)reportWithBytes:(NSData *)data forReportType:(BSUSBHIDReportType)reportType forDescriptor:(BSUSBHIDReportDescriptor *)descriptor withName:(NSString *)descriptorName;

@end
