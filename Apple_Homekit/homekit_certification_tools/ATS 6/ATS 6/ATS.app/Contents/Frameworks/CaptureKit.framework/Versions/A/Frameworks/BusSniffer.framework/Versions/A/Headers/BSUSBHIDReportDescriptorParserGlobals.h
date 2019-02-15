//
//  BSUSBHIDDescriptorParserGlobals.h
//  BusSniffer
//
//  Created by Kip Nicol on 2/25/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@class BSUSBHIDReportDescriptorShortItem;

/*!
 @interface BSUSBHIDReportDescriptorParserGlobals
 @abstract  Represents the current set of global values set during 
            HID report descriptor parsing.
 */
@interface BSUSBHIDReportDescriptorParserGlobals : NSObject <NSCopying>

/*!
 @property  usagePage
 @abstract  The last usage page set during HID report parsing.
 */
@property (nonatomic, assign) UInt16 usagePage;

/*!
 @property  logicalMin
 @abstract  The last logicalMin value set during HID report parsing.
 */
@property (nonatomic, assign) SInt32 logicalMin;

/*!
 @property  logicalMax
 @abstract  The last logicalMax value set during HID report parsing.
 */
@property (nonatomic, assign) SInt32 logicalMax;

/*!
 @property  physicalMin
 @abstract  The last physicalMin value set during HID report parsing.
 */
@property (nonatomic, assign) SInt32 physicalMin;

/*!
 @property  physicalMax
 @abstract  The last physicalMax value set during HID report parsing.
 */
@property (nonatomic, assign) SInt32 physicalMax;

/*!
 @property  unitExponent
 @abstract  The last unitExponent value set during HID report parsing.
 */
@property (nonatomic, assign) SInt32 unitExponent;

/*!
 @property  unit
 @abstract  The last unit value set during HID report parsing.
 */
@property (nonatomic, assign) UInt32 unit;

/*!
 @property  reportSize
 @abstract  The last reportSize set during HID report parsing.
 */
@property (nonatomic, assign) UInt32 reportSize;

/*!
 @property  reportCount
 @abstract  The last reportCount set during HID report parsing.
 */
@property (nonatomic, assign) UInt32 reportCount;

/*!
 @property  reportID
 @abstract  The last reportID set during HID report parsing.
 */
@property (nonatomic, assign) UInt8 reportID;

/*!
 @property  sawUsagePageItem
 @abstract  Whether the Usage Page item was seen during processing.
 */
@property (nonatomic, readonly) BOOL sawUsagePageItem;

/*!
 @property  sawLogicalMinItem
 @abstract  Whether the Usage Page item was seen during processing.
 */
@property (nonatomic, readonly) BOOL sawLogicalMinItem;

/*!
 @property  sawLogicalMaxItem
 @abstract  Whether the Logical Max item was seen during processing.
 */
@property (nonatomic, readonly) BOOL sawLogicalMaxItem;

/*!
 @property  sawReportSizeItem
 @abstract  Whether the Report Size item was seen during processing.
 */
@property (nonatomic, readonly) BOOL sawReportSizeItem;

/*!
 @property  sawReportCountItem
 @abstract  Whether the Report Count item was seen during processing.
 */
@property (nonatomic, readonly) BOOL sawReportCountItem;

/*!
 @property  globals
 @abstract  Creates and returns a globals object with properties initalized
            to their default states.
 */
+ (instancetype)globals;

@end
