//
//  ChaChaKeyPacketEvent.h
//  ATSMacApp
//
//  Created by Vinod Madigeri on 6/7/18.
//  Copyright © 2018 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureEvent.h>
#import <TrafficKit/TrafficKit.h>

/**
 This packet is created in ATS Internal only 
 */
@interface ChaChaKeyPacketEvent : CaptureEvent

/**
 Reference to ChaCha packet seen over network
 */
@property (nonatomic, strong) APChaChaKeyPacket *chachaPacket;

/**
 Type is ChaCha20
 */
@property (nonatomic, strong) NSString *typeDescription;

/**
 Source is Apple device
 */
@property (nonatomic, strong) NSString *sourceDescription;

/**
 Creates an instance
 */
- (instancetype)initWithChaChaKeyPacket:(APChaChaKeyPacket *)chachaPacket;

@end
