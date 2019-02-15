//
//  CaptureDocument+USBVbusLoadTest.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 5/7/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureDocument.h>

@interface CaptureDocument (USBVbusLoadTest)

/*!
 @method    canRunLoadTest
 @abstract  Determines if all constraints are met to run the load test
 */
- (BOOL)canRunLoadTest;

/*!
 @method    runLoadTest:
 @abstract  User action to initiate the load test
 */
- (void)runLoadTest:(id)sender;

/*!
 @method        cleanupLoadTest
 @abstract      Cleans up any resources associated with the load test
 @discussion    Safe to call at any time. Must be called on -dealloc
 */
- (void)cleanupLoadTest;

@end
