//
//  TrafficEventTableViewDelegate.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 7/21/11.
//  Copyright 2011 Apple Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class TrafficEventTableView;

@protocol TrafficEventTableViewDelegate <NSTableViewDelegate>

@optional
- (NSMenu *)tableView:(TrafficEventTableView *)tableView menuForTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)tableRow;

@end
