//
//  CaptureTransport+Protected.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 5/8/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureTransport.h>
#import <CaptureKit/RuleEngineDelegate.h>
#import <CaptureKit/DeviceCaptureConfigurationTypes.h>
#import <CaptureKit/CaptureEvent+Protected.h>
#import <CaptureKit/EventChanneling.h>
#import <CaptureKit/HCIContainerEventsStorage.h>
#import <CaptureKit/ComprobeFrameEventsStorage.h>
#import <ATSImportKit/IKImportPacket.h>

@class CaptureGeoData;
@class Stenographer;
@class ATSRuleEngine;
@class TrafficViewController;
@class ATSCaptureProtocol;
@class RuleTestError;
@class ATSAppEvent;
@class CaptureConfiguration;
@class CaptureEvent;
@class TrafficAttribute;
@class ResourceSwapHandle;
@class MessageEvent;

@interface CaptureTransport () <RuleEngineDelegate>
@property (nonatomic, readwrite, weak) CaptureDocument *document;
@property (nonatomic, readwrite, strong) ATSRuleEngine *ruleEngine;
@property (nonatomic, readwrite, copy) NSArray *captureProtocols;
@property (nonatomic, readwrite, copy) NSArray *trafficViewControllers;
@property (nonatomic, readwrite) DeviceTransportType deviceTransportType;
@property (nonatomic, readwrite) BOOL shouldAccumulateDeliveredEvents;
@property (nonatomic, readonly, strong) CaptureGeoData *captureGeoData;

/**
 Instance of a stenographer / per capture transport that's required by the rule engine eventually.
 */
@property (nonatomic, strong) Stenographer *stenographer;

/*!
 @property  resourceSwapHandle
 @abstract  Handle for events/etc to temporarily swap resources in and out of
 */
@property (nonatomic, strong) ResourceSwapHandle *resourceSwapHandle;

/*!
 @method    canReadFromSavedTraceAtPath:
 @abstract  Whether or not the CaptureTransport of this class can read back from the directory
 passed as path. This uses +requiredFilenamesFromSavedTrace to determine it, and should
 probably not be subclassed
 */
+ (BOOL)canReadFromSavedTraceAtPath:(NSString *)path;

/**
 Method to restore initial system timestamp information from the saved trace.
 
 @param tempPath path to the unzipped directory
 @param error error if any
 */
- (void)restoreSystemTimestampInfoFromPath:(NSString *)tempPath;

#pragma mark - Traffic Insertion and Management

/*!
 @method    storeAnnotation:originator:
 @abstract  Stores an annotation
 */
- (void)storeAnnotation:(TrafficAttribute *)attribute
             originator:(TrafficViewController *)originator;

/*!
 @method    removeAnnotation:
 @abstract  Removes an annotation from storage
 */
- (void)removeAnnotation:(TrafficAttribute *)attribute;

/*!
 @method    recordRuleTestErrorAtLatestTimestamp:
 @abstract  Insert a rule test error into the TVC's at the latest timestamp
 */
- (void)recordRuleTestErrorAtLatestTimestamp:(RuleTestError *)error;

/*!
 @method    insertMessageAtLatestTimestamp:fromSource:recordToFile:
 @abstract  Inserts a generic message into the default traffic view
 @return The MessageEvent that was inserted
 */
- (MessageEvent *)insertMessageAtLatestTimestamp:(NSString *)message fromSource:(NSString *)source recordToFile:(BOOL)shouldRecordToFile;

/*!
 @method    insertMessageAtLatestTimestamp:fromSource:recordToFile:
 @abstract  Inserts a generic message into the default traffic view
 @return The MessageEvent that was inserted
 */
- (MessageEvent *)insertMessageAtLatestTimestamp:(NSString *)message fromSource:(NSString *)source severity:(MessageEventSeverity)severity recordToFile:(BOOL)shouldRecordToFile;

/*!
 @method    insertMessage:fromSource:atTimestamp:shouldRecordToFile:
 @abstract  Inserts a message at the given timestamp, from the given source
 @return The MessageEvent that was inserted
 */
