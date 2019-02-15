//
//  BSUSBHIDReportDescriptor.h
//  BusSniffer
//
//  Created by Joe Basu on 11/9/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <BusSniffer/BSUSBDescriptor.h>
#import <BusSniffer/BSUSBHIDTypes.h>

@class BSUSBHIDReportStructure;
@class BSUSBHIDReportDescriptorShortItem;

/*!
 @class     BSUSBHIDReportDescriptor
 @abstract  A class that represents a USB HID report descriptor
 */

@interface BSUSBHIDReportDescriptor : BSUSBDescriptor

/*!
 @property  hasReportIDs
 @abstract  Returns YES if the descriptor contains report ID's.
 */
@property (nonatomic, readonly, assign) BOOL hasReportIDs;

/*!
 Array of top level BSUSBHIDReportDescriptorCollection objects representing the top level collections in the descriptor.
 */
@property (nonatomic, retain, readonly) NSArray *allCollections;

/*!
 @property -reportDescriptorItems
 @abstract Parses the raw data to provide an array of HID report descriptor items (BSUSBHIDReportDescriptorShortItem)
 */
@property (nonatomic, readonly, strong) NSArray<BSUSBHIDReportDescriptorShortItem *> *reportDescriptorItems;

/*!
 @property    allReportStructures
 @abstract  Returns all report structures that are in the HID report descriptor.
 */
@property (nonatomic, readonly, strong) NSArray<BSUSBHIDReportStructure *> *allReportStructures;

/*!
 @method    -hidReportFieldValuesForReportData:reportType:error:
 @abstract  Returns an array of BSUSBHIDReportFieldStructure objects each representing 
            the value of a field in the data of a HID report.
 @param     data
            The HID report data.
 @param     reportType
            The type of the report (Input, Output, Feature)
 @param     error
            Upon encountering an error, this parameter is set to the corresponding NSError object.
 */
- (NSArray *)hidReportFieldValuesForReportData:(NSData *)data reportType:(BSUSBHIDReportType)reportType error:(NSError **)error;

/*!
 @method reportIDForReportData:
 @abstract returns the report ID from the given HID report data, or returns BSUSBHIDReportIDUndefined
           if this report descriptor doesn't have report IDs.
 */
- (UInt8)reportIDForReportData:(NSData *)data;

/*!
 @method    -reportStructureForReportType:reportID:
 @abstract  Returns the BSUSBHIDReportStructure object which represents the report structure
            for the given parameters.
 @param     reportType
            The type of report to be retrieved (eg Input, Output, Feature)
 @param     reportID
            The report ID of the report. If reportID's are not set in this descriptor, pass BSUSBHIDReportIDUndefined to 
            get the default report for the given report type.
 @discussion 
            Raises NSInvalidArgumentException if the reportType is not valid.
 */
- (BSUSBHIDReportStructure *)reportStructureForReportType:(BSUSBHIDReportType)reportType reportID:(UInt8)reportID error:(NSError **)error;

/*!
 @method    -reportStructuresForReportType:
 @abstract  Returns all report structures for the given reportType in a dictionary.
 @param     reportType
            The type of report to be retrieved (eg Input, Output, Feature)
 @param     reportID
            The report ID of the report. If reportID's are not set in this descriptor, pass 0 to
            get the default report for the given report type.
 @discussion
            Dictionary is keyed by the reportID for the report. Use 0 to retrieve the default report
            if reportID's are not used. Raises NSInvalidArgumentException if the reportType is not valid.
 */
- (NSDictionary *)reportStructuresForReportType:(BSUSBHIDReportType)reportType error:(NSError **)error;

@end
