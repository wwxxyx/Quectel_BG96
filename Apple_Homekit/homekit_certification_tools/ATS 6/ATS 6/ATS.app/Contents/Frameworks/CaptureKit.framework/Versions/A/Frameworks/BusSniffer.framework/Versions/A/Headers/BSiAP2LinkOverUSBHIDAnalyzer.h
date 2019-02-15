//
//  BSiAP2LinkOverUSBHIDAnalyzer.h
//  BusSniffer
//
//  Created by Joe Basu on 3/12/12.
//  Copyright © 2012 Apple Inc. All Rights Reserved.
//

#import <Foundation/Foundation.h>
#import <BusSniffer/BSUSBHIDReport.h>
#import <BusSniffer/BSiAP2Message.h>
#import <BusSniffer/BSiAP2LinkPacket.h>

@class BSiAP2LinkPacketizer;
@class BSiAP2MessageMaker;

@interface BSiAP2LinkOverUSBHIDAnalyzer : NSObject {
	NSMutableArray *_currentiPodiAP2LinkPacketHIDReports;
	NSMutableArray *_currentAccessoryiAP2LinkPacketHIDReports;

    BSiAP2LinkPacketizer *_iPodiAP2LinkPacketizer;
	BSiAP2LinkPacketizer *_accessoryiAP2LinkPacketizer;
    
    UInt64 _currentiPodiAP2MessageTimestamp;
    UInt64 _currentAccessoryiAP2MessageTimestamp;
    
    BSiAP2MessageMaker *_iPodiAP2MessageMaker;
    BSiAP2MessageMaker *_accessoryiAP2MessageMaker;
}

- (void)reset;

- (NSArray *)processInputReport:(id <BSUSBHIDReport>)inputReport reportContentStructure:(struct BSUSBHIDReportForiAPContentStructure *)structure;
- (NSArray *)processOutputReport:(id <BSUSBHIDReport>)outputReport reportContentStructure:(struct BSUSBHIDReportForiAPContentStructure *)structure;

- (NSArray *)processiAP2LinkPacket:(id <BSiAP2LinkPacket>)linkPacket;

@end
