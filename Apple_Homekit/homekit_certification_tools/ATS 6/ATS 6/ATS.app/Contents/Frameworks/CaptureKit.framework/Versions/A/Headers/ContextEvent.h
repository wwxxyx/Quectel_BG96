//
//  ContextEvent.h
//  ATSMacApp
//
//  Created by Zachary Church on 8/9/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <CaptureKit/ATSAppEvent.h>

@interface ContextEvent : ATSAppEvent

/*!
 @property identifier
 @abstract uniquely identifies the class of context event. Context-dependent rules will use this property to decide whether a StartContextEvent/StopContextEvent pair is relevant to them.
 */
@property (nonatomic, readonly) NSString *identifier;


/*!
 @abstract convenience initialization given the identifier and timestamp
 @param identifier The string identifier for the context event
 @param timestamp The timestamp that the event
 @return The ContextEvent
 */
- (instancetype)initWithContextIdentifier:(NSString *)identifier timestamp:(UInt64)timestamp __deprecated;

/*!
 @abstract convenience class initialization given the identifier and timestamp
 @param identifier The string identifier for the context event
 @param timestamp The timestamp that the event
 @return The ContextEvent
 */
+ (instancetype)eventWithContextIdentifier:(NSString *)identifier timestamp:(UInt64)timestamp __deprecated;

/*!
 @abstract convenience initialization given the identifier. Since no timestamps is specified
 the event gets inserted at the latest timestamp
 @param identifier The string identifier for the context event
 @return The ContextEvent
 */
- (instancetype)initWithContextIdentifier:(NSString *)identifier;

/*!
 @abstract convenience class initialization given the identifier. Since no timestamps is specified
 the event gets inserted at the latest timestamp
 @param identifier The string identifier for the context event
 @return The ContextEvent
 */
+ (instancetype)eventWithContextIdentifier:(NSString *)identifier;


@end
