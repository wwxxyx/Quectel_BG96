//
//  ATSR3BoxPayloadEmulation.h
//  ATSR3BoxSupport
//
//  Created by Mark Hamlin on 8/3/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "ATSR3BoxTypes.h"

/*!
 @class     ATSR3BoxPayloadEmulation
 @abstract  Helper methods to construct frame payloads
            The returned payload data is suitable for passing into ATS's EmulatedATFR3CaptureDevice's
            emulateATFR3Command:theCommandId payloadData:theReturnedData
 */
@interface ATSR3BoxPayloadEmulation : NSObject

/*!
 @method    accessoryPowerPayloadWithVoltage:current:
 @abstract  Creates an AccessoryPower payload with the given voltage and current (everything after timestamp)
 @param     voltageInMillivolts
            The reported voltage
 @param     currentInMilliamps
            The reported current
 @param     command
            The command which the frame should be created with
 @return    Payload data
 */
+ (NSData *)accessoryPowerPayloadWithVoltage:(UInt16)voltageInMillivolts current:(UInt16)currentInMilliamps command:(ATFR3Command *)command;

/*!
 @method    devicePowerPayloadWithVoltage:current:
 @abstract  Creates an Device Power payload with the given voltage and current (everything after timestamp)
 @param     voltageInMillivolts
            The reported voltage
 @param     currentInMilliamps
            The reported current
 @param     command
            The command which the frame should be created with
 @return    Payload data
 */
+ (NSData *)devicePowerPayloadWithVoltage:(UInt16)voltageInMillivolts current:(UInt16)currentInMilliamps command:(ATFR3Command *)command;

/*!
 @method    accessoryConnectionPayloadWithState:
 @method    Payload for Accessory Connection State
 @param     state
            Whether or not the accessory is connected
 @param     command
            The command which the frame should be created with
 @return    Payload data
 */
+ (NSData *)accessoryConnectionPayloadWithState:(BOOL)state command:(ATFR3Command *)command;

/*!
 @method    deviceConnectionPayloadWithState:
 @method    Payload for Device Connection State
 @param     state
            Whether or not the device is connected
 @param     command
            The command which the frame should be created with
 @return    Payload data
 */
+ (NSData *)deviceConnectionPayloadWithState:(BOOL)state command:(ATFR3Command *)command;

/*!
 @method    brickDetectionPayloadWithFlags:v1:v2:v3:v4:v5:v6
 @abstract  Brick Detection payload
 @param     flags
            Test flags (see T69_firmware_spec_v09.txt)
 @param     v1
            USB Vbus voltage in millivolts
 @param     v2
            USB D- voltage in millivolts with extra 220 kΩ shunt resistor to ground
 @param     v3
            USB D+ voltage in millivolts with extra 220 kΩ shunt resistor to ground
 @param     v4
            ADC measurement in millivolts when D+ and D- are disconnected.
 @param     v5
            USB D- voltage in millivolts with extra 220 kΩ shunt resistor to Vcc
 @param     v6
            USB D+ voltage in millivolts with extra 220 kΩ shunt resistor to Vcc
 @param     command
            The command which the frame should be created with
 @return    Payload data
 */
+ (NSData *)brickDetectionPayloadWithFlags:(UInt16)flags v1:(UInt16)v1 v2:(UInt16)v2 v3:(UInt16)v3 v4:(UInt16)v4 v5:(UInt16)v5 v6:(UInt16)v6 command:(ATFR3Command *)command;

/*!
 @method    resistorBrickDetectionPayloadWithR2:R2:R3:R4
 @abstract  Converts resistor values into v1/v2/etc, and calls -brickDetectionPayloadWithFlags:
 @param     r1
            D+ pull up (kΩ)
 @param     r2
            D+ pull down (kΩ)
 @param     r3
            D- pull up (kΩ)
 @param     r4
            D- pull down (kΩ)
 @param     command
            The command which the frame should be created with 
 */
+ (NSData *)resistorBrickDetectionPayloadWithR1:(double)r1 R2:(double)r2 R3:(double)r3 R4:(double)r4 command:(ATFR3Command *)command;

/*!
 @method    serialTimingStatisticsWithBaudFF:baud55:sourceIsAccessory:command:
 @abstract  Creates either a Serial UART1 Timing Statistics or Serial UART2 Timing Statistics payload
 @param     baudFF
            Baud rate for FF
 @param     baud55
            Baud rate for 55 or 5A
 @param     sourceIsAccessory
            Whether or not the data is from an accessory
 @param     command
            The command which the frame should be created with
 */
+ (NSData *)serialTimingStatisticsWithBaudFF:(UInt32)baudFF baud55:(UInt32)baud55 sourceIsAccessory:(BOOL)sourceIsAccessory command:(ATFR3Command *)command;

/*!
 @method    accessoryOrientationWithState:command:
 @abstract  Emulates accessory orientation report
 @param     orientation
            Orientation state
 @param     command
            The command which the frame should be created with
 */
+ (NSData *)accessoryOrientationWithState:(ATFR3AccessoryOrientation)orientation commmand:(ATFR3Command *)command;

/*!
 @method    idBusRawBreakCommmand:
 @abstract  Emulates an ID Bus break.
 */
+ (NSData *)idBusRawBreakCommmand:(ATFR3Command *)command;

@end
