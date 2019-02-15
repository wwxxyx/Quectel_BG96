//
//  EightPinDevicePowerStenograph.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 8/27/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <CaptureKit/StenographModule.h>

@class EightPinUSBBrickDetectEvent;
@class EightPinDeviceVoltageEvent;
@class iAP2MessageEvent;
@class iAP1PacketEvent;
@class ATSEightPinUSBSmartBrickDetectEvent;

@interface ATSEightPinStenograph : StenographModule

/*!
 The last USB brick detect event seen
 */
@property (nonatomic, strong, readonly) EightPinUSBBrickDetectEvent *lastUSBBrickDetectEvent;

/*!
 Get all USB brick detect events seen
 */
@property (nonatomic, strong, readonly) NSArray <EightPinUSBBrickDetectEvent *>* allUSBBrickDetectEvent;

/*!
 The last USB smart brick detect event seen
 */
@property (nonatomic, strong) ATSEightPinUSBSmartBrickDetectEvent *lastUSBSmartBrickDetectEvent;

/*!
 The last device voltage report event seen
 */
@property (nonatomic, strong) EightPinDeviceVoltageEvent *lastDeviceVoltageEvent;

/*!
 The last power source update
 */
@property (nonatomic, strong) iAP2MessageEvent *lastPowerSourceUpdateMessage;

/*!
 The last SetAvailableCurrent command seen
 */
@property (nonatomic, strong) iAP1PacketEvent *lastSetAvailableCurrentCommand;

/*!
 Whether or not device voltage is being driven
 */
@property (nonatomic) BOOL isDriving;

/*!
 If voltage has been seen too low
 */
@property (nonatomic) BOOL seenVoltageTooLow;

/*!
 If voltage has was just seen going too low
 */
@property (nonatomic) BOOL justSawVoltageTooLow;

/*!
 If voltage has been seen too high
 */
@property (nonatomic) BOOL seenVoltageTooHigh;

/*!
 If voltage has was just seen going too high
 */
@property (nonatomic) BOOL justSawVoltageTooHigh;

/*!
 If voltage just returned to an acceptable level
 */
@property (nonatomic) BOOL voltageJustChangedToAcceptable;

/*!
 If the USB Vbus load test is currently running
 */
@property (nonatomic) BOOL isRunningLoadTest;

/*!
 Determines and returns the minimum allowable device voltage for the current brick type
 */
- (double)minAllowableVoltage;

/*!
 Determines and returns the maximum allowable device voltage for the current brick type
 */
- (double)maxAllowableVoltage;

@end
