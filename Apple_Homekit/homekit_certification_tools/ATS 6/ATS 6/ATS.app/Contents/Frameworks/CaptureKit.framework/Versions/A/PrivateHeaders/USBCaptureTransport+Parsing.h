//
//  USBCaptureTransport+Parsing.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 5/1/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/USBCaptureTransport.h>

#import <BusSniffer/BusSniffer.h>

@interface USBCaptureTransport (Parsing) <NCMAnalyzerDelegate, BSUSBAnalyzerDelegate>

- (void)nullifyUSBAnalyzers;
- (void)resetUSBAnalyzers;
- (void)recordUSBState:(UInt32)state atTimestamp:(UInt64)timestamp;
- (void)processUSBState:(UInt32)usbState atTimestamp:(UInt64)timestamp;
- (void)processUSBPacketBytes:(const void *)bytes length:(UInt16)length atTimestamp:(UInt64)timestamp;
- (void)processUSBTransaction:(id<BSUSBTransaction>)transaction;
- (void)processUSBTransfer:(id<BSUSBTransfer>)transfer;
- (void)reprocessUSBSavedData:(NSData *)usbSavedData filteredEthernetData:(NSData *)filteredEthernetData readProgressIndicator:(void (^)(float))readProgressIndicator;

- (void)processUSBHIDInputReport:(id<BSUSBHIDReport>)processedUSBHIDReport structure:(struct BSUSBHIDReportForiAPContentStructure *)structure;
- (void)processUSBHIDOutputReport:(id<BSUSBHIDReport>)processedUSBHIDReport structure:(struct BSUSBHIDReportForiAPContentStructure *)structure;

- (void)reprocessSavedBeagleUSBReadData:(NSData *)beagleUSBReadData readProgressIndicator:(void (^)(float))readProgressIndicator;

- (void)processBeagleUSBReadWithStatus:(UInt32)status
                                events:(UInt32)events
                               timeSOP:(UInt64)timeSOP
                          timeDuration:(UInt64)timeDuration
                        timeDataOffset:(UInt32)timeDataOffset
                           packetBytes:(const void *)packetBytes
                          packetLength:(UInt16)packetLength;

/*!
 @method    processEthernetFrameData:atTimestamp:
 @abstract  Process a single ethernet frame from any link
 */
- (void)processEthernetFrameData:(NSData *)data atTimestamp:(UInt64)timestamp;

/*!
 @method        deliverUSBEvent:
 @abstract      Deliver a USB event
 @discussion    This will conditionally gate delivery if it's a filtered packet
 */
- (void)deliverUSBEvent:(CaptureEvent *)event;

@end