- (MessageEvent *)insertMessage:(NSString *)message fromSource:(NSString *)source atTimestamp:(UInt64)timestamp recordToFile:(BOOL)shouldRecordToFile;

/*!
 @method    insertMessage:fromSource:atTimestamp:shouldRecordToFile:
 @abstract  Inserts a message at the given timestamp, from the given source
 @return The MessageEvent that was inserted
 */
- (MessageEvent *)insertMessage:(NSString *)message fromSource:(NSString *)source severity:(MessageEventSeverity)severity atTimestamp:(UInt64)timestamp recordToFile:(BOOL)shouldRecordToFile;

/*!
 @method    insertATSAppEvent:
 @abstract  Inserts an ATSAppEvent into this transport's TVC's
 */
- (void)insertATSAppEvent:(ATSAppEvent *)event;

/*!
 @method    clearTraffic
 @abstract  Instructs all TVCs to clear their traffic
 */
- (void)clearTraffic;

/*!
 @method    lastDeliveredEvents
 @abstract  Returns a copy of the events delivered since the last time this was called.
 */
- (NSArray *)lastDeliveredEvents;

/*!
 @method    deliverEvent:
 @abstract  Delivers an event to the appropriate receivers.
 
 @note: This method is effectively calling - (void)deliverEvent:(Event *)event onChannel:(id<EventChanneling>)channel; with nil channel. You should call super when done if you choose to override.
 */
- (void)deliverEvent:(CaptureEvent *)event;

/**
 Delivers an event to the appropriate receiver on a specific channel.
 
 @note Only subscribers with this channel will receive the event. If channel is nil, the event is delivered to every subscriber that cares about the event's class. You should call super when done if you choose to override.
 */
- (void)deliverEvent:(CaptureEvent *)event onChannel:(id<EventChanneling>)channel;

/*!
 @method    close
 @abstract  Clean up any resources, invalidate, unbind, etc
 */
- (void)close;

#pragma mark -
#pragma mark Subclassers - File Operations

/*!
 @method    key
 @abstract  The transport key used (short name, may be prepended to identifiers)
 */
+ (NSString *)key;

/*!
 @method    annotationsFilename
 @abstract  Return the filename for stored annotations
 @default   @"%@Annotations.plist", self.key
 */
+ (NSString *)annotationsFilename;

/*!
 @method    messagesFilename
 @abstract  Return the filename for stored messages
 @default   @"%@Messages", self.key
 */
+ (NSString *)messagesFilename;

/*!
 @method    requiredFilenamesFromSavedTrace
 @abstract  Returns an array of filenames which are required to read back this capture transport class.
 If all these files exist in a saved trace, CaptureDocument will attempt to read this transport back from disk
 */
+ (NSArray *)requiredFilesnamesFromSavedTrace;

/*!
 @method    baseReadFromPath:error:readProgressIndicator:
 @abstract  Read data for this transport back from a file. tempPath is the root directory for this
 @note      readFromPath* may still be called without required files if there are imported packets, so -hasImportedPackets should be queried
 */
- (BOOL)baseReadFromPath:(NSString *)tempPath error:(NSError **)error readProgressIndicator:(void (^)(float))readProgressIndicator;

/*!
 @method    writeToPath:error:
 @abstract  Write to the directory at tempPath
 */
- (BOOL)writeToPath:(NSString *)tempPath error:(NSError **)error;

/*!
 @method    notifyLoadCancelledEarly
 @abstract  Posts an error event notifying that the trace load was canceled
 */
- (void)notifyLoadCancelledEarly;

#pragma mark -
#pragma mark Subclassers - Loading Hints

/*!
 @method    willBeginLoading
 @abstract  A hint that the transport will begin to receive events. This should be implemented such that it's optional
 @example   Measurements overriding this to disable animation
 */
- (void)willBeginLoading;

/*!
 @method    didFinishLoading
 @abstract  A hint that the transport has finished heavily loading events. This should be implemented such that it's optional
 @example   Measurements overriding this to re-enable animation
 */
- (void)didFinishLoading;

/*!
 @method    flushFinalEvents
 @abstract  Opportunity to flush any events. This is called after capture or reprocess has ended when there should be no more transport events (so Messages can finish up)
 */
