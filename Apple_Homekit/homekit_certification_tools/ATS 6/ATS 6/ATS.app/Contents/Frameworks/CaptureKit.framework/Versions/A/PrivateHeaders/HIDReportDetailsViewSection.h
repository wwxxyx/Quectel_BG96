//
//  HIDReportDetailsViewSection.h
//  ATSMacApp
//
//  Created by Kip Nicol on 3/5/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CaptureKit/OrderedListDetailsViewSection.h>

/*!
 @interface HIDReportDetailsViewSection
 @abstract  Details view section for HID report field value data.
 */
@interface HIDReportDetailsViewSection : OrderedListDetailsViewSection

/*!
 @method    +detailsViewSectionForHIDReportFieldValues:title:identifier:
 @abstract  Creates and initializes a HID report details view.
 @param     hidReportFieldValues
            Array of BSUSBHIDReportFieldValues representing values from a HID report.
 */
+ (instancetype)detailsViewSectionForHIDReportFieldValues:(NSArray *)hidReportFieldValues title:(NSString *)title identifier:(NSString *)identifier;

@end
