//
//  MBTypes.h
//  MikeyBusKit
//
//  Created by Vinod Madigeri on 10/3/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

// Derived these enums from MikeyBus spec and some from headers given by Georgi Beloev.

/**
 Function group types.
 */
typedef NS_ENUM(uint8_t, MBFunctionGroupType) {
    /// This is the function group type for the functiong group ID where device information / configuration is tracked.
    MBFunctionGroupTypeConfiguration = 0x00,
    /// This is the function group type for the functiong group ID where External Accessory protocol payload is tracked.
    MBFunctionGroupTypeExtendedAccessory = 0x05,
    /// This is the function group type for the functiong group ID where HID data is tracked.
    MBFunctionGroupTypeHID = 0x06,
    /// This is the function group type for the functiong group ID where Vendor Specific payload is sent. Including but not limited to UART (iAP2, I2C,…).
    MBFunctionGroupTypeVendorSpecific = 0xFF,
};

/**
 */
typedef NS_ENUM(uint8_t, MBFunctionGroupID) {
    /// MikeyBus device configuration and control
    MBFunctionGroupIDConfiguration = 0x00,
    /// EA FW update. Speaks a custom protocol.
    MBFunctionGroupIDEAUpdate = 0x02,
    /// Built-In HID. Three buttons.
    MBFunctionGroupIDHIDX90 = 0x03,
    /// Custom HID implemented by the accessory
    MBFunctionGroupIDHID3PP = 0x04,
    /// External accessory protocol for iOS app communication
    MBFunctionGroupIDEAP = 0x05,
    /// UART data (iAP1, iAP2, I2C…)
    MBFunctionGroupIDVendorSpecific = 0xFF,
};

/**
 Command IDs sent over configuration functiong group ID. More details in MikeyBus spec.
 */
typedef NS_ENUM(uint8_t, MBConfigurationCommand) {
    MBConfigurationCommandReserved                          = 0x00,
    MBConfigurationCommandGetDeviceInfo                     = 0x01,
    MBConfigurationCommandRetDeviceInfo                     = 0x02,
    MBConfigurationCommandGetStrDesignator                  = 0x03,
    MBConfigurationCommandRetStrDesignator                  = 0x04,
    MBConfigurationCommandGetConfiguration                  = 0x05,
    MBConfigurationCommandRetConfiguration                  = 0x06,
    MBConfigurationCommandSetConfiguration                  = 0x07,
    MBConfigurationCommandGetErrorStatus                    = 0x08,
    MBConfigurationCommandRetErrorStatus                    = 0x09,
    MBConfigurationCommandResetErrorStatus                  = 0x0A,
    MBConfigurationCommandGetFunctionGroupProperties        = 0x0B,
    MBConfigurationCommandRetFunctionGroupProperties        = 0x0C,
    MBConfigurationCommandGetFunctionGroupPropertyInfo      = 0x0D,
    MBConfigurationCommandRetFunctionGroupPropertyInfo      = 0x0E,
    MBConfigurationCommandSetFunctionGroupPropertyInfo      = 0x0F,
    MBConfigurationCommandRetFunctionGroupPropertyChanged   = 0x10,
    MBConfigurationCommandSetAudioErrorControl              = 0x11,
    MBConfigurationCommandGetAudioErrorControl              = 0x12,
    MBConfigurationCommandRetAudioErrorControl              = 0x13,
    MBConfigurationCommandDeviceTransmitTestModeStart       = 0x14,
    MBConfigurationCommandDeviceTransmitTestModeStop        = 0x15,
    MBConfigurationCommandDeviceReceiveTestModeStart        = 0x16,
    MBConfigurationCommandDeviceReceiveTestModeStop         = 0x17,
    MBConfigurationCommandHostTransmitTestModeStart         = 0x18,
    MBConfigurationCommandHostTransmitTestModeStop          = 0x19,
    MBConfigurationCommandHostReceiveTestModeStart          = 0x1A,
    MBConfigurationCommandHostReceiveTestModeStop           = 0x1B,
    MBConfigurationCommandHostGetErrorStatus                = 0x1C,
    MBConfigurationCommandHostRetErrorStatus                = 0x1D,
    MBConfigurationCommandHostResetErrorStatus              = 0x1E,
    MBConfigurationCommandEndReserved                       = 0x1F, // Reserved from 0x1F—0xFF
};

/**
 Vendor specific commands sent over Vendor specific function group ID (0xFF)
 */
