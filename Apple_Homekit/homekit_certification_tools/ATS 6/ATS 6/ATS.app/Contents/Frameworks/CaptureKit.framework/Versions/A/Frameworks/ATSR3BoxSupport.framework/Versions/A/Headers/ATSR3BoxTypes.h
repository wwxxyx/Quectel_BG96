/*
 *  ATSR3BoxTypes.h
 *  ATSR3BoxSupport
 *
 *  Created by Marin Balde on 4/16/12.
 *  Copyright © 2012 Apple Inc. All Rights Reserved.
 *
 */

struct ATFR3Header {
    UInt8 startOfFrame;
    UInt8 command;
};

extern const UInt8 gATFR3FrameBoundaryCharacter;
extern const UInt8 gATFR3EscapeCharacter;
extern const UInt8 gATFR3EscapeValue;

extern const SInt8 gATFR3VariableLengthPayload;

// commands sent from the ATS Host Application have bit 7 set in the command value
enum {
    ATFR3SourceATSR3Box             = 0,
    ATFR3SourceATSHostApplication   = 0x80,
};
typedef UInt8 ATFR3Source;

enum {
/* Source is ATS Lightning Box  or ATS Smart Connector Box (i.e. Response Command) */
    ATFR3CommandAbsoluteTime                            = 0x01,
    ATFR3CommandTimerTick                               = 0x02,
    ATFR3CommandSystemInfo                              = 0x03,
    ATFR3CommandAccessoryConnectionState                = 0x04,
    ATFR3CommandAccessoryPowerInfo                      = 0x05,
    ATFR3CommandDevicePowerInfo                         = 0x06,
    ATFR3CommandBoardTemperature                        = 0x07,
    ATFR3CommandReportDevicePowerLoadTest               = 0x08,
    ATFR3CommandReportAccessoryPowerSourceAndState      = 0x09,
    ATFR3CommandReportMuxConfigAndState                 = 0x0A,       
    ATFR3CommandReportBrickDetection                    = 0x0B,
    ATFR3CommandReportPinStateAndConfig                 = 0x0C,
    ATFR3CommandReportDACValue                          = 0x0D,
    ATFR3CommandRawADCReading                           = 0x0E,
    ATFR3CommandReportOperatingMode                     = 0x0F,
    ATFR3CommandReportReports                           = 0x10,
    ATFR3CommandDeviceConnectionInfo                    = 0x11,
    ATFR3CommandAccessoryOrientationState               = 0x12,
    ATFR3CommandReportDevicePowerSourceAndState         = 0x13,
    ATFR3CommandReportDeviceDetection                   = 0x14,
    ATFR3CommandReportADCCalibration                    = 0x15,
    ATFR3CommandReportADCOffsetData                     = 0x16,
    ATFR3CommandReportSmartBrickDetection               = 0x17,

    ATFR3CommandSerialRawUART0Data                      = 0x20,
    ATFR3CommandSerialRawUART1Data                      = 0x21,
    ATFR3CommandSerialRawUART2Data                      = 0x22,
    ATFR3CommandSerialRawUART3Data                      = 0x23,
    ATFR3CommandSerialRawUART1Timing                    = 0x24,
    ATFR3CommandSerialRawUART2Timing                    = 0x25,
    ATFR3CommandSerialUART1TimingStatistics             = 0x26,
    ATFR3CommandSerialUART2TimingStatistics             = 0x27,
    
    ATFR3CommandIDBUSRawData                            = 0x30,
    ATFR3CommandIDBUSRawWake                            = 0x31,
    ATFR3CommandIDBUSRawBreak                           = 0x32,
    ATFR3CommandIDBUSRawEdgeTiming                      = 0x33,
    ATFR3CommandIDBUSTimingStatistics                   = 0x34,
    ATFR3CommandReportIDBUSMode                         = 0x35,
    ATFR3CommandReportIDBUSHostSendData                 = 0x36,
    
