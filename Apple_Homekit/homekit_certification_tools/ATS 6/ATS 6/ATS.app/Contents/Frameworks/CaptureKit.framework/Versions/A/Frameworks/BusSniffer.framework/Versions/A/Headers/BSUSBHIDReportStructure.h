//
//  BSUSBHIDReportStructure.h
//  BusSniffer
//
//  Created by Kip Nicol on 2/25/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <BusSniffer/BSUSBHIDTypes.h>

@class BSUSBHIDReportFieldStructure;

/*!
 @interface BSUSBHIDReportStructure
 @abstract  Represents the structure of a single HID report. The object is derived
            from HID report descriptor data.
 */
@interface BSUSBHIDReportStructure : NSObject

/*!
 @property  reportType
 @abstract  The type of the report. (eg Input, Output, or Feature)
 */
@property (nonatomic, readonly, assign) BSUSBHIDReportType reportType;

/*!
 @property  reportID
 @abstract  The report ID for the HID report represented by this object. Only
            valid if hasReportID retuns YES.
 */
@property (nonatomic, readonly, assign) UInt8 reportID;

/*!
 @property  hasReportID
 @abstract  Returns YES if the report uses report ID's.
 */
@property (nonatomic, readonly) BOOL hasReportID;

/*!
 @property  reportFieldStructures
 @abstract  Array of BSUSBHIDReportFieldStructures which represent the report's fields.
 */
@property (nonatomic, readonly) NSArray<BSUSBHIDReportFieldStructure *> *reportFieldStructures;

/*!
 @property  reportBitLength
 @abstract  The total number of bits in the report.
 @discussion This length includes the report ID byte if applicable.
 */
@property (nonatomic, readonly) NSUInteger reportBitLength;

/*!
 @property  reportByteLength
 @abstract  The total number of bytes required to represent the report.
 @discussion This length includes the report ID byte if applicable. If the number of bits
             defined in the report isn't evenly divisable by 8, then an extra byte is added
             to accomidate the leftover bits.
 */
@property (nonatomic, readonly) NSUInteger reportByteLength;

/*!
 @property  reportTypeString
 @abstract  The string name of the report's report type.
 */
@property (nonatomic, readonly) NSString *reportTypeString;

/*!
 @method    +reportStructureWithReportType:reportID:
 @abstract  Returns an object initialized with the given parameters.
 @param     reportType
            The report type of the report structure (eg Input, Output, Feature)
 @param     reportID
            The report's reportID. Pass in 0 to create a default report structure that has
            no reportID (reportID 0 is marked reserved, so we use it to denote a report
            that has no reportID.
 */
+ (instancetype)reportStructureWithReportType:(BSUSBHIDReportType)reportType reportID:(UInt8)reportID;

/*!
 @method    -reportStructureWithReportType:reportID:
 @abstract  Initializes an object with the given parameters.
 @param     reportType
            The report type of the report structure (eg Input, Output, Feature)
 @param     reportID
            The report's reportID. Pass in 0 to create a default report structure that has
            no reportID (reportID 0 is marked reserved, so we use it to denote a report
            that has no reportID.
 */
- (instancetype)initWithReportType:(BSUSBHIDReportType)reportType reportID:(UInt8)reportID;

/*!
 @method    -addReportFieldStructure:
 @abstract  Appends a report field structure to the array of reportFieldStructures.
 @param     reportFieldStructure
            The BSUSBHIDReportFieldStructure object to append to the reportFieldStructures.
 */
- (void)addReportFieldStructure:(BSUSBHIDReportFieldStructure *)reportFieldStructure;

@end
