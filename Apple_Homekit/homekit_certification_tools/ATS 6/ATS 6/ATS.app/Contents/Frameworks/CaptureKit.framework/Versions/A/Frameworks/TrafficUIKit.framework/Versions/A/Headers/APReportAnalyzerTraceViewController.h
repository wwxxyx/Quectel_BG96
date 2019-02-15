//
//  APReportAnalyzerTraceViewController.h
//  TrafficKit
//
//  Created by Mark Hamlin on 6/25/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "APReportAnalyzerEntry.h"
#import "APReportAnalyzerEntryViewController.h"

@class APReportAnalyzerTraceViewController;

@protocol APReportAnalyzerTraceDataSource

/*!
 @method    displayedSources
 @abstract  Returns an array of the sources which should be displayed
 */
- (NSArray *)displayedSources;

/*!
 @method    doubleClickedEntry:inTrace:
 @abstract  Callbcak for when the user double clicks an entry
 */
- (void)doubleClickedEntry:(APReportAnalyzerEntry *)entry inTrace:(APReportAnalyzerTraceViewController *)trace;

/*!
 @method    trace:highlightColorForEntry:
 @abstract  Returns the highlight color, if any, for the passed entry
 */
- (NSColor *)trace:(APReportAnalyzerTraceViewController *)trace highlightColorForEntry:(APReportAnalyzerEntry *)entry;

/*!
 @method    traceSelectionIsChanging:
 @abstract  Notification that selection is changing in trace
 */
- (void)traceSelectionIsChanging:(APReportAnalyzerTraceViewController *)trace;

/*!
 @method    traceSelectionDidChange:
 @abstract  Notification that selection changed in trace
 */
- (void)traceSelectionDidChange:(APReportAnalyzerTraceViewController *)trace;

/*!
 @method    traceWillDisplay:
 @abstract  Notification that the trace will redraw
 */
- (void)traceWillDisplay:(APReportAnalyzerTraceViewController *)trace;

- (CGFloat)trace:(APReportAnalyzerTraceViewController *)trace heightOfRowByItem:(id)item;
- (NSInteger)trace:(APReportAnalyzerTraceViewController *)trace numberOfChildrenOfItem:(id)item;
- (id)trace:(APReportAnalyzerTraceViewController *)trace child:(NSInteger)index ofItem:(id)item;
- (BOOL)trace:(APReportAnalyzerTraceViewController *)trace isItemExpandable:(id)item;
- (id)trace:(APReportAnalyzerTraceViewController *)trace objectValueForTableColumn:(NSTableColumn *)tableColumn byItem:(id)item;
- (void)trace:(APReportAnalyzerTraceViewController *)trace setObjectValue:(id)object forTableColumn:(NSTableColumn *)tableColumn byItem:(id)item;
- (BOOL)trace:(APReportAnalyzerTraceViewController *)trace isGroupItem:(id)item;
- (NSCell *)trace:(APReportAnalyzerTraceViewController *)trace dataCellForTableColumn:(NSTableColumn *)tableColumn item:(id)item;
- (void)trace:(APReportAnalyzerTraceViewController *)trace willDisplayElement:(id)element withTableView:(NSTableView *)tableView forTableColumn:(NSTableColumn *)tableColumn item:(id)item row:(NSInteger)row;

@end

@interface APReportAnalyzerTraceViewController : APReportAnalyzerEntryViewController

@property (nonatomic, weak) id<APReportAnalyzerTraceDataSource> dataSource;

/*!
 Whether or not the table view needs to show the delta timestamps vs the normal timestamps
 */
@property (nonatomic, readonly) BOOL isDeltaTimestampModeOn;

/**
 Call when the document-level timestamp mode has changed. This is a result of the toolbar button

 @param deltaTimestampModeOn Whether delta timestamps or normal timestamps should be shown
 */
- (void)documentLevelDeltaTimestampToggled:(BOOL)deltaTimestampModeOn;

/*!
 @method        selectedEntry
 @abstract      Returns the selected entry, if any
 @discussion    This is not necessarily the root item
 */
- (APReportAnalyzerEntry *)selectedEntry;

/*!
 @method        selectedRootEntries
 @abstract      Returns an array of root items which are selected (or who have their children selected)
 */
- (NSArray *)selectedRootEntries;

/*!
 @method    rootForEntry:
 @abstract  Returns the root entry for the passed entry
 */
