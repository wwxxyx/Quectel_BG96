//
//  USBTransferEvent.h
//  ATS
//
//  Created by Joe Basu on 2/26/09.
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <BusSniffer/BusSniffer.h>
#import <CaptureKit/USBEvent.h>
#import <CaptureKit/DisplayProtocols.h>

@interface USBTransferEvent : USBEvent <USBTransferDisplay, ReportAnalyzerDisplayProtocol> {
    BSUSBTransfer *_transfer;
    BSUSBConfigurationDescriptor *_configuration;
}
@property (nonatomic, retain) BSUSBTransfer *transfer;
@property (nonatomic, retain) BSUSBConfigurationDescriptor *configuration;
@property (nonatomic, retain) NSError *error;

/*!
 @property   hidInterfaceID
 @abstract   The interface descriptor number (bInterfaceNumber) for the 
             transfer.
 */
@property (nonatomic, readonly) UInt8 hidInterfaceNumber;

+ (id)usbTransferEventWithBSUSBTransfer:(BSUSBTransfer *)transfer;
- (id)initWithBSUSBTransfer:(BSUSBTransfer *)transfer;

- (NSString *)typeDescription;
- (NSString *)addressDescription;
- (NSString *)endpointDescription;
- (NSString *)lengthDescription;
// The following method was turned into a property so that Facets can KVC it
// Note: the above methods were not changed to properties because they follow
// the normal practise.
// - (NSString *)transferDescription;
@property (nonatomic, readonly) NSString *transferDescription;

/*!
 @method    transferContainsUSBGetConfigurationDescriptor
 @abstract  Whether or not the represented transfer contains a USB Get Configuration Descriptor
 */
- (BOOL)transferContainsValidUSBGetConfigurationDescriptor;

@end
