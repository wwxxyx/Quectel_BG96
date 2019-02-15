//
//  CaptureDocument+Protected.h
//  ATSMacApp
//
//  Created by Zachary Church on 7/27/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureDocument.h>
#import <CaptureKit/RuleTestsDelegate.h>
#import <CaptureKit/CaptureEvent+Protected.h>
#import <CaptureKit/RuleTestsDelegate.h>

@class TrafficViewController;
@class RuleTestsWindowController;
@class SelfCertWindowController;
@class ErrorSummaryIndicatorViewController;
@class RuleTestError;
@class SKProductPlan;
@class ProductPlan;
@class TimestampTextField;

@class CaptureDevice;
@class CaptureEvent;
@class EventBroker;
@class ATSCaptureImporter;

// Capture Transports
@class ATFR3CaptureTransport;
@class ATSBoxCaptureTransport;
@class USBCaptureTransport;
@class BluetoothCaptureTransport;
@class SummaryCaptureTransport;
@class ATSWiFiCaptureTransport;
@class ATSMikeyBusCaptureTransport;
@class PerformanceCaptureTransport;

@class CaptureVideoPlayer;
@class AVPlayerView;

@protocol VBusLoadTestDelegate
@required
- (void)didFinishUSBVbusLoadTestWithError:(BOOL)encounteredError;
@end

@interface CaptureDocument () <RuleTestsDocumentDelegate>

// Document state information

/**
 Tracks whether or not the data is being loaded from a trace file; rather than a live capture of data.
 */
@property (nonatomic) BOOL rebuildActive;
@property (nonatomic) BOOL importedFile;

@property (nonatomic) NSString *timestampStringWhenEditingBegan;
@property (nonatomic) BOOL inTimestampFieldEditing;

@property (weak) IBOutlet CaptureVideoPlayer *playerView;
@property (weak) IBOutlet CaptureVideoPlayer *playerViewWindow;

@property (nonatomic, assign) id<VBusLoadTestDelegate> vbusLoadTestDelegate;
@property (nonatomic, assign) IBOutlet NSWindow *videoWindow;

@property (nonatomic, copy) NSString *customDisplayName;
@property (nonatomic) BOOL shouldExcludeFromRecentDocumentList;

// Progress info
@property (nonatomic, retain) IBOutlet NSWindow *progressSheet;
@property (nonatomic, retain) IBOutlet NSProgressIndicator *progressIndicator;
@property (nonatomic, retain) IBOutlet NSTextField *loadingLabel;
@property (nonatomic, retain) IBOutlet NSButton *loadingCancelButton;
@property (nonatomic, assign) BOOL loadingCancelButtonPressed;

// Timing inspector state
@property (nonatomic, retain) CaptureEvent *startItem;
@property (nonatomic, retain) CaptureEvent *endItem;
@property (nonatomic, retain) TrafficViewController *startItemTrafficViewController;
@property (nonatomic, retain) TrafficViewController *endItemTrafficViewController;

/*!
 @property  savedClaims
 @abstract  Claims selected in the CA
 */
@property (nonatomic, copy) NSArray *savedClaims;

// Toolbar elements
@property (nonatomic, retain) IBOutlet NSToolbarItem *stopCaptureToolbarItem;
@property (nonatomic, retain) IBOutlet NSToolbarItem *clearCaptureToolbarItem;
@property (nonatomic, retain) IBOutlet NSToolbarItem *timestampToolbarItem;
@property (nonatomic, retain) IBOutlet NSToolbarItem *filterToolbarItem;
@property (nonatomic) IBOutlet NSToolbarItem *exclusionFilterToolbarItem;
@property (nonatomic, retain) IBOutlet NSSegmentedControl *viewToolbarSegmentItem;

/**
 This text field is populated when a table view entry (event) in a trace is selected. When you move from one TrafficView to another, this value is used to automatically scroll and select the closest event in this view with this timestamp value.
 */
@property (nonatomic, retain) IBOutlet TimestampTextField *timestampTextField;
@property (nonatomic, retain) IBOutlet NSTextField *filterTextField;
@property (nonatomic, retain) IBOutlet NSTextField *exclusionFilterTextField;
@property (nonatomic, retain) IBOutlet NSTextField *exclusionFilterLabel;

// Traffic elements
@property (nonatomic, retain) IBOutlet NSOutlineView *trafficCategoryOutlineView;
@property (nonatomic, retain) IBOutlet NSSplitView *mainDocumentSplitView;
@property (nonatomic, retain) IBOutlet NSSplitView *videoSplitView;
@property (nonatomic, retain) IBOutlet NSTextField *noTrafficLabelView;

