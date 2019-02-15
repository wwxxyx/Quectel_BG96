//
//  TriggerEventRuleClassification.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 4/24/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/RuleClassification.h>

/*!
 @class     TriggerEventRuleClassification
 @abstract  A classification describing a type of event which the rule should be executed for
 */
@interface TriggerEventRuleClassification : RuleClassification

/*!
 @property  eventClass
 @abstract  The class specified
 */
@property (nonatomic, readonly) Class eventClass;

/*!
 @property  type
 @abstract  The type classifier, if any, of the class
 */
@property (nonatomic, retain, readonly) id type;

/*!
 @property  essential
 @abstract  Whether or not this event classification is essential for the rule to run. YES means that RuleEngine must have the event available to even consider running the rule
 */
@property (nonatomic, readonly) BOOL essential;

/*! 
 Disntiguishes an eventTrigger classification that can enable a rule from one that can only trigger an already-running rule
 */
@property (nonatomic, readonly) BOOL activate;

/*!
 @method    eventTriggerClassificationForEventClass:type:description:
 @abstract  Constructor given the class the rule is interested in, and an optional type classifier (arbitrary object)
 */
+ (TriggerEventRuleClassification *)eventTriggerClassificationForEventClass:(Class)eventClass type:(id)type essential:(BOOL)essential;

/*!
    Constructor for the classification
    @param eventClass The event class for this event trigger
    @param type The type classifier
    @param essential Whether or not this event classification is essential
    @param activate Disntiguishes an eventTrigger classification that can enable a rule from one that can only trigger an already-running rule
    @return The new TriggerEventClassification
 */
+ (TriggerEventRuleClassification *)eventTriggerClassificationForEventClass:(Class)eventClass type:(id)type essential:(BOOL)essential activate:(BOOL)activate;

@end
