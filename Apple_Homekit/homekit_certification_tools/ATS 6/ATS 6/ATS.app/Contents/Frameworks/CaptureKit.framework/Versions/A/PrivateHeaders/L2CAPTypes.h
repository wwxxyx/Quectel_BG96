//
//  L2CAPTypes.h
//  Bluetooth L2CAP Common Types
//
//  Created by Edgard Lima on 08/13/18.
//  Copyright (c) 2018 Apple. All rights reserved.
//

/// Connection Request Protocol/Service Multiplexer. The PSM is minimum 16-bits. If the second byte is odd, then there will be an extra byte. If that extra byte is odd there will be another one, and so on. That is the ISO 3309 extension mechanism for address fileds.
typedef NS_ENUM(UInt32, L2CAPPsm) {
    /// Service Discovery Protocol (SDP)
    L2CAPPsmSdp = 0x0001,
    /// RFCOMM with TS 07.10
    L2CAPPsmRFCOMM = 0x0003,
    /// TCS-BIN. Telephony Control Specification / TCS Binary
    L2CAPPsmTcsBin = 0x0005,
    /// TCS-BIN-CORDLESS. Telephony Control Specification / TCS Binary
    L2CAPPsmTcsBinCordless = 0x0007,
    /// Bluetooh Nerwork Encapsulation Protocol (BNEP)
    L2CAPPsmBnep = 0x000F,
    /// Human Interface Device
    L2CAPPsmHIDControl = 0x0011,
    /// Human Interface Device
    L2CAPPsmHIDInterrupt = 0x0013,
    /// UPnP (ESDP)
    L2CAPPsmUPnP = 0x0015,
    /// Audio/Video Control Transport Protocol
    L2CAPPsmAVCTP = 0x0017,
    /// Audio/Video Control Distribution Protocol
    L2CAPPsmAVDTP = 0x0019,
    /// Audio/Video Remote Control Profile
    L2CAPPsmAVCTPBrowsing = 0x001B,
    /// Unrestricited Digital Information Profile (UDI)
    L2CAPPsmUDICPlane = 0x001D,
    /// Attributes
    L2CAPPsmATT = 0x001F,
    /// 3D Synchronization Profile
    L2CAPPsm3DSP = 0x0021,
    /// Internet Protocol Support Profile (IPSP)
    L2CAPPsmLePsmIpsp = 0x0023,
    /// Object Transfer Service (OTS)
    L2CAPPsmOTS = 0x0025,
};
