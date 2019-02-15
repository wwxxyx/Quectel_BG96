//
//  ATSiAPMissingPacketEvent.h
//  ATSMacApp
//
//  Created by Natália Cabral on 15/07/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureEvent.h>
#import <CaptureKit/DisplayProtocols.h>
#import <BusSniffer/BSiAPTypes.h>

/** ATSiAPMissingPacketEvent instances represent points in time at which the parser detected that some incoming traffic may have been lost. */
@interface ATSiAPMissingPacketEvent : CaptureEvent <iAPPacketDisplay>

/** The assumed source of the missing packet. */
@property (nonatomic, readonly) BSiAPSource source;

/** The transaction ID of the missing packet. */
@property (nonatomic, readonly) UInt16 transactionID;

/** An error object appropriate for APIs that accept packet errors. */
@property (readonly) NSError *error;

/**
 Initializes and returns a new iAPMissingPacketEvent with the given source, transaction ID and timestamp.
 
 @param source The source of the missing packet.
 @param transactionID The transaction ID of the missing packet.
 @param timestamp The timestamp marking when the error was detected.
 @return The new event.
 */
+ (instancetype)iAPMissingPacketEventWithSource:(BSiAPSource)source transactionID:(UInt16)transactionID timestamp:(UInt64)timestamp;

/**
 Initializes and returns a new iAPMissingPacketEvent with the given source, transaction ID and timestamp.
 
 @param source The source of the missing packet.
 @param transactionID The transaction ID of the missing packet.
 @param timestamp The timestamp marking when the error was detected.
 @return The new event.
 */
- (instancetype)initWithSource:(BSiAPSource)source transactionID:(UInt16)transactionID timestamp:(UInt64)timestamp;

@end
