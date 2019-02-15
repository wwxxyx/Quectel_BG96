//
//  IDBTypes.h
//  IDBusKit
//
//  Created by Cody Brimhall on 4/17/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//
//  The IDBus spec can be found by running `make aidspec` from the aispec repo

#import <Foundation/Foundation.h>

typedef NS_ENUM(UInt8, IDBCommand) {
    // Basic Commands
    IDBCommandSetState                           = 0x70,
    IDBCommandReportSetState                     = 0x71,
    IDBCommandGetState                           = 0x72,
    IDBCommandState                              = 0x73,
    IDBCommandGetID                              = 0x74,
    IDBCommandID                                 = 0x75,
    IDBCommandGetInterfaceDeviceInfo             = 0x76,
    IDBCommandInterfaceDeviceInfo                = 0x77,
    IDBCommandGetInterfaceModuleSerialNumber     = 0x78,
    IDBCommandInterfaceModuleSerialNumber        = 0x79,
    IDBCommandGetAccessorySerialNumber           = 0x7A,
    IDBCommandAccessorySerialNumber              = 0x7B,
    IDBCommandTriStar2SystemIDGetESNResponse     = 0x7C,
    IDBCommandTriStar2SystemIDGetESNRequest      = 0x7D,
    IDBCommandTriStar2SystemIDServiceModeRequest = 0x7E,
    IDBCommandTriStar2SystemIDServiceModeAck     = 0x7F,
    
    // Accessory Info Commands
    IDBCommandGetAccessoryInfoString             = 0x80,
    IDBCommandAccessoryInfoString                = 0x81,
    IDBCommandGetAccessoryInfoVersion            = 0x82,
    IDBCommandAccessoryInfoVersion               = 0x83,
    IDBCommandSystemInfoString                   = 0x84,
    
    // Authentication Commands
    IDBCommandGetAuthenticationInfo              = 0x90,
    IDBCommandAuthenticationInfo                 = 0x91,
    IDBCommandGetAuthenticationCertificate       = 0x92,
    IDBCommandAuthenticationCertificate          = 0x93,
    IDBCommandStartAuthentication                = 0x94,
    IDBCommandStartAuthenticationResponse        = 0x95,
    IDBCommandCheckAuthentication                = 0x96,
    IDBCommandCheckAuthenticationResponse        = 0x97,
    IDBCommandGetAuthenticationSignature         = 0x98,
    IDBCommandAuthenticationSignature            = 0x99,
    IDBCommandGetCertificateSerialNumber         = 0x9C,
    IDBCommandGetCertificateSerialNumberResponse = 0x9D,
    
    // Accessory Info Commands (Continued)
    IDBCommandGetLastError                       = 0x9A,
    IDBCommandLastError                          = 0x9B,
    
    // Bulk Data Info Commands
    IDBCommandBulkDataIdentify                   = 0xB0,
    IDBCommandBulkDataIdentifyResponse           = 0xB1,
    IDBCommandBulkDataEndpointInfo               = 0xB2,
    IDBCommandBulkDataEndpointInfoResponse       = 0xB3,
    
    // Bulk Data Transfer Commands
    IDBCommandBulkDataErrorResponse              = 0xBA,
    IDBCommandBulkDataRead                       = 0xBB,
    IDBCommandBulkDataReadResponse               = 0xBC,
    IDBCommandBulkData                           = 0xBD,
    IDBCommandBulkDataResponse                   = 0xBE,
    IDBCommandBulkDataContinue                   = 0xBF,
    
    // Vendor-specific Commands
    IDBCommandProgramOTP                         = 0xE0,
    IDBCommandProgramOTPResponse                 = 0xE1,
    IDBCommandProgramOTPStatus                   = 0xE2,
    IDBCommandProgramOTPStatusResponse           = 0xE3,
    IDBCommandSelfTestControl                    = 0xE4,
    IDBCommandSelfTestControlResponse            = 0xE5,
    
    // Additional Commands
    IDBCommandContinueData                       = 0xFF,
};

/*!
 Enum of possible sources of traffic in IDBus
 */
typedef NS_ENUM(UInt8, IDBSource) {
    IDBSourceAppleDevice = 0,
    IDBSourceAccessory,
    IDBSourceUnknown,
};

/*!
 Returns the length of a given ID Bus packet.
 
 @param commandID The command whose length should be returned.
 @return The length of the passed command ID, or -1 if the command doesn't have a known length.
 */
