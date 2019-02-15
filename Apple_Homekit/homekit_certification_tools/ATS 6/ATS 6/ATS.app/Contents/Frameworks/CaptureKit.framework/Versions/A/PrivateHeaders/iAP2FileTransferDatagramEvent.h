//
//  iAP2FileTransferDatagramEvent.h
//  ATSMacApp
//
//  Created by Joe Basu on 8/5/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <CaptureKit/iAP2Event.h>
#import <CaptureKit/CaptureEvent.h>
#import <BusSniffer/BusSniffer.h>

/** Error domain for the iAP2 File Transfer Packet errors */
extern NSString * const kiAP2FileTransferPacketErrorDomain;

/**
 Enum to declate all the possible file transfer packet parse errors 
 */
typedef NS_ENUM(NSUInteger, iAP2FileTransferPacketError) {
    /// Error when identifier byte can't be parsed.
    iAP2FileTransferPacketErrorInvalidDatagramLength,
    /// Error when type is invalid.
    iAP2FileTransferPacketErrorInvalidType,
    /// Error when the type expects a payload but there is none.
    iAP2FileTransferPacketErrorNoPayload,
    /// Error when the type doesn't not expect payload but there is.
    iAP2FileTransferPacketErrorUnexpectedPayload,
};

/** iAP2 File Transfer Event */ 
@interface iAP2FileTransferDatagramEvent : iAP2Event

/** Transfet identifier.
    From 0 to 127 means the Accessory is transfering a file to the Device.
    From 128 to 255 means the Device is transfering a file to the Device. */
@property (nonatomic, readonly) NSNumber *fileTransferIdentifier;

/** If the captured packet has been sent by the Accessory or by the Device. */
@property (nonatomic) BSiAPSource source;

/** The types of packets in FileTransfer protocol. */
@property (nonatomic) BSiAP2FileTransferSessionDatagramType type;

/** Errors if any */
@property (nonatomic, strong) NSArray<NSError *> *errors;

/** Datagram length - header length (2 bytes) */
@property (nonatomic, readonly) NSData *payloadData;

/**
 Factory method to get a iAP2 FileTransfer Event.
 
 @param The datagramData that will be parsed into a iAP2FileTransferDatagramEvent.
 @param The timestamp when the packet was captured.
 @param If the captured (sniffed) packet has been sent by the Accessory or by the Device.
 
 @return An instance of a iAP2FileTransferDatagramEvent.
 */
- (instancetype)initWithData:(NSData *)datagramData timestamp:(UInt64)timestamp source:(BSiAPSource)source;


@end
