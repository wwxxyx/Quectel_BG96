//
//  CaptureEvent+ReportAnalyzer.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 6/13/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureEvent+Protected.h>

#import <TrafficKit/TrafficKit.h>

@interface CaptureEvent ()

@property (nonatomic) BOOL hasResourcesLoaded;
@property (nonatomic, strong) NSThread *currentlySwappingOutThread;
@property (nonatomic) unsigned long long serializedResourceBlobListOffset;
@property (nonatomic) uint64_t queuedResourceLoadOperationSerial;

/*!
 @property  displayedFilterIndex
 @abstract  The index which this source lives at in the filter's display table
 */
@property (nonatomic, assign) NSInteger displayedFilterIndex;

#pragma mark -
#pragma mark Dependencies

/*!
 @property      identifier
 @abstract      Unique identifier for the event, used for determining event relationships. This gets passed along to APReportAnalyzerEntry's dependencyIdentifier
 */
@property (nonatomic, assign) NSUInteger identifier;

/*!
 @property      dependencyIdentifiers
 @abstract      Array of identifiers which are related to this event
 */
@property (nonatomic, retain) NSMutableArray *dependencyIdentifiers;

/*!
 @property      cachedReportAnalyzerEntry
 @abstract      The cached report analyzer entry, if any
 */
@property (nonatomic, retain) APReportAnalyzerEntry *cachedReportAnalyzerEntry;

@end

@interface CaptureEvent (ReportAnalyzer) <APReportAnalyzerEntrySource>

/*!
 @method    dependencies
 @abstract  Array of events which helped to trigger this event
 */
- (NSArray *)dependencies;

/*!
 @method    addDependencyEvent:
 @abstract  Tracks passed in event as a dependency
 @param     event
 Event or nil. If nil, this is a no-op
 */
- (void)addDependencyEvent:(CaptureEvent *)event;

/*!
 @method    addDependencyIdentifier:
 @abstract  Adds a dependency identifier
 */
- (void)addDependencyIdentifier:(NSUInteger)identifier;

#pragma mark -
#pragma mark Description

/*!
 @method    reportAnalyzerEntry
 @abstract  Generates and returns a report analyzer entry for this event
 */
- (APReportAnalyzerEntry *)reportAnalyzerEntry;

/*!
 @method    addReportAnalyzerEntryForControlMessage:toEntry:
 @abstract  Nest a report analyzer entry which represents a key/value control message from the Stark spec
 */
+ (APReportAnalyzerEntry *)addReportAnalyzerEntryForControlMessage:(NSDictionary *)message toEntry:(APReportAnalyzerEntry *)rootEntry;

#pragma mark -
#pragma mark Resource Management

/*!
 @method        resourceBlobList
 @abstract      Return an NSArray of resource blobs
 */
- (NSArray *)resourceBlobList;

/*!
 @method        unloadableResourceNames
 @abstract      Return an array of the names of resources which may be unloaded
 @discussion    Each of these methods will be swizzled so that resources can be reloaded on-demand
                Furthermore, each of these will be automatically unloaded by setting the value for the given name to nil
 */
+ (NSArray *)unloadableResourceNames;

/*!
 @method        loadResourcesFromBlobList:
 @abstract      Opportunity to restore resources given the blob list that was previously returned
 */
- (void)loadResourcesFromBlobList:(NSArray *)blobs;

@end
