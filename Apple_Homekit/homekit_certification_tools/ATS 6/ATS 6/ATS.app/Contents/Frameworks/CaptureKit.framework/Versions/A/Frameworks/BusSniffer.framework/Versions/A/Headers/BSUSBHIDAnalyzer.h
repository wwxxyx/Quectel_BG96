//
//  BSUSBHIDAnalyzer.h
//  BusSniffer
//
//  Created by Joe Basu on 6/13/09.
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <Foundation/Foundation.h>
#import <BusSniffer/BSUSBTransaction.h>
#import <BusSniffer/BSUSBTransfer.h>
#import <BusSniffer/BSUSBHIDReport.h>

@class BSUSBAnalyzer;
@class BSUSBHIDDescriptor;
@class BSUSBHIDReportDescriptor;
@class BSUSBHIDAnalyzerTransferState;


@interface BSUSBHIDAnalyzer : NSObject {
	BSUSBAnalyzer *_usbAnalyzer;
	BSUSBHIDDescriptor *_hidDescriptor;
	BSUSBHIDReportDescriptor *_hidReportDescriptor;
	BSUSBHIDAnalyzerTransferState *_activeInterruptTransferState;
	UInt8 _interruptEndpointNumber;
    BSUSBEndpointDirection _interruptEndpointDirection;
	UInt16 _maxPacketSizeForInterruptEndpoint;
}

+ (id)usbHIDAnalyzerWithUSBAnalyzer:(BSUSBAnalyzer *)usbAnalyzer
					  hidDescriptor:(BSUSBHIDDescriptor *)hidDescriptor
				hidReportDescriptor:(BSUSBHIDReportDescriptor *)hidReportDescriptor
            interruptEndpointNumber:(UInt8)endpointNumber
         interruptEndpointDirection:(BSUSBEndpointDirection)endpointDirection
							  error:(NSError **)error;

- (id)initWithUSBAnalyzer:(BSUSBAnalyzer *)usbAnalyzer
			hidDescriptor:(BSUSBHIDDescriptor *)hidDescriptor
	  hidReportDescriptor:(BSUSBHIDReportDescriptor *)hidReportDescriptor
  interruptEndpointNumber:(UInt8)endpointNumber
interruptEndpointDirection:(BSUSBEndpointDirection)endpointDirection
					error:(NSError **)error;

- (id <BSUSBTransfer>)processHIDInterruptTransaction:(id <BSUSBTransaction>)hidInterruptTransaction error:(NSError **)error;
- (id <BSUSBHIDReport>)processHIDInterruptTransfer:(id <BSUSBTransfer>)hidInterruptTransfer error:(NSError **)error;
- (id <BSUSBHIDReport>)processHIDControlTransfer:(id <BSUSBTransfer>)hidControlTransfer error:(NSError **)error;

@end
