//
//  ATFRetHardwareInfoEvent.h
//  ATSMacApp
//
//  Created by Bob Burrough on 9/1/10.
//  Copyright 2010 Apple Inc. All rights reserved.
//

#import <CaptureKit/ATFEvent.h>

extern const double infiniteKilohms;

@class NSColor;

@interface ATFRetHardwareInfoEvent : ATFEvent

@property (nonatomic, readonly) ATFHardwareInfo infoType;

/*
    isAccDetectEvent
    
    returns YES if this event object represents an accessory detect frame
*/
- (BOOL)isAccDetectEvent;
- (BOOL)isiPodDetectEvent;
- (BOOL)isAccPowerEvent;
- (BOOL)isAccPowerAtAccEvent;
- (BOOL)isIDResistorEvent;
- (BOOL)isFirmwareVersionEvent;
- (BOOL)isHardwareVersionEvent;
- (BOOL)isUSBVbusVoltageEvent;
- (BOOL)isUSBDPlusVoltageEvent;
- (BOOL)isUSBDMinusVoltageEvent;
- (BOOL)isAccAverageCurrentEvent;
- (BOOL)isAccPeakCurrentEvent;
- (BOOL)isAccDetectVoltageEvent;
- (BOOL)isFirewireVoltageEvent;
- (BOOL)isReservedPin14StateEvent;
- (BOOL)isReservedPin17StateEvent;
- (BOOL)isAudioReturnStateEvent;
- (BOOL)isAccTxVoltageEvent;
- (BOOL)isAccUSBVbusVoltageWith500mALoadEvent;
- (BOOL)isAccUSBVbusVoltageWith1ALoadEvent;

- (NSNumber *)numberValue;
- (NSString *)stringValue;

/*
    isIDResistorApproximatelyEqualTo: withinPercentage:
    
    This is similar isIDResistorInSpecEqualTo, but evaluates whether this
    event's resistor value is within the specified percentage.
    
    Range: 0 <= percent <= 1
*/
- (BOOL)isIDResistorEqualTo:(double)targetKilohms withinPercentage:(double)percent;

/*
    isIDResistorInSpecEqualTo
    
    This is used to evaluate whether this hardware event's ID resistor
    reading is within 1% of the target kilohms (spec tolerance is 1%).
    This can be used to categorize a real-world resistor reading into its
    corresponding identification bucket.  i.e. If you want to know whether
    an accessory is a USB accessory, when you receive the ID resistor event,
    call isIDResistorInSpecEqualTo:28.0.
    
    If you would like to assess whether the ID resistor is currently
    "high impedance" (i.e. the ID resistor is missing) call 
    iDResistorInSpecEqualTo:infiniteKilohms.
    
    Always call isIDResistorEvent before calling isIDResistorInSpecEqualTo.
*/
- (BOOL)isIDResistorInSpecEqualTo:(double)targetKilohms;

/*
    isIDResistorApproximatelyEqualTo
    
    This is similar isIDResistorInSpecEqualTo, but evaluates whether this
    event's resistor value is within 10%.
*/
- (BOOL)isIDResistorApproximatelyEqualTo:(double)targetKilohms;

/*
    specificationColor
    
    If the iAP specification defines valid values for this hardware event,
    return an NSColor representing green for valid values, red for invalid
    values, and black for everything else.
*/
- (NSColor *)specificationColor;

@end