    ATFR3CommandReportEnterBootloader                   = 0x70,

    ATFR3CommandReportFirmwareUpdateMode                = 0x72,
    ATFR3CommandReportCRC32Checksum                     = 0x73,
    ATFR3CommandReportExecuteFromAddress                = 0x74,
    ATFR3CommandReportReadData                          = 0x75,
    ATFR3CommandReportEraseSectors                      = 0x76,
    ATFR3CommandReportWriteData                         = 0x77,
    ATFR3CommandReportWriteConfigInfo                   = 0x78,
    ATFR3CommandReportTest                              = 0x79,
    ATFR3CommandReportMCUSerialNumber                   = 0x7B,
    
    ATFR3CommandErrorMsg                                = 0x7F,
    
/* Source is ATS host application (i.e. Request Command) */                                  
    ATFR3CommandGetAbsoluteTime                         = 0x81, 
    ATFR3CommandGetTimerTick                            = 0x82,
    ATFR3CommandGetSystemInfo                           = 0x83,
    ATFR3CommandGetAccessoryConnectionState             = 0x84,
    ATFR3CommandGetAccessoryPowerInfo                   = 0x85,
    ATFR3CommandGetDevicePowerInfo                      = 0x86,
    ATFR3CommandGetBoardTemperature                     = 0x87,
    ATFR3CommandRunDevicePowerLoadTest                  = 0x88,
    ATFR3CommandGetOrSetAccessoryPowerSourceAndState    = 0x89,
    ATFR3CommandGetOrSetMuxConfigAndState               = 0x8A,
    ATFR3CommandRunBrickDetection                       = 0x8B,
    ATFR3CommandGetOrSetPinStateAndConfig               = 0x8C, 
    ATFR3CommandGetOrSetDACValue                        = 0x8D,
    ATFR3CommandGetRawADCReading                        = 0x8E,
    ATFR3CommandGetOrSetOperatingMode                   = 0x8F,
    ATFR3CommandGetOrSetReports                         = 0x90,
    ATFR3CommandGetDeviceConnectionInfo                 = 0x91,
    ATFR3CommandGetAccessoryOrientationState            = 0x92,
    ATFR3CommandControlDevicePowerSourceAndState        = 0x93,
    ATFR3CommandRunDeviceDetection                      = 0x94,
    ATFR3CommandRunADCCalibration                       = 0x95,
    ATFR3CommandSetADCOffsetData                        = 0x96,
    ATFR3CommandRunSmartBrickDetection                  = 0x97,
    
    ATFR3CommandGetOrSetIDBUSMode                       = 0xB5,
    ATFR3CommandIDBUSHostSendData                       = 0xB6,
    
    ATFR3CommandEnterBootloader                         = 0xF0,

    ATFR3CommandFirmwareUpdateMode                      = 0xF2,
    ATFR3CommandComputeCRC32Checksum                    = 0xF3,
    ATFR3CommandExecuteFromAddress                      = 0xF4,
    ATFR3CommandReadData                                = 0xF5,
    ATFR3CommandEraseSectors                            = 0xF6,
    ATFR3CommandWriteData                               = 0xF7,
    ATFR3CommandWriteConfigInfo                         = 0xF8,
    ATFR3CommandRunTest                                 = 0xF9,
    ATFR3CommandRebootMCU                               = 0xFA,
    ATFR3CommandGetMCUSerialNumber                      = 0xFB,
};
typedef UInt8 ATFR3Command;

extern const SInt8 gATFR3CommandPayloadLength[];
extern NSString * const gATFR3CommandName[];