extern UInt8 IDBPacketGetLength(IDBCommand commandID);

/*!
 Returns a flag indicating whether or not a response is expected for the passed commandID. If a response is expected, the response command ID is assigned to the responseID in-out parameter.
 
 @param commandID The command to test for an expected response.
 @param responseID On return, if a response is expected, this will be set to the response command ID.
 @return YES if a response is expected; NO otherwise.
 */
extern BOOL IDBCommandGetResponseIDs(IDBCommand commandID, NSArray **responseIDs);

/*!
 Converts a packet ID to a human-readable name.
 
 @return The name of the command, or the command ID's hex value if no name can be found.
 */
extern NSString *IDBPacketGetName(IDBCommand commandID);

typedef NS_ENUM(UInt64, IDBFunctionalVariantID) {
    IDBFunctionalVariantIDC1XA     = 0x08C000000000,
    IDBFunctionalVariantIDC1XC48B  = 0x10C000000000,
    IDBFunctionalVariantIDC1XC     = 0x02C000000000,
    IDBFunctionalVariantIDC1XD     = 0x100020000000,
    IDBFunctionalVariantIDC1XF     = 0x12C000000000,
    IDBFunctionalVariantIDB141     = 0x100C00000000,
    IDBFunctionalVariantIDB139B164 = 0x14F080000000,
    IDBFunctionalVariantIDC12C68H  = 0x08F000000000,
};

#pragma mark Command 0x70, Response 0x71    'Set State'
#pragma mark Command 0x72, Response 0x73    'Get State'

struct IDBStateInfo {
    UInt8 ph;
    UInt8 pt;
    UInt16 rsvd0;
    UInt8 sph;
    UInt8 spt;
    UInt16 rsvd1;
};
typedef struct IDBStateInfo IDBStateInfo;

typedef NS_ENUM(UInt8, IDBStateInfoMasks) {
    /* byte 1 */
    IDBStateInfoMaskPH  = 0x80,
    IDBStateInfoMaskPT  = 0x40,
    
    /* byte 3 */
    IDBStateInfoMaskSPH = 0x80,
    IDBStateInfoMaskSPT = 0x40,
};

typedef NS_ENUM(UInt8, IDBStateInfoValues) {
    /* byte 1 */
    IDBStateInfoPHEnabled = 1 << 7,
    IDBStateInfoPTEnabled = 1 << 6,
    
    /* byte 3 */
    IDBStateInfoSPHSupported = 1 << 7,
    IDBStateInfoSPTSupported = 1 << 6,
};

#pragma mark Command 0x74, Response 0x75    'ID'

struct IDBIDInfo {
    UInt8 accx;
    UInt8 db;
    UInt8 usb;
    UInt8 mb;
    UInt8 uart;
    UInt8 pwr;
    UInt8 hv;
    UInt8 ai;
    UInt8 au;
    UInt8 ps;
    UInt8 bp;
    UInt8 rsvd0;
    UInt8 di;
    UInt32 rsvd1;
    IDBFunctionalVariantID functionalVariantID;
};
typedef struct IDBIDInfo IDBIDInfo;

typedef NS_ENUM(UInt8, IDBIDInfoMasks) {
    /* byte 1 */
    IDBIDInfoMaskACCx  = 0xC0,
    IDBIDInfoMaskDB    = 0x20,
    IDBIDInfoMaskUSB   = 0x18,
    IDBIDInfoMaskMB    = 0x04,
    IDBIDInfoMaskUART  = 0x03,
    
    /* byte 2 */
    IDBIDInfoMaskPWR   = 0xF0,
    IDBIDInfoMaskHV    = 0x0C,
    IDBIDInfoMaskAI    = 0x02,
    IDBIDInfoMaskAU    = 0x01,
    
    /* byte 3 */
    IDBIDInfoMaskPS    = 0x80,
    IDBIDInfoMaskBP    = 0x40,
    IDBIDInfoMaskRSVD0 = 0x20,
    IDBIDInfoMaskDI    = 0x10,
};

typedef NS_ENUM(UInt8, IDBIDInfoACCxValues) {
    IDBIDInfoACCxIDOrACC_PWR                  = 0 << 6,
    IDBIDInfoACCxAPDebugUARTRxOrAPDebugUARTTx = 1 << 6,
    IDBIDInfoACCxJTAGSWDDIOOrJTAGSWDCLK       = 2 << 6,
};

typedef NS_ENUM(UInt8, IDBIDInfoDBValues) {
    IDBIDInfoDBDebugAccessory    = 1 << 5,
};

