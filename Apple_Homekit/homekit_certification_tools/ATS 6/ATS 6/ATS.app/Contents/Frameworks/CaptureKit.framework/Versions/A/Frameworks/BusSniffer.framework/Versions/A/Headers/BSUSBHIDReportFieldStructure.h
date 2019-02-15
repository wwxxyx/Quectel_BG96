//
//  BSUSBHIDReportFieldStructure.h
//  BusSniffer
//
//  Created by Kip Nicol on 2/25/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@class BSUSBHIDReportStructure;
@class BSUSBHIDReportDescriptorParserGlobals;
@class BSUSBHIDReportDescriptorParserLocals;
@class BSUSBHIDReportFieldUsageBase;

/*!
 @interface BSUSBHIDReportFieldStructure
 @abstract  Represents the structure of a subset of contiguous fields in a HID report.
 */
@interface BSUSBHIDReportFieldStructure : NSObject

/*!
 @property  usages
 @abstract  Array of ordered BSUSBHIDReportFieldUsageBase's that are associated with the field structure.
 */
@property (nonatomic, readonly, retain) NSArray<BSUSBHIDReportFieldUsageBase *> *usages;

/*!
 @property  reportStructure
 @abstract  The report structure the field structure is associated with.
 @discussion Property is "assign" because the reportStructure will already retain it
             and we don't want a circular reference.
 */
@property (nonatomic, readonly, assign) BSUSBHIDReportStructure *reportStructure;

/*!
 @property  fieldSize
 @abstract  Conceptually, the number of bits representable by the field.
 @discussion
            Array fields can only represent on or off for a usage. Therefore,
            their fieldSize is only 1 bit. Variable fields will be equal to the
            reportSize of the field. This property should be used when the calling
            code needs to know how many bits the HID control has in its field.
 */
@property (nonatomic, readonly) UInt32 fieldSize;

/*!
 @property  reportSize
 @abstract  Conceptually, the minimum value that the field can represent.
 @discussion
            Array fields can only represent on or off for a usage. Therefore,
            their min value is 0. Variable fields will be equal to the
            logicalMin of the field. This property should be used when the calling
            code needs to know min value a usage the field can represent.
 */
@property (nonatomic, readonly) SInt32 fieldValueMin;

/*!
 @property  reportSize
 @abstract  Conceptually, the maximum value that the field can represent.
 @discussion
            Array fields can only represent on or off for a usage. Therefore,
            their max value is 1. Variable fields will be equal to the
            logicalMax of the field. This property should be used when the calling
            code needs to know max value a usage in the field can represent.
 */
@property (nonatomic, readonly) SInt32 fieldValueMax;

/*!
 @property  logicalMin
 @abstract  The logicalMin value associated with the field structure.
 */
@property (nonatomic, readonly, assign) SInt32 logicalMin;

/*!
 @property  logicalMax
 @abstract  The logicalMax value associated with the field structure.
 */
@property (nonatomic, readonly, assign) SInt32 logicalMax;

/*!
 @property  physicalMin
 @abstract  The physicalMin value associated with the field structure.
 */
@property (nonatomic, readonly, assign) SInt32 physicalMin;

/*!
 @property  physicalMax
 @abstract  The physicalMax value associated with the field structure.
 */
@property (nonatomic, readonly, assign) SInt32 physicalMax;

/*!
 @property  unitExponent
 @abstract  The unitExponent value associated with the field structure.
 */
@property (nonatomic, readonly, assign) SInt32 unitExponent;

/*!
 @property  unit
 @abstract  The unit value associated with the field structure.
 */
@property (nonatomic, readonly, assign) UInt32 unit;

/*!
 @property  reportSize
 @abstract  The reportSize associated with the field structure.
 */
@property (nonatomic, readonly, assign) UInt32 reportSize;

/*!
 @property  reportCount
 @abstract  The reportCount associated with the field structure.
 */
@property (nonatomic, readonly, assign) UInt32 reportCount;

/*!
 @property  usageCount
 @abstract  The count of all of the usages defined in the report field.
 @discussion Non-range usages add 1 to the count, while ranged usages
             add the total number of usages in their range to the count.
 */
