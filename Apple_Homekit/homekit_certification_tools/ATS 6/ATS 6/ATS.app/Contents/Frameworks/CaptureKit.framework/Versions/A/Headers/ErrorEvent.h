//
//  ErrorEvent.h
//  ATSMacApp
//
//  Created by Marin Balde on 8/27/10.
//  Copyright 2010 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureEvent.h>
#import <CaptureKit/DisplayProtocols.h>
#import <CaptureKit/EventIndirectlySubscribable.h>

#define kErrorEventNoSpecsLoadedLink @"ErrorEventNoSpecsLoadedLink"

@interface ErrorEvent : CaptureEvent <iAPPacketDisplay, ReportAnalyzerDisplayProtocol, EventIndirectlySubscribable>

/**
 Underlying NSError backing this error event.
 */
@property (nonatomic, strong) NSError *error;

@property (nonatomic, readonly) NSSet<CaptureEvent *> *relatedEvents;

/**
 Processes the spec entry into a set of fields ([key: value, key: value, …])
 that can be displayed in a key-value detail view section. SpecificationEntry
 provides the core pieces of information to display, and this method post-processes
 them to ensure they are suitable for the details sidebar.
 
 @note When there is no valid spec reference, a place holder key-value string is returned ([Section: --, Source: --, …]) 
 */
@property (nonatomic, readonly) NSArray<NSString *> *specReferenceFields;

+ (instancetype)errorEventWithTimestamp:(UInt64)timestamp
                        error:(NSError *)error;

/*!
    calls the super method dictionaryDescription and removes UniqueID, if the contained error is a RulerError also contains RuleName, Code and Description.
    @return the dictionary represention for this ErrorEvent
 */
- (NSMutableDictionary *)dictionaryDescription;

/*!
    calls dictionaryDescription and includes dicionary representations for the RuleError's events
    @return the dictionary represention for this ErrorEvent with dictionary representaions for the RuleError's contained events
 */
- (NSMutableDictionary *)dictionaryDescriptionWithSubscribers:(NSArray *)subscribers;

@end
