//
//  ATSR3Utilities.h
//  ATSR3BoxSupport
//
//  Created by Mark Hamlin on 8/6/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ATFR3Frame.h"

typedef enum : uint8_t {
    // Apple brick ids -- R2 and R4 are fixed at 49.9 kΩ
    EightPinUSBApple1BrickID = 0, // R1: 75 kΩ, R3: 75 kΩ
    //EightPinUSBApple2BrickID, // R1: 75 kΩ, R3: 43.2 kΩ -- Duplicate for 1.0A
    EightPinUSBApple3BrickID, // R1: 75 kΩ, R3: 24.9 kΩ
    //EightPinUSBApple4BrickID, // R1: 43.2 kΩ, R3: 75 kΩ  -- Duplicate for 2.1A
    //EightPinUSBApple5BrickID, // R1: 43.2 kΩ, R3: 43.2 kΩ -- Duplicate for 2.4A
    EightPinUSBApple6BrickID, // R1: 43.2 kΩ, R3: 24.9 kΩ
    EightPinUSBApple7BrickID, // R1: 24.9 kΩ, R3: 75 kΩ
    EightPinUSBApple8BrickID, // R1: 24.9 kΩ, R3: 43.2 kΩ
    EightPinUSBApple9BrickID, // R1: 24.9 kΩ, R3: 24.9 kΩ
    EightPinUSBApple0BrickID, // R1: floating, R3: floating
    
    // 3rd party brick IDs (from aispec)
    EightPinUSB2_4ABrickID, // 2.4A at 5V -- R1: 43.2 kΩ R2: 49.9 kΩ R3: 43.2 kΩ R4: 49.9 kΩ
    EightPinUSB2_1ABrickID, // 2.1A at 5V -- R1: 43.2 kΩ R2: 49.9 kΩ R3: 75.0 kΩ R4: 49.9 kΩ
    EightPinUSB1_0ABrickID, // 1.0A at 5V -- R1: 75.0 kΩ R2: 49.9 kΩ R3: 43.2 kΩ R4: 49.9 kΩ
    
    EightPinUSBUnknownBrickID, // Resistor ratio doesn't match any known.
    // Unknown also functions as count -- Must be last!
} EightPinUSBBrickID;

@interface ATSR3Utilities : NSObject

/*!
 @method    brickDetectFloatingWithFrame:
 @abstract  Determines if the accessory's D+/D- lines are left floating (no resistors present)
 */
+ (BOOL)brickDetectFloatingWithFrame:(ATFR3Frame *)frame;

/*!
 @method    brickIDFromFrame:
 @abstract  Determines power brick type from brick detection report
 @param     frame
            Frame where command is ATFR3CommandReportBrickDetection
 */
+ (EightPinUSBBrickID)brickIDFromFrame:(ATFR3Frame *)frame;

/*!
 @method    brickDetectVoltagesFromFrame:vbus:
 @abstract  Extracts brick voltages from frame
 @param     frame
            Frame to extract from
 @param     flags 
            Pointer to UInt16 where flags are assigned
 @param     vbus
            Pointer to double where vbus voltage is assigned
 @param     dPlusShuntToGround
            Pointer to double where D+ shunt to ground voltage is assigned
 @param     dPlusShuntToVcc
            Pointer to double where D+ shunt to vcc voltage is assigned
 @param     adcMeasurement
            Pointer to double where ADC measurement when D+ and D- are disconnected
 @param     dMinusShuntToGround
            Pointer to double where D- shunt to ground voltage is assigned
 @param     dMinusShuntToVcc
            Pointer to double where D- shunt to vcc voltage is assigned
 */
+ (void)brickDetectVoltagesFromFrame:(ATFR3Frame *)frame
                               flags:(UInt16 *)flags
                                vbus:(double *)vbus
                  dPlusShuntToGround:(double *)dPlusShuntToGround
                     dPlusShuntToVcc:(double *)dPlusShuntToVcc
                      adcMeasurement:(double *)adcMeasurement
                 dMinusShuntToGround:(double *)dMinusShuntToGround
                    dMinusShuntToVcc:(double *)dMinusShuntToVcc;

/*!
 Get brick ID description for input ID.
 
 @param Brick ID
 
 @return Brick ID description
 */
+ (NSString *)brickIDDescriptionForID:(EightPinUSBBrickID)brickID;

/*!
 Get brick ID description for input frame.
 
 @param Brick ID
 
 @return Brick ID description
 */
+ (NSString *)brickIDDescriptionFromFrame:(ATFR3Frame *)frame;

/*!
 Extracts smart brick voltages from frame
 
 @param frame ATF Frame to extract from
 @param vbus Pointer to the double where vbus voltage is assigned
 @param flags Pointer to UInt16 where flags are assigned
 @param dMinusShuntToGround Pointer to the double where D- shunt to ground voltage is assigned
 @param dPlus Pointer to the double where D+ voltage is assigned
 @param dMinus Pointer to the double where D- voltage is assigned
 */
+ (void)smartBrickDetectVoltagesFromFrame:(ATFR3Frame *)frame
                                    flags:(UInt16 *)flags
                                     vbus:(double *)vbus
                      dMinusShuntToGround:(double *)dMinusShuntToGround
                                    dPlus:(double *)dPlus
                                   dMinus:(double *)dMinus;

/*!
 @method    baudRateFromFrame:sourceIsAccessory:baudFF:baud55:
 @abstract  Returns the baud rate detected by 0x55 or 0x5A. Assigns the source to the passed BOOL
 @param     frame
            Frame to extract from
 @param     sourceIsAccesory
            Whether or not the source of this data is the accessory
 @param     baudFF
            FF baud rate
 @param     baud55
            55 or 5A baud rate
 */
+ (void)baudRateFromFrame:(ATFR3Frame *)frame sourceIsAccessory:(BOOL *)sourceIsAccessory baudFF:(UInt32 *)baudFF baud55:(UInt32 *)baud55;

/**
 Error string for the input Lightning box board status
 
 @param status Lightning box board status
 
 @return Board status error string
 */
+ (NSString *)errorStringForATSLightningBoardStatus:(ATFR3LightningBoardStatus)status;

/**
 Error string for the input Smart Connector box board status
 
 @param status Smart Connector box board status
 
 @return Board status error string
 */
+ (NSString *)errorStringForATSSmartConnectorBoardStatus:(ATFR3SmartConnectorBoardStatus)status;

@end
