//
//  ConnectEvent.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 4/25/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureEvent.h>

/** The base class for events generated when something is connected to a sniffer. */
@interface ConnectEvent : CaptureEvent

/** The underlying cause of the connection event. */
@property (nonatomic, readonly, copy) NSString *triggerReason;

/** A description of the capture device that produced the event. */
@property (nonatomic, readonly, copy) NSString *sourceDescription;

/** A short description of the hardware involved in the event (e.g. "Apple device" or "hardware"). */
@property (nonatomic, readonly, copy) NSString *hardwareDescription;

/**
 A new event with the given timestamp.
 
 @param timestamp The timestamp to associate with the new event.
 @param triggerReason A string describing what caused the event.
 @return The new event.
 */
- (instancetype)initWithTimestamp:(UInt64)timestamp triggerReason:(NSString *)triggerReason;

/**
 A new event with the given timestamp.
 
 @param timestamp The timestamp to associate with the new event.
 @param triggerReason A string describing what caused the event.
 @param hardwareDescription A string describing the piece of hardware that was connected or disconnected (e.g. "Apple device" or "hardware").
 @return The new event.
 */
- (instancetype)initWithTimestamp:(UInt64)timestamp triggerReason:(NSString *)triggerReason hardwareDescription:(NSString *)hardwareDescription;

@end
