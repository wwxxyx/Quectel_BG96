//
//  BSUSBHIDReportFieldUsageBase.h
//  BusSniffer
//
//  Created by Cody Brimhall on 3/22/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

/*!
 @header     BSUSBHIDReportFieldUsageBase
 @discussion This header file describes the interface for the abstract base 
             class for objects that model HID report field usages, namely 
             individual usages and usage ranges.
 */

#import <Foundation/Foundation.h>

/*!
 @class      BSUSBHIDReportFieldUsageBase
 @abstract   Abstract base class for objects representing types of HID report 
             field usages, such as individual usages and usage ranges.
 */
@interface BSUSBHIDReportFieldUsageBase : NSObject<NSFastEnumeration>

/*!
 @property   usagePage
 @abstract   The usage page that the usage is associated with.
 */
@property (nonatomic, readonly) UInt16 usagePage;

/*!
 @method     usageWithUsagePage:
 @abstract   Creates and returns a new field usage object with the given usage 
             page.
 @param      usagePage
             The usage page associated with the usage.
 @return     A new usage object.
 */
+ (instancetype)usageWithUsagePage:(UInt16)usagePage;

/*!
 @method     initWithUsagePage:
 @abstract   Creates and returns a new field usage object with the given usage
             page.
 @param      usagePage
             The usage page associated with the usage.
 @return     A new usage object.
 */
- (instancetype)initWithUsagePage:(UInt16)usagePage;

@end
