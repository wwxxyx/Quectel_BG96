//
//  BSUSBHIDReportFieldUsage.h
//  BusSniffer
//
//  Created by Kip Nicol on 2/25/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BSUSBHIDReportFieldUsageBase.h"

/*!
 @interface BSUSBHIDReportFieldUsage
 @abstract  Represents a single usage or ranged usage value for a HID report.
 */
@interface BSUSBHIDReportFieldUsage : BSUSBHIDReportFieldUsageBase<NSFastEnumeration>

/*!
 @property  usage
 @abstract  The usage represented by the object. Only applicable if
            isRange returns NO.
 */
@property (nonatomic, readonly, assign) UInt16 usage;

/*!
 @property  name
 @abstract  The name of the usage.
 */
@property (nonatomic, readonly) NSString *name;

/*!
 @method    usageWithUsagePage:usage:
 @abstract  Returns an instance of BSUSBHIDReportFieldUsage initialized to the given values.
 @param     usagePage
            The usage page associated with the usage.
 @param     usage
            The usage represented by the object.
 */
+ (instancetype)usageWithUsagePage:(UInt16)usagePage usage:(UInt16)usage;

/*!
 @method    initWithUsagePage:usage:
 @abstract  Returns an instance of BSUSBHIDReportFieldUsage initialized to the given values.
 @param     usagePage
            The usage page associated with the usage.
 @param     usage
            The usage represented by the object.
 */
- (instancetype)initWithUsagePage:(UInt16)usagePage usage:(UInt16)usage;

@end
