//
//  BSUSBHIDReportFieldStructure+Private.h
//  BusSniffer
//
//  Created by Kip Nicol on 3/11/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import "BSUSBHIDReportFieldStructure.h"

@class BSUSBHIDReportStructure;

@interface BSUSBHIDReportFieldStructure (Private)

/*!
 @property  reportStructure
 @abstract  The associated report structure.
 */
@property (nonatomic, readwrite, assign) BSUSBHIDReportStructure *reportStructure;

@end
