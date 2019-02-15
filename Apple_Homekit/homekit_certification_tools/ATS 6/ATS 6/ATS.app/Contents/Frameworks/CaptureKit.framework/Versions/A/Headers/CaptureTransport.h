//
//  CaptureTransport.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 4/30/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//
// https://gitlab.sd.apple.com/ats/CaptureKitDocumentation

#import <CaptureKit/DeviceCaptureConfigurationTypes.h>

@class CaptureDocument;
@class CaptureConfiguration;
@class CaptureEvent;
@class TrafficViewController;

/** Status to represent the state of the capture transport when ending a capture. */
typedef NS_ENUM(UInt8, CaptureTransportEndCaptureStatus) {
    /** End capture completed successfully. */
    CaptureTransportEndCaptureStatusSuccess = 1,
    
    /** End capture completed with an error. */
    CaptureTransportEndCaptureStatusError,
    
    /** The ending of the capture is pending. */
    CaptureTransportEndCaptureStatusPending,
};

/** Notification name posted for every emulated event if configured for emulation. The notification object is the capture transport that produced the event; the user info dictionary contains the list of emulated events. */
extern NSString *CaptureTransportEmulatedEventsNotificationName;

/** Abstract base class for objects that manage a specific physical transport. Transport objects parse input data into the native transport frame structure and pass derived protocol data up to higher-level parsers. */
@interface CaptureTransport : NSObject

/** The capture document that owns the transport. */
@property (readonly, weak) CaptureDocument *document;

/** A flag indicating whether or not an accessory is connected over this transport. */
@property BOOL isAccessoryConnected;

/** A flag indicating whether or not an Apple device is connected. */
@property BOOL isAppleDeviceConnected;

/** The array of TrafficViewControllers for this transport */
@property (readonly) NSArray<TrafficViewController *> *trafficViewControllers;

/**
 A new capture transport instance with the given owning document, capture configuration datamodel, and transport type. Returns sets the given error and returns `nil` when initialization fails.
 
 @param document The capture document instance that owns this transport.
 @param deviceTransportType The identifier for the type of transport the new instance represents.
 @return The new transport instance.
 */
- (instancetype)initWithDocument:(CaptureDocument *)document
             deviceTransportType:(DeviceTransportType)deviceTransportType
                           error:(NSError **)error;

#pragma mark -
#pragma mark - Subclassers - Document Support

/** A string describing the physical capture device hardware. */
@property (readonly) NSString *transportBoxInfo;

/** The name of the transport's section in the capture document's categories sidebar. */
@property (readonly) NSString *categoryName;

/** The name of the transport itself, such as "UART", "USB HID", etc. */
@property (readonly) NSString *transportName;

/** An array of available events (for RuleEngine) on this transport in the current configuration. */
@property (readonly) NSArray<CaptureEvent *> *availableEvents;

@end
