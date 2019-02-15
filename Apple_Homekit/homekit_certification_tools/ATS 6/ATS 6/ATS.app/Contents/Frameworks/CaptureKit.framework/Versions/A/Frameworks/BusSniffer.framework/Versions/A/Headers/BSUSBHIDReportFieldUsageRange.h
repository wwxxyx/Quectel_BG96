//
//  BSUSBHIDReportFieldUsageRange.h
//  BusSniffer
//
//  Created by Cody Brimhall on 3/20/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

/*!
 @header     BSUSBHIDReportFieldUsageRange
 @discussion This header file describes the interface for the class that 
             represents a continuous range of HID usages.
 */

#import <Foundation/Foundation.h>
#import "BSUSBHIDReportFieldUsageBase.h"

@class BSUSBHIDReportFieldUsage;

/*!
 @class      BSUSBHIDReportFieldUsageRange
 @abstract   BSUSBHIDReportFieldUsageRange represents a contiguous range of HID
             usages, as defined with the Usage Min and Usage Max HID report 
             descriptor items.
 */
@interface BSUSBHIDReportFieldUsageRange : BSUSBHIDReportFieldUsageBase <NSFastEnumeration>

/*!
 @property   usageMin
 @abstract   The lowest usage value represented in the range.
 */
@property (nonatomic, readonly) UInt16 usageMin;

/*!
 @property   usageMax
 @abstract   The highest usage value represented in the range.
 */
@property (nonatomic, readonly) UInt16 usageMax;

/*!
 @property   length
 @abstract   The number of usages represented in the range.
 */
@property (nonatomic, readonly) NSUInteger length;

/*!
 @method     usageRangeWithUsagePage:usageMin:usageMax:
 @abstract   Creates and returns a new usage range.
 @param      usagePage
             The usage page associated with the usage range.
 @param      usageMin
             The lowest usage value represented by the range.
 @param      usageMax
             The highest usage value represented by the range.
 @return     A new usage range.
 */
+ (instancetype)usageRangeWithUsagePage:(UInt16)usagePage usageMin:(UInt16)usageMin usageMax:(UInt16)usageMax;

/*!
 @method     initWithUsagePage:usageMin:usageMax:
 @abstract   Creates and returns a new usage range.
 @param      usagePage
             The usage page associated with the usage range.
 @param      usageMin
             The lowest usage value represented by the range.
 @param      usageMax
             The highest usage value represented by the range.
 @return     A new usage range.
 @discussion This is the designated initializer.
 */
- (instancetype)initWithUsagePage:(UInt16)usagePage usageMin:(UInt16)usageMin usageMax:(UInt16)usageMax;

/*!
 @method     allUsages
 @abstract   Returns the collection of individual usages represented by the 
             usage range.
 @return     A collection of individual usages.
 */
- (NSArray<BSUSBHIDReportFieldUsage *> *)allUsages;

/*!
 @method     includesUsage:
 @abstract   Returns a flag indicating whether or not the given usage is 
             included in the range.
 @param      usage
             The usage to check for.
 @return     YES if the usage is included in the range; NO otherwise.
 */
- (BOOL)includesUsage:(BSUSBHIDReportFieldUsage *)usage;

@end