typedef NS_ENUM(UInt8, IDBIDInfoUSBValues) {
    IDBIDInfoUSBNone          = 0 << 3,
    IDBIDInfoUSBHostMode      = 1 << 3,
    IDBIDInfoUSBDeviceMode    = 2 << 3,
    IDBIDInfoUSBAlternateMode = 3 << 3,
};

typedef NS_ENUM(UInt8, IDBIDInfoMBValues) {
    IDBIDInfoMBMikeyBus = 1 << 2,
};

typedef NS_ENUM(UInt8, IDBIDInfoUARTValues) {
    IDBIDInfoUARTNone      = 0,
    IDBIDInfoUART19200bps  = 1,
    IDBIDInfoUART57600bps  = 2,
    IDBIDInfoUART115200bps = 3,
};

typedef NS_ENUM(UInt8, IDBIDInfoPWRValues) {
    IDBIDInfoPWR0V            =  0 << 4,
    // 1-3 treated as 0V today
    IDBIDInfoPWR2_5V          =  4 << 4,
    IDBIDInfoPWR2_6V          =  5 << 4,
    IDBIDInfoPWR2_7V          =  6 << 4,
    IDBIDInfoPWR2_8V          =  7 << 4,
    IDBIDInfoPWR2_9V          =  8 << 4,
    IDBIDInfoPWR3_0V          =  9 << 4,
    IDBIDInfoPWR3_1V          = 10 << 4,
    IDBIDInfoPWR3_2V          = 11 << 4,
    IDBIDInfoPWR3_3V          = 12 << 4,
    // 13-14 treated as 3.3V today
    IDBIDInfoPWRUltraHighMode = 15 << 4,
};

typedef NS_ENUM(UInt8, IDBIDInfoHVValues) {
    IDBIDInfoHV5V  = 0 << 2,
    IDBIDInfoHV16V = 2 << 2,
    IDBIDInfoHV20V = 3 << 2,
};

typedef NS_ENUM(UInt8, IDBIDInfoAIValues) {
    IDBIDInfoAISupported   = 1 << 1,
};

typedef NS_ENUM(UInt8, IDBIDInfoAUValues) {
    IDBIDInfoAUSupported   = 1,
};

typedef NS_ENUM(UInt8, IDBIDInfoPSValues) {
    IDBIDInfoPSAlwaysEnable  = 1 << 7,
};

typedef NS_ENUM(UInt8, IDBIDInfoBPValues) {
    IDBIDInfoBPPowerDeviceOnly = 1 << 6,
};

typedef NS_ENUM(UInt8, IDBIDInfoDIValues) {
    IDBIDInfoDIAutomaticDiagnostics = 1 << 4,
};

#pragma mark Command 0x76, Response 0x77    'Interface Device Info'

struct IDBInterfaceDeviceInfo {
    UInt8 vid;
    UInt8 pid;
    UInt8 rev;
    UInt8 av;
    UInt8 rsvd;
    UInt64 idsn;
};
typedef struct IDBInterfaceDeviceInfo IDBInterfaceDeviceInfo;

typedef NS_ENUM(UInt8, IDBInterfaceDeviceInfoMasks) {
    /* byte 4 */
    IDBInterfaceDeviceInfoMaskAV = 0x80,
};

typedef NS_ENUM(UInt8, IDBInterfaceDeviceInfoVIDValues) {
    IDBInterfaceDeviceInfoVIDApple = 0,
    IDBInterfaceDeviceInfoVIDTI    = 1,
    IDBInterfaceDeviceInfoVIDNXP   = 2,
    // 3-254 allocated to vendors
    IDBInterfaceDeviceInfoVIDDebug = 255,
};

typedef NS_ENUM(UInt8, IDBInterfaceDeviceInfoAVValues) {
    IDBInterfaceDeviceInfoAVApple = 1 << 7,
};

#pragma mark Command 0x78, Response 0x79    'Interface Module Serial Number'

#define IDBInterfaceModuleSerialNumberLength 20

struct IDBInterfaceModuleSerialNumberInfo {
    UInt8 imsn[IDBInterfaceModuleSerialNumberLength];
};
typedef struct IDBInterfaceModuleSerialNumberInfo IDBInterfaceModuleSerialNumberInfo;

#pragma mark Command 0x7A, Response 0x7B    'Accessory Serial Number'

#define IDBAccessorySerialNumberLength 20