typedef NS_ENUM(uint8_t, MBVendorSpecificCommand) {
    MBVendorSpecificCommandGetAmpCaps = 0x17,
    MBVendorSpecificCommandRetAmpCaps = 0x18,
    MBVendorSpecificCommandGetAmpGain = 0x19,
    MBVendorSpecificCommandRetAmpGain = 0x1A,
    MBVendorSpecificCommandSetAmpGain = 0x1B,
    MBVendorSpecificCommandGetAmpMute = 0x1C,
    MBVendorSpecificCommandRetAmpMute = 0x1D,
    MBVendorSpecificCommandSetAmpMute = 0x1E,
    
    MBVendorSpecificCommandSetDSPModeAndSR = 0x20,
    MBVendorSpecificCommandGetDSPModeAndSR = 0x21,
    MBVendorSpecificCommandRetDSPModeAndSR = 0x22,
    
    MBVendorSpecificCommandSetPCMBits = 0x50,
    MBVendorSpecificCommandGetPCMBits = 0x51,
    MBVendorSpecificCommandRetPCMBits = 0x52,
    
    MBVendorSpecificCommandSetSR = 0x54,
    MBVendorSpecificCommandGetSR = 0x55,
    MBVendorSpecificCommandRetSR = 0x56,
    
    MBVendorSpecificCommandGetCHDelay = 0x59,
    MBVendorSpecificCommandRetCHDelay = 0x5A,
    
    MBVendorSpecificCommandSetEndpointEnable = 0x60,
    MBVendorSpecificCommandGetEndpointEnable = 0x61,
    MBVendorSpecificCommandRetEndpointEnable = 0x62,
    
    MBVendorSpecificCommandSetEndpointCHID = 0x64,
    MBVendorSpecificCommandGetEndpointCHID = 0x65,
    MBVendorSpecificCommandRetEndpointCHID = 0x66,
    
    MBVendorSpecificCommandSetOTPBufferValue = 0x68,
    MBVendorSpecificCommandGetOTPBufferValue = 0x69,
    MBVendorSpecificCommandRetOTPBufferValue = 0x6A,
    
    /// iAP2 over UART over MB Bulk data for Function Group ID 0xFF (VendorSpecific); Host—To—Device
    MBVendorSpecificCommandSetComFifo = 0x6D,
    MBVendorSpecificCommandGetComFifo = 0x6E,
    /// iAP2 over UART over MB Bulk data for Function Group ID 0xFF (VendorSpecific); Device-To-Host
    MBVendorSpecificCommandRetComFifo = 0x70,
    
    MBVendorSpecificCommandSetComCtrl = 0x71,
    MBVendorSpecificCommandGetComCtrl = 0x72,
    MBVendorSpecificCommandRetComCtrl = 0x74,
    
    MBVendorSpecificCommandGetComStat = 0x75,
    MBVendorSpecificCommandRetComStat = 0x76,
    
    MBVendorSpecificCommandSetGPIOConfig = 0x7C,
    MBVendorSpecificCommandGetGPIOConfig = 0x7D,
    MBVendorSpecificCommandRetGPIOConfig = 0x7E,
    
    MBVendorSpecificCommandRetIntMsg = 0x8A,
    
    MBVendorSpecificCommandSetChipStatus = 0x91,
    MBVendorSpecificCommandGetChipStatus = 0x92,
    MBVendorSpecificCommandRetChipStatus = 0x94,
};


/**
 HID commands sent over Function group type HID.
 */
typedef NS_ENUM(uint8_t, MBHIDCommand) {
    MBHIDCommandGetReport = 0x01,
    MBHIDCommandSetReport = 0x09,
    MBHIDCommandRetReport = 0x81,
    MBHIDCommandRetIntReport = 0x89,
};

/**
 Vendor specific endpoints sent over Vendor Specific function group ID.
 */
typedef NS_ENUM(uint8_t, MBVendorSpecificEndPoint) {
    MBVendorSpecificEndPointChipTop = 0x00,
    MBVendorSpecificEndPointLineOutA = 0x01,
    MBVendorSpecificEndPointLineOutB = 0x02,
    MBVendorSpecificEndPointRMeter = 0x03,
    
    /// iAP2 over UART endpoint
    MBVendorSpecificEndPointUART = 0x04,
    MBVendorSpecificEndPointI2C = 0x05,
    MBVendorSpecificEndPointGPIO0 = 0x06,
    MBVendorSpecificEndPointGPIO1 = 0x07,
    MBVendorSpecificEndPointGPIO2 = 0x08,
    MBVendorSpecificEndPointGPIO3 = 0x09,
    MBVendorSpecificEndPointGPIO4 = 0x0A,
    MBVendorSpecificEndPointGPIO5 = 0x0B,
    MBVendorSpecificEndPointGPIO6 = 0x0C,
    MBVendorSpecificEndPointGPIO7 = 0x0D,
    MBVendorSpecificEndPointWake = 0x0E,
    MBVendorSpecificEndPointMBBusIf = 0x0F,
    MBVendorSpecificEndPointOTP = 0x10,
    MBVendorSpecificEndPointVLLDO = 0x11,
    MBVendorSpecificEndPointLineInA = 0x12,
    MBVendorSpecificEndPointLineInB = 0x14,
    MBVendorSpecificEndPointDSPMode = 0x15,
};

typedef NS_ENUM(UInt8, MBSource) {
    MBSourceAccessory    = 0x00,
    MBSourceAppleDevice  = 0x01,
    MBSourceUnknown      = 0xFF,
};

/**
 Enum to define HID function group property number
 */
typedef NS_ENUM(UInt8, MBHIDFunctionGroupProperty) {
    MBHIDFunctionGroupPropertyCountryCode       = 0x01,
    MBHIDFunctionGroupPropertyReportDescriptor  = 0x02,
};

/**
 Enum to define EA function group property number
 */
typedef NS_ENUM(UInt8, MBEAFunctionGroupProperty) {
    MBEAFunctionGroupPropertyProtocolString    = 0x01,
    MBEAFunctionGroupPropertySessionControl    = 0x02,
};