enum {
    ATFR3ATSResponseStatusCodeCommandSuccess                     = 0x00,
    ATFR3ATSResponseStatusCodeInvalidCommand                     = 0x01,
    ATFR3ATSResponseStatusCodeSourceAddressError                 = 0x02,
    ATFR3ATSResponseStatusCodeDestinationAddressError            = 0x03,
    ATFR3ATSResponseStatusCodeSourceAddressNotMapped             = 0x04,
    ATFR3ATSResponseStatusCodeDestinationAddressNotMapped        = 0x05,
    ATFR3ATSResponseStatusCodeCountError                         = 0x06,
    ATFR3ATSResponseStatusCodeInvalidSector                      = 0x07,
    ATFR3ATSResponseStatusCodeSectorNotBlank                     = 0x08,
    ATFR3ATSResponseStatusCodeSectorNotPreparedForWriteOperation = 0x09,
    ATFR3ATSResponseStatusCodeCompareError                       = 0x0A,
    ATFR3ATSResponseStatusCodeBusy                               = 0x0B,
    ATFR3ATSResponseStatusCodeInvalidParameter                   = 0x0C,
    ATFR3ATSResponseStatusCodeInvalidMode                        = 0x0D,
    
    ATFR3ATSResponseStatusCodeCommandError                       = 0xFF,
};
typedef UInt8 ATFR3ATSResponseStatusCode;

/* Command 0x83, Response 0x03    'Get System Information' */
enum {
    ATFR3ModeBootloader     = 0,
    ATFR3ModeApplication    = 1,
};
typedef UInt8 ATFR3Mode;


typedef NS_ENUM(NSUInteger, ATFR3LightningBoardStatus) {
    ATFR3LightningBoardStatusTemperatureCircuitError                    = (0x01 << 0),
    ATFR3LightningBoardStatusI2CDACCircuitError                         = (0x01 << 1),
    ATFR3LightningBoardStatusGroundDetectionCircuitError                = (0x01 << 2),
    ATFR3LightningBoardStatusPowerDetectionAndEmulationCircuitError     = (0x01 << 3),
    ATFR3LightningBoardStatusIDBUSSniffAndEmulationCircuitError         = (0x01 << 4),
    ATFR3LightningBoardStatusSupplementaryFlagError                     = (0x01 << 5),
    ATFR3LightningBoardStatusUSBTransmitBufferFullError                 = (0x01 << 6),
    ATFR3LightningBoardStatusHardwareFault                              = (0x01 << 7),
};

typedef NS_ENUM(NSUInteger, ATFR3SmartConnectorBoardStatus) {
    ATFR3SmartConnectorBoardStatusTemperatureError          = (0x01 << 0),
    ATFR3SmartConnectorBoardStatusPowerError                = (0x01 << 1),
    ATFR3SmartConnectorBoardStatusAIDError                  = (0x01 << 2),
    ATFR3SmartConnectorBoardStatusAuthenticationError       = (0x01 << 3),
    ATFR3SmartConnectorBoardStatusMCUError                  = (0x01 << 4),
    ATFR3SmartConnectorBoardStatusEloadError                = (0x01 << 5),
    ATFR3SmartConnectorBoardStatusINAError                  = (0x01 << 6),
    ATFR3SmartConnectorBoardStatusHardwareFault             = (0x01 << 7),
};

/* Command 0x84, Response 0x04    'Accessory Connection State' */
enum {
    ATFR3AccessoryConnectionStateDisconnected   = 0,
    ATFR3AccessoryConnectionStateConnected      = 1,
    ATFR3AccessoryConnectionStateDisconnectStart = 2,
    ATFR3AccessoryConnectionStateConnectStart   = 3,
};
typedef UInt8 ATFR3AccessoryConnectionState;

/* All GetOrSet commands */
enum {
    ATFR3CommandTypeSet = 0,
    ATFR3CommandTypeGet = 1,
};
typedef UInt8 ATFR3CommandType;

/* Command 0x88, Response 0x08    'Power Load Test' */
enum { // bit-field
    ATFR3PowerLoadFlagDACError                   = 0x0001,
    ATFR3PowerLoadFlagBoardTemperatureError      = 0x0002,
    ATFR3PowerLoadFlagInvalidParameterDuration   = 0x0004,
    ATFR3PowerLoadFlagInvalidParameterCurrent    = 0x0008,
    ATFR3PowerLoadFlagVbusBelow4Dot5Volts        = 0x0010,
    
