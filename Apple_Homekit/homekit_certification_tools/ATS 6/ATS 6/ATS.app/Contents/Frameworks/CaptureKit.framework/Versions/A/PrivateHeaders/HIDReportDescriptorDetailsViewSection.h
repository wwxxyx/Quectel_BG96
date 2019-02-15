//
//  HIDReportDescriptorDetailsViewSection.h
//  ATSMacApp
//
//  Created by Joe Basu on 11/27/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <CaptureKit/OrderedListDetailsViewSection.h>

@class BSUSBHIDReportDescriptor;

/*!
 @class     HIDReportDescriptorDetailsViewSection
 @abstract  A details view section that shows the contents of a HID report descriptor
 */

@interface HIDReportDescriptorDetailsViewSection : OrderedListDetailsViewSection

/*!
 @method +detailsViewSectionForHIDReportDescriptor:title:identifier:
 @abstract Creates and returns a details view section using given HID report descriptor
 @param hidReportDescriptor  The HID report descriptor
 @return The newly created and initialized details view section
 */
+ (instancetype)detailsViewSectionForHIDReportDescriptor:(BSUSBHIDReportDescriptor *)hidReportDescriptor title:(NSString *)title identifier:(NSString *)identifier;

/*!
 @method -initWithHIDReportDescriptor:title:identifier:
 @abstract Initializes a details view section using given HID report descriptor
 @param hidReportDescriptor  The HID report descriptor
 @return The newly initialized details view section
 */
- (instancetype)initWithHIDReportDescriptor:(BSUSBHIDReportDescriptor *)hidReportDescriptor title:(NSString *)title identifier:(NSString *)identifier;

@end
