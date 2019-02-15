//
//  ATSMikeyBusSuperFramePacketEvent.h
//  ATSMacApp
//
//  Created by Wesley McCloy on 12/8/15.
//  Copyright © 2015 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureEvent+Protected.h>
#import <MikeyBusKit/MikeyBusKit.h>

@interface ATSMikeyBusSuperFramePacketEvent : CaptureEvent

/**
 Superframe reference
 */
@property (nonatomic, retain) MBSuperFrame *mbSuperFrame;

/**
 Superframe number.
 */
@property (nonatomic, readonly) NSString *superframeNumber;

/**
 Downlink channel allocations number.
 */
@property (nonatomic, readonly) NSString *DLChannelAllocationsNumber;

/**
 Downlink channel usage and allocations description
 */
@property (nonatomic, readonly) NSString *DLChannelUsageAndAllocationsDescription;

/**
 Uplink channel allocation number.
 */
@property (nonatomic, readonly) NSString *ULChannelAllocationsNumber;

/**
 Uplink channel usage and allocations description.
 */
@property (nonatomic, readonly) NSString *ULChannelUsageAndAllocationsDescription;

@end
