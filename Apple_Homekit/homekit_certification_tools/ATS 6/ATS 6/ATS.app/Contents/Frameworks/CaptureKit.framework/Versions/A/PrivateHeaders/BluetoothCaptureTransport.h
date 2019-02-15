//
//  BluetoothCaptureTransport.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 4/30/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureTransport.h>
#import <CaptureKit/iAP1CaptureProtocol.h>
#import <CaptureKit/iAP2CaptureProtocol.h>
#import <CaptureKit/HIDCaptureProtocol.h>
#import <BusSniffer/BusSniffer.h>
#import <ComprobeSupport/ComprobeSupport.h>
#import <CaptureKit/L2CAPTrafficViewController.h>
#import <CaptureKit/ComprobeFrameTrafficViewController.h>

@class ComprobeDataFrameParser;
@class HCIContainerEventsStorage;
@class ComprobeFrameEventsStorage;

@interface BluetoothCaptureTransport : CaptureTransport

/// The iAP1 protocol parser
@property (nonatomic, readonly, retain) iAP1CaptureProtocol *iAP1CaptureProtocol;

/// The iAP2 protocol parser
@property (nonatomic, readonly, retain) iAP2CaptureProtocol *iAP2CaptureProtocol;

/// The HID protocol parser
@property (nonatomic, readonly, retain) HIDCaptureProtocol *HIDCaptureProtocol;

/// The comprobe frame parser.
@property (nonatomic, readonly, retain) ComprobeDataFrameParser *comprobeDataFrameParser;

/// iAP2 Link Layer analyzer
@property (nonatomic, retain) BSiAP2LinkOverBluetoothAnalyzer *linkAnalyzer;

/// iAP2 over BT analyzer
@property (nonatomic, retain) BSiAPOverBluetoothAnalyzer *iAPAnalyzer;

/// The file handle for the saved bluetooth file.
@property (nonatomic, retain) NSFileHandle *bluetoothSavedDataFileHandle;

/// Whether the first comprobe frame has been seen yet.
@property (nonatomic, assign) BOOL sawFirstComprobeFrame;

/// The first timestamp used to normalize the following timestamps.
@property (nonatomic, assign) UInt64 firstComprobeFrameTimestamp;

/// The bluetooth address of the accessory.
@property (atomic, copy) NSData *accessoryAddress;

/// The bluetooth address of the Apple device.
@property (atomic, copy) NSData *appleDeviceAddress;

/// The previous ComprobeFrameParserResult from the Apple device
@property (nonatomic, retain) ComprobeFrameParserResult *previousAppleDeviceComprobeFrameResult;

/// The previous ComprobeFrameParserResult from the Apple device
@property (nonatomic, retain) ComprobeFrameParserResult *previousAccessoryComprobeFrameResult;

/// Whether or not the Apple device has an active/acknowledged session, so that reset/drop conditions can be calculated
@property (nonatomic, assign) BOOL hasAppleDeviceRFCOMMSession;

/// Whether or not the accessory has an active/acknowledged session, so that reset/drop conditions can be calculated
@property (nonatomic, assign) BOOL hasAccessoryRFCOMMSession;

/// Represents an instance for Bluetooth ComProbe/Baseband traffic view controller
@property (nonatomic, readonly, strong) ComprobeFrameTrafficViewController *comprobeFrameTrafficViewController;

/// Represents an instance for Bluetooth L2CAP traffic view controller.
@property (nonatomic, readonly, strong) L2CAPTrafficViewController *l2CAPTrafficViewController;

/// An instance that access all events captured of the Hci Container Events to export of BT Snoop Data.
@property (nonatomic, readonly, strong) HCIContainerEventsStorage *hciContainerEventsStorage;

/// An instance that access all events captured of the Comprobe Frame Events to export of BT Snoop Data.
@property (nonatomic, readonly, strong) ComprobeFrameEventsStorage *comprobeFrameEventsStorage;

/*!
 Exports to BT Snoop file format or to PacketLogger file format
 @param events List of events to export in file
 @param format defines the export format (use kExtensionForPacketLoggerFileFormat or kExtensionForBtSnoopFileFormat)
 @param savePanelURL the URL of file and connected by the UI panel component
 */
- (void)exportToBtSnoopOrToPacketLogger:(NSArray *)events toFormat:(NSString *)format andPanelURL:(NSURL *)savePanelURL;


@end
