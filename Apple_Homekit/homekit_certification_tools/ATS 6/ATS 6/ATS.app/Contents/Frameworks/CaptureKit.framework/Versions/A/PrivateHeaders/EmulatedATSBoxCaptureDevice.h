//
//  EmulatedATSBoxCaptureDevice.h
//  ATS
//
//  Created by Joe Basu on 8/2/10.
//  Copyright © 2010 Apple Inc. All Rights Reserved.
//

#import <CaptureKit/ATSBoxCaptureDevice.h>

enum {
    ReservedPinValueFloating = 0,
    ReservedPinValueDrivenLow,
    ReservedPinValueDrivenHigh
};
typedef UInt8 ReservedPinValue;

@interface EmulatedATSBoxCaptureDevice : ATSBoxCaptureDevice {
    NSData *_timerRolloverATFData;
    UInt16 _currentClockTicks;
}

- (UInt64)passLittleLessThanDurationInMilliseconds:(double)durationInMilliseconds;
- (UInt64)passLittleMoreThanDurationInMilliseconds:(double)durationInMilliseconds;

- (NSArray *)emulateSerialDataATFsFromSource:(ATFSource)source usingData:(NSData *)data;
- (NSArray *)emulateSerialDataWithBaudATFFromSource:(ATFSource)source byte:(char)byte baudRate:(UInt32)baudRate;

- (NSArray *)emulateUARTiAPTrafficFromSource:(ATFSource)source usingData:(NSData *)data;

// emulates an ATF RetHardwareInfo for ACC_DETECT.  Sending YES means the accessory is present (pin 20 is grounded)
- (NSArray *)emulateAccDetect:(BOOL)accDetect;
// emulates an ATF RetHardwareInfo for IPOD_DETECT.  Sending YES means the iPod is present (pin 10 is grounded)
- (NSArray *)emulateiPodDetect:(BOOL)iPodDetect;
// emulates an ATF RetHardwareInfo for ACC_POWER.  Sending YES means the iPod is providing Accessory Power (pin 13)
- (NSArray *)emulateAccPower:(BOOL)accPower;
// emulates an ATF RetHardwareInfo for peak current over the last 1 second.  Provide a floating point number representing the number of milliamps in the range [0.0, 118.0].
- (NSArray *)emulateAccPeakCurrent:(double)peakCurrentMilliAmps;
// emulates an ATF RetHardwareInfo for average current over the last 1 second.  Provide a floating point number representing the number of milliamps in the range [0.0, 118.0].
- (NSArray *)emulateAccAverageCurrent:(double)averageCurrentMilliAmps;
// emulates an ATF RetHardwareInfo for firewire voltage.  Provide a floating point number representing the number of volts in the range [0.0, 29.94].
- (NSArray *)emulateAccFirewireVoltage:(double)volts;
// emulates an ATF RetHardwareInfo for id resistor.  Provide a floating point number representing the number of kΩ.
- (NSArray *)emulateAccIDResistor:(double)kilohms;
// emulates an ATF RetHardwareInfo for USB Vbus voltage.
- (NSArray *)emulateAccUSBVbus:(double)volts;
// emulates an ATF RetHardwareInfo for USB D+ voltage.
- (NSArray *)emulateAccUSBDPlus:(double)volts;
// emulates an ATF RetHardwareInfo for USB D- voltage.
- (NSArray *)emulateAccUSBDMinus:(double)volts;
// emulates an ATF RetHardwareInfo for USB Tx marking-state voltage.  Provide a floating point number representing the number of volts in the range [0.0, 6.60].
- (NSArray *)emulateAccTx:(double)volts;
// emulates an ATF RetHardwareInfo for Pin 14.
- (NSArray *)emulateReservedPin14:(ReservedPinValue)pin14;
// emulates an ATF RetHardwareInfo for Pin 17.
- (NSArray *)emulateReservedPin17:(ReservedPinValue)pin17;
// emulates an ATF RetHardwareInfo for Audio Return
- (NSArray *)emulateAudioReturnState:(UInt8)state;
// emulates an ATF RetHardwareInfo for USB Vbus with 500 mA load
- (NSArray *)emulateAccUSBVbusWith500mALoad:(double)volts;
// emulates an ATF RetHardwareInfo for USB Vbus with 1 A load
- (NSArray *)emulateAccUSBVbusWith1ALoad:(double)volts;

@end
