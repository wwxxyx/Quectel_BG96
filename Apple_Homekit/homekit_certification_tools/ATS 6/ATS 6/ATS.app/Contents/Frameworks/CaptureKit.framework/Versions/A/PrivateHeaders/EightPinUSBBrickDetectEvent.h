//
//  EightPinUSBBrickDetectEvent.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 8/6/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <CaptureKit/EightPinHardwareEvent.h>
#import <ATSR3BoxSupport/ATSR3BoxSupport.h>

@interface EightPinUSBBrickDetectEvent : EightPinHardwareEvent

/*!
 @property  floating
 @abstract  Whether or not the D+/D- lines were terminated and left floating
 */
@property (nonatomic, assign) BOOL floating;

/*!
 @property  brickID
 @abstract  The detected brick ID
 */
@property (nonatomic, assign) EightPinUSBBrickID brickID;

#pragma mark -
#pragma mark Raw Measurements

/*!
 @property vbus
 @abstract vbus voltage
 */
@property (nonatomic, assign) double vbus;

/*!
 @property dPlusShuntToGround
 @abstract D+ shunt to ground voltage
 */
@property (nonatomic, assign) double dPlusShuntToGround;

/*!
 @property dPlusShuntToVcc
 @abstract D+ shunt to vcc voltage
 */
@property (nonatomic, assign) double dPlusShuntToVcc;

/*!
 @property dMinusShuntToGround
 @abstract D- shunt to ground voltage
 */
@property (nonatomic, assign) double dMinusShuntToGround;

/*!
 @property dMinusShuntToVcc
 @abstract D- shunt to vcc voltage
 */
@property (nonatomic, assign) double dMinusShuntToVcc;

@end
