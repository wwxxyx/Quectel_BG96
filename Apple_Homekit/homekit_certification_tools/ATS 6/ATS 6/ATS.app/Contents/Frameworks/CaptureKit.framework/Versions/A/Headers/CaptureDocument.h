//
//  CaptureDocument.h
//  CaptureKit
//
//  Created by Joe Basu on 2/19/09.
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//
// https://gitlab.sd.apple.com/ats/CaptureKitDocumentation

#import <Cocoa/Cocoa.h>

@class CaptureConfiguration;
@class EventBroker;
@class CaptureDocument;
@class ATSAppEvent;
@class CaptureDevice;
@class CaptureTransport;
@class TrafficViewController;

NS_ASSUME_NONNULL_BEGIN

#pragma mark Notification Names

/** Posted when a file read operation completes. */
extern NSString *const CaptureDocumentFinishedReadingFileNotificationName;

/** The file extension for BT Snoop format  */
extern NSString *const kExtensionForBtSnoopFileFormat;

/** The file extension for PacketLogger format */
extern NSString *const kExtensionForPacketLoggerFileFormat;

#pragma mark - Error Codes

/** Error domain for errors produced by CaptureDocument. */
extern NSString *const CaptureDocumentErrorDomain;

/** Error codes identifying the different error conditions CaptureDocument can encounter. */
typedef NS_ENUM(NSInteger, CaptureDocumentError) {
    
    /** A 30-pin box disappeared while in use. */
    CaptureDocumentErrorATSBoxYanked = -3,
    
    /** A Beagle disappeared while in use. */
    CaptureDocumentErrorBeagleYanked = -5,
    
    /** A Lightning box disappeared while in use. */
    CaptureDocumentErrorATSR3BoxYanked = -6,
    
    /** Tried to start a 30-pin capture with a misconfigured capture document. */
    CaptureDocumentErrorNoATSBoxesFound = -7,
    
    /** There were no 30-pin boxes available when a 30-pin capture started. */
    CaptureDocumentErrorATSBoxMissing = -8,
    
    /** Tried to start a Lightning capture with a misconfigured capture document. */
    CaptureDocumentErrorNoATSR3BoxesFound = -9,
    
    /** There were no Lightning boxes available when a Lightning capture started. */
    CaptureDocumentErrorATSR3BoxMissing = -10,
    
    /** Tried to start a Beagle USB capture with a misconfigured capture document. */
    CaptureDocumentErrorNoBeaglesFound = -11,
    
    /** There were no Beagle boxes available when a USB capture started. */
    CaptureDocumentErrorBeagleMissing = -12,
    
    /** Tried to start a USB capture with an unrecognized Beagle box. */
    CaptureDocumentErrorUnknownBeagle = -14,
    
    /** Tried to open a trace while a software update was in progress. */
    CaptureDocumentErrorTracesCannotBeOpenedWhenPresentingUpdate = -15,
    
    /** A capture ended because the Mac went to sleep. */
    CaptureDocumentErrorSystemSleep = -16,
    
    /** Tried to print a capture document, which isn't supported. */
    CaptureDocumentErrorPrintNotSupported = -17,
    
    /** A Capture ended because a Lightning box reported a hardware error. */
    CaptureDocumentErrorATSR3ReportedIssue = -18,
    
    /** Data from a capture file is corrupt or missing. */
    CaptureDocumentErrorTraceCannotBeRestored = -19,
    
    /** An I/O error occurred while trying to write data to a capture file. */
    CaptureDocumentErrorTraceCannotBeSaved = -20,
    
    /** A Bluetooth ComProbe disappeared while in use. */
    CaptureDocumentErrorComprobeYanked = -21,
    
    /** Tried to start a Bluetooth capture with a missing or invalid link key. */
    CaptureDocumentErrorInvalidLinkKey = -22,
    
    /** Tried to start a Bluetooth capture while one was already running. */
    CaptureDocumentErrorBluetoothCaptureCannotBeStartedWhenBluetoothCaptureIsRunning = -23,
    
    /** A Wi-Fi capture interface disappeared while in use. */
    CaptureDocumentErrorEthernetInterfaceYanked = -24,
    
    /** A Lightning Audio box disappeared while in use. */
    CaptureDocumentErrorATSLightningAudioBoxYanked = -25,
    
    /** Tried to start a Lightning Audio capture with a misconfigured capture document. */
    CaptureDocumentErrorNoATSLightningAudioBoxesFound = -26,
    
    /** There were Lightning Audio boxes available when a Lightning Audio capture started. */
    CaptureDocumentErrorATSLightningAudioBoxMissing = -27,

    /** TKCaptureAssistant was not granted access */
    CaptureDocumentErrorTKCaptureAssistantPermissionDenied = -28,
    
    /** Could not find rvictl */
    CaptureDocumentErrorRvictlNotFound = -29,
    
    /** No Mobile Devices were connected */
    CaptureDocumentErrorNoMobileDeviceConnected = -30,
    
    /** There are Mobile Devices connected but none are trusted/paired with host mac */
    CaptureDocumentErrorNoTrustedMobileDeviceConnected = -31,
};

