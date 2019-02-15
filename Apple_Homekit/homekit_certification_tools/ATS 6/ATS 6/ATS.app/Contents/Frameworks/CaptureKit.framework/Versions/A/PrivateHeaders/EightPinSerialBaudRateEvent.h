//
//  EightPinSerialBaudRateEvent.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 8/15/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <CaptureKit/EightPinHardwareEvent.h>

@interface EightPinSerialBaudRateEvent : EightPinHardwareEvent

/*!
 @property  baudRate
 @abstract  Detected baud rate
 */
@property (nonatomic, assign) UInt32 baudRate;

/*!
 @property  sourceIsAccessory
 @abstract  Whether or not this is the bad rate of the accessory-sourced bytes
 */
@property (nonatomic, assign) BOOL sourceIsAccessory;

/*!
 @property  is55
 @abstract  If this was detected on the 0x55 or 0x5A
 */
@property (nonatomic, assign) BOOL is55;

/*!
 @property  isFF
 @abstract  If this was detected on the 0xFF
 */
@property (nonatomic, assign) BOOL isFF;

@end
