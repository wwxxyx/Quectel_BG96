//
//  ATFR3CaptureTransport+BoardEvents.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 5/1/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/ATFR3CaptureTransport.h>

@class ATSSerialCaptureDevice;

@interface ATFR3CaptureTransport (BoardEvents)

- (void)atsR3Box:(ATSSerialCaptureDevice *)captureDevice reportedBoardStatusErrorWithBitmask:(UInt8)boardStatus;

/*!
 @method    completeLoadTestWithErrors:
 @abstract  Note that the test completed. Errors may have no objects, in which case the test was successful
 @param     errors
 Array of error strings. May have 0 count, in which case the test was successful
 @param     initialVoltage
 Initial voltage in millivolts
 @param     averageVoltage
 Average voltage in millivolts
 @param     finalVoltage
 Final voltage in millivolts
 */
- (void)completeLoadTestWithErrors:(NSArray *)errors initialVoltage:(UInt16)initialVoltage averageVoltage:(UInt16)averageVoltage finalVoltage:(UInt16)finalVoltage;

@end
