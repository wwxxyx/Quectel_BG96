//
//  USBHIDReport.h
//  ATS
//
//  Created by Joe Basu on 6/13/09.
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <BusSniffer/BusSniffer.h>
#import <CaptureKit/USBEvent.h>
#import <CaptureKit/DisplayProtocols.h>

@interface USBHIDReportEvent : USBEvent <USBHIDReportDisplay> {
    BSUSBHIDReport *_report;
  @public
    struct BSUSBHIDReportForiAPContentStructure structure;
}
@property (nonatomic, retain) BSUSBHIDReport *report;

+ (id)usbHIDReportEventWithBSUSBHIDReport:(BSUSBHIDReport *)report;
- (id)initWithBSUSBHIDReport:(BSUSBHIDReport *)report;

- (NSString *)typeDescription;
- (NSString *)reportIDDescription;
- (NSString *)lengthDescription;
- (NSString *)continuationDescription;
- (NSString *)moreToFollowDescription;
- (NSString *)reportDescription;

@end
