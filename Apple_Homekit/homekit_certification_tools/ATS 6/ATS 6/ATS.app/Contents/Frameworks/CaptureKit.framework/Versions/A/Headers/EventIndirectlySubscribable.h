//
//  EventIndirectlySubscribable.h
//  CaptureKit
//
//  Created by Vinod Madigeri on 10/7/18.
//  Copyright © 2018 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureEvent.h>

/**
 Protocol or encapsulate and generalize event objects that can have related events and be treated as special in Event Broker.
 */
@protocol EventIndirectlySubscribable <NSObject>

/**
 Property to return a set of related events
 */
@property (nonatomic, readonly) NSSet<CaptureEvent *> *relatedEvents;

@end