    ATFR3PowerLoadFlagAccessoryNotPresentError   = 0x0040,
    ATFR3PowerLoadFlagDeviceConnectedError       = 0x0080,
    ATFR3PowerLoadFlagBoardInIDBUSEmulationMode  = 0x0100,
};

/* Command 0x89, Response 0x09    'Report/Control Accessory Power Source and State' */
enum {
    ATFR3SourceATSBoardAndStateDisabled     = 0,
    ATFR3SourceATSBoardAndStateEnabled      = 1,
    ATFR3SourceAppleDeviceAndStateEnabled   = 2,
};
typedef UInt8 ATFR3SourceAndState;

/* Command 0x8A, Response 0x0A    'Mux Config and State' */
enum {
    ATFR3MuxDP1Accessory    = 0,
    ATFR3MuxDP1AppleDevice  = 1,
    ATFR3MuxDP2Accessory    = 2,
    ATFR3MuxDP2AppleDevice  = 3,
    ATFR3MuxUART            = 4,
};
typedef UInt8 ATFR3MuxID;

enum {
    ATFR3MuxSignalDPSniff           = 0,
    ATFR3MuxSignalDPNoConnection    = 1,
    ATFR3MuxSignalDPPassthrough     = 2,
    ATFR3MuxSignalDPUARTPassthrough = 3,
};
typedef UInt8 ATFR3MuxSignalState;

enum {
    ATFR3MuxConnectionDisabled  = 0,
    ATFR3MuxConnectionEnabled   = 1,
};
typedef UInt8 ATFR3MuxConnectionState;

/* Command 0x8B, Response 0x0B     'Brick Detection' */
enum { // bit-field
    ATFR3BrickDetectionFlagDMinusFollowsDPlusLow220kShuntToGroundConnectedToDPlus  = 0x0001,
    ATFR3BrickDetectionFlagDPlusFollowsDMinusLow220kShuntToGroundConnectedToDMinus = 0x0002,
    ATFR3BrickDetectionFlagDMinusFollowsDPlusHigh220kShuntToVccConnectedToDPlus    = 0x0004,
    ATFR3BrickDetectionFlagDPlusFollowsDMinusHigh220kShuntToVccConnectedToDMinus   = 0x0008,
    ATFR3BrickDetectionFlagErrorVbusBelow4Point5Volts                              = 0x0010,
    ATFR3BrickDetectionFlagErrorAccessoryNotPresent                                = 0x0040,
    ATFR3BrickDetectionFlagErrorDeviceConnected                                    = 0x0080,
    ATFR3BrickDetectionFlagBoardInIDBUSEmulationMode                               = 0x0100,
};

/* Command 0x17, Response 0x97     'Smart Brick Detection' */
enum { // bit-field
    ATFR3SmartBrickDetectionFlagErrorVbusBelow4Point5Volts                         = (1 << 4),
    ATFR3SmartBrickDetectionFlagErrorAccessoryNotPresent                           = (1 << 6),
    ATFR3SmartBrickDetectionFlagErrorDeviceConnected                               = (1 << 7),
    ATFR3SmartBrickDetectionFlagBoardInIDBUSEmulationMode                          = (1 << 8),
};

/* Command 0x8C, Response 0x0C    'Pin State and Config' */
enum {
    ATFR3IOPinResistanceModeInternalPullUp   = 0,
    ATFR3IOPinResistanceModeRepeater         = 1,
    ATFR3IOPinResistanceModeTriState         = 2,
    ATFR3IOPinResistanceModeInternalPullDown = 3,
};
typedef UInt8 ATFR3IOPinResistanceMode;

enum {
    ATFR3IOPinDrainModeNormal = 0,
    ATFR3IOPinDrainModeOpen   = 1,
};
typedef UInt8 ATFR3IOPinDrainMode;

