//
//  BSiAPOverUSBHIDAnalyzer.h
//  BusSniffer
//
//  Created by Joe Basu on 4/10/09.
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <Foundation/Foundation.h>
#import <BusSniffer/BSUSBHIDReport.h>

@class BSiAPPacketizer;
@class BSiAPTransactionIDAnalyzer;

@interface BSiAPOverUSBHIDAnalyzer : NSObject {
	NSMutableArray *_currentiPodiAPPacketHIDReports;
	NSMutableArray *_currentAccessoryiAPPacketHIDReports;

	BSiAPPacketizer *_iPodiAPPacketizer;
	BSiAPPacketizer *_accessoryiAPPacketizer;

	BSiAPTransactionIDAnalyzer *_iAPTransactionIDAnalyzer;
}

- (void)reset;

- (NSArray *)processInputReport:(id <BSUSBHIDReport>)inputReport reportContentStructure:(struct BSUSBHIDReportForiAPContentStructure *)structure;
- (NSArray *)processOutputReport:(id <BSUSBHIDReport>)outputReport reportContentStructure:(struct BSUSBHIDReportForiAPContentStructure *)structure;

@end