struct IDBAccessorySerialNumberInfo {
    UInt8 asn[IDBAccessorySerialNumberLength];
};
typedef struct IDBAccessorySerialNumberInfo IDBAccessorySerialNumberInfo;

#pragma mark Command 0x7C, Response 0x7D    'System ID' (TriStar 2)
#pragma mark Command 0x7E, Response 0x7F    'System ID' (TriStar 2)

#pragma mark Command 0x80, Response 0x81    'Accessory Info String'

typedef NS_ENUM(UInt8, IDBAccessoryInfoString) {
    IDBAccessoryInfoManufacturerString = 0x00,
    IDBAccessoryInfoModelNumberString = 0x01,
    IDBAccessoryInfoNameString = 0x02,
};

#pragma mark Command 0x84    'System Info String'

typedef NS_ENUM(UInt8, IDBSystemInfoString) {
    IDBSystemInfoModelString = 0x00,
    IDBSystemInfoVersionString = 0x01,
};

#pragma mark Command 0x9B    'LastError'

typedef NS_ENUM(UInt8, IDBErrorCode) {
    IDBErrorCodeNoError = 0x00,
    IDBErrorCodeErrorNotSpecified = 0x01,
    IDBErrorCodeInvalidAttributeForRead = 0x02,
    IDBErrorCodeInvalidAttributeForWrite = 0x03,
    IDBErrorCodeInvalidNumberOfParameterBytes = 0x04,
    IDBErrorCodeCommandOutOfSequence = 0x05,
    IDBErrorCodeInternalProcessErrorDuringSignatureGeneration = 0x06,
    
    IDBErrorCodeSelfTestDidNotSucceed = 0x0C,
};

/*!
 @function  IDBStringFromErrorCode
 @abstract  Returns the string representation of the error code.
 */
extern NSString *IDBStringFromErrorCode(IDBErrorCode errorCode);

#pragma mark - Command 0xBD, Response 0xBE    'Bulk Data'

typedef NS_ENUM(UInt8, IDBBulkDataMasks) {
    IDBBulkDataMaskPartial = 0x01 << 7,
    IDBBulkDataMaskLength = 0x3F,
};

typedef NS_ENUM(UInt16, IDBBulkDataFunction) {
    IDBBulkDataFunctionEA         = 0x01,
    IDBBulkDataFunctionHID        = 0x02,
    IDBBulkDataFunctionL21        = 0x03,
    IDBBulkDataFunctioniAP        = 0x04,
    IDBBulkDataFunctionPD         = 0x05,
};


typedef NS_ENUM(UInt8, IDBBulkDataHIDCommand) {
    IDBBulkDataHIDCommandGetHIDDescriptor      = 0x0c,
    IDBBulkDataHIDCommandHIDDescriptor         = 0x0d,
    IDBBulkDataHIDCommandSetHIDReport          = 0x10,
    IDBBulkDataHIDCommandGetHIDReport          = 0x01,
    IDBBulkDataHIDCommandHIDReport             = 0x02,
    IDBBulkDataHIDCommandRetHIDReport          = 0x11,
};

extern NSString *IDBBulkDataPacketGetCommandName(IDBBulkDataFunction functionID, UInt8 commandID);
extern NSString *IDBBulkDataHIDPacketGetCommandName(IDBBulkDataHIDCommand commandID);
extern NSString *IDBBulkDataGetFunctionName(IDBBulkDataFunction function);

/**
 EA commands, derived from spec attached to <rdar://problem/15136299> [IDB] [Content Add] AID Bulk Data Command Response
 */
typedef NS_ENUM(NSUInteger, IDBBulkDataEACommandType) {
    IDBBulkDataEACommandGetProtocolString = 0x0D,
    IDBBulkDataEACommandProtocolString = 0x0E,
    IDBBulkDataEACommandOpenDataSession = 0x3F,
    IDBBulkDataEACommandCloseDataSession = 0x40,
    IDBBulkDataEACommandSetDataTransfer = 0x43,
    IDBBulkDataEACommandRetDataTransfer = 0x42,
};

/**
 Returns the string representation of EA commands over IDBus, given the command ID.
 */
extern NSString *IDBBulkDataEAPacketGetCommandName(IDBBulkDataEACommandType commandID);

/*!
 @function  IDBPacketGetHeaderLength
 @abstract  Returns the length of the packet, excluding the command byte, CRC, and bulk data payload.
 */
extern UInt8 IDBPacketGetHeaderLength(IDBCommand commandID);
