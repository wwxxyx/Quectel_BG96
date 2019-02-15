//
//  EmulatedBeagleUSBCaptureDevice.h
//  ATSMacApp
//
//  Created by Joe Basu on 10/20/10.
//  Copyright © 2010 Apple Inc. All Rights Reserved.
//

#import <CaptureKit/BeagleUSBCaptureDevice.h>
#import <BusSniffer/BusSniffer.h>

@interface EmulatedBeagleUSBCaptureDevice : BeagleUSBCaptureDevice {
    UInt64 _currentTimestamp;
}

/*!
 @property  interPacketDelay
 @abstract  Delay in nanoseconds which is automatically inserted between each emulated packet
 @default   0
 */
@property (nonatomic, assign) UInt64 interPacketDelay;

- (UInt64)passDurationInNanoseconds:(UInt64)nanoseconds;

- (void)emulateUSBReset;

- (void)emulateUSBFullSpeed;

- (NSArray *)emulateUSBTokenPacketWithPacketID:(BSUSBPacketID)packetID
                                       address:(UInt8)address
                                      endpoint:(UInt8)endpoint;

- (NSArray *)emulateUSBHandshakePacketWithPacketID:(BSUSBPacketID)packetID;

- (NSArray *)emulateUSBDataPacketWithPacketID:(BSUSBPacketID)packetID
                                  payloadData:(NSData *)payloadData;

- (NSArray *)emulateUSBDataPacketWithPacketID:(BSUSBPacketID)packetID
                             payloadByteCount:(UInt16)byteCount
                                 payloadBytes:(UInt)firstByte, ...;

@end
