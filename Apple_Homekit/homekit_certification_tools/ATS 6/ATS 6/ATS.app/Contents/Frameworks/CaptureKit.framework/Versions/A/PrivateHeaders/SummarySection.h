//
//  SummarySection.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 8/13/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <CaptureKit/CaptureEvent.h>
#import <CaptureKit/iAP1PacketEvent.h>
#import <CaptureKit/iAP2MessageEvent.h>
#import <CaptureKit/ATFRetHardwareInfoEvent.h>
#import <CaptureKit/NSColor+ATSAdditions.h>
#import <CaptureKit/EventBroker.h>

#define SUMMARY_VAL(val) (val ? val : @"--")
#define SUMMARY_COLOR(color) color ? [NSString stringWithFormat:@"color: %@;", [color htmlColorString]] : @""

@class SummaryGroupTrafficViewController;

@interface SummarySection : NSObject <EventSubscribing>

/*!
 @property  summaryGroup
 @abstract  The owning group
 */
@property (nonatomic, assign) SummaryGroupTrafficViewController *summaryGroup;

/*!
 @property  elementId
 @abstract  DOM element id. Simply used to lookup
 */
@property (nonatomic, retain) NSString *elementId;

/*!
 @method    section
 @abstract  Returns an autoreleased instance of this section
 */
+ (instancetype)section;

/*!
 @method    monitoredEventClasses
 @abstract  An array of all event classes which the section is interested in
 */
- (NSArray *)monitoredEventClasses;

/*!
 @method    processEvent:
 @abstract  Update for a certain event
 @param     event
            Info event
 */
- (void)processEvent:(CaptureEvent *)event;

/*!
 @method    processiAP1PacketEvent:
 @abstract  Convenience called by processEvent: for packet events
 */
- (void)processiAP1PacketEvent:(iAP1PacketEvent *)event;

/*!
 @method    processiAP2MessageEvent:
 @abstract  Convenience called by processEvent: for message events
 */
- (void)processiAP2MessageEvent:(iAP2MessageEvent *)event;

/*!
 @method    processATFRetHardwareInfoEvent:
 @abstract  Convenience called by processEvent: for hardware events
 */
- (void)processATFRetHardwareInfoEvent:(ATFRetHardwareInfoEvent *)event;

/*!
 @method    title
 @abstract  Title of the section
 */
- (NSString *)title;

/*!
 @method    htmlString
 @abstract  The content string to represent this section
 */
- (NSString *)htmlString;

/*!
 @method        handleEventWithName:
 @abstract      Handle an event reported by another summary section, such as iAP reset
 */
- (void)handleEventWithName:(NSString *)name;

#pragma mark -
#pragma mark Templating Convenience

/*!
 @method        templateString
 @abstract      Template HTML string
 @discussion    Optional override for subclassers
 @default       Attempts to load NSStringFromClass([self class]).html
 */
- (NSString *)templateString;

/*!
 @method        templateValues
 @abstract      Dictionary of values to replace in the template
 @discussion    Must be defined if templateString is overridden
                NOTE: If a key has the "$" prefix, then it is allowed to have html in the body
 */
- (NSDictionary *)templateValues;

/*!
 @method        templateStyles
 @abstract      Apply a style to template ids. For example, TEMPLATE_KEY => @"color: red;" will make that id red
 */
- (NSDictionary *)templateStyles;

@end
