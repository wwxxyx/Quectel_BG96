//
//  RangeHighlightingTableView.h
//  ATS
//
//  Created by Joe Basu on 8/12/09.
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <Cocoa/Cocoa.h>
#import <CaptureKit/RangeHighlightingTableViewDelegate.h>
#import <CaptureKit/TrafficEventTableView.h>

@interface RangeHighlightingTableView : TrafficEventTableView <RangeHighlightingTableViewDelegate>

@property (nullable, weak) id<RangeHighlightingTableViewDelegate> delegate;

@end
