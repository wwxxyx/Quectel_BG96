//
//  HCIContainerEventsStorage.h
//  CaptureKit
//
//  Created by Edgard Lima on 11/06/18.
//  Copyright © 2018 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CaptureKit/EventBroker.h>

@class HciContainerEvent;

@interface HCIContainerEventsStorage : NSObject

/*!
 Initializes a new CaptureHciContainerEvents
 */
+ (HCIContainerEventsStorage *)hciContainerEventsStorage;

/*!
 Add itself as a subscriber to the specified event broker
 @param  eventBroker the event broker
 @param  transport the transport the event should be on
 */
- (void)subscribeForEventBroker:(EventBroker *)eventBroker transport:(CaptureTransport *)transport;

/*!
 Maintains a list of events for retrieval during Bt Snoop export process.
 */
@property (nonatomic, strong) NSArray<HciContainerEvent *> *events;

@end
