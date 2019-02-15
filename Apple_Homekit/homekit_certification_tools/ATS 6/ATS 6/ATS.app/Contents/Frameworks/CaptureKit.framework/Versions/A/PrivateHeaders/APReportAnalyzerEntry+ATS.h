//
//  APReportAnalyzerEntry+ATS.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 6/13/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <TrafficKit/TrafficKit.h>

@class BSUSBHIDReportDescriptor;

@interface APReportAnalyzerEntry (ATS)

/*!
 @method    populateWithHIDReportDescriptor:
 @abstract  Populates the receiver with a descriptor of the passed descriptor
 */
- (void)populateWithHIDReportDescriptor:(BSUSBHIDReportDescriptor *)descriptor;

@end
