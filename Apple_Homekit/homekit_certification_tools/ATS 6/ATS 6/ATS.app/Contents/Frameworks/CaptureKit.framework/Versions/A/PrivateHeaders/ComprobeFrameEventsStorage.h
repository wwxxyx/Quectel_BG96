//
//  ComprobeFrameEventsStorage.h
//  CaptureKit
//
//  Created by Douglas Frari on 10/23/18.
//  Copyright © 2018 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CaptureKit/EventBroker.h>

@class ComprobeFrameEvent;

@interface ComprobeFrameEventsStorage : NSObject

/*!
 Initializes a new CaptureComprobeFrameEvents
 */
+ (ComprobeFrameEventsStorage *)comprobeFrameEventsStorage;

/*!
 Add itself as a subscriber to the specified event broker
 @param  eventBroker the event broker
 @param  transport the transport the event should be on
 */
- (void)subscribeForEventBroker:(EventBroker *)eventBroker transport:(CaptureTransport *)transport;

/*!
 Maintains a list of events to be used during the BtSnoop export time.
 */
@property (nonatomic, strong) NSArray<ComprobeFrameEvent *> *events;

@end
