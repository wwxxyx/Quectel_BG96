//
//  CaptureRuleUnitTest.h
//  ATSMacApp
//
//  Created by Sergio Sette on 10/29/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <iAP2MessageKit/I2MMessage.h>
#import <OCMock/OCMock.h>
#import <CaptureKit/CaptureRule.h>
#import <CaptureKit/CaptureConfigurationDescription.h>
#import <CaptureKit/CaptureRuleGroup.h>
#import <CaptureKit/iAP2Constants.h>
#import <CaptureKit/iAP2MessageEvent.h>

@interface CaptureRuleUnitTest : XCTestCase

/*!
    The mocked capture rule information of the current rule
 */
@property (nonatomic, strong) CaptureRuleInformation *captureRuleInformation;

/*!
 Stenographer instance that holds the reference to the old-way of writing Stenographs( using Stenograph modules) to be able to share one instance of it per transport.
 */
@property (nonatomic, strong) Stenographer *stenographer;

/*!
 The mocked capture rule of the current rule
 */
@property (nonatomic, strong) CaptureRule *captureRule;

/*!
    The mocked capture configuration
 */
@property (nonatomic, strong) CaptureConfigurationDescription *captureConfiguration;

/*!
    The mocked capture rule group stenograph
 */
@property (nonatomic, strong) CaptureRuleStenograph *captureRuleStenograph;

/*!
    The rule group to be tested
 */
@property (nonatomic, strong) CaptureRuleGroup *ruleGroup;

/** Initializes the rule test environment with a mock stenograph and rule configuration. Subclasses that override this method should call super's implementation before doing anything else. */
- (void)resetMockEnvironment;

/*!
 Mocks an iAP2 message with the specified message ID
 
 @param messageID  the ID of the iAP2Message to be mocked
 @return the mocked I2MMessage with the specified message ID
 */
- (I2MMessage *)mockMessageWithID:(iAP2MessageID)messageID;

/*!
 Mocks an iAP2 message event with the specified message
 
 @param message     the message of the iAP2MessageEvent to be mocked
 @return the mocked iAP2MessageEvent with the specified message
 */
- (iAP2MessageEvent *)mockEventWithMessage:(I2MMessage *)message;

/*!
 Mocks an iAP2 message event that contains a message with the specified message ID
 
 @param messageID   the ID of the message to be mocked
 @return the mocked iAP2MessageEvent with the specified message ID
 */
- (iAP2MessageEvent *)mockEventWithMessageID:(iAP2MessageID)messageID;

/*!
 Mocks a capture rule group stenograph
 
 @return the mocked CaptureRuleStenograph
 */
- (CaptureRuleStenograph *)mockStenograph;

@end
