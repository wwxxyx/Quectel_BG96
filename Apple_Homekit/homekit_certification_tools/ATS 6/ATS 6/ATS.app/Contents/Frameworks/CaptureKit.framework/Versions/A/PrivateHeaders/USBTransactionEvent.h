//
//  USBTransaction.h
//  ATS
//
//  Created by Joe Basu on 2/26/09.
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <BusSniffer/BusSniffer.h>
#import <CaptureKit/USBEvent.h>
#import <CaptureKit/DisplayProtocols.h>
#import <CaptureKit/CaptureEvent+ReportAnalyzer.h>

@interface USBTransactionEvent : USBEvent <USBTransactionDisplay> {
    BSUSBTransaction *_transaction;
    BSUSBConfigurationDescriptor *_configuration;
}
@property (nonatomic, retain) BSUSBTransaction *transaction;
@property (nonatomic, retain) BSUSBConfigurationDescriptor *configuration;

+ (id)usbTransactionEventWithBSUSBTranaction:(BSUSBTransaction *)transaction;
- (id)initWithBSUSBTransaction:(BSUSBTransaction *)transaction;

- (NSString *)typeDescription;
- (NSString *)addressDescription;
- (NSString *)endpointDescription;
- (NSString *)lengthDescription;

@end