- (APReportAnalyzerEntry *)rootForEntry:(APReportAnalyzerEntry *)entry;

/*!
 The root for the selected child (if it's a child, otherwise just the selected entry)
 Note: Only updated once a selection has been completed. (Was previously constantly updated during a selection)
 */
@property (nonatomic, strong) APReportAnalyzerEntry *rootSelectedEntry;

/*!
 @method    entryForItem:
 @abstract  Given an item, return the entry
 */
- (APReportAnalyzerEntry *)entryForItem:(id)item;

/*!
 @method    sourceForItem:
 @abstract  Given an item, return the source
 */
- (id<APReportAnalyzerEntrySource>)sourceForItem:(id)item;

/*!
 @method    itemUnderMouse
 @abstract  Returns the item which is under the mouse, or nil
 */
- (id)itemUnderMouse;

/*!
 @method    selectAndScrollToItem:
 @abstract  Selects and scrolls to the passed item
 */
- (void)selectAndScrollToItem:(id)item;

/*!
 @method    selectAndScrollToItem:
 @abstract  Selects and scrolls to the passed items
 */
- (void)selectAndScrollToItems:(id)items;

/*!
 @method    beginEditingOnItem:
 @abstract  Tells the table/outline view to edit the passed item
 */
- (void)beginEditingOnItem:(id)item;

/*!
 @method    isItemVisible:
 @abstract  Returns whether or not the passed item is visible
 */
- (BOOL)isItemVisible:(id)item;

/*!
 Tells the table/outline view that it needs to be redisplayed on the next run loop.
 
 Note: This is a costly operation that should be avoided, it is recommended to directly reload specific rows with `redisplayEntries`
 */
- (void)redisplay;

/*!
 Redraws the specific entries given. Use this instead of `redisplay` if you don't want to redisplay the whole table
 
 @param entrySources The entries to redisplay (the sources of these entries to be specific)
 */
- (void)redisplayEntrySources:(NSArray *)entrySources;

/*!
 Redraw the specific row indexes
 
 @param indexSet The indexes of the rows to redisplay
 */
- (void)redisplayRowIndexes:(NSIndexSet *)indexSet;

/*!
 Must be called every time a selection change has occurred. This handles setting the `rootSelectedEntry` and informs the dataSource of the selection change
 */
- (void)handleTraceSelectionChange;

/*!
 @method    reloadRowHeights
 @abstract  Triggers trace to reload all row heights
 */
- (void)reloadRowHeights;

/*!
 @method    reloadIndexes:
 @abstract  Triggers a reload of the passed indexes. If nil, will reload everything
 */
- (void)reloadIndexes:(NSIndexSet *)indexSet;

/*!
 Use this to determine the highlighting for a given row. It looks at the currently selected row's related entries to determine if the color is nonnull. 
 
 @param view The view that is sending this message
 @param row The row that needs to be colored
 @return The color of the row should it be highlighted
 */
- (NSColor *)reportAnalyzerOutlineView:(APReportAnalyzerTraceViewController *)view highlightColorForRow:(NSInteger)row;

/*!
 Returns the indentation level for a given row.
 For outline view: The levels are zero-based—that is, the first level of displayed items is level 0.
 For table view: The indentation level for tow is always 0, because there's no indentation like in outline view.
 
 @param row The index of a row in the receiver.
 @return The indentation level for row. For an invalid row, returns –1.
 */
- (NSInteger)levelForRow:(NSInteger)row;

/*!
 Use to determine if the item (backing the row) is selected in the table.
 
 @param item The backing item for the row (commonly an APReportAnalyzerEntry)
 @retrun Whether the row for that item is selected
 */
- (BOOL)isItemSelected:(id)item;

/*!
 Convenience function to reload only the rows that are currently selected
 */
- (void)redisplaySelectedRows;

/**
 Delegate method that is part of the two protocols: APReportAnalyzerOutlineViewDelegate, APReportAnalyzerTableViewDelegate.
 Exposed here so that the subclasses can use the common implementation. This complexity is necessary because the OutlineView and TableView do not share the same delegate protocols.
 */
- (void)tableView:(NSTableView *)sender modifierFlagsChanged:(NSEventModifierFlags)modifierFlags;

/**
 An outlet to the menu that is invoked on the tableView. Exposed in the header so that subclasses can have access to it for modification.
 */
@property (strong) IBOutlet NSMenu *contextualMenu;

@end
