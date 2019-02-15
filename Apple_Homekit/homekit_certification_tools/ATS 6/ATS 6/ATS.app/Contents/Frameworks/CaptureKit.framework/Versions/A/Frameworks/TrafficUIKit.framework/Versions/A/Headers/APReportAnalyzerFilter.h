//
//  APReportAnalyzerFilter.h
//  APTS
//
//  Created by Mark Hamlin on 12/19/12.
//  Copyright (c) 2012 Apple. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "APReportAnalyzerEntry.h"

/*!
 @extern    kAPReportAnalyzerFilterDisplayedEntriesChangedNotificationName
 @abstract  Posted whenever dispalyed entries changes
 */
extern NSString *kAPReportAnalyzerFilterDisplayedEntriesChangedNotificationName;

@interface APReportAnalyzerFilter : NSObject

/*!
 @method    invalidate
 @abstract  Invalidate filter by cleaning up any timers, etc
 */
- (void)invalidate;

/*!
 @method    displayedEntrySources
 @abstract  Entry sources which should be displayed
 */
- (NSArray *)displayedEntrySources;

#pragma mark -
#pragma mark Entry Manipulation

/*!
 @method    appendEntrySources:
 @abstract  Append multiple entry sources
 */
- (void)appendEntrySources:(NSArray *)entrySources;

/*!
 @method    insertEntrySource:atIndex:
 @abstract  Inserts an entry source at the given index
 */
- (void)insertEntrySource:(id<APReportAnalyzerEntrySource>)source atIndex:(NSInteger)index;

/*!
 @method    removeEntrySource:
 @abstract  Removes entry source
 */
- (void)removeEntrySource:(id<APReportAnalyzerEntrySource>)source;

/*!
 @method    removeAllEntrySources
 @abstract  Removes all entry sources from the filter
 */
- (void)removeAllEntrySources;

/*!
 @method    clearEntryCaches
 @abstract  Clears the cache of every entry source, so that the information will 
            be generated fresh the next time it is requested.
 */
- (void)clearEntryCaches;

#pragma mark -
#pragma mark Filtering

/*!
 @method    applyInclusiveFilterString:
 @abstract  Includes entries with phrases from the filter string
 */
- (void)applyInclusiveFilterString:(NSString *)string;

/*!
 @method    applyExclusiveFilterString:
 @abstract  Excludes entries with phrases from the filter string
 */
- (void)applyExclusiveFilterString:(NSString *)string;

/*!
 @method    applyInclusiveFilterString:exclusiveFilterString:
 @abstract  Applies a new inclusive and exclusive filter string
 */
- (void)applyInclusiveFilterString:(NSString *)inclusiveFilterString exclusiveFilterString:(NSString *)exclusiveFilterString;

@end
