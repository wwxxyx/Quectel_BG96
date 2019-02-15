//
//  ATFR3CaptureTransport.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 4/30/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureTransport.h>
#import <CaptureKit/ATSSerialCaptureDevice.h>

#import <CaptureKit/iAP1CaptureProtocol.h>
#import <CaptureKit/iAP2CaptureProtocol.h>
#import <CaptureKit/IDBusCaptureProtocol.h>

#import <CaptureKit/UARTATFR3TrafficViewController.h>
#import <ATSR3BoxSupport/ATSR3BoxSupport.h>

@class IDBusPacketizer;
@class ATFR3CaptureDeviceManager;
@class CaptureConfiguration;

@interface ATFR3CaptureTransport : CaptureTransport <ATSSerialCaptureDeviceDelegate> {
    UInt64 _atsR3BoxLastTimerTickTimestamp;
    UInt64 _atsR3BoxLastFrameTimestamp;
}

- (instancetype)initWithDocument:(CaptureDocument *)document
             deviceTransportType:(DeviceTransportType)deviceTransportType
       atfR3CaptureDeviceManager:(ATFR3CaptureDeviceManager *)atfR3CaptureDeviceManager
                           error:(NSError **)error;

@property (strong) NSNumber *clockTickLength;

/*!
 Timestamp of the first timer tick / absolute timestamp from the ATFR3 box. Used to calculate relative timestamp values so that the first event start with zero.
 */
@property (strong) NSNumber *firstAbsoluteTimestamp;

/*!
 @property  captureDevice
 @abstract  The current capture device
 */
@property (nonatomic, readonly, retain) ATSSerialCaptureDevice *captureDevice;

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
 @property  idBusCaptureProtocol
 @abstract  The IDBus protocol parser
 */
@property (nonatomic, readonly, retain) IDBusCaptureProtocol *idBusCaptureProtocol;

#if APPLE_INTERNAL
/*!
 @property  atfR3TrafficViewController
 @abstract  TVC for displaying ATF traffic
 */
@property (nonatomic, readonly, retain) UARTATFR3TrafficViewController *atfR3TrafficViewController;
#endif

#pragma mark -
#pragma mark Parsing

@property (nonatomic, retain) BSiAP2LinkOverUARTAnalyzer *linkAnalyzer;
@property (nonatomic, retain) BSiAPOverUARTAnalyzer *iAPAnalyzer;

@property (nonatomic, retain) ATSR3BPacketizer *atfR3Packetizer;

@property (nonatomic, retain) NSString *atsR3BoxVersion;

/**
 Reference to the instance of capture device manager that tracks all the available devices for this transport.
 */
@property (nonatomic, strong) ATFR3CaptureDeviceManager *atfR3CaptureDeviceManager;

#pragma mark -
#pragma mark Box Control

/*!
 @method    enableSniffingAndReportingATSR3
 @abstract  Puts the R3 box operating mode into sniffer mode and enables reporting.
 */
- (void)enableSniffingAndReportingATSR3;

/*!
 @method    requestFirmwareVersionATSR3
 @abstract  Requests the ATS R3 box's firmware version
 */
- (void)requestFirmwareVersionATSR3;

@end
