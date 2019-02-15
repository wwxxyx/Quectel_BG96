//
//  PhysicalATFR3CaptureDevice.h
//  ATSMacApp
//
//  Created by Marin Balde on 4/11/12.
//  Copyright © 2012 Apple Inc. All Rights Reserved.
//

#import <CaptureKit/ATSSerialCaptureDevice.h>
#import <ATSR3BoxSupport/ATSR3BoxSupport.h>

@interface PhysicalATFR3CaptureDevice : ATSSerialCaptureDevice {
    int _fd;
    BOOL _readPending;

    BOOL _connectReadLoopShouldRun;
    BOOL _readLoopShouldRun;

    BOOL _requestFirmwareVersionPending;
    BOOL _setModeToAckReceived;
    BOOL _setSerialRateAckReceived;

    ATSR3BPacketizer *_atfR3Packetizer;
    NSMutableData *_preconnectionATFR3Data;
}

@property (nonatomic, readwrite, assign) BOOL requestFirmwareVersionPending;

/*!
 @property  operatingModeResponsePending
 @abstract  Whether a SetOperatingMode response is still pending.
 */
@property (nonatomic, assign) BOOL operatingModeResponsePending;

/*!
 @property  reportsResponsePending
 @abstract  Whether a SetReports response is still pending.
 */
@property (nonatomic, assign) BOOL reportsResponsePending;

// send command with no additional payload
- (NSData *)sendATFR3WithCommand:(ATFR3Command)command;
// send command with additional payload
- (NSData *)sendATFR3WithCommand:(ATFR3Command)command
                         payload:(const UInt8 *)payload
                     payloadSize:(UInt8)payloadSize;
// send a raw frame buffer, ideally you should use one of other 2 methods above
- (void)sendATFR3RawFrameBuffer:(NSData *)atfR3Data;

- (void)sendGetFirmwareVersionATFR3;

/*!
 @method    sendSetOperatingMode:
 @abstract  Puts the board into the given operating mode.
 @param     mode
            The operating mode to set the board to.
 */
- (void)sendSetOperatingMode:(ATFR3OperatingMode)mode;

/*!
 @method    sendConfigureATFFrameReporting:
 @abstract  Configures the ATF reporting of the board.
 @param     reportingMask
            The mask declaring what ATF's to report.
 */
- (void)sendConfigureATFFrameReporting:(UInt32)reportingMask;

/*!
 @method    sendRunBrickDetection
 @abstract  Requests the brick detection information from the box
 */
- (void)sendRunBrickDetection;

/*!
 Requests smart brick detection information from the box
 */
- (void)sendRunSmartBrickDetection;

/*!
 @method    sendPowerLoadTestWithCurrent:duration:
 @abstract  Requests that the ATS board begins the load test
 @param     currentInMilliamps
            The current to draw in milliamps
 @param     durationInMilliseconds
            The duration of the test in milliseconds
 */
- (void)sendPowerLoadTestWithCurrent:(UInt16)currentInMilliamps duration:(UInt16)durationInMilliseconds;

@end
