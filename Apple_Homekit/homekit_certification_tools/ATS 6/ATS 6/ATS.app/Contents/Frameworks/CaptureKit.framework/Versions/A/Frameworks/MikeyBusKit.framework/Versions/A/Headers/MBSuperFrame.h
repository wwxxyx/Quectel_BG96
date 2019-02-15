//
//  MBSuperFrame.h
//  MikeyBusKit
//
//  Created by Wesley McCloy on 9/18/15.
//  Copyright © 2015 Apple Inc. All rights reserved.
//

/*!
 @header        MBSuperFrame
 @discussion    This header defines the properties inherent to the SuperFrame object as well as an interface to access data useful for display as well as Uplink and Downlink Bulk Data
 */

#import <Foundation/Foundation.h>

@class MBBulkDataPacket;
@class MBBroadcastFrame;
@class MBError;

@interface MBSuperFrame : NSObject

/// Each Superframe is defined by a BroadcastFrame. This helps define channel mappings for DL and UL data storage
@property (nonatomic) MBBroadcastFrame *broadcastFrame;
/// [channelID(NSNumber):data(NSData)] dictionary for downlink channels
@property (nonatomic) NSDictionary *downlinkData;
/// [channelID(NSNumber):data(NSData)] dictionary for uplink channels
@property (nonatomic) NSDictionary *uplinkData;
/// Encoded Bulk Data for the UL channel
@property (nonatomic) NSData *ULBulkDataFrameEncoded;
/// Encoded Bulk Data for the DL channel
@property (nonatomic) NSData *DLBulkDataFrameEncoded;

/**
 An array of Bulk Data Packets with direction, timestamp and payload.
 */
@property (nonatomic, strong) NSArray <MBBulkDataPacket *> *bulkDataPacketSequence;

/// Relative timestamp from start of capture
@property (nonatomic) NSTimeInterval timestamp;

/// validity of superframe is evaluated by calling this function or set by its creator. This is paired with the
/// error property
@property (nonatomic, getter=isValid) BOOL valid;
/// If the Superframe is invalid, there will be errors
@property (nonatomic) NSMutableArray<MBError *> *errors;

/**
 Initializes a new super frame with the given broadcast frame as its definition
 */
+ (instancetype)superFrameWithDefinition:(MBBroadcastFrame *)broadcaseFrame;

// Methods to access data that is useful for display
- (NSNumber *)SFUNumber;
- (NSNumber *)numberOfDLChannels;
- (NSNumber *)numberOfULChannels;
- (NSString *)DLchannelUsageAndAllocations;
- (NSString *)ULchannelUsageAndAllocations;

- (void)addError:(MBError *)error;

@end
