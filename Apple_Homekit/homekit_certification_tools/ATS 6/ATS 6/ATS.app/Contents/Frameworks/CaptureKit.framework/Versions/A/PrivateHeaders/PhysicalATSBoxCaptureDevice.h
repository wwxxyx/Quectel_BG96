//
//  PhysicalATSBoxCaptureDevice.h
//  ATS
//
//  Created by Joe Basu on 8/2/10.
//  Copyright © 2010 Apple Inc. All Rights Reserved.
//

#import <CaptureKit/ATSBoxCaptureDevice.h>

@interface PhysicalATSBoxCaptureDevice : ATSBoxCaptureDevice {
    NSString *_devicePath;
    int _fd;
    BOOL _readPending;

    BOOL _connectReadLoopShouldRun;
    BOOL _readLoopShouldRun;

    BOOL _requestFirmwareVersionPending;
    BOOL _setModeToAckReceived;
    BOOL _setSerialRateAckReceived;

    ATSBPacketizer *_atfPacketizer;
    NSMutableData *_preconnectionATFData;
}

@property (nonatomic, readonly, copy) NSString *devicePath;
@property (nonatomic, readwrite, assign) BOOL requestFirmwareVersionPending;

+ (id)deviceWithSerialNumber:(NSString *)serialNumber devicePath:(NSString *)devicePath;
- (id)initWithSerialNumber:(NSString *)serialNumber devicePath:(NSString *)devicePath;

- (void)sendSyncByteAndWait100ms;
- (void)sendGetHardwareInfoiPodModelInfoATF;
- (void)sendGetHardwareInfoResistorIDATF;
- (void)sendGetHardwareInfoAccPowerLogicLevelAtiPodATF;
- (void)sendGetHardwareInfoBoardFirmwareVersionATF;
- (void)sendGetHardwareInfoAccFireWireVoltageATF;
- (void)sendGetHardwareInfoAccUSBVbusVoltageATF;
- (void)sendGetHardwareInfoAccUSBDPlusVoltageATF;
- (void)sendGetHardwareInfoAccUSBDMinusVoltageATF;
- (void)sendGetHardwareInfoAccAverageCurrentATF;
- (void)sendGetHardwareInfoiPodDetectLogicLevelATF;
- (void)sendGetHardwareInfoAccDetectVoltageATF;
- (void)sendGetHardwareInfoAccTxMarkingStateVoltageATF;
- (void)sendGetHardwareInfoReservedPin14StateATF;
- (void)sendGetHardwareInfoReservedPin17StateATF;
- (void)sendGetHardwareInfoAudioReturnStateATF;
- (void)sendGetHardwareInfoAccUSBVbusVoltageWith1ALoadATF;
- (void)sendGetHardwareInfoAccUSBVbusVoltageWith500mALoadATF;
- (void)sendGetHardwareInfoAccPowerLogicLevelAtAccATF;
- (void)sendGetHardwareInfoSendTestLingoGetModelStringATF;

@end
