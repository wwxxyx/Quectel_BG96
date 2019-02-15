//
//  VBusLoadTestEvent.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 8/7/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <CaptureKit/ATSAppEvent.h>

typedef enum : uint8_t {
                   VBusLoadTestEventStartedType = 0,
                   VBusLoadTestEventFinishedType,
                   VBusLoadTestEventErrorType,
                   VBusLoadTestEventInfoType,
               } VBusLoadTestEventType;

@interface VBusLoadTestEvent : ATSAppEvent

/*!
 @method    eventType
 @abstract  Type of the event
 */
@property (readonly, nonatomic, assign) VBusLoadTestEventType eventType;

/*!
 @method    testStartedEvent
 @abstract  Returns an autoreleased test started event
 */
+ (VBusLoadTestEvent *)testStartedEvent;

/*!
 @method    testFinishedEventWithPotentialErrors:
 @abstract  Returns an autoreleased test finished event
 @param     potentialErrors
            Whether or not errors were encountered during the load test
 */
+ (VBusLoadTestEvent *)testFinishedEventWithPotentialErrors:(BOOL)potentialErrors;

/*!
 @method    testErrorEventWithMessage:description:
 @abstract  Returns an autoreleased test error event with message and detailed description
 @param     message
            One line message to show in the traffic table
 @param     description
            Multi-line description to show in the details view
 */
+ (VBusLoadTestEvent *)testErrorEventWithMessage:(NSString *)message description:(NSString *)description;

/*!
 @method    testInfoEventWithMessage:description:
 @abstract  Returns an autoreleased test info event with message and detailed description
 @param     message
            One line message to show in the traffic table
 @param     description
            Multi-line description to show in the details view
 */
+ (VBusLoadTestEvent *)testInfoEventWithMessage:(NSString *)message description:(NSString *)description;

/*!
 @property  errorMessage
 @abstract  Short description for the traffic row
 */
@property (nonatomic, retain) NSString *errorMessage;

/*!
 @property  errorDescription
 @abstract  Long-form description of the error
 */
@property (nonatomic, retain) NSString *errorDescription;

@end
