//
//  IDBusStenograph.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 8/2/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/StenographModule.h>
@class IDBusPacketEvent;

@interface IDBusStenograph : StenographModule

/*!
 @property  currentIDBusPacketEvent
 @abstract  The current IDBus packet event
 */
@property (nonatomic, retain) IDBusPacketEvent *currentIDBusPacketEvent;

/*!
 @property  lastIDBusIDResponsePacketEvent
 @abstract  Last ID Bus packet for command IDBCommandID.
 */
@property (nonatomic, retain) IDBusPacketEvent *lastIDBusIDResponsePacketEvent;

/*!
 @property  currentIDBusPassthroughCommand
 @abstract  Current ID Bus 0xBD command seen.
 */
@property (nonatomic, retain) IDBusPacketEvent *currentIDBusPassthroughCommand;

/*!
 @property  justSawB139B164ConnectedEvent
 @abstract  Whether or not a B139 or B164 connected to the accessory side of the ATS Lightning Box
 */
@property (nonatomic, assign) BOOL justSawB139B164ConnectedEvent;

@end
