//
//  USBCaptureTransport.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 4/30/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureTransport.h>

#import <CaptureKit/BeagleUSBCaptureDevice+Protected.h>

#import <CaptureKit/iAP1CaptureProtocol.h>
#import <CaptureKit/iAP2CaptureProtocol.h>

#import <NCMKit/NCMKit.h>
#import <CaptureKit/EthernetCaptureProtocol.h>

#if APPLE_INTERNAL
#import <CaptureKit/EthernetOverUSBStreamReader.h>
#endif

#import <CaptureKit/USBTransferTrafficViewController.h>
#import <CaptureKit/USBTransactionTrafficViewController.h>
#import <CaptureKit/USBPacketTrafficViewController.h>
#import <CaptureKit/USBHIDReportTrafficViewController.h>

// FUTURE: Move this to BusSniffer framework with its friends
//          There's no need to branch BusSniffer for the initial
//          StarkSupport except for these lines... so let's
//          leave them here for now

// Unencapsulated Ethernet frames over USB Bulk Pipe
#define BSUSBInterfaceSubClassStarkulatorAccessory 0xFD
#define BSUSBInterfaceProtocolStarkulator 0x01

#define BSUSBInterfaceSubClassNVIDIAReferenceAccessory 0xFF
#define BSUSBInterfaceProtocolNVIDIAReference 0x00

// Class 0xFF
#define BSUSBVendorSpecificSubClassCarPlay 0xF0
#define BSUSBVendorSpecificProtocolCarPlay 0x02

// For class 0x02
#define BSUSBCommInterfaceSubClassNetworkControlModel 0x0D
#define BSUSBCommInterfaceProtocolNoEncapsulatedCR 0x00

// For class 0x0A, just set to 0x00 there's none
#define BSUSBDataInterfaceSubClassDefault 0x00
#define BSUSBDataInterfaceProtocolNCMData 0x01

enum {
    BSUSBStateNextEthernetFrame = 0x00000400,
};

@interface USBCaptureTransport : CaptureTransport <BeagleUSBCaptureDeviceDelegate>

/**
 Whether or not iAP2 data is expected in the USB trace.
 */
@property (nonatomic, readonly) BOOL isiAP2Expected;
    
/*!
 @property  captureDevice
 @abstract  The current capture device
 */
@property (nonatomic, readonly, retain) BeagleUSBCaptureDevice *captureDevice;

/*!
 @property  iAP1CaptureProtocol
 @abstract  The iAP1 protocol parser
 */
@property (nonatomic, readonly, retain) iAP1CaptureProtocol *iAP1CaptureProtocol;

/*!
 @property  iAP2CaptureProtocol
 @abstract  The iAP2 protocol parser
 */
@property (nonatomic, readonly, retain) iAP2CaptureProtocol *iAP2CaptureProtocol;

/*!
 @property  ethernetCaptureProtocol
 @abstract  The Ethernet parser
 */
@property (nonatomic, readonly, retain) EthernetCaptureProtocol *ethernetCaptureProtocol;

/*!
 @property  [transfer|transaction|packet]TrafficViewController
 @abstract  TVC for USB transfers/transactions/packets
 */
@property (nonatomic, readonly, retain) USBTransferTrafficViewController *transferTrafficViewController;
@property (nonatomic, readonly, retain) USBTransactionTrafficViewController *transactionTrafficViewController;
@property (nonatomic, readonly, retain) USBPacketTrafficViewController *packetTrafficViewController;
@property (nonatomic, readonly, retain) USBHIDReportTrafficViewController *hidReportTrafficViewController;

@property (nonatomic, retain) BSiAP2LinkOverUSBHIDAnalyzer *hidiAP2LinkAnalyzer;
@property (nonatomic, retain) BSiAPOverUSBHIDAnalyzer *hidiAPAnalyzer;

@property (nonatomic, retain) BSiAP2LinkOverUSBBulkPipeAnalyzer *bulkPipeiAP2LinkAnalyzer;
@property (nonatomic, retain) BSiAPOverUSBBulkPipeAnalyzer *bulkPipeiAPAnalyzer;

