//
//  RuleListClassification.h
//  ATSMacApp
//
//  Created by Sergio Sette on 9/22/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <CaptureKit/RuleClassification.h>

#define WITH_CLASSIFICATION(classType, classificationType, ...) [classType classificationForType:classificationType andClassifications:((UInt8[]){ __VA_ARGS__ }) classificationsCount:sizeof((UInt8[]){ __VA_ARGS__ })]

/*!
 Defines the type of RuleListClassification
 
 @constant RuleListClassificationTypeBlackList
           The BlackList type defines the rules should not run if the classification is present
 @constant RuleListClassificationTypeWhiteList
           The WhiteList type defines the rules should only run if the classification is present
 */
typedef enum : UInt8 {
                   RuleListClassificationTypeBlackList,
                   RuleListClassificationTypeWhiteList,
               } RuleListClassificationType;

@interface RuleListClassification : RuleClassification

/*!
 The type of the RuleListClassification
 */
@property (nonatomic, readonly) RuleListClassificationType type;

@property (nonatomic, readonly, retain) NSArray *classifications;

/*!
 Initialize a RuleListClassification with the specified RuleListClassificationType ans list of classifications
 @param type               the type of RuleListClassification
 @param classifications    list of classifications for this RuleListClassification
 @return a new RuleListClassification
 */
- (instancetype)initWithType:(RuleListClassificationType)type andClassifications:(NSArray *)classifications;

/*!
 Defines if the RuleListClassificationType is whiteList
 @return YES if type is RuleListClassificationTypeWhiteList
 */

- (BOOL)isWhiteList;

/*!
 Defines if the RuleListClassificationType is blackList
 @return YES if type is RuleListClassificationTypeBlackList
 */
- (BOOL)isBlackList;

/*!
 Constructor given the list of classifications and the type of RuleListClassification
 @param type               the type of RuleListClassification
 @param classifications    list of classifications for this RuleListClassification
 @return a new RuleListClassification
 */
+ (instancetype)classificationForType:(RuleListClassificationType)type andClassifications:(NSArray *)classifications;

/*!
 Constructor given the list of classifications and the type of RuleListClassification
 @param type               the type of RuleListClassification
 @param classifications    list of classifications for this RuleListClassification
 @return a new RuleListClassification
 */
+ (instancetype)classificationForType:(RuleListClassificationType)type andClassifications:(UInt8[])classifications classificationsCount:(UInt8)classificationsCount;

/*!
 Defines if the rule that owns this RuleListClassification is eligible to run in a capture with the specified classification
 @param classification    the current capture classification
 @return YES if the rule should run, NO otherwise
 */

- (BOOL)isEligibleFor:(NSNumber *)classification;

/*!
 Defines if the rule that owns this RuleListClassification is eligible to run in a capture with the specified classification list
 @param classificationList    the current capture classification list
 @return YES if the rule should run, NO otherwise
 */

- (BOOL)isEligibleForTypes:(NSArray *)classificationList;

@end
