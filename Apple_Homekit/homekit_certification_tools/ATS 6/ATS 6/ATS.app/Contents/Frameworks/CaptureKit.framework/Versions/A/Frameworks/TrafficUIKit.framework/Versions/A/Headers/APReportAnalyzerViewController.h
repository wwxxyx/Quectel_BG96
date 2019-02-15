//
//  APReportAnalyzerViewController.h
//  APTS
//
//  Created by Mark Hamlin on 12/17/12.
//  Copyright (c) 2012 Apple. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class APReportAnalyzerEntry;
@class APReportAnalyzerFilter;
@class APReportAnalyzerViewController;
@class APReportAnalyzerTraceViewController;
@protocol APReportAnalyzerEntrySource;

@protocol APReportAnalyzerViewControllerDelegate

@optional

/*!
 @method    reportAnalyzerViewControllerSelectionChanged:
 @abstract  Notification that the selection just changed
 */
- (void)reportAnalyzerViewControllerSelectionChanged:(APReportAnalyzerViewController *)viewController;

/*!
 Used by the APReportAnalyzerViewController to tell the CaptureDocument that it
 has changed whether the details pane is showing or not.
 */
- (void)updateDetailsPaneState;

@end

@interface APReportAnalyzerViewController : NSSplitViewController<NSSplitViewDelegate>

/*!
 @method        reportAnalyzerUsingExpandableItems:
 @abstract      Create a report analyzer, specifying whether or not expandable items should be used
 @discussion    There is a significant performance hit for using expandable items, so they aren't default
                <rdar://problem/14261977> NSOutlineView is slow to add items as the number of root items increases
 */
+ (APReportAnalyzerViewController *)reportAnalyzerUsingExpandableItems:(BOOL)usesExpandableItems;

/*!
 @property  delegate
 @abstract  Delegate for our analyzer view
 */
@property (nonatomic, weak) NSObject<APReportAnalyzerViewControllerDelegate> *delegate;


/*!
 @property  trace
 @abstract  A reference to the trace view controller
 */

@property (nonatomic, strong) APReportAnalyzerTraceViewController *trace;

/*!
 @property  filter
 @abstract  The filter object used
 */
@property (readonly, nonatomic, strong) APReportAnalyzerFilter *filter;

/*!
 @property  elapsedTime
 @abstract  How much time, in seconds, has elapsed since the first timestamp
 */
@property (readonly, nonatomic, assign) double elapsedTime;

/*!
 @method    invalidate
 @abstract  Invalidate controller by cleaning up any timers, etc
 */
- (void)invalidate;

/*!
 @method    addEntrySources:
 @abstract  Add an array of report analyzer entry sources
 */
- (void)addEntrySources:(NSArray *)entrySources;

/*!
 @method        insertEntrySource:anchorTimestamp:aboveExisting:movable:beginEditing:
 @abstract      Inserts an entry which will be sorted relative to other existing entries
 @param         timestamp
                The timestamp that this should be anchored to
 @param         aboveExisting
                Whether or not the entry should be displayed above (YES) or below (NO) other entries with identical timestamps
 @param         movable
                Whether or not the entry should be movable by the user. Passing YES will allow the anchor timestamp and above/below preferences to be changed.
 */
- (void)insertEntrySource:(id<APReportAnalyzerEntrySource>)entrySource anchorTimestamp:(UInt64)timestamp aboveExisting:(BOOL)aboveExisting movable:(BOOL)movable beginEditing:(BOOL)beginEditing;

/*!
 @method    removeEntrySource:
 @abstract  Removes entry source from view
 */
- (void)removeEntrySource:(id<APReportAnalyzerEntrySource>)entrySource;

/*!
 @method    removeAllEntries
 @abstract  Removes all entries from the view
 */
- (void)removeAllEntries;

/*!
 @method    toggleDetailsPane
 @abstract  Toggles visibility of details pane
 */
- (void)toggleDetailsPane;

/*!
 @method    isDetailsPaneShowing
 @abstract  Returns whether or not the details pane is currently showing
 */
- (BOOL)isDetailsPaneShowing;

/*!
 @method    reloadDetailsPaneContent
 @abstract  Clears the cache of report analyzer entries and reloads the details 
            sidebar.
 */
- (void)reloadDetailsPaneContent;

/*!
 @method    synchronize
 @abstract  Synchronizes threaded state (accumulator queue, filter?). This is useful to call before some operation which assumes a stable state
 */
- (void)synchronize;

/*!
 @method    selectedEntries
 @abstract  Returns array of currently selected entries
 */
- (NSArray<APReportAnalyzerEntry *> *)selectedEntries;

/*!
 @method    rootSelectedEntry
 @abstract  Returns a single selected entry, if any
 */
- (APReportAnalyzerEntry *)rootSelectedEntry;

/*!
 @method    addColumnWithAuxiliaryValueSelector:title:index:exampleValue:
 @abstract  Adds a table column which invokes the selector on the source (if it responds to it) to obtain the object value, and moves that column to the passed index
 @param     index
            Index to move to, or negative if it should just go at the end
 @param     exampleValue
            A value which demonstrates the maximum length that this will reasonably display, so that the width can be calculated
 */
- (void)addColumnWithAuxiliaryValueSelector:(SEL)selector title:(NSString *)title index:(NSInteger)index exampleValue:(id)exampleValue;

/*!
 @method    setFormatter:forColumnWithKey:
 @abstract  Sets a formatter for the column with the given key
 */
- (void)setFormatter:(NSFormatter *)formatter forColumnWithKey:(NSString *)key;

/*!
 @method    setTitle:forColumnWithKey:
 @abstract  Sets the column title for the column with the given key
 */
- (void)setTitle:(NSString *)title forColumnWithKey:(NSString *)key;

/*!
 @method    setReportFont:
 @abstract  Sets the font used to display reports
 */
- (void)setReportFont:(NSFont *)font;

/*!
 @method    selectAndScrollToEntryClosestToTimestamp:
 @abstract  Selects the closest item to the timestamp and scrolls to it
 */
- (void)selectAndScrollToEntryClosestToTimestamp:(uint64_t)timestamp;

/*!
 @method    jumpToNextRowPassingCondition:wrapAround:
 @abstract  Selects and scrolls to the next row which matches the passed block
 */
- (BOOL)jumpToNextRowPassingCondition:(BOOL(^)(id<APReportAnalyzerEntrySource> source))block wrapAround:(BOOL)wrapAround limitOnFailure:(BOOL)limitOnFailure;

/*!
 @method    jumpToPreviousRowPassingCondition:wrapAround:
 @abstract  Selects and scrolls to the last row which matches the passed block
 */
- (BOOL)jumpToPreviousRowPassingCondition:(BOOL(^)(id<APReportAnalyzerEntrySource> source))block wrapAround:(BOOL)wrapAround limitOnFailure:(BOOL)limitOnFailure;

@end
