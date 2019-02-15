//
//  TrafficViewController.h
//  ATS
//
//  Created by Joe Basu on 1/15/10.
//  Copyright © 2010 Apple Inc. All Rights Reserved.
//

#import <Cocoa/Cocoa.h>
#import <CoreMedia/CoreMedia.h>
#import <CaptureKit/EventBroker.h>
#import <CaptureKit/TrafficEventTableViewDelegate.h>
#import <CaptureKit/RangeHighlightingTableViewDelegate.h>
#import <CaptureKit/DetailsViewTreeDelegate.h>

@class CaptureDocument;
@class CaptureEvent;
@class TrafficAttribute;
@class CaptureTransport;

@interface TrafficViewController : NSViewController <EventSubscribing, TrafficEventTableViewDelegate, NSOutlineViewDataSource, NSOutlineViewDelegate, DetailsViewTreeDelegate, RangeHighlightingTableViewDelegate>

@property (nonatomic, assign) CaptureTransport *transport;

@property (nonatomic, assign) IBOutlet CaptureDocument *captureDocument;
@property (nonatomic, retain) IBOutlet NSArrayController *arrayController;
@property (nonatomic, copy, readonly) NSString *captureItemDescription;
@property (nonatomic, copy, readonly) NSString * (^pasteboardStringBlock)(CaptureEvent *event);
@property (nonatomic, retain, readonly) NSColor *appleDeviceTrafficColor;
@property (nonatomic, retain, readonly) NSColor *accessoryTrafficColor;

@property (nonatomic, readwrite, assign) NSUInteger numDisplayedWarnings;
@property (nonatomic, readwrite, assign) NSUInteger numDisplayedErrors;

/*!
 @property  subscribedToBroker
 @abstract  Whether or not this TVC has already subscribed to the document's broker
 */
@property (nonatomic, readonly, assign) BOOL subscribedToBroker;

/*!
 @property  shouldDisplayTrafficCategoryStatusIcon
 @abstract  Returns whether the Traffic View will display the status icon next
            to its category name in the outline view.
 @discussion Override in subclasses to change this behavior. Default is YES.
 */
@property (nonatomic, readonly) BOOL shouldDisplayTrafficCategoryStatusIcon;

/*!
 @method    invalidate
 @abstract  Clean up any asynchronous tasks and prepare for a smooth dealloc
 */
- (void)invalidate;

- (void)selectClosestItemToEnteredTimestamp:(uint64_t)enteredTimestamp;
- (void)scrollToSelectedItem;
- (BOOL)isScrollerPinnedToBottom;
- (void)selectAndScrollToIndex:(NSUInteger)index;
- (void)processEvent:(CaptureEvent *)event;
- (void)drainPendingEvents;         // FIXME: drainPendingEvents should be private, as this is an implementation detail of TrafficViewController

/*!
 @method    findEventsExactMatchingTimeStampsAndType
 @abstract  This function returns an array of CaptureEvents that occured at the
            provided timestamp and given the specific type
 @param     targetTimestamp The timestamp used to find the event
 @param     eventClass The class name of the event (subclass of CaptureEvent)
 */
- (NSArray<CaptureEvent *> *)findEventsExactMatching:(UInt64)targetTimestamp andType:(NSString *)eventClassName;

/*!
 @method    findEventsApproxMatchingTimeStampsAndType
 @abstract  This function returns an array of CaptureEvents that occured close to the
            provided timestamp and given the specific type. (5ms threshold if no exact match)
 @param     targetTimestamp The timestamp used to find the event
 @param     eventClassName The class name of the event (subclass of CaptureEvent)
 */
- (NSArray<CaptureEvent *> *)findEventsApproxMatching:(UInt64)targetTimestamp andType:(NSString *)eventClassName;

/*!
 @method    jumpToEvents
 @abstract  Scrolls to and selects the specified events
 @param     events The CaptureEvent array that you want to jump to
*/
- (void)jumpToEvents:(NSArray<CaptureEvent *> *)events; // Q: is this a duplicate of [TrafficViewController selectAndScrollToItem:]? A: Not exactly, this also updates the timestamp field and makes the table view the first responder. This is used by the timing inspector and Facets.

- (void)clearDisplayedEvents;
- (void)addError:(NSError *)error withTimestamp:(UInt64)timestamp; // TODO: Not to be used
- (void)setAppleDeviceTrafficColor:(NSColor *)color;
- (void)setAccessoryTrafficColor:(NSColor *)color;
- (void)scrollToEnd;

/**
 Takes the time parameter, converts it into the capture timestamp ratio using the first CA event deteced and scrolls the traffic view to the first 
 event that happens before the time value.
 @param time The time value for which the first event immediately before should be selected.
 */
- (void)selectAndScrollToFirstEventBeforeVideoTime:(CMTime)time;

/*!
 @method    selectedEvent
 @abstract  Return the event at the selected row, if any
 */
- (CaptureEvent *)selectedEvent;

/*!
 @method    trafficViewTextFieldCellColorForEvent:
 @abstract  Returns the color for the current text field cell representing the given event.
 @param     event
            The event the text field cell represents.
 @abstract  Subclasses should override this method to provide custom colors for text field
            cells for given events. Return nil to use the default color.
 */
