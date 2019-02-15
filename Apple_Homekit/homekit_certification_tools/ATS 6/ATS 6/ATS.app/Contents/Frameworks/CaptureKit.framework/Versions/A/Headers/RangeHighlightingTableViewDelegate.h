//
//  RangeHighlightingTableViewDelegate.h
//  ATS
//
//  Created by Joe Basu on 8/27/09.
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <Cocoa/Cocoa.h>

@class RangeHighlightingTableView;

@protocol RangeHighlightingTableViewDelegate <NSTableViewDelegate>

@optional
- (NSColor *)rangeHighlightingTableView:(RangeHighlightingTableView *)tableView highlightColorForRow:(NSUInteger)rowIndex;

@end
