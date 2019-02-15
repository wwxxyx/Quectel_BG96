//
//  EmulatedATFR3CaptureDevice.h
//  ATSR3MacApp
//
//  Created by Marin Balde on 4/11/12.
//  Copyright © 2012 Apple Inc. All Rights Reserved.
//

#import <CaptureKit/ATSSerialCaptureDevice.h>
#import <BusSniffer/BSiAPTypes.h>
#import <ATSR3BoxSupport/ATSR3BoxSupport.h>

@interface EmulatedATFR3CaptureDevice : ATSSerialCaptureDevice {
    UInt64 _lastTimerTickTimestamp;
    UInt64 _lastFrameTimestamp;
}

- (instancetype)initWithName:(NSString *)emulatedDeviceName;

- (UInt64)passDurationInMilliseconds:(UInt32)durationInMilliseconds;

- (NSArray *)emulateATFR3Command:(ATFR3Command)command;
- (NSArray *)emulateATFR3Command:(ATFR3Command)command payloadData:(NSData *)data;

- (NSArray *)emulateUARTiAPTrafficFromSource:(BSiAPSource)source usingData:(NSData *)data;

/*!
 @method    emulateAccessoryPowerWithVoltage:current:
 @abstract  Emulates accessory power frame
 @param     voltageInMillivolts
            The voltage in millivolts
 @param     currentInMilliamps
            The current in milliamps
 @return    Events returned by delegate
 */
- (NSArray *)emulateAccessoryPowerWithVoltage:(UInt16)voltageInMillivolts current:(UInt16)currentInMilliamps;

/*!
 @method    emulateDevicePowerWithVoltage:current:
 @abstract  Emulates device power frame
 @param     voltageInMillivolts
            The voltage in millivolts
 @param     currentInMilliamps
            The current in milliamps
 @return    Events returned by delegate
 */
- (NSArray *)emulateDevicePowerWithVoltage:(UInt16)voltageInMillivolts current:(UInt16)currentInMilliamps;

/*!
 @method    emulateAccessoryConnected:
 @abstract  Emulate an accessory connected report, "Accessory Connection State"
 @param     connected
            Whether or not the accessory should be reported as connected
 @return    Events returned by delegate
 */
- (NSArray *)emulateAccessoryConnected:(BOOL)connected;

/*!
 @method    emulateDeviceConnected:
 @abstract  Emulate a device connected report, "Device Connection Info"
 @param     connected
            Whether or not the device should be reported as connected
 @return    Events returned by delegate
 */
- (NSArray *)emulateDeviceConnected:(BOOL)connected;

/*!
 @method    emulateBrickDetectionWithFlags:v1:v2:v3:v4:v5:v6
 @abstract  Brick Detection emulation
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
 @return    Events returned by delegate
 */
- (NSArray *)emulateBrickDetectionWithFlags:(UInt16)flags v1:(UInt16)v1 v2:(UInt16)v2 v3:(UInt16)v3 v4:(UInt16)v4 v5:(UInt16)v5 v6:(UInt16)v6;

/*!
 @method    emulateUSBR1Resistor:R2:R3:R4:
 @abstract  Emulates USB pull up/down resistors brick detect
 @param     r1
            D+ pull up (kΩ)
 @param     r2
            D+ pull down (kΩ)
 @param     r3
            D- pull up (kΩ)
 @param     r4
            D- pull down (kΩ)
 */
- (NSArray *)emulateUSBR1Resistor:(double)r1 R2:(double)r2 R3:(double)r3 R4:(double)r4;

/*!
 @method    emulateUSBDataLinesFloating
 @abstract  Emulates floating USB resistors
 */
- (NSArray *)emulateUSBDataLinesFloating;

/*!
 @method    emulateUSBDataLinesTied
 @abstract  Emulates china brick config with D+ and D- tied together
 */
- (NSArray *)emulateUSBDataLinesTied;

/*!
 @method    emulateAccessoryOrientationWithState:
 @abstract  Emulates accessory orientation report
 @param     orientation
            Orientation state
 @param     command
            The command which the frame should be created with
 */
- (NSArray *)emulateAccessoryOrientationWithState:(ATFR3AccessoryOrientation)orientation;

/*!
 @method    emulateSerialTimingStatisticsWithBaudFF:baud55:sourceIsAccessory:
 @abstract  Emulate either a Serial UART1 Timing Statistics or Serial UART2 Timing Statistics
 @param     baudFF
            Baud rate for FF
 @param     baud55
            Baud rate for 55 or 5A
 @param     sourceIsAccessory
            Whether or not the data is from an accessory
 */
- (NSArray *)emulateSerialTimingStatisticsWithBaudFF:(UInt32)baudFF baud55:(UInt32)baud55 sourceIsAccessory:(BOOL)sourceIsAccessory;

/*!
 @method    emulateIDBusRawBreak
 @abstract  Emulate an ID Bus break.
 */
- (NSArray *)emulateIDBusRawBreak;

/*!
 @method    emulateIDBusRawBytesUsingData:
 @abstract  Emulate an ID Bus raw byte stream.
 @param     data
            NSData object containing the ID Bus bytes to be sent.
 */
- (NSArray *)emulateIDBusRawBytesUsingData:(NSData *)data;

@end
