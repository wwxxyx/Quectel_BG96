//
//  ATFR3CaptureTransport+Parsing.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 5/1/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/ATFR3CaptureTransport.h>

@class ATFR3Frame;

@interface ATFR3CaptureTransport (Parsing)

/*!
 @method    processUARTByte:atTimestamp:fromSource:
 @abstract  Process a single UART byte
 */
- (void)processUARTByte:(UInt8)byte atTimestamp:(UInt64)timestamp fromSource:(BSiAPSource)source;

/*!
 @method    processATFR3:
 @abstract  Process an ATFR3 frame
 */
- (void)processATFR3:(ATFR3Frame *)atfR3Frame;

@end
