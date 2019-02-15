//
//  TrafficViewController+Protected.h
//  ATS
//
//  Created by Bob Burrough on 12/7/2012
//  Copyright © 2012 Apple Inc. All Rights Reserved.
//

#import <CaptureKit/EventBroker.h>

/*!
 @category   TrafficViewController ()
 @abstract   Protected interface for the TrafficViewController class.  Must only
             be used by TrafficViewConroller or classes which inherit from TrafficViewController.
 */
@interface TrafficViewController () {
}

@property (nonatomic, retain) IBOutlet NSTableView *tableView;
@property (nonatomic, retain) IBOutlet NSSplitView *splitView;
@property (nonatomic, retain) IBOutlet NSOutlineView *detailsOutlineView;
@property (nonatomic, copy, readwrite) NSString *captureItemDescription;
@property (nonatomic, retain) DetailsViewTree *currentDetailsViewTree;

/*!
 @method    addPasteboardStringBlock:forColumn:
 @abstract  Registers a block to determine the CSV column string for a given event/column identifier
 @param     block
 The block to generate the string. This method will copy the block for you.
 @param     identifier
 The column identifier
 */
- (void)addPasteboardStringBlock:(NSString * (^)(CaptureEvent *))block forColumn:(NSString *)identifier;

- (void)updateDetailsViewForSelectedItem;
- (void)updateTimestampFieldWithFirstSelectedItem;

/*!
 @method        drainEvents:
 @abstract      Drain an array of events on the main queue
 @param         events
                Array of events to drain
 @discussion    This is called from -drainPendingEvents so that subclassers don't have to override both -drainPendingEvents and processEvent:
 */
- (void)drainEvents:(NSArray *)pendingEvents;

/*!
 @method    fontDidChangeToFont:
 @abstract  Notification that the font just changed to the passed font
 */
- (void)fontDidChangeToFont:(NSFont *)font;

/*!
 @method    timestampUnitsDidChangeWithFormatter:title:
 @abstract  Notification hook that the timestamp units just changed
 */
- (void)timestampUnitsDidChangeWithFormatter:(NSFormatter *)formatter title:(NSString *)title;

@end
