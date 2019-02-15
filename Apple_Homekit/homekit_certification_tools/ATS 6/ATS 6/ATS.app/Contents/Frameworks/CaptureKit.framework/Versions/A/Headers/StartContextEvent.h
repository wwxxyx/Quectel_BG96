//
//  StartContextEvent.h
//  ATSMacApp
//
//  Created by Zachary Church on 8/9/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <CaptureKit/ContextEvent.h>

@interface StartContextEvent : ContextEvent

/**
 @property context
 @abstract optionally provides additional information to the rule engine that can help in deciding whether or not to enforce a rule
 */
@property (nonatomic, readonly) NSDictionary<NSString *, id> *context;

/*!
 @abstract convenience class initialization given the identifier, timestamp and context dictionary
 @param identifier The string identifier for the context event
 @param context A dictionary to supply information that can be used in validation logic
 @param timestamp The timestamp that the event
 @return The StartContextEvent
 */
+ (instancetype)eventWithContextIdentifier:(NSString *)identifier context:(NSDictionary *)context timestamp:(UInt64)timestamp __deprecated;

/*!
 @abstract convenience initialization given the identifier and context. Since no timestamps is specified
 the event gets inserted at the latest timestamp
 @param identifier The string identifier for the context event
 @param context A dictionary to supply information that can be used in validation logic
 @return The StartContextEvent
 */
+ (instancetype)eventWithContextIdentifier:(NSString *)identifier context:(NSDictionary *)context;

@end
