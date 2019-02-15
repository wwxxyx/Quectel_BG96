//
//  Rule.h
//  ATS
//
//  Created by Marin Balde on 7/14/10.
//  Copyright 2010 Apple Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class RuleClassification;

@interface Rule : NSObject {
    NSUInteger _ruleID;
    SEL _selector; // selector within stenographer
    IMP _implementation;
    NSUInteger *_dependencies; // ruleIDs of dependencies (e.g. the rules that this rule depends on)
    NSUInteger _dependencyCount;
    NSUInteger *_children; // the reverse of dependencies (e.g. the dependents of this rule)
    NSUInteger _childCount;
    NSArray *_classifications;
}

@property (nonatomic, readonly) NSUInteger ruleID;
@property (nonatomic, readonly) SEL selector;
@property (nonatomic, readonly) IMP implementation;
@property (nonatomic, readonly) NSUInteger *dependencies;
@property (nonatomic, readonly) NSUInteger dependencyCount;
@property (nonatomic, readonly) NSUInteger *children;
@property (nonatomic, readonly) NSUInteger childCount;
@property (nonatomic, readonly) NSArray *classifications;
// these are derived getters
@property (nonatomic, readonly) NSString *name;
@property (nonatomic, readonly) NSString *specFilePredicate;
@property (nonatomic, readonly) NSString *specReferenceLabel;
@property (nonatomic, readonly) NSNumber *specPageNumber;
@property (nonatomic, readonly) NSString *specSectionName;
@property (nonatomic, readonly) NSUInteger errorLevel;
@property (nonatomic, readonly) NSUInteger errorLevelBluetooth;
@property (nonatomic, readonly) NSString *errorStringFormat;
@property (nonatomic, readonly) BOOL productPlanRule;

+ (NSString *)ruleNameForRuleID:(NSUInteger)ruleID;
+ (NSUInteger)findRuleIDWithRuleName:(NSString *)ruleName; // returns NSNotFound if it cannot find ruleName
+ (NSString *)specFilePredicateForRuleID:(NSUInteger)ruleID;
+ (NSUInteger)errorLevelForRuleID:(NSUInteger)ruleID;
+ (NSUInteger)errorLevelBluetoothForRuleID:(NSUInteger)ruleID;
+ (NSString *)errorStringFormatForRuleID:(NSUInteger)ruleID; // might contain formatting charaters like %s, %d, etc...
+ (BOOL)productPlanRuleForRuleID:(NSUInteger)ruleID;

/*!
 @method        classificationsForRuleID
 @abstract      Fetches the RuleClassification array for the passed ruleID
 @param         ruleID  The rule to fetch classifications for
 @return        Array of RuleClassification * objects
 */
+ (NSArray *)classificationsForRuleID:(NSUInteger)ruleID;

+ (Rule *)ruleWithRuleID:(NSUInteger)ruleID
      dependenciesNoCopy:(NSUInteger *)dependencies
         dependencyCount:(NSUInteger)dependencyCount
          childrenNoCopy:(NSUInteger *)children
              childCount:(NSUInteger)childCount
         classifications:(NSArray *)classifications;

- (id)initWithRuleID:(NSUInteger)ruleID
    dependenciesNoCopy:(NSUInteger *)dependencies
       dependencyCount:(NSUInteger)dependencyCount
        childrenNoCopy:(NSUInteger *)children
            childCount:(NSUInteger)childCount
       classifications:(NSArray *)classifications;

@end

@interface RuleWithBlock : NSObject {
    Rule *_rule;
    void (^_block)(void);
}
@property (nonatomic, readwrite, retain) Rule *rule;
@property (nonatomic, readwrite, copy) void (^block)(void);

+ (id)ruleWithRule:(Rule *)rule block:(void (^)(void))block;
- (id)initWithRule:(Rule *)rule block:(void (^)(void))block;

@end
