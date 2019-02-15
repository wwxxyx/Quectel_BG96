//
//  EventBroker.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 4/30/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//
// https://gitlab.sd.apple.com/ats/CaptureKitDocumentation

#import <Foundation/Foundation.h>
#import <CaptureKit/EventChanneling.h>

@class CaptureEvent;
@class CaptureTransport;

NS_ASSUME_NONNULL_BEGIN

@protocol EventSubscribing <NSObject>

/**
 Method called to publish the event to subscriber. Subscriber method called when an event, that the subscriber cares about, is delivered to event broker.
 
 Note: It is not guaranteed that the ATS window is still up and running during this call. All external subscribers need to rely on CaptureDocument's `didClose` callback to know when the document is fully closed and where all `processEvent` calls have completed. rdar://38767866 ([CaptureKit+Facets] processEvent deadlock)
 */
- (void)processEvent:(CaptureEvent *)event;

@end


/**
 Publishing priority. A subscriber can set its prority to receive the events.
 */
typedef NS_ENUM(NSUInteger, EventSubscriberPriority) {
    EventSubscriberPriorityLow = 0,
    EventSubscriberPriorityNormal = 127,
    EventSubscriberPriorityHigh = 255,
};


@interface EventBroker : NSObject

/**
 Default channel that an event, that's not associated with another channel, is published on.
 
 @return Default channel for an event to be published on.
 */
+ (id<EventChanneling>)defaultEventChannel;

/**
 Create a new event channel with input name.
 
 @param channelName Channel name.
 @return Returns a new channel object with unique identifier.
 */
- (id<EventChanneling>)createNewEventChannelWithName:(NSString *)channelName;

/**
 Add a subscriber to the broker, given a particular selector to invoke on queue 'queue' when events of class 'aClass' are seen
 
 @param         subscriber
 Subscriber object that implements selector.
 @param         aClass
 The event class this subscriber cares about.
 @param         priority
 The priority of the subscriber, used to determine the ordering of invocations. All high priority invocations will have returned before normal are invoked, and all normal returned before low, etc
 @param         queue
 The queue to dispatch to.
 @param         transport
 The transport the event should be on. Transport should be nil, when indicating any transport.
 */
- (void)addSubscriber:(id<EventSubscribing>)subscriber
           eventClass:(Class)aClass
             priority:(EventSubscriberPriority)priority
                queue:(nullable dispatch_queue_t)queue
            transport:(nullable CaptureTransport *)transport;

/**
 Add a subscriber to the broker, given a particular selector to invoke on queue 'queue' when events of class 'aClass' are seen
 
 @param         subscriber 
 Subscriber object that implements selector.
 @param         aClass
 The event class this subscriber cares about.
 @param         channel
 The event channel this subscriber cares to receive the event on. If the channel is nil, subscriber receives every event on every channel.
 @param         priority
 The priority of the subscriber, used to determine the ordering of invocations. All high priority invocations will have returned before normal are invoked, and all normal returned before low, etc
 @param         queue
 The queue to dispatch to.
 @param         transport
 The transport the event should be on. Transport should be nil, when indicating any transport.
 @param allowIndirection Whether the subscription will allow indirect event delivery. This mechanism is used for delivering rule errors
 */
- (void)addSubscriber:(id<EventSubscribing>)subscriber
           eventClass:(Class)aClass
            onChannel:(id<EventChanneling>)channel
             priority:(EventSubscriberPriority)priority
                queue:(nullable dispatch_queue_t)queue
            transport:(nullable CaptureTransport *)transport
     allowIndirection:(BOOL)allowIndirection;

/**
 Removes the passed subscriber
 
 @param subscriber to be removed.
 */
- (void)removeSubscriber:(id<EventSubscribing>)subscriber;

/**
 Publish an event given the transport and channel.
 
 @param event Event being published.
 @param channel Channel this event is published on. If channel is nil, the event belongs to every subscriber that cares about its class.
 @param transport Capture transport this event belongs to.
 */
- (void)publishEvent:(CaptureEvent *)event onChannel:(id<EventChanneling>)channel onTransport:(CaptureTransport *)transport;

/**
 Dispatches a block asynchronously to be executed on the delivery queue
 @discussion    This allows other operations such as rule delivery to bypass the publish mechanism, but retain ordering
 */
- (void)dispatchAsync:(void (^)(void))block;

/**
 Shuts down object
 */
- (void)invalidateWithCompletion:(nullable void (^)(EventBroker *eventBroker))completionHandler;

/**
 Returns whether or not the subscriber would receive the event _if_ it were to be published.

 @param subscriber Subscriber under test, whether or not subscriber has subscribed to the event.
 @param event Event to check if the subscriber has subscribed to.
 
 @note Channel should already be associated with event before calling this. When the event is published (publishEvent:(Event *) onChannel:(id<EventChanneling>) onTransport:(CaptureTransport *)), the channel is associated with the event.
 */
- (BOOL)isSubscriber:(id<EventSubscribing>)subscriber subscribedToEvent:(CaptureEvent *)event;

/**
 Returns whether or not the subscriber would receive one of the events _if_ it were to be published.
 
 @param subscriber Subscriber under test, whether or not subscriber has subscribed to the event.
 @param events Events to check if the subscriber has subscribed to one of them.
 
 @note Channel should already be associated with event before calling this. When the event is published (publishEvent:(CaptureEvent *) onChannel:(id<EventChanneling>) onTransport:(CaptureTransport *)), the channel is associated with the event.
 */
- (BOOL)isSubscriber:(id<EventSubscribing>)subscriber subscribedToOneOfEvents:(NSArray <CaptureEvent *> *)events;

@end

NS_ASSUME_NONNULL_END
