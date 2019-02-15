//
//  CaptureTransport.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 4/30/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureTransport.h>

#import <CaptureKit/ATSBoxCaptureDevice.h>

#import <CaptureKit/iAP1CaptureProtocol.h>
#import <CaptureKit/iAP2CaptureProtocol.h>

#import <CaptureKit/ATFTrafficViewController.h>

@interface ATSBoxCaptureTransport : CaptureTransport <ATSBoxCaptureDeviceDelegate> {
    BOOL _receivedHardwareInfoTypes[ATFHardwareInfoTypeCount];
    UInt64 _atsBoxTimerRolloverCount;

    BOOL _iPodDetectPinStateEverReported;
    BOOL _accDetectPinStateEverReported;
    BOOL _accDetectPinGrounded;
    BOOL _accFireWireVoltagePresent;
    BOOL _accUSBVbusVoltagePresent;
    BOOL _accUSBDPlusVoltagePresent;
    BOOL _accUSBDMinusVoltagePresent;
    BOOL _accTxToiPodMarkingStateVoltagePresent;
}

/*!
 @property  captureDevice
 @abstract  The current capture device
 */
@property (nonatomic, readonly, retain) ATSBoxCaptureDevice *captureDevice;

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

#if APPLE_INTERNAL
/*!
 @property  atfTrafficViewController
 @abstract  The ATF TVC
 */
@property (nonatomic, readonly, retain) ATFTrafficViewController *atfTrafficViewController;
#endif

#pragma mark -
#pragma mark Parsing

@property (nonatomic, retain) BSiAP2LinkOverUARTAnalyzer *linkAnalyzer;
@property (nonatomic, retain) BSiAPOverUARTAnalyzer *iAPAnalyzer;

@property (nonatomic, retain) NSString *atsBoxVersion;

@property (nonatomic, retain) ATSBPacketizer *atfPacketizer;

@end
