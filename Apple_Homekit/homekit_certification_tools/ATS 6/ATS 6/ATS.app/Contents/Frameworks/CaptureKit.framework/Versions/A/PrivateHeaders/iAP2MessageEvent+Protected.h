//
//  iAP2MessageEvent+Protected.h
//  ATSMacApp
//
//  Created by Zachary Church on 8/2/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <CaptureKit/iAP2MessageEvent.h>
#import <CaptureKit/CaptureEvent+Protected.h>

/*!
 @macro     WITH_IAP2_MESSAGE_TRIGGER
 @abstract  Create an event classification given an iAP2 message ID. By default, this is essential, but passing the second arg as kEventIsNotEssential will create a non-essential event classification
 @example   This is an essential IdentificationInformation: WITH_IAP2_MESSAGE_TRIGGER(iAP2IdentificationInformationMessageID)
 @example   This is a non-essential one: WITH_IAP2_MESSAGE_TRIGGER(iAP2IdentificationInformationMessageID, kEventIsNotEssential)
 */
#define WITH_IAP2_MESSAGE_TRIGGER(messageID, ...) CONSTRUCT_WITH_EVENT_TRIGGER(iAP2MessageEvent, [iAP2MessageEvent classificationForMessageWithID:messageID], ##__VA_ARGS__, YES)
#define ACTIVATE_WITH_IAP2_MESSAGE_TRIGGER(messageID, ...) CONSTRUCT_ACTIVATOR_WITH_EVENT(iAP2MessageEvent, [iAP2MessageEvent classificationForMessageWithID:messageID], ##__VA_ARGS__, YES)

@interface iAP2MessageEvent()

+ (id)iAP2MessageEventWithI2MMessage:(I2MMessage *)message timestamp:(UInt64)timestamp;
- (id)initWithI2MMessage:(I2MMessage *)message timestamp:(UInt64)timestamp;

/*!
 @method    classificationForMessageWithID:
 @abstract  Returns the type classification which would appear for a message with the passed ID
 */
+ (id)classificationForMessageWithID:(UInt16)messageID;

@end
