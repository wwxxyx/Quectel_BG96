/*
 *  ATSBoxTypes.h
 *  ATSBoxSupport
 *
 *  Created by Joe Basu on 1/8/10.
 *  Copyright © 2010 Apple Inc. All Rights Reserved.
 *
 */

// NOTE: this may break when changing processor architectures and/or the compiler being used
struct ATFHeader {
	UInt8 stray:1;
	UInt8 command:4;
	UInt8 source:2;
	BOOL isHeader:1;
};

enum {
	ATFSourceiPod		= 0,
	ATFSourceAccessory	= 1,
	ATFSourceATSBox		= 2,
	ATFSourceHost		= 3,
};

typedef UInt8 ATFSource;

enum {
	ATFCommandGetHardwareInfo		= 0,
	ATFCommandRetHardwareInfo		= 1,
	ATFCommandSetModeTo				= 2,
	ATFCommandSetHardwareState		= 3,
	ATFCommandSerialDataNoTimestamp	= 4,
	ATFCommandAck					= 5,
	ATFCommandSerialData			= 6,
	ATFCommandTimerRollover			= 7,
	ATFCommandSetSerialRate			= 9,
	ATFCommandSerialDataWithBaud	= 10,
	ATFCommandSendSerialByte		= 11,
	ATFCommandSerialDataBuffer		= 12,
	ATFCommandTimerRolloverCount	= 15,
};

typedef UInt8 ATFCommand;

enum {
	ATFHardwareInfoResistorID								= 0,
	ATFHardwareInfoAccPowerLogicLevelAtiPod					= 1,
	ATFHardwareInfoAccDetectLogicLevel						= 2,
	ATFHardwareInfoBoardFirmwareVersion						= 3,
	ATFHardwareInfoAccFireWireVoltage						= 4,
	ATFHardwareInfoAccUSBVbusVoltage						= 5,
	ATFHardwareInfoAccUSBDPlusVoltage						= 6,
	ATFHardwareInfoAccUSBDMinusVoltage						= 7,
	ATFHardwareInfoAccAverageCurrent						= 8,
	ATFHardwareInfoAccPeakCurrent							= 9,
	ATFHardwareInfoiPodDetectLogicLevel						= 10,
	ATFHardwareInfoAccDetectVoltage							= 11,
	ATFHardwareInfoAccUSBDPlusVoltageWith220kShuntGround	= 12,
	ATFHardwareInfoAccUSBDMinusVoltageWith220kShuntGround	= 13,
	ATFHardwareInfoAccUSBDPlusVoltageWith220kShuntVcc		= 14,
	ATFHardwareInfoAccUSBDMinusVoltageWith220kShuntVcc		= 15,
	ATFHardwareInfoBoardHardwareVersion						= 16,
	ATFHardwareInfoiPodModelInfo							= 17,
	ATFHardwareInfoAccTxMarkingStateVoltage					= 18,
	ATFHardwareInfoReservedPin14State						= 19,
	ATFHardwareInfoReservedPin17State						= 20,
	ATFHardwareInfoAudioReturnState							= 21,
	ATFHardwareInfoAccUSBVbusVoltageWith500mALoad			= 22,
	ATFHardwareInfoAccUSBVbusVoltageWith1ALoad				= 23,
	ATFHardwareInfoAccPowerLogicLevelAtAcc					= 24,
	ATFHardwareInfoSendTestLingoGetModelString				= 25,
	ATFHardwareInfoInterbyteTimingViolation					= 120,
	ATFHardwareInfoTypeCount								= 121,
    ATFHardwareInfoTimerRolloverCount                       = 127,
};

typedef UInt8 ATFHardwareInfo;

enum {
	ATSBoxModeSniffer					= 0,
	ATSBoxModeThroughToAcc				= 2,
	ATSBoxModeDisconnectAll				= 5,
	ATSBoxModeSnifferNoTimestamps		= 251,
	ATSBoxModeSnifferMultibytePacked	= 252,
    ATSBoxModeResetRolloverCount        = 254,
};

typedef UInt8 ATSBoxMode;
