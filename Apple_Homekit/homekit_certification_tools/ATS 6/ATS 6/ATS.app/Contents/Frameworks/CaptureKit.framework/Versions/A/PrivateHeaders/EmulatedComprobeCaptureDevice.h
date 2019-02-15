//
//  EmulatedComprobeCaptureDevice.h
//  ATSMacApp
//
//  Created by Natália Cabral on 18/06/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CaptureKit/ComprobeCaptureDevice.h>
#import <BusSniffer/BSiAPTypes.h>

@interface EmulatedComprobeCaptureDevice : ComprobeCaptureDevice

/*!
 @method    emulateUARTiAPTrafficFromSource:usingData:
 @abstract  emulates a bluetooth traffic with the given iAp data
 @param    	source
			BSiAPSource indicating the source of the data
 @param     data
			iAPData The iAP data value
 @return    The generated frame data
 */
- (NSArray *)emulateBluetoothiAPTrafficFromSource:(BSiAPSource)source usingData:(NSData *)data;

/*!
 Emulates a bluetooth traffic with the given iAp data ans skips the frame at the specified position
 
 @param    	source
            BSiAPSource indicating the source of the data
 @param     data
            iAPData The iAP data value
 @param     framesLostPosition
            The position of the frame to be skipped
 @return    The generated frame data
 */
- (NSArray *)emulateBluetoothiAPTrafficFromSource:(BSiAPSource)source usingData:(NSData *)data skipFrameAtPosition:(UInt8)framesLostPosition;
/*!
 @method    emulateDevicesConnected:
 @abstract  emulates connection and disconnection between the devices (apple device and accessory)
 @param    	connected
            BOOl to indicate if it will emulate connection or disconnection
 @return    The generated events
 */
- (NSArray *)emulateDevicesConnected:(BOOL)connected;

/*!
 @method    passDurationInMilliseconds:
 @abstract  pass the given duration time 
 @param    	durationInMilliseconds
            The duration to be passed in miliseconds
 @return    The duration in nanoseconds
 */
- (UInt64)passDurationInMilliseconds:(UInt32)durationInMilliseconds;
@end
