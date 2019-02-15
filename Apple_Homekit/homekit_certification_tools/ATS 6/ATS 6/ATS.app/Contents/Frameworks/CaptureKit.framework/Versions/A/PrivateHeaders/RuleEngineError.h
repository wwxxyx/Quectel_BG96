//
//  RuleEngineError.h
//  ATSMacApp
//
//  Created by Marin Balde on 10/8/10.
//  Copyright 2010 Apple Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#if APPLE_INTERNAL

// error domain
extern NSString *const RuleEngineErrorDomain;
// userInfo keys
extern NSString *const RuleEngineErrorNameKey;
extern NSString *const RuleEngineRuleNameKey;

enum RuleEngineErrorCodes {
    RuleEngineErrorGeneric = 3000,
    RuleEngineErrorExceptionCaught,
};

@interface RuleEngineError : NSError {
}

+ (id)errorWithName:(NSString *)name reason:(NSString *)reason ruleName:(NSString *)ruleName;
- (id)initWithName:(NSString *)name reason:(NSString *)reason ruleName:(NSString *)ruleName;

- (NSString *)name;
- (NSString *)ruleName;

@end

#endif