// Bottom bar elements
@property (nonatomic, retain) IBOutlet NSImageView *captureDevicePowerLEDImageView;
@property (nonatomic, retain) IBOutlet NSImageView *captureDeviceHostLEDImageView;
@property (nonatomic, retain) IBOutlet NSImageView *captureDeviceActiveLEDImageView;
@property (nonatomic, retain) IBOutlet NSTextField *elapsedTimeLabelTextField;
@property (nonatomic, retain) IBOutlet NSTextField *elapsedTimeValueTextField;
@property (nonatomic, retain) IBOutlet NSTextField *boxInfoLabelTextField;
@property (nonatomic, retain) IBOutlet NSView *errorSummaryContainerView;
@property (nonatomic, retain) ErrorSummaryIndicatorViewController *errorSummaryIndicatorController;

// Auxiliary window controllers
@property (nonatomic, retain) SelfCertWindowController *selfCertWindowController;
@property (nonatomic, retain) RuleTestsWindowController *ruleTestsWindowController;

@property (nonatomic) BOOL forceSaveAs;

// computed properties used for binding selection and availability of UI elements
@property (nonatomic, readonly) BOOL trafficViewsHidden;

/*!
 @property  isLoadTestRunning
 @abstract  Whether or not there is a current load test in progress
 */
@property (nonatomic, assign) BOOL isLoadTestRunning;

/*!
 @property  errorEncounteredDuringLoadTest
 @abstract  Set by CaptureDocument if an error was encountered. Reset by the load test starting.
 */
@property (nonatomic, assign) BOOL errorEncounteredDuringLoadTest;

/*!
 @property  loadTestTimer
 @abstract  Timer for the load test
 */
@property (nonatomic, retain) NSTimer *loadTestTimer;

/*!
 @property      productPlan
 @abstract      The CA's product plan loaded from .ats or from the selfCertWindowController
 @discussion    This allows us to re-load the PP for a trace, instead of using legacy savedClaims
 */
@property (nonatomic, retain) ProductPlan *productPlan;

@property (readonly) BOOL popoutVideoButtonEnabled;

#pragma mark -
#pragma mark Capture Transports

/*!
 @property  captureTransports
 @abstract  Array of all active capture transports
 */
@property (nonatomic, strong) NSArray <CaptureTransport *> *captureTransports;

/*!
 Capture transport for the ATS Lightning box
 */
@property (nonatomic, strong) ATFR3CaptureTransport *atsLightningBoxCaptureTransport;

/*!
 Capture transport for the ATS Smart Connector box; because Smart Connector box uses the same underlying protocol as Lightning box
 */
@property (nonatomic, strong) ATFR3CaptureTransport *atsSmartConnectorBoxCaptureTransport;

/*!
 @property  atsBoxCaptureTransport
 @abstract  Capture transport for the ATS box
 */
@property (nonatomic, strong) ATSBoxCaptureTransport *atsBoxCaptureTransport;

/*!
 @property  usbCaptureTransport
 @abstract  Capture transport for the Beagle box
 */
@property (nonatomic, strong) USBCaptureTransport *usbCaptureTransport;

/*!
 @property  bluetoothCaptureTransport
 @abstract  Capture transport for bluetooth imports (no capture available)
 */
@property (nonatomic, strong) BluetoothCaptureTransport *bluetoothCaptureTransport;

/*!
 Capture transport for WiFi data
 */
@property (nonatomic, readwrite, retain) ATSWiFiCaptureTransport *WiFiCaptureTransport;

/*!
 Capture transport for MikeyBus data for ATS Lightning Audio Box
 */
@property (nonatomic, readwrite, retain) ATSMikeyBusCaptureTransport *mikeyBusCaptureTransport;

/*!
 Capture transport for simulated events created to test UI performance
 */
@property (nonatomic, strong) PerformanceCaptureTransport *performanceCaptureTransport;

/*!
 @property  summaryCaptureTransport
 @abstract  Capture transport which captures nothing, but displays summaries and measurements
 */
@property (nonatomic, strong) SummaryCaptureTransport *summaryCaptureTransport;

//Unique Id set for each event in the current document
@property (nonatomic) UInt64 uniqueEventID;

#pragma mark -
#pragma mark Traffic View Handling

/*!
 @method    refreshTrafficViews
 @abstract  Drain events and update toolbar state for the active traffic view
 */
- (void)refreshTrafficViews;

/*!
 @method    resetTrafficCategoryOutlineView
 @abstract  Reload traffic categories and update selections and active traffic view
 */
- (void)resetTrafficCategoryOutlineView;

/*!
 @method    updateCaptureTransportsArray
 @abstract  Updates the captureTransports array to reflect which transports are currently set
 */
- (void)updateCaptureTransportsArray;

#pragma mark -
#pragma mark Configuration and Status

/*!
 @method    readFromURL:ofType:headless:error:
 @abstract  Reads a document
 @param     absoluteURL
 URL of the doucment
 @param     typeName
 Type of document to read
 @param     headless
 Whether or not the document should be read synchronously without requiring the UI.
 @param     error
 The error if the read fails.
 */
