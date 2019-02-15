//
//  BSUSBHIDDescriptorParserLocals.h
//  BusSniffer
//
//  Created by Kip Nicol on 2/25/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@class BSUSBHIDReportDescriptorShortItem;


/*!
 @interface BSUSBHIDReportDescriptorParserLocals
 @abstract  Represents the current set of local values set during
            HID report descriptor parsing.
 */
@interface BSUSBHIDReportDescriptorParserLocals : NSObject

/*!
 @property  usages
 @abstract  Array of BSUSBHIDReportFieldUsage's currently set in the local 
            scope when parsing HID report descriptors.
 */
@property (nonatomic, retain) NSMutableArray *usages;

/*!
 @property  usageMinItem
 @abstract  The last usageMin item seen in the local scope when parsing 
            HID report descriptors.
 */
@property (nonatomic, retain) BSUSBHIDReportDescriptorShortItem *usageMinItem;

/*!
 @property  usageMaxItem
 @abstract  The last usageMax item seen in the local scope when parsing
            HID report descriptors.
 */
@property (nonatomic, retain) BSUSBHIDReportDescriptorShortItem *usageMaxItem;

/*!
 @property  delimiterSet
 @abstract  Whether the Delimiter item has been set in the local scope when
            parsing HID report descriptors.
 */
@property (nonatomic, assign, getter = isDelimiterSet) BOOL delimiterSet;

/*!
 @property  seenFirstUsageInDelimiter
 @abstract  Whether we've processed a single usage when Delimiter is set when
            parsing HID report descriptors.
 */
@property (nonatomic, assign) BOOL seenFirstUsageInDelimiter;

/*!
 @property  locals
 @abstract  Creates and returns a lcoals object with properties initalized
            to their default states.
 */
+ (instancetype)locals;

@end
