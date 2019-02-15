//
//  MikeyBusPacketWindow.h
//  ATSMacApp
//
//  Created by Vinod Madigeri on 1/3/17.
//  Copyright © 2017 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <BusSniffer/BusSniffer.h>
#import <MikeyBusKit/MikeyBusKit.h>

@class ATSMikeyBusBulkDataPacketEvent;

/**
 This interface defines the ability to track the segment numbers in current packet window of a MikeyBus transmitting entity.
 */
@interface MikeyBusPacketWindow : NSObject

/**
 Array of packets that were not ACKed.
 */
@property (nonatomic, readonly) NSArray <ATSMikeyBusBulkDataPacketEvent *> *packetsNotACKed;

/**
 Array of packets that were seen with segment numbers outside of the packet window.
 */
@property (nonatomic, readonly) NSArray <ATSMikeyBusBulkDataPacketEvent *> *unexpectedPackets;

/**
 Array of ACKs that were seen with segment numbers outside of the packet window.
 */
@property (nonatomic, readonly) NSArray <ATSMikeyBusBulkDataPacketEvent *> *unexpectedACKs;

/**
 Packets that were retransmitted.
 */
@property (nonatomic, readonly) NSArray <ATSMikeyBusBulkDataPacketEvent *> *retransmittedPackets;

/**
 Method to initialize the packet window.
 
 @param receivingEntity to which this packet window belongs to / is an instance of.
 */
- (instancetype)initWithReceivingEntityType:(BSiAPSource)receivingEntity;

/**
 Pass in all of the valid bulk data events in the order received. Error packets are tracked in the exposed properties. This method takes care of only processing the events that are relevent to this instance of the packet window.
 */
- (void)processBulkDataEvent:(ATSMikeyBusBulkDataPacketEvent *)event;

/**
 Method to get non acked packets if any and clear them.
 */
- (NSArray <ATSMikeyBusBulkDataPacketEvent *> *)flushNonACKedPackets;

/**
 Method to get unexpected packets if any and clear them.
 */
- (NSArray <ATSMikeyBusBulkDataPacketEvent *> *)flushUnexpectedPackets;

/**
 Method to get unexpected ACKs if any and clear them.
 */
- (NSArray <ATSMikeyBusBulkDataPacketEvent *> *)flushUnexpectedACKs;

/**
 Method to get retransmitted packets if any and clear them.
 */
- (NSArray <ATSMikeyBusBulkDataPacketEvent *> *)flushRetransmittedPackets;

@end


