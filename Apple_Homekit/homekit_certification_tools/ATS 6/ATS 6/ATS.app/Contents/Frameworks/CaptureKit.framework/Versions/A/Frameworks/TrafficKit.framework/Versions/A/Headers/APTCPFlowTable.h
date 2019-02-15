//
//  APTCPFlowTable.h
//  APTS
//
//  Created by Mark Hamlin on 6/26/12.
//  Copyright (c) 2012 Apple. All rights reserved.
//

#import <Foundation/Foundation.h>

@class APTCPFlow;
@class APTCPFlowTable;
@class APTime;
@class APTCPPacket;
@class APChaChaKeyPacket;

/**
 Protocol to let the delegate object know when a new TCP connection is started or ended.
 */
@protocol APTCPFlowTableConnectionDelegate <NSObject>

@required
/**
 Delegate method called by APTCPFlowTable to let the delegate object know about a new TCP connection (flow)
 
 @param flowTable TCP Flow table
 @param flow New TCP flow
 
 @return Whether or not the packet is relevant.
 
 @note If the return value is NO, the bytes responsible for this packet is not saved to disk.
 */
- (BOOL)tcpFlowTable:(APTCPFlowTable *)flowTable didStartTCPConnection:(APTCPFlow *)flow;

/**
 Delegate method called by APTCPFlowTable to let the delegate object when the TCP connection ends (flow)
 
 @param flowTable TCP Flow table
 @param flow TCP connection that was ended
 
 @return Whether or not the packet is relevant.
 
 @note If the return value is NO, the bytes responsible for this packet is not saved to disk.
 */
- (BOOL)tcpFlowTable:(APTCPFlowTable *)flowTable didEndTCPConnection:(APTCPFlow *)flow;

@end

/*!
 @class     APTCPFlowTable
 @abstract  Detects TCP flows
 */
@interface APTCPFlowTable : NSObject

/**
 Time manager used to get current time.
 */
@property (nonatomic, readonly) APTime *timeManager;

/*!
 @property  tcpFlowsUseTimeouts
 @abstract  Whether or not a timeout mechanism should be used to remove TCP flows
 @default   YES
 */
@property (nonatomic, assign) BOOL tcpFlowsUseTimeouts;

/*!
 @property  idleFlowTimeoutInterval
 @abstract  The time interval after which an idle flow is considered dead.
 */
@property (nonatomic) NSTimeInterval idleFlowTimeoutInterval;

/**
 Property to hold weak reference to delegate object that receives information about when new TCP connection is observed or when TCP connection ends.
 */
@property (nonatomic, weak) id<APTCPFlowTableConnectionDelegate> connectionDelegate;

/*!
 Initializes flow table with capture
 @param     timeManager Time manager reference to get current time.
 @return    Initialized flow table
 */
- (instancetype)initWithTimeManager:(APTime *)timeManager;

/*!
 Resets the flow table by removing all the flows that are being tracked.
 
 @note This method should be called whenever there is a discontiniouty in the trace that renders all the known TCP connections irrelevant.
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

/**
 Method to track a new TCP packet. This method internally makes a new Flow and adds it to the flow table dictionary. This method also hands the packet over to the Flow itself for further processing the packet (for decryption).
 
 @param packet A new TCP packet
 
 @return Whether or not the packet is relevant.
 
 @note If the return value is NO, the bytes responsible for this packet is not saved to disk.
 */
- (BOOL)processTCPPacket:(APTCPPacket *)packet;

/**
 Method to process a new ChaChaKey packet.
 
 @param packet ChaChaKey packet.
 
 @return Whether or not the packet is relevant.
 */
- (BOOL)processChaChaKeyPacket:(APChaChaKeyPacket *)packet;

@end
