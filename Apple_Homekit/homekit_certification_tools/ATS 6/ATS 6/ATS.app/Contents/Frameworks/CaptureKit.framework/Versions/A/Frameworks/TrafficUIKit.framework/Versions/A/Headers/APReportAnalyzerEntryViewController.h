//
//  APReportAnalyzerEntryViewController.h
//  TrafficKit
//
//  Created by Wesley McCloy on 11/1/18.
//  Copyright © 2018 Apple Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class APReportAnalyzerEntry;

NS_ASSUME_NONNULL_BEGIN

@interface APReportAnalyzerEntryViewController : NSViewController <NSMenuDelegate>

@property (nonatomic, strong) NSTableView *tableView;

/**
 @return An array of entries which are either selected or clicked on during a contextual menu invocation. These are the entries that the user is focused on. Includes child entries.
 */
- (NSArray<APReportAnalyzerEntry *> *)targetEntries;

/**
 Use this to find the Entry for the given row in the table view. Accomodates for when the row's item is actually a data source and not an entry
 
 @note Use this instead of `self.outlineView itemAtRow:`
 @param row The row index for the entry of interest
 @return The entry for the row, nil if invalid row or if the row is out of bounds
 */
- (APReportAnalyzerEntry *)entryAtRow:(NSInteger)row;

/**
 This is called by the First Responder and gets the pasteboard string for the entries that are either selected or clicked on
 */
- (IBAction)copy:(id)sender;

/**
 This is called by the First Responder and gets the raw bytes of entries that support providing raw data
 */
- (IBAction)copyRawBytes:(id)sender;

@end

NS_ASSUME_NONNULL_END