- (BOOL)readFromURL:(NSURL *)absoluteURL ofType:(NSString *)typeName headless:(BOOL)headless error:(NSError **)error;
- (BOOL)readFromURL:(NSURL *)absoluteURL ofType:(NSString *)typeName headless:(BOOL)headless synchronous:(BOOL)synchronous error:(NSError **)error;

/*!
 @method    updateDataPresent
 @abstract  Mark the trace as having data
 */
- (void)updateDataPresent;

/*!
 @method    updateToCaptureInactive
 @abstract  Invalidates the capture, such that no more events are expected to be seen
 */
- (void)updateToCaptureInactive;

/*!
 @method    updateInternalLabel
 @abstract  Reload the internal (bottom right) label
 */
- (void)updateInternalLabel;

/*!
 Called by capture transports when a pending capture has finished.
 @param     error
 Set if there was an capture transport error.
 */
- (void)captureTransportDidEndPendingCaptureWithError:(NSError *)error;

#pragma mark -
#pragma mark Trace loading status

/*!
 @method    beginProgressSheet / endProgressSheet
 @abstract  Begin/end the loading progress sheet
 */
- (BOOL)beginProgressSheet;
- (void)endProgressSheet;

- (void)insertMessageAtLatestTimestamp:(NSString *)message fromSource:(NSString *)source severity:(MessageEventSeverity)severity recordToFile:(BOOL)shouldRecordToFile;

/*!
 @method    recordRuleTestsErrorAtLatestTimestamp:
 @abstract  Create a message denoting the rule test error
 */
- (void)recordRuleTestErrorAtLatestTimestamp:(RuleTestError *)error;

/*!
 Updates the category outline view and the error summary to indicate an error occurred.
 */
- (void)updateParserErrorIndication;

/*! Determines whether a firmware update for the attached box is necessary and
 launches the updater app if an update is necessary.
 @param     captureDevice The capture device the update will be performed on.
 @param     majorVersion The major version of the capture device.
 @param     minorVersion The minor version of the capture device.
 @param     revisionVersion The revision version of the capture device.
 */
- (void)atsBox:(CaptureDevice *)captureDevice reportedFirmwareMajorVersion:(UInt8)majorVersion minorVersion:(UInt8)minorVersion revisionVersion:(UInt8)revisionVersion;

/*!
 @method    updateFirmwareOnATSBox:forced:
 @abstract  Closes the document and alerts the user that an update is required for the connected
 ATS (30-pin or Lightning) Box.
 @param     captureDevice
 The capture device the update will be performed on.
 @param     forced
 Whether the update was forced because there was an issue communcating with the box.
 */
- (void)updateFirmwareOnATSBox:(CaptureDevice *)captureDevice forced:(BOOL)forced;

/*!
 @method    updateErrorSummaryNotification:
 @abstract  Refreshes the error summary in the UI
 @param     notification
 The notification to handle. Object should self, and must have @"FailureType" set for the level
 */
- (void)updateErrorSummaryNotification:(NSNotification *)notification;

#pragma mark -
#pragma mark Self Cert Accessors

/*!
 @method    areClaimSpecificRulesEnabled
 @abstract  Whether or not claim specific rules are enabled
 */
- (BOOL)areClaimSpecificRulesEnabled;

/*!
 @method    isClaimEnabled:
 @abstract  Whether or not the claim is enabled
 */
- (BOOL)isClaimEnabled:(NSString *)claimName;

/*!
 @method    createRuleTestsWindowControllerIfNeeded
 @abstract  Creates the rule tests window controller if it hasn't already been created
 */
- (void)createRuleTestsWindowControllerIfNeeded;

#pragma mark -
#pragma mark Video Methods
/**
 Seeks the video playback to the time passed in the timestamp parameter. Timestamp should be considering the video playback time, 0 means 0 seconds of video.
 @param timestamo Time to which the video playback should seek in nanoseconds.
 */
- (void)syncVideoToTimestamp:(UInt64)timestamp;

+ (BOOL)isChecksumCorrect:(NSString *)unzipPath error:(NSError **)error;

/*!
 Stops the video.
 */
- (void)stopVideo;

/**
 Prepares the expanded video window by copying the small playback's video player.
 */
- (void)prepareExpandedWindow;

#pragma mark -
#pragma mark Error and Notification Handling

/*!
 @method    handleFatalError:
 @abstract  Displays error to the user, either by forwarding to the CA window or displaying directly
 @param     error
 The error to display
 */
- (void)handleFatalError:(NSError *)error;

#pragma mark -
#pragma mark Transport Helpers

/*!
 @method        atomicallyPerformBackgroundWriteOperation:
 @abstract      Invoke a block to write to a temporary file in a manner that is safe for file saving
 @discussion    This will also mark the document as dirty
 */
- (void)atomicallyPerformBackgroundWriteOperation:(void (^)(void))block;

@end