- (void)flushFinalEvents;

#pragma mark -
#pragma mark Subclassers - Live Capture Handling

/*!
 @method    updateToCaptureInactive
 @abstract  Invalidate capture, set to not-living (no more capture data should come in after this)
 */
- (void)updateToCaptureInactive;

/*!
 @method    beginCapture
 @abstract  Set up and begin the capture for this transport
 @return    Returns YES on success, NO on failure
 */
- (BOOL)beginCapture;

/*!
 @method    endCaptureWithError:
 @abstract  Stop the capture, disconnect any boxes. -updateToCaptureInactive will be called soon after this returns
 */
- (CaptureTransportEndCaptureStatus)endCaptureWithError:(NSError **)error;

/*!
 @method    endCaptureStatus
 @abstract  Called by CaptureDocument to check if capture ending is still pending from the transport.
 */
- (CaptureTransportEndCaptureStatus)endCaptureStatus;

#pragma mark -
#pragma mark Subclassers - Import Handling

/*!
 @method    hasImportedPackets
 @abstract  Whether or not any packets have been imported
 */
- (BOOL)hasImportedPackets;

/*!
 @method    canImportPacket:
 @abstract  Whether or not this transport wants to import the passed packet
 */
- (BOOL)canImportPacket:(IKImportPacket *)packet;

/*!
 @method    importPacket:
 @abstract  Import data from the given packet info dictionary. Super should be called to trace the import
 */
- (void)importPacket:(IKImportPacket *)packet;

@end

@interface CaptureTransport (Protected)

/*!
 @method    addTrafficViewController:
 @abstract  Subclassers can add traffic view controllers to the array of TVC's
 */
- (void)addTrafficViewController:(TrafficViewController *)tvc;

/*!
 @method    removeTrafficViewController:
 @abstract  Removes a traffic view controller
 */
- (void)removeTrafficViewController:(TrafficViewController *)tvc;

/*!
 Removes all traffic view controllers
 */
- (void)removeAllTrafficViewControllers;

/*!
 @method    addProtocol:
 @abstract  Adds a CaptureProtocol and handles additional registration (eg. adds TVC's)
 */
- (void)addProtocol:(ATSCaptureProtocol *)protocol;

/*!
 @method    defaultTrafficViewController
 @abstract  Returns the topmost (default) traffic view controller
 */
- (TrafficViewController *)defaultTrafficViewController;

/*!
 @method    notifyParserErrorWithLocalizedDescription:
 @abstract  Notifies listeners that a parser error occurred
 */
- (void)notifyParserErrorWithLocalizedDescription:(NSString *)localizedDescription;
- (void)notifyParserErrorWithLocalizedDescription:(NSString *)localizedDescription failureReason:(NSString *)localizedFailureReason;

/*!
 @method    readFromPath:error:readProgressIndicator:
 @abstract  Read data for this transport back from a file. tempPath is the root directory for this
 @note      readFromPath* may still be called without required files if there are imported packets, so -hasImportedPackets should be queried
 */
- (BOOL)readFromPath:(NSString *)tempPath error:(NSError **)error readProgressIndicator:(void (^)(float))readProgressIndicator;

#pragma mark -
#pragma mark Event Routing

/*!
 @property  shouldAccumulateDeliveredEvents
 @abstract  Whether or not events should be accumulated in deliverEvent:. The default is NO.
 This should only be set to YES if the -lastDeliveredEvents array is polled frequently
 */
@property (nonatomic, assign) BOOL shouldAccumulateDeliveredEvents;

#pragma mark -
#pragma mark Vbus Load Test

/*!
 @method    canRunVbusLoadTest
 @abstract  Whether or not this transport can run the Vbus load test
 */
- (BOOL)canRunVbusLoadTest;

/*!
 @method    runVbusLoadTestWithErrorMessage:description:timeout:
 @abstract  Run the Vbus load test. If error is encountered, return NO and assign approprite strings to the error message and description
 */
- (BOOL)runVbusLoadTestWithErrorMessage:(NSString **)errorMessage description:(NSString **)description timeout:(double *)timeout;

@end
