//
//  USBPacket.h
//  ATS
//
//  Created by Joe Basu on 2/21/09.
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <BusSniffer/BusSniffer.h>
#import <CaptureKit/USBEvent.h>
#import <CaptureKit/DisplayProtocols.h>
#import <CaptureKit/CaptureEvent+ReportAnalyzer.h>

@interface USBPacketEvent : USBEvent <USBPacketDisplay, ReportAnalyzerDisplayProtocol> {
    BSUSBPacket *_packet;
}
@property (nonatomic, retain) BSUSBPacket *packet;

+ (id)usbPacketEventWithBSUSBPacket:(BSUSBPacket *)packet;
- (id)initWithBSUSBPacket:(BSUSBPacket *)packet;

- (NSString *)packetIDDescription;
- (NSString *)payloadDescription;

@end