- (NSColor *)trafficViewTextFieldCellColorForEvent:(CaptureEvent *)event;

/*!
 @method    subscribeToEventsFromBroker:
 @abstract  Opportunity for subclasses to subscribe to the events they'd like to display
 */
- (void)subscribeToEventsFromBroker:(EventBroker *)eventBroker;

/*!
 Convenience method to add a subscription for our document's event broker.
 
 @param eventClass Event class to subscriber for. Event channel is default channel.
 */
- (void)subscribeProcessEventForClass:(Class)eventClass;

/*!
 Convenience method to add a subscription for our document's event broker.
 
 @param eventClass Event class to subscriber for.
 @param channel Event channel for this event. Only events of class `eventClass` and on event channel `channel` will be published to this TVC.
 */
- (void)subscribeProcessEventForClass:(Class)eventClass onChannel:(id<EventChanneling>)channel;

/*!
 Method to update system timestamp formatter when there's one available. CaptureDocument updates this formatter whenever there is system timestamp information available.
 
 @param foramtter Instance of SystemTimestampFormatter eventually set as formatter for TableView cells for column with identifer @"system_timestamp"
 
 @note: During a live capture: Each capture transport builds the system timestamp information on receiving the first event on the transport this TVC belongs to.
 During a trace reload: Capture document sets this property from the disk, which it stored when saving this trace.
 */
- (void)updateSystemTimestampFormatter:(NSFormatter *)formatter;

/*!
 @method    specificationAuxiliaryFilesDidChange:
 @abstract  Handles the event that the auxiliary files for SpecKit's specs
            changed, necessitating a detail view reload.
 */
- (void)specificationAuxiliaryFilesDidChange:(NSNotification *)notification;

@end

@interface TrafficViewController (DetailsView)

- (BOOL)isDetailsViewShowing;
- (void)toggleDetailsShowing;

@end

@interface TrafficViewController (Annotations)

- (void)selectNextAnnotation:(id)sender;
- (void)selectPreviousAnnotation:(id)sender;
- (void)insertAnnotation:(TrafficAttribute *)attrib;
- (void)removeAnnotationAction:(NSMenuItem *)sender;
- (void)removeAnnotation:(TrafficAttribute *)attrib;
- (void)insertSeparatorAboveCurrentlySelectedRow;
- (void)insertSeparatorBelowCurrentlySelectedRow;
- (void)insertCommentAboveCurrentlySelectedRow;
- (void)insertCommentBelowCurrentlySelectedRow;
- (void)insertSeparatorAboveAction:(NSMenuItem *)sender;
- (void)insertSeparatorBelowAction:(NSMenuItem *)sender;
- (void)insertCommentAboveAction:(NSMenuItem *)sender;
- (void)insertCommentBelowAction:(NSMenuItem *)sender;
- (void)removeAttributesFromSuperviews;

@end

/** A category on TrafficViewController to handle error- and warning-specific actions. */
@interface TrafficViewController (ErrorsWarnings)

/** Causes the traffic view controller to scan forwards to the next row representing either an error event or an event with an error status. If no such event exists, the last event in the traffic view is selected. */
- (IBAction)selectNextError:(id)sender;

/** Causes the traffic view controller to scan forwards to the next row representing either an error event or an event with an error status. If no such event exists, the last event in the traffic view is selected. */
- (IBAction)selectPreviousError:(id)sender;

/** Causes the traffic view controller to scan forwards to the next row representing an event with a warning status. If no such event exists, the last event in the traffic view is selected. */
- (IBAction)selectNextWarning:(id)sender;

/** Causes the traffic view controller to scan forwards to the next row representing an event with a warning status. If no such event exists, the last event in the traffic view is selected. */
- (IBAction)selectPreviousWarning:(id)sender;

@end

@interface TrafficViewController (Filtering)

/*!
 @method    wantsFilterField
 @abstract  Whether or not CaptureDocument should enable filter input for us
 @default   YES
 */
- (BOOL)wantsFilterField;

/*!
 @method        applyFilter:
 @abstract      Filters all stored events and updates traffic table
 @param         filter
                Filter string to apply, or nil to use captureDocument.filterTextField.stringValue
 */
- (void)applyFilter:(NSString *)filter;

/*!
 @method        applyExclusionFilter:
 @abstract      Applies a filter string where matched events are removed
 @param         filter
                Exclusion filter string
 */
- (void)applyExclusionFilter:(NSString *)filter;

/*!
 @method        applyInclusionFilter:excluisionFilter:
 @abstract      Applies both an inclusion and exclusion filter at once
 */
- (void)applyInclusionFilter:(NSString *)inclusionFilter exclusionFilter:(NSString *)exclusionFilter;

/*!
 @method    terminateAndBlockOnCompletedFiltering
 @abstract  Blocks until the current filtering is completed
 */
- (void)terminateAndBlockOnCompletedFiltering;

- (void)schedulePostFilterBlock:(void (^)(void))block;
- (void)applyPostFilterBlocks;
- (void)registerEventFilterBlock:(BOOL (^)(CaptureEvent *))block;
- (void)unregisterEventFilterBlock:(BOOL (^)(CaptureEvent *))block;

@end
