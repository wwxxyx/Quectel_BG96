//
//  BSPowerDeliveryTypes.h
//  BusSniffer
//
//  Created by Zachary Church on 3/29/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#ifndef BSPowerDeliveryTypes_h
#define BSPowerDeliveryTypes_h

typedef NS_ENUM(UInt8, BSPDCommandType) {
    BSPDCommandTypeControl,
    BSPDCommandTypeData,
};

typedef NS_ENUM(UInt8, BSPDControlCommand) {
    BSPDControlCommandNoCommand = 0x00,
    BSPDControlCommandGoodCRC = 0x01,
    BSPDControlCommandGotoMin = 0x02,
    BSPDControlCommandAccept = 0x03,
    BSPDControlCommandReject = 0x04,
    BSPDControlCommandPSReady = 0x06,
    BSPDControlCommandGetSourceCapabilities = 0x07,
    BSPDControlCommandGetSinkCapabilities = 0x08,
    BSPDControlCommandPRSwap = 0x0A,
    BSPDControlCommandWait = 0x0C,
    
    BSPDControlCommandHardReset = 0x80,
};

typedef NS_ENUM(UInt8, BSPDDataCommand) {
    BSPDDataCommandNoCommand = 0x00,
    BSPDDataCommandSourceCapabilities = 0x01,
    BSPDDataCommandRequest = 0x02,
    BSPDDataCommandSinkCapabilities = 0x04,
};

typedef NS_ENUM(UInt8, BSPDPDOType) {
    BSPDPDOTypeFixed = 0x00,
    BSPDPDOTypeBattery = 0x01,
    BSPDPDOTypeVariable = 0x02,
};

typedef NS_ENUM(UInt8, BSPDRole) {
    BSPDRoleSource,
    BSPDRoleSink,
};

typedef NS_ENUM(UInt8, BSPDDataObjectField) {
    BSPDDataObjectFieldDualRolePower,
    BSPDDataObjectFieldUSBSuspendSupported,
    BSPDDataObjectFieldExternallyPowered,
    BSPDDataObjectFieldUSBCommunicationsCapable,
    BSPDDataObjectFieldDataRoleSwap,
    BSPDDataObjectFieldPeakCurrent,
    BSPDDataObjectFieldHigherCapability,

    BSPDDataObjectFieldVoltageInFiftymVUnits,
    BSPDDataObjectFieldMaxVoltageInFiftymVUnits,
    BSPDDataObjectFieldMinVoltageInFiftymVUnits,

    BSPDDataObjectFieldMaxCurrentInTenmAUnits,
    BSPDDataObjectFieldOperationalCurrentInTenmAUnits,
    BSPDDataObjectFieldOperatingCurrentInTenmAUnits,
    BSPDDataObjectFieldMaxOperatingCurrentInTenmAUnits,
    BSPDDataObjectFieldMinOperatingCurrentInTenmAUnits,

    BSPDDataObjectFieldMaxAllowablePowerInTwoHundredFiftymWUnits,
    BSPDDataObjectFieldOperationalPowerInTwoHundredFiftymWUnits,
    BSPDDataObjectFieldOperatingPowerInTwoHundredFiftymWUnits,
    BSPDDataObjectFieldMaxOperatingPowerInTwoHundredFiftymWUnits,
    BSPDDataObjectFieldMinOperatingPowerInTwoHundredFiftymWUnits,
    
    BSPDDataObjectFieldObjectPosition,
    BSPDDataObjectFieldGiveBack,
    BSPDDataObjectFieldCapabilityMismatch,
    BSPDDataObjectFieldNoUSBSuspend,
};

extern NSString * BSPDLocalizedNameForDataCommand(BSPDDataCommand command);
extern NSString * BSPDLocalizedNameForControlCommand(BSPDControlCommand command);
extern NSString * BSPDLocalizedNameForPDOType(BSPDPDOType type);
extern NSString * BSPDLocalizedNameForPDOField(BSPDDataObjectField field);
extern NSString * BSPDLocalizedValueForPDOField(BSPDDataObjectField field, UInt16 value);


#endif /* BSPowerDeliveryTypes_h */
