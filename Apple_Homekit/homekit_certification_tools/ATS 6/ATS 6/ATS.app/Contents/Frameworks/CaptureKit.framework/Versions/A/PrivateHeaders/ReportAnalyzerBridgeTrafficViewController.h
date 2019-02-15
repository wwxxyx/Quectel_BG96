//
//  ReportAnalyzerBridgeTrafficViewController.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 5/15/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/TrafficViewController.h>
#import <TrafficUIKit/TrafficUIKit.h>
#import <CaptureKit/CaptureEvent+Protected.h>
#import <CaptureKit/ImageCache.h>

@interface ReportAnalyzerBridgeTrafficViewController : TrafficViewController

/*!
 @property  reportAnalyzer
 @abstract  The report analyzer instance
 */
@property (readonly, nonatomic, retain) APReportAnalyzerViewController *reportAnalyzer;

/*!
 Create a traffic view controller with the passed category title and an array of event classes it should display.
 
 @param title Title of the traffic view controller in the categories pane.
 @return Traffic View controller instance.
 */
+ (instancetype)trafficViewControllerWithTitle:(NSString *)title eventsClasses:(NSArray *)eventClasses;

/*!
 Create an traffic view controller with the passed category title and a dictionary of event classes to Channels. Event classes over channel that this TVC should display.
 
 @param title Title of the traffic view controller in the categories pane.
 @param eventsClassesOnChannelMap Dictionary with one–to–one map of event class and the channel it's carried over.
 @return Traffic View controller instance.
 */
+ (instancetype)trafficViewControllerWithTitle:(NSString *)title eventsClassesOnChannelMap:(NSDictionary <Class, id<EventChanneling> > *)eventsClassesOnChannelMap;

/*!
 @method    usesExpandableItems
 @abstract  Whether or not this TVC class uses expandable items
 @default   NO
 */
+ (BOOL)usesExpandableItems;

/*!
 @method    jumpToNextEventMatchingText:ignoreCase:wrapAround:
 @abstract  Jumps to the next event which matches the passed text
 */
- (BOOL)jumpToNextEventMatchingText:(NSString *)text ignoreCase:(BOOL)ignoreCase wrapAround:(BOOL)wrapAround;

/*!
 @method    jumpToPreviousEventMatchingText:ignoreCase:wrapAround:
 @abstract  Jumps to the next event which matches the passed text
 */
- (BOOL)jumpToPreviousEventMatchingText:(NSString *)text ignoreCase:(BOOL)ignoreCase wrapAround:(BOOL)wrapAround;

/*!
 Via First Responder — invoked via delete key-press or Menu Item — removes the selected annotation
 */
- (IBAction)delete:(id)sender;

@end
