//
//  BSUSBHIDReportFieldValue.h
//  BusSniffer
//
//  Created by Kip Nicol on 3/2/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@class BSUSBHIDReportFieldUsage;
@class BSUSBHIDReportFieldStructure;


/*!
 @interface BSUSBHIDReportFieldValue
 @abstract  Represents the value of a specific field in a HID report.
 */
@interface BSUSBHIDReportFieldValue : NSObject

/*!
 @property  value
 @abstract  Returns the value represented by the field.
 @discussion
            For Variable fields and padding, this value will return the signed value of the field.
            For non-padding Array fields, the return value will be 1 if the field represents a valid usage.
 */
@property (nonatomic, readonly, assign) SInt64 value;

/*!
 @property  fieldIndex
 @abstract  The index of the field with respect to the reportFieldStructure's fields.
 */
@property (nonatomic, readonly, assign) UInt32 fieldIndex;

/*!
 @property  reportFieldStructure
 @abstract  The associated report structure the field resides in.
 */
@property (nonatomic, readonly, retain) BSUSBHIDReportFieldStructure *reportFieldStructure;

/*!
 @property  reportFieldUsage
 @abstract  The specific usage associated with the value, as determined by the 
            associated report structure.
 */
@property (nonatomic, readonly) BSUSBHIDReportFieldUsage *reportFieldUsage;

/*!
 @property  isNoControlsAsserted
 @abstract  Returns whether the value for an Array field is considered 
            "no controls asserted" (ie no button was pressed).
 @discussion Only applicable if reportFieldStructure.hasArrayAttribute returns YES.
             See <rdar://problem/13600961>.
 */
@property (nonatomic, readonly) BOOL isNoControlsAsserted;

/*!
 @method    +reportFieldValueWithUnsignedValue:fieldIndex:reportFieldStructure
 @abstract  Creates and returns an object initialized to the given parameters.
 @param     unsignedValue
            The unsigned value of the field in the report.
 @param     fieldIndex
            The index of the field with respect to the reportFieldStructure's fields.
 @param     reportFieldStructure
            The associated report field structure the field resides in.
 */
+ (instancetype)reportFieldValueWithUnsignedValue:(UInt32)unsignedValue
                                       fieldIndex:(UInt32)fieldIndex
                             reportFieldStructure:(BSUSBHIDReportFieldStructure *)reportFieldStructure;

/*!
 @method    -initWithUnsignedValue:fieldIndex:reportFieldStructure
 @abstract  Initializes an object to the given parameters.
 @param     unsignedValue
            The unsigned value of the field in the report.
 @param     fieldIndex
            The index of the field with respect to the reportFieldStructure's fields.
 @param     reportFieldStructure
            The associated report field structure the field resides in.
 */
- (instancetype)initWithUnsignedValue:(UInt32)unsignedValue
                           fieldIndex:(UInt32)fieldIndex
                 reportFieldStructure:(BSUSBHIDReportFieldStructure *)reportFieldStructure;

@end
