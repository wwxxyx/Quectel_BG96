//
//  iAP2LinkLayerSynchronization.h
//  ATSMacApp
//
//  Created by Natália Cabral on 06/08/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <CaptureKit/iAP2LinkLayerSequence.h>
@class BSiAP2LinkSYNPacket;

/**
 States for Link Layer synchronization.
 */
typedef NS_ENUM(NSUInteger, iAP2LinkLayerSynchronizationState) {
    iAP2LinkLayerSynchronizationNotStarted,
    iAP2LinkLayerSynchronizationSYNSentByAccessory,
    iAP2LinkLayerSynchronizationSYNSentByAppleDevice,
    iAP2LinkLayerSynchronizationACKSentByAccessory,
    iAP2LinkLayerSynchronizationACKSentByAppleDevice,
    iAP2LinkLayerSynchronizationNegotiationConcluded,
};

@interface iAP2LinkLayerSynchronization : iAP2LinkLayerSequence

/*!
    The last SYN packet the apple device sent
 */
@property (nonatomic, readonly, retain) BSiAP2LinkSYNPacket *lastSYNPacketSentByAppleDevice;

/*!
    The last SYN packet the accessory sent
 */
@property (nonatomic, readonly, retain) BSiAP2LinkSYNPacket *lastSYNPacketSentByAccessory;

/*!
    Returns whether or not the negotiation is concluded
 */
- (BOOL)isNegotiationConcluded;

/**
 True when the accessory has negotiated a ZeroACK/ZeroRetransmit link configuration that does not expect retransmitted packets or packet acknowledgements.
 
 @reference: In this case, the underlying transport has its own mechanisms for reliable delivery and the accessory fully utilizes those mechanisms.
 */
@property (nonatomic, readonly) BOOL isZeroACKZeroRetransmitLinkConfigured;


@end
