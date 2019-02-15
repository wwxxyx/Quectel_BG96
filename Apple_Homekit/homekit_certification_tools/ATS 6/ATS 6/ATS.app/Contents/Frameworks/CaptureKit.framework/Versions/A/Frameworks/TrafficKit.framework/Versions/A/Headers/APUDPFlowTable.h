//
//  APUDPFlowTable.h
//  TrafficKit
//
//  Created by Mark Hamlin on 7/11/12.
//  Copyright (c) 2012 Apple. All rights reserved.
//

#import <Foundation/Foundation.h>

@class APTime;
@class APUDPFlow;
@class APUDPFlowTable;
@class APPacket;
@class APmDNSFlowTable;
@class APmDNSPacket;
@class APUDPPacket;

/**
 Protocol to let the delegate object know when a new UDP connection is started or ended.
 */
@protocol APUDPFlowTableConnectionDelegate <NSObject>

@required
/**
 Delegate method called by APUDPFlowTable to let the delegate object know about a new UDP connection (flow)
 
 @param flowTable UDP Flow table
 @param flow New UDP flow
 
 @return Whether or not the packet is relevant.
 
 @note If the return value is NO, the bytes responsible for this packet is not saved to disk.
 */
- (BOOL)udpFlowTable:(APUDPFlowTable *)flowTable didStartUDPConnection:(APUDPFlow *)flow;

/**
 Delegate method called by APUDPFlowTable to let the delegate object when the UDP connection ends (flow)
 
 @param flowTable UDP Flow table
 @param flow UDP connection that was ended
 
 @return Whether or not the packet is relevant.
 
 @note If the return value is NO, the bytes responsible for this packet is not saved to disk.
 */
- (BOOL)udpFlowTable:(APUDPFlowTable *)flowTable didEndUDPConnection:(APUDPFlow *)flow;

@end

/*!
 @class     APUDPFlowTable
 @abstract  Detects TCP flows
 */
@interface APUDPFlowTable : NSObject

/*!
 Time manager to get current time from
 */
@property (nonatomic, readonly) APTime *timeManager;

/*!
 @property  udpFlowsUseTimeouts
 @abstract  Whether or not a timeout mechanism should be used to remove UDP flows
 @default   YES
 */
@property (nonatomic, assign) BOOL udpFlowsUseTimeouts;

/*!
 @property  idleFlowTimeoutInterval
 @abstract  The time interval after which an idle flow is considered dead.
 @default   20.0
 */
@property (nonatomic) NSTimeInterval idleFlowTimeoutInterval;

/**
 Property to hold weak reference to delegate object that receives information about when new UDP connection is observed or when UDP connection ends.
 */
@property (nonatomic, weak) id<APUDPFlowTableConnectionDelegate> connectionDelegate;

/**
 mDNS flow table for mDNS packets seen over the UDP flows
 */
@property (nonatomic, readonly) APmDNSFlowTable *mDNSFlowTable;

/*!
 Initializes flow table with capture
 @param     timeManager Time manager to get current time from
 @param     mDNSFlowTable mDNS flow table to process mDNS packets seen over the UDP flows
 
 @return    Initialized flow table
 */
- (instancetype)initWithTimeManager:(APTime *)timeManager mDNSFlowTable:(APmDNSFlowTable *)mDNSFlowTable;

/*!
 Resets the flow table by removing all the flows that are being tracked.
 
 @note This method should be called whenever there is a discontiniouty in the trace that renders all the known UDP connections irrelevant.
 */
- (void)reset;

/*!
 @method        invalidate
 @abstract      Shuts down the flow table; Cancelling timers, cleaning up memory
 @discussion    This MUST be called before discarding the flow
 */
- (void)invalidate;

/*!
 @method    flows
 @abstract  Returns all current flows
 */
- (NSArray *)flows;

/*!
 Method to receive and process UDP packets. This method internally makes a new Flow and adds it to the flow table dictionary. This method also hands the packet over to the Flow itself for further processing the packet.
 
 @return Whether or not the packet is relevant.
 
 @note If the return value is NO, the bytes responsible for this packet is not saved to disk.
 */
- (BOOL)processUDPPacket:(APPacket *)packet;

/*!
 Method to receive and process mDNS packets.
 
 @return Whether or not the packet is relevant.
 
 @note If the return value is NO, the bytes responsible for this packet is not saved to disk.
 */
- (BOOL)processmDNSPacket:(APPacket *)packet;

@end