@property (nonatomic, retain) BSUSBPacketFilter *usbPacketFilter;
@property (nonatomic, retain) BSUSBAnalyzer *usbAnalyzer;
@property (nonatomic, retain) BSUSBHIDAnalyzer *usbHIDAnalyzer;

@property (nonatomic) BOOL usbHostConnectionStatusKnown;
@property (nonatomic) BOOL usbHostConnected;

@property (nonatomic) UInt8 usbDeviceAddress;
@property (nonatomic) UInt8 hidInterruptInEndpointNumber;
@property (nonatomic) UInt8 iAPBulkInPipeEndpointNumber;
@property (nonatomic) UInt8 iAPBulkOutPipeEndpointNumber;
@property (nonatomic) UInt8 iAPInterruptInPipeEndpointNumber;

@property (nonatomic) UInt8 carPlayBulkInPipeEndpointNumber;
@property (nonatomic) UInt16 carPlayBulkInMaxPacketSize;
@property (nonatomic) UInt32 carPlayMaxNTBInSize;
@property (nonatomic) UInt8 carPlayBulkOutPipeEndpointNumber;
@property (nonatomic) UInt16 carPlayBulkOutMaxPacketSize;
@property (nonatomic) UInt32 carPlayMaxNTBOutSize;

@property (nonatomic) UInt8 usbDeviceHubAddress;

@property (nonatomic, retain) NCMAnalyzer *ncmAnalyzer;

// Allow internal Stark builds to parse Starkulator data
#if APPLE_INTERNAL
@property (nonatomic, retain) EthernetOverUSBStreamReader *ethernetOverUSBStreamReader;
#endif

@property (nonatomic) UInt64 lastDataReadyZLPTimestamp;
@property (nonatomic, retain) BSUSBConfigurationDescriptor *currentConfigurationDescriptor;

@property (nonatomic) BOOL appleDeviceActingAsUSBDevice;

@property (nonatomic, assign) BOOL poshBeagle;
@property (nonatomic, assign) UInt32 beagleSampleRate;
@property (nonatomic, assign) double timeScaleFactor;

@property (nonatomic, assign) BOOL lookForZLP;
@property (nonatomic, assign) BOOL missingHIDReportDescriptorMessageReported;

@property (nonatomic, assign) BOOL inReset;
@property (nonatomic, assign) UInt32 lastKnownSpeed;

@property (nonatomic, assign) BOOL badUSBSplitPacketReported;

@property (nonatomic, assign) UInt64 currentTimestamp;

/*!
 @property  lastBeagleStateTimerIncrement
 @abstract  Timestamp of the last inserted timer increment timestamp during a live capture
 */
@property (nonatomic, assign) UInt64 lastBeagleStateTimerIncrement;

/*!
 @property  nativeHIDAnalyzersByInterruptEndpointNumberAndDirection
 @abstract  Dictionary of Dictionaries of BSUSBHIDAnalyzers keyed by the Interrupt In endpoint numbers
 and endpoint directions specified by HID Interface descriptors that have a corresponding USBHostHIDComponent.
 */
@property (nonatomic, retain) NSMutableDictionary *nativeHIDAnalyzersByInterruptEndpointNumberAndDirection;

@property (nonatomic, copy) NSString *usbSavedDataTempFilePath;
@property (nonatomic, retain) NSFileHandle *usbSavedDataFileHandle;

/*!
 @property  filteredEthernetDataTempFilePath
 @abstract  Path to temp file where ethernet data is recorded if the ethernet filter is enabled
 */
@property (nonatomic, copy) NSString *filteredEthernetDataTempFilePath;

/*!
 @property  filteredEthernetDataTempFileHandle
 @abstract  file handle for ethernet data recording if the ethernet filter is enabled
 */
@property (nonatomic, retain) NSFileHandle *filteredEthernetDataTempFileHandle;

/*!
 @property      processingFilteredCarPlayUSBPacket
 @abstract      Whether or not we are currently processing a Stark USB packet
 @discussion    This gets set on the IN or OUT packet and sticks around until the
                transaction completes.
 */
@property (nonatomic, assign) BOOL processingFilteredCarPlayUSBPacket;

@end