enum {
    ATFR3IOPinDirectionInput  = 0,
    ATFR3IOPinDirectionOutput = 1,
};
typedef UInt8 ATFR3IOPinDirection;

/* Command 0x8F, Response 0x0F    'Get/Set or Report Board Mode' */
enum {
    ATFR3OperatingModeSniffingDisabled = 0,
    ATFR3OperatingModeSniffingEnabled  = 1,
};
typedef UInt8 ATFR3OperatingMode;

/* Command 0x91, Response 0x11    'Device Connection Info' */
enum {
    ATFR3DeviceConnectionStateDisconnected     = 0,
    ATFR3DeviceConnectionStateConnected        = 1,
};
typedef UInt8 ATFR3DeviceConnectionState;

enum { // bit-field
    ATFR3IOPinValueOutput = 0x01,
    ATFR3IOPinValueInput  = 0x02,
};

enum { // bit-field
    ATFR3IOPinSetMaskInitializeIOToDefaultState = 0x01,
    ATFR3IOPinSetMaskUpdateIOPinValue           = 0x02,
    ATFR3IOPinSetMaskUpdateIOPinDirection       = 0x04,
    ATFR3IOPinSetMaskUpdateIOPinDrainMode       = 0x08,
    ATFR3IOPinSetMaskUpdateIOPinResistanceMode  = 0x10,
    ATFR3IOPinSetMaskUpdateIOPinFunction        = 0x20,
};

// state is set to 0xFF if an incorrect muxID is selected
extern const UInt8 ATFR3MuxIDIncorrect;

/* Command 0x8E, Response 0x0E    'Get Raw ADC reading',
   Command 0x96, Response 0x16	  'Set/Report ADC offset data'*/
enum {
    ATFR3ChannelBrickDetectLine              = 0,
    ATFR3ChannelAccessoryPowerVoltage        = 1,
    ATFR3ChannelAccessoryPowerCurrent        = 2, 
    ATFR3ChannelTemperatureSensor            = 3,

    ATFR3ChannelDevicePowerInCurrent         = 5,
    ATFR3ChannelDevicePowerInVoltage         = 6,
};
typedef UInt8 ATFR3Channel;

/* Report 0x24    'Serial Raw UART1 Edge Timing Information' */
enum {
    ATFR3TimingEdgeFalling  = 0,
    ATFR3TimingEdgeRising   = 1,
};
typedef UInt8 ATFR3TimingEdge;

/* Command 0x90, Response 0x10        'Configure ATF frame-reporting' */
enum { // bit-field
    ATFR3ReportAll                  = 0x0001,
    ATFR3ReportIDBUSRawWakeAndBreak = 0x0002,
    ATFR3ReportIDBUSModeAndPacketResponses = 0x0004,
    ATFR3ReportIDBUSRawEdgeTiming   = 0x0008,
    ATFR3ReportSerialRawUART1Data   = 0x0010,
    ATFR3ReportSerialRawUART2Data   = 0x0020,
    ATFR3ReportSerialRawUART1Timing = 0x0040,
    ATFR3ReportSerialRawUART2Timing = 0x0080,
    ATFR3ReportTimerTick            = 0x0100,
    ATFR3ReportAccessoryPowerInfo   = 0x0200,
    ATFR3ReportDevicePowerInfo      = 0x0400,
    ATFR3ReportBoardTemperature     = 0x0800,
    ATFR3ReportSerialUART1TimingStatistics = 0x1000,
    ATFR3ReportSerialUART2TimingStatistics = 0x2000,
    ATFR3ReportMuxConfigAndState    = 0x4000,
};

/* Command 0x92, Response 0x12     'Accessory Orientation Info' */
enum {
    ATFR3AccessoryOrientationError    = 0,
    ATFR3AccessoryOrientationOK       = 1,
    ATFR3AccessoryOrientationUnknown  = 2,
};
typedef UInt8 ATFR3AccessoryOrientation;