#pragma mark - Capture Document Delegate

/** Conforming types receive callbacks from a capture document at interesting points in the capture lifecycle, such as when a capture begins or ends and when a capture device's firmware version is detected. */
@protocol CaptureDocumentDelegate <NSObject>

@optional

/**
 Sent when a capture document is about to begin receiving data from capture devices. The receiver can use this opportunity to subscribe to events using the document's event broken.
 
 @param captureDocument The capture document that is about to begin capturing data.
 */
- (void)captureDocumentWillBeginCapture:(CaptureDocument *)captureDocument;

/**
 Sent when one of the capture devices in use reports its firmware version.
 
 @param captureDevice The capture device that reported its firmware version.
 @param majorVersion The major version number.
 @param minorVersion The minor version number.
 @param revisionVersion The revision version number.
 */
- (void)captureDevice:(CaptureDevice *)captureDevice reportedFirmwareMajorVersion:(UInt8)majorVersion minorVersion:(UInt8)minorVersion revisionVersion:(UInt8)revisionVersion;

/**
 Sent when a capture document has finished receiving data from capture devices. If the capture ended because of an error condition, then sender will pass an error object describing the problem. If `error` is `nil`, the capture ended normally.
 
 @param captureDocument The capture document that finished capturing data.
 @param error An error object describing the reason why the capture ended, or `nil` if the capture ended normally.
 */
- (void)captureDocument:(CaptureDocument *)captureDocument didEndCaptureWithError:(NSError * _Nullable)error;

/**
 Sent when the document's background tasks have terminated after the document was closed.
 
 @param captureDocument The capture document that finished capturing data.
 */
- (void)captureDocumentDidClose:(CaptureDocument *)captureDocument;

@end

#pragma mark - Capture Document

/** Capture documents define a context in which data from capture devices is collected and serialized. CaptureDocument is responsible for initializing capture hardware, building up the parsing pipeline, and translating between the captured data's on-disk and in-memory representations. */
@interface CaptureDocument : NSDocument

/** The capture document's deletate. Delegates are informed of important document lifecycle events, such as when the capture is about to begin and when it has just finished. */
@property (nonatomic, weak) id<CaptureDocumentDelegate> _Nullable delegate;

/** The data model containing all of the properties used to initialize capture hardware and configure the parsing pipeline. */
@property (nonatomic, strong, readonly) CaptureConfiguration * _Nullable configurationModel;

/** The event broker that publishes all of the events generated during a capture. Objects can receive notifications about traffic events by subscribing to a capture document's event broker and specifying the desired event types. */
@property (nonatomic, strong, readonly) EventBroker *eventBroker;

/** A flag indicating whether or not the document has captured any data yet. */
@property (nonatomic, getter=isDataPresent) BOOL dataPresent;

/** A flag indicating whether or not the document is currently capturing data. */
@property (nonatomic, getter=isCaptureActive) BOOL captureActive;

/** A flag indicating whether or not the document is in the process of stopping an active capture. */
@property (nonatomic, readonly, getter=isStoppingCapture) BOOL stoppingCapture;

/** A flag indicating whether or not an accessory is currently connected to a capture device. */
@property (nonatomic, readonly, getter=isAccessoryConnected) BOOL accessoryConnected;

/** A flag indicating whether or not an Apple device is currently connected to a capture device. */
@property (nonatomic, readonly, getter=isAppleDeviceConnected) BOOL appleDeviceConnected;

/** The array of CapureTransports */
@property (readonly) NSArray<CaptureTransport *> *captureTransports;

/** A flag indicating whether to show the save sheet or not.  The default is show. */
@property (nonatomic, getter=isShowSaveSheet) BOOL showSaveSheet;

