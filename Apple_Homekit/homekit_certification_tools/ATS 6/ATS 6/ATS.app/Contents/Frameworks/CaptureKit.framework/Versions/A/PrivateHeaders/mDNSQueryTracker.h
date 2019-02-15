//
//  mDNSQueryTracker.h
//  ATSMacApp
//
//  Created by Vinod Madigeri on 6/29/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@class NetworkPacketEvent;

/**
 Tracker status on whether or not bonjour question records are seen before answer records, for a direct link enabled CarPlay accessory.
 */
typedef NS_ENUM(NSUInteger, mDNSQueryTrackerStatus) {
    /**
     IP address has not been identified as whether or not it's a CarPlay accessory to check for questions before answers
     */
    mDNSQueryTrackerStatusNotIdentified,
    /**
     IP address has been identified as CarPlay accessory, and it has sent questions before answers [Direct link disabled]
     */
    mDNSQueryTrackerStatusHasQuestionBeforeAnswer,
    /**
     IP address has been identified as CarPlay accessory, and it has sent answers before questions [Direct link enabled]
     */
    mDNSQueryTrackerStatusHasAnswerBeforeQuestion,
};

/**
 mDNS query tracker tracks network events with Bonjour mDNS packets for questions sent before answer. The tracker needs the information on whether or not the source IP address belongs to a CarPlay accessory, which isn't readily available at the time of tracking. Helps track down when a CarPlay accessory has not enabled direct link.
 */
@interface mDNSQueryTracker : NSObject

/**
 Create an instance of tracker.
 
 @param event Network event.

 @return An instance of mDNSQueryTracker
 */
+ (instancetype)trackerWithNetworkEvent:(NetworkPacketEvent *)event;

/**
 Source IP address associated with the network events with Bonjour questions and answers.
 */
@property (nonatomic, strong, readonly) NSString *sourceIPAddress;

/**
 Whether or not Bonjour question records are seen before answer records, for a CarPlay accessory.
 */
@property (nonatomic, readonly) mDNSQueryTrackerStatus queryStatus;

/**
 All the Network events with mDNS packets associated with the question and answer records being tracked.
 */
@property (nonatomic, strong, readonly) NSArray <NetworkPacketEvent *> *networkEvents;

/**
 Add a network event associated with the question and answer records that are supposed to be tracked.
 
 @param event Network event with mDNS packet associated with the question and answer records that are supposed to be tracked.
 */
- (void)trackQuestionAnswerBonjourRecordsFromNetworkEvent:(NetworkPacketEvent *)event;

/**
 Method to let query tracker know that it has all the information it needs to be able to check whether or not there was a question record seen before answer record for a direct link enabled CarPlay accessory. The status of this resolution can be accessed from queryStatus property.
 */
- (mDNSQueryTrackerStatus)resolve;

@end
