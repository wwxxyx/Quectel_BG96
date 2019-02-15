//
//  TestingLifecycleEvent.h
//  ATSMacApp
//
//  Created by Wesley McCloy on 8/18/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <CaptureKit/ATSAppEvent.h>

/**
 TestingLifecycleEventSignalType in an enum.
 
 The signal types are:
 
 - Unknown
 - Start
 - Stop
 - Info
 - Error
 
 This enum is to distinguish the nature of the event. Currently only distinguishes
 between messages that signal the start of the test lifecycle and its end.
 */
typedef NS_ENUM(NSUInteger, TestingLifecycleEventSignalType) {
    /** Unknown if not specified */
    TestingLifecycleEventSignalTypeUnknown = 0,
    /** Start is when a test entity has started */
    TestingLifecycleEventSignalTypeStart,
    /** Stop is when a test entity has stopped */
    TestingLifecycleEventSignalTypeStop,
    /** Info used for informative messages to show the user */
    TestingLifecycleEventSignalTypeInfo,
    /** Error used for errors communicated to the user */
    TestingLifecycleEventSignalTypeError,
};

/**
 TestingLifecycleEventTestEntity is an enum.
 
 The Entities are:
 
 - Unknown
 - TestSuite
 - TestCase
 - TestStep
 
 This enum is to distinguish between what type of test entity the lifecycle
 event is for. These are usually just test suite/case/step.
 */
typedef NS_ENUM(NSUInteger, TestingLifecycleEventTestEntity) {
    /** Unknown if not specified */
    TestingLifecycleEventTestEntityUnknown = 0,
    /** The largest test entity container, which contains test cases */
    TestingLifecycleEventTestEntityTestSuite,
    /** The test case container comprises of test steps */
    TestingLifecycleEventTestEntityTestCase,
    /** Test steps are the smalles test entities, defined by an atomic test
     action */
    TestingLifecycleEventTestEntityTestStep,
    /** Test is for a generic test that doesn't use the above structure */
    TestingLifecycleEventTestEntityTest,
};

/**
 TestingLifecycleEvent is a class. Subclass of ATSAppEvent. It's usage is for
 when testing lifecycle messages need to be presented to the user.
 
 ie. "Test Case "HID message validation" has started/stoped"
 
 Some interesting logic can be used on these events. The above declared enums
 give identity cues that can make it easier.
 
 The "visible to the user" part of this event will be generated based on entity
 type, signal type and source. It will be accompanied by an identifier for the 
 entity and by an optional description provided to the event on initialization.
 
 For context events that need to be analyzed. The ContextEvent class is what you
 want.
 */
@interface TestingLifecycleEvent : ATSAppEvent

/**
 Convenience initializer which sets all properties accordingly
 @param name The name of the test entity
 @param number The number (ie sequence number) of the test entity. Supply unsigned integer max if you don't have a number.
 @param identifier The identifier of the test entity
 @param type The signal type of the event
 @param entity The test entity type
 @param source The source of this event (i.e. you)
 @param info Optional info useful for Info and Error events
 @return The initialized event
 */
+ (nullable TestingLifecycleEvent *)eventWithName:(nullable NSString *)name
                                           number:(NSUInteger)number
                                       identifier:(nullable NSString *)identifier
                                             type:(TestingLifecycleEventSignalType)type
                                           entity:(TestingLifecycleEventTestEntity)entity
                                           source:(nonnull NSString *)source
                                     optionalInfo:(nullable NSString *)info;

@end