@property (nonatomic, readonly) NSUInteger usageCount;

/*!
 @property  fieldsBitLength
 @abstract  The total number of bits in all of the fields defined in the report field.
 */
@property (nonatomic, readonly) NSUInteger fieldsBitLength;

/*!
 @property  fieldSigned
 @abstract  Returns YES if the fields values should be represented as a signed value.
 */
@property (nonatomic, readonly, getter = isFieldSigned) BOOL fieldSigned;

/*!
 @property  padding
 @abstract  Returns YES if the field structure represents padding fields (ie no usages).
 */
@property (nonatomic, readonly, getter = isPadding) BOOL padding;

/*!
 @property  arrayAttribute
 @abstract  Returns YES if the field structure has the Array attribute. If NO is returned
            the field structure has the Variable attribute.
 */
@property (nonatomic, readonly, getter = hasArrayAttribute) BOOL arrayAttribute;

/*!
 @property  dataAttribute
 @abstract  Returns YES if the field structure has the Data attribute. If NO is returned
            the field structure has the Constant attribute.
 */
@property (nonatomic, readonly, getter = hasDataAttribute) BOOL dataAttribute;

/*!
 @property  hasBitfieldAttribute
 @abstract  Returns YES if the field structure has the Bitfield attribute. If NO is returned
            the field structure has the Buffered Bytes attribute.
 */
@property (nonatomic, readonly) BOOL hasBitfieldAttribute;

/*!
 @method    +hidReportFieldStructureWithAttributesBitmask:currentGlobals:currentLocals:
 @abstract  Creates and returns a hid report field structure with the given parameters.
 @param     attributes
            The data contained in the Input/Output/Feature item of the HID report descriptor.
 @param     currentGlobals
            The object representing the state table for the current set of globals during parsing.
 @param     currentLocals
            The object representing the state table for the current set of locals during parsing.
 */
+ (instancetype)hidReportFieldStructureWithAttributesBitmask:(UInt32)attributes
                                              currentGlobals:(BSUSBHIDReportDescriptorParserGlobals *)globals
                                               currentLocals:(BSUSBHIDReportDescriptorParserLocals *)locals;

/*!
 @method    +hidReportFieldStructureWithAttributesBitmask:currentGlobals:currentLocals:
 @abstract  Initializes a newly allocated HID report field structure with the given parameters.
 @param     attributes
            The data contained in the Input/Output/Feature item of the HID report descriptor.
 @param     currentGlobals
            The object representing the state table for the current set of globals during parsing.
 @param     currentLocals
            The object representing the state table for the current set of locals during parsing.
 */
- (instancetype)initWithAttributesBitmask:(UInt32)attributes
                           currentGlobals:(BSUSBHIDReportDescriptorParserGlobals *)globals
                            currentLocals:(BSUSBHIDReportDescriptorParserLocals *)locals;

/*!
 @property  -getUsagePage:usage:fromUsagesAtIndex:
 @abstract  Upon successful return, usagePage and usage are set to the values associated
 with the given index.
 @param     usagePage
 Upon successful return, the usage page.
 @param     usage
 Upon successful return, the usage.
 @param     usageIndex
 The index of the usage in the range of usages.
 @discussion Searches the usages array looking for a usage range or usage value which
 contains the usageIndex.
 @return    If a usage value is found, YES is returned. If no usage at the given index
 is found, then NO is returned.
 */
- (BOOL)getUsagePage:(UInt16 *)usagePage usage:(UInt16 *)usage fromUsagesAtIndex:(NSUInteger)usageIndex;

/*!
 @property  containsUsagePage:usageID:
 @abstract  Returns whether the usage with the given usage ID and page has been declared in the structure.
 @param     usagePage
            The usage Page.
 @param     usageID
            The usage ID.
 */
- (BOOL)containsUsagePage:(UInt16)usagePage usageID:(UInt16)usageID;

/*!
 @property  usageCountForUsagePage:
 @abstract  Returns the number of usages that contains the given usage page.
 @param     usagePage
 The usage Page.
 */
- (NSUInteger)usageCountForUsagePage:(UInt16)usagePage;

@end
