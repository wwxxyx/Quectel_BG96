//
//  L2CAPFrame.h
//  Header for Bluetooth L2CAP
//
//  Created by Edgard Lima on 08/13/18.
//  Copyright (c) 2018 Apple. All rights reserved.
//

#import <Foundation/Foundation.h>

// From Bluetooth Core Specification 4.2
typedef NS_ENUM(UInt16, L2CAPChannelId) {
    /// Not allowed.
    L2CAPChannelIdNull = 0x0000,
    /// Signaling over ACL-U logical link.
    L2CAPChannelIdSignaling = 0x0001,
    /// Connectionless Data Frame (G-Frame).
    L2CAPChannelIdConnectionlessData = 0x0002,
    /// Asynchronous Management Protocol.
    L2CAPChannelIdAMPManagerProtocol = 0x0003,
    // 0x4 - 0x6 reserved.
    /// Basic Rate and Extended Data Rate Security Manager.
    L2CAPChannelIdBREDRSecurityManager = 0x0007,
    // 0x8 - 0x3E reserved.
    /// Asynchronous Management Protocol Test Manager.
    L2CAPChannelIdAMPTestManager = 0x003F,
    /// Minimal Dynamically allocated
    L2CAPChannelIdDynamicMin = 0x0040,
    /// Maximun Dynamically allocated
    L2CAPChannelIdDynamicMax = 0xFFFF,
    
};
// Channels, other than 0x2, 0x1 and 0x5 are considered Connection-Oriented

// Protocol Data Unit (PDU) types
typedef NS_ENUM(UInt8, L2CAPduType) {
    L2CAPPduTypeUnknown,
    /// Basic Information Frame. SDU as Payload.
    L2CAPPduTypeBFrame,
    /// Information Frame. SDU Segment + Protocol Information.
    L2CAPPduTypeIFrame,
    /// Supervisory Frame. Protocol information with no SDU data.
    L2CAPPduTypeSFrame,
    /// Control Frame. Contains Signaling messages. See L2CAPChannelIdSignaling.
    L2CAPPduTypeCFrame,
    /** Group Frame. For Connectionless L2CAP data channel. May be used to:
     1- Broadcast all slaves: via Piconet
     2- Broadcast active slaves: via Active broadcast
     3- send unicast to single remote device
     */
    L2CAPPduTypeGFrame,
};

@interface L2CAPFrame : NSObject

/// do not call this method directly. call buildL2CAPFrameFromFrameData instead.
- (instancetype)initWithChannelAndFrameData:(NSNumber*)channelID frameData:(NSData *)frameData errors:(NSMutableArray<NSError *> *)errors;

/** Use this method to create the proper sub-class for your L2CAP frame.
 @seealso L2CAPFrameBFrame
 @seealso L2CAPFrameGFrame
 @seealso L2CAPFrameSignaling
 @return An instance of L2CAPFrame, L2CAPFrameBFrame, L2CAPFrameGFrame or L2CAPFrameSignaling depending on how the L2CAP Frame is parsed.
 */
+ (instancetype)buildL2CAPFrameFromFrameData:(NSData *)frameData;

/// Channel ID. See L2CAPChannelId.
@property (readonly) NSNumber *channelId;

/// L2CAP playload. It returns data after the L2CAP header. Notice the payload is implemented at specialized classes. Headers at all depths are skipped and just the very end payload is returned to be consumed by upper layers.
@property (readonly) NSData *payloadData;

/*!
 Property that represents the raw data of the L2CAP frame (i.e. header + payload)
 */
@property (nonatomic, readonly) NSData *frameData;

/** Errors if any */
@property (nonatomic, strong) NSArray<NSError *> *errors;

/// The PDU Type for certain Channel ID
+ (L2CAPduType)pduTypeFromChannelId:(L2CAPChannelId)channelId;

/** If this L2CAP object has enought data to hold the size reported on L2CAP header. Actually, if there is not enough data errors properties will be set.
    Notice you can't modify L2CAP frameData, so, only create the L2CAP object when you have already collected enough data for it. You can't create it imcomplete if you want to report an error of an incomplete L2CAP fragmented frame.
 */
+ (BOOL)frameDataHasEnoughData:(NSData*)frameData;

@end