/* Command 0x94, Response 0x14    'Run/Report Device Detection' */
enum {
    ATFR3DeviceDetectionStatusOK                = 0,
    ATFR3DeviceDetectionStatusAccessoryPresent  = 1,
    ATFR3DeviceDetectionStatusNoDevice          = 2,
    ATFR3DeviceDetectionStatusInProgress        = 3,
    ATFR3DeviceDetectionStatusIDBUSError        = 4,
    ATFR3DeviceDetectionStatusInvalidResponse   = 5,
    ATFR3DeviceDetectionStatusRTXCDevice        = 6,
};
typedef UInt8 ATFR3DeviceDetectionStatus;

/* Command 0x95, Response 0x15    'Set/Report ADC offset data' */
typedef enum : UInt8 {
    ATFR3ADCCalibrationStatusOK                                     = 0,
    ATFR3ADCCalibrationStatusErrorAccessoryCurrentMonitorChannel    = 1,
    ATFR3ADCCalibrationStatusErrorIDevicePINCurrentMonitorChannel   = 2,
    ATFR3ADCCalibrationStatusErrorAccessoryAndIDevicePINCurrentMonitorChannels = 3,
} ATFR3ADCCalibrationStatus;

/* Command 0xB5, Response 0x35    'Report/Control IDBUS Mode' */
enum {
    ATFR3IDBUSModeIdle                  = 0,
    ATFR3IDBUSModePassthroughSniffing   = 1,
    ATFR3IDBUSModeHostEmulation         = 2,
    ATFR3IDBUSModeSlaveEmulation        = 3,
};
typedef UInt8 ATFR3IDBUSMode;

/* Command 0xB6, Response 0x36    'IDBUS Host Send Data' */
enum { // bit-field
    ATFR3IDBUSHostSendDataStateNoResponse                 = 0x01,
    ATFR3IDBUSHostSendDataStateCommandFailedIDBUSIdleMode = 0x02,
};

/* Command 0xF2, Response 0x72    'Firmware Update Mode' */
enum {
    ATFR3FirmwareUpdateModeSetupContractAndStartUpdate = 0,
    ATFR3FirmwareUpdateModeValidateFirmwareImage       = 1,
    ATFR3FirmwareUpdateModeExecuteApplication          = 2,
};
typedef UInt8 ATFR3FirmwareUpdateMode;

/* Command 0xF7, Response 0x77    'Write Data' */
extern const UInt16 ATFR3WriteDataMaxDataSize;

/* Command 0xF9, Response 0x79    'Test Commands' */
enum {
    ATFR3TestModeDisableDebugLogging                     = 0x00,
    ATFR3TestModeDebugLoggingLevel1                      = 0x01,
    ATFR3TestModeDebugLoggingLevel2                      = 0x02,
    ATFR3TestModeDebugLoggingLevel3                      = 0x03,
    ATFR3TestModeDebugLoggingLevel4                      = 0x04,
    
    ATFR3TestModeAllBoardTests                           = 0x10,
    ATFR3TestModeBoardInformation                        = 0x11,
    ATFR3TestModeGetSystemStatusFlags                    = 0x12,
    ATFR3TestModeClearSystemStatusFlags                  = 0x13,
    ATFR3TestModeEnableClockOutput                       = 0x14,
    ATFR3TestModeDisableClockOutput                      = 0x15,

    ATFR3TestModeTemperatureCircuitTest                  = 0x20,
    ATFR3TestModeI2CDACCircuitTest                       = 0x21,
    ATFR3TestModeGroundDetectionCircuitTest              = 0x22,
    ATFR3TestModePowerDetectionAndEmulationCircuitTest   = 0x23,
    ATFR3TestModeIDBUSSniffAndEmulationCircuitTest       = 0x24,
    
    ATFR3TestModeDisableASCIIReporting                   = 0xFE,
    ATFR3TestModeEnableASCIIReporting                    = 0xFF,
};
typedef UInt8 ATFR3TestMode;
