//
//  RuleTestError.h
//  ATS
//
//  Created by Marin Balde on 7/15/10.
//  Copyright 2010 Apple Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>

// error domain
extern NSString *const RuleTestErrorDomain;
// userInfo keys
extern NSString *const RuleTestErrorNameKey;
extern NSString *const RuleTestRuleNameKey;

enum RuleTestErrorCodes {
    RuleTestErrorGeneric = 2000,
    RuleTestErrorNotSingleATFRetHardwareInfoEvent,
    RuleTestErrorNotAnATFRetHardwareInfoEvent,
    RuleTestErrorNotSingleiAPPacket,
    RuleTestErrorNotAniAPPacket,
    RuleTestErrorInvalidiAPPacket,
    RuleTestErrorAssertRuleError,
    RuleTestErrorAssertExceptionError,
    RuleTestErrorUnassertedRuleErrors,
    RuleTestErrorDetectedErrors,
    RuleTestErrorNotAniAP2Message,
    RuleTestErrorNotAniAP2LinkPacket,
    RuleTestErrorNoPositiveTestCase,
    RuleTestErrorNoNegativeTestCase,
    RuleTestErrorNoATFEvent,
    RuleTestErrorNotExpectedEvent,
    RuleTestErrorNotAHIDReportDescriptorEvent,
    RuleTestErrorNotAUSBNativeHIDReportDescriptorTransferEvent,
    RuleTestErrorNotAUSBNativeHIDReportTransferEvent,
};

@interface RuleTestError : NSError

+ (id)errorWithName:(NSString *)name reason:(NSString *)reason;
+ (id)errorWithName:(NSString *)name reason:(NSString *)reason ruleName:(NSString *)ruleName;
- (id)initWithName:(NSString *)name reason:(NSString *)reason;
- (id)initWithName:(NSString *)name reason:(NSString *)reason ruleName:(NSString *)ruleName;

- (NSString *)name;

@end
