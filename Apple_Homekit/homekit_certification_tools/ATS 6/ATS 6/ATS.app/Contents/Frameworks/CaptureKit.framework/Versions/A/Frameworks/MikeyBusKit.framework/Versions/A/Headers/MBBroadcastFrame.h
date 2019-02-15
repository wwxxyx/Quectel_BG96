//
//  MBBroadcastFrame.h
//  MikeyBusKit
//
//  Created by Wesley McCloy on 9/16/15.
//  Copyright © 2015 Apple Inc. All rights reserved.
//

/*!
 @header        MBBroadcastFrame
 @discussion    This header file defines the properties inherent to each BroadcastFrame object as well as an interface to extract data channel mappings
 */

#import <Foundation/Foundation.h>

// Constants used for accessing properties within the data buffer
extern const int MBSFSOFValue;
extern const int MBSFFramesPerSuperframe;
extern const int MBSFSuperframeChannels;
extern const int MBSFDLChannel;
extern const int MBSFULChannel;

extern const int MBBFStartOfDL;
extern const int MBBFEndOfDL;
extern const int MBBFStartOfUL;
extern const int MBBFEndOfUL;

/* type definitions in original command line tool by Georgi
//
//  This is important as it outlines the order of the bytes and
//  their significance with respect to the bcframe data
 
typedef struct {
    uint8_t     channel;
    uint16_t    numBytes;
} __attribute__((packed)) ChannelAlloc;

typedef struct {
    uint8_t                 SOF;
    uint8_t                 version;
    uint16_t                SFN;
    uint8_t                 downlinkSlots;
    uint8_t                 uplinkSlots;
    uint8_t                 reserved;
    uint8_t                 rate;
    ChannelAlloc            TCA[SF_CHANNELS];
    uint16_t                CRC;
} __attribute__((packed)) bcfr;

typedef union BFrame {
    bcfr frame;
    uint8_t *bytes;
}BFrame;

*/

@interface MBBroadcastFrame : NSObject <NSCopying>

/// All properties derived from data. Must be 160 bytes
@property (nonatomic) NSData *data;

/// Accesses data with index 0
@property (nonatomic) uint8_t SOF;
/// Accesses data with index 1
@property (nonatomic) uint8_t version;
/// Accesses data with index 2 to 3
@property (nonatomic) uint16_t SFN;
/// Accesses data with index 4
@property (nonatomic) uint8_t downlinkSlots;
/// Accesses data with index 5
@property (nonatomic) uint8_t uplinkSlots;
/// Accesses data with index 6
@property (nonatomic) uint8_t reserved;
/// Accesses data with index 7
@property (nonatomic) uint8_t rate;
/// Accesses data with index 8 to 157
@property (nonatomic) NSArray *TCA;
/// Accesses data with index 158 to 159
@property (nonatomic) uint16_t CRC;

/// Calling this BOOL runs a set of functions that verify the integrity of the broadcast frame
/// With respect to size, identifiers and CRC
@property (nonatomic) BOOL valid;
/// Error(s) if not valid
@property (nonatomic) NSArray<NSError *> *errors;
/// [slotNumber(NSNumber):channelID(NSNumber)] dictionary for downlink data
@property (nonatomic) NSDictionary *downlinkChannelMap;
/// [slotNumber(NSNumber):channelID(NSNumber)] dictionary for uplink data
@property (nonatomic) NSDictionary *uplinkChannelMap;

/// initializes a MBBroadcastFrame with a data buffer
- (instancetype)initWithData:(NSData *)data;

/// Prints frame and channel info
-(void)printInfo;

/// If the SOF can be accessed
@property (readonly) BOOL hasSOF;
/// If the Version can be accessed
@property (readonly) BOOL hasVersion;
/// If the SFN can be accessed
@property (readonly) BOOL hasSFN;
/// If the DownlinkSlots can be accessed
@property (readonly) BOOL hasDownlinkSlots;
/// If the UplinkSlots can be accessed
@property (readonly) BOOL hasUplinkSlots;
/// If the Reserved can be accessed
@property (readonly) BOOL hasReserved;
/// If the Rate can be accessed
@property (readonly) BOOL hasRate;
/// If the TCA can be accessed
@property (readonly) BOOL hasTCA;
/// If the CRC can be accessed
@property (readonly) BOOL hasCRC;

/*!
 Accesses Traffic Channel Allocation array
 
 @param index The index of the TCA array
 @return the channel for the given index
 */
-(uint8_t)channelForTCA:(uint8_t)index;

/*!
 Accesses Traffic Channel Allocation array
 
 @param index The index of the array
 @return the number of bytes allocated for the given index
 */
-(uint16_t)numBytesForTCA:(uint8_t)index;

/*!
 Maps slot indexes of each frame to channels as defined by the Traffic Channel Allocation array
 
 @param cFrom       Lower bound of filter
 @param cTo         Upper bound of filter
 @param cBulk       Bulk channel in byte format
 @param startSlot   Slot offset to begin indexing
 @return A dictionary that is composed of slot to channel key value pairs
 */
-(NSDictionary *)mapChannelFrom:(uint8_t)cFrom To:(uint8_t)cTo Bulk:(uint8_t)cBulk StartSlot:(uint8_t)startSlot;


@end