/** A flag indicating whether to disallow Save/Save As functionality. */
@property (nonatomic) BOOL disableSave;


/**
 Initializes and returns a new capture document configured with data from the capture file at the given URL.
 
 @param url A file URL pointing to a capture document.
 @param typeName A string that identifies the document type.
 @param delegate The capture document delegate that will receive delegate callbacks.
 @param error If an error occurs during initialization, on return this will point to an error object describing the failure. Pass `nil` if no error information is needed.
 @return The new capture document, or `nil` if initialization fails.
 */
- (instancetype _Nullable)initWithContentsOfURL:(NSURL *)url ofType:(NSString *)typeName delegate:(id<CaptureDocumentDelegate>)delegate error:(NSError **)outError;

/*!
 @method    activeTrafficViewController
 @abstract  The traffic view controller currently being displayed
 */
@property (readonly, nullable) TrafficViewController *activeTrafficViewController;

#pragma mark - Live Capture Control

/**
 Configure the document's parsing pipeline according to the given configuration data model.
 
 @param dataModel The capture configuration information to use for this capture.
 @param error If an error occurs during configuration, on return this will point to an error object describing the failure. Pass `nil` if no error information is needed.
 @return `YES` if the document was configured successfully; `NO` otherwise.
 */
- (BOOL)configureUsingDataModel:(CaptureConfiguration *)dataModel error:(NSError **)error;

/** Create and show the window for this document. */
- (void)displayDocumentWindow;

/** Create and show the window for this document, ordered relative to the specified otherWindowNumber.
    This is useful for bringing the capture window up behind some other app window when CaptureDocument
    is used in CaptureKit. */
- (void)displayWindowInOrderingMode:(NSWindowOrderingMode)orderingMode relativeTo:(NSInteger)otherWindowNumber;

/** Causes the capture document to begin receiving data from capture devices. */
- (void)beginCapture;

/** Causes the capture document to stop receiving data from capture devices. */
- (void)endCapture;

/**
 Causes the capture document top stop receiving data from capture devices and presents the given error.
 
 @param error The error to present as the reason why the capture stopped.
 */
- (void)endCaptureWithError:(NSError *)error;

#pragma mark - Traffic/Error Insertion

/**
 Inserts a message into the traffic event stream. If `shouldRecordToFile` is `YES`, the message will be persisted with the capture document and restored when the document is re-loaded; otherwise, the message will be lost when the document is closed.
 
 @param message The string to insert into traffic views.
 @param shouldRecordToFile A flag indicating whether or not the message will be persisted in the capture document for display when the document is reloaded.
 */
- (void)insertMessageAtLatestTimestamp:(NSString *)message recordToFile:(BOOL)shouldRecordToFile;
- (void)insertMessageAtLatestTimestamp:(NSString *)message fromSource:(NSString * _Nullable)source recordToFile:(BOOL)shouldRecordToFile;

/**
 Delivers an out-of-band event to all relevant traffic views and rule engines.
 
 @param event The out-of-band event to insert in traffic views.
 */
- (void)insertATSAppEvent:(ATSAppEvent *)event;

#pragma mark - Menu Actions

/**
 Toggles visibility of the Details sidebar.
 
 @param sender The object that invoked the action.
 */
- (IBAction)showDetails:(id _Nullable)sender;

/**
 Visibility status of Details sidebar
 
 @return YES if the Details sidebar is visible
 */
@property (readonly, getter=isDetailsShown) BOOL detailsShown;

/**
 Makes the capture document filter field the first responder.
 
 @param sender The object that invoked the action.
 */
- (IBAction)focusOnFilterField:(id _Nullable)sender;

/**
 Toggles visibility of the Categories sidebar.
 
 @param sender The object that invoked the action.
 */
- (IBAction)showCategories:(id _Nullable)sender;

/**
 Clears the traffic from all traffic views. Cleared traffic is still present in the capture document and will be visible again if you save and re-load the document.
 
 @param sender The object that invoked the action.
 */
- (void)clearTraffic:(id _Nullable)sender;

/**
 Stops the active capture.
 
 @param sender The object that invoked the action.
 */
- (void)stopCapture:(id _Nullable)sender;

/**
 Inform the CaptureDocument that it needs to update the selection state of the 
 sidebar segment controls in the toolbar.
 
 This is used since the APReportAnalyzerViewController can show/hide the details 
 bar after a double click.
 */
- (void)updateViewToolbarItemSelection;

@end

NS_ASSUME_NONNULL_END
