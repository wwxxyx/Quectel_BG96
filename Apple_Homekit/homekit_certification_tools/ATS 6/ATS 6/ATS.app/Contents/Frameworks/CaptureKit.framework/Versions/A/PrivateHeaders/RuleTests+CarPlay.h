//
//  RuleTests+CarPlay.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 5/22/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/RuleTests.h>

#import <CaptureKit/RuleTests+NCM.h>
#import <CaptureKit/RuleTests+Network.h>
#import <CaptureKit/RuleTests+HTTP.h>

@interface RuleTests (CarPlay)

/*!
 @method    openedControllerRTSPControlPipe
 @abstract  Returns an emulated TCP connection set up to be the controller RTSP control pipe, and emulates opening it
 */
- (RTEmulatedTCPConnection *)openedControllerRTSPControlPipe;

/*!
 @method    openedAccessoryRTSPControlPipe
 @abstract  Returns an emulated TCP connection set up to be the accessory RTSP control pipe, and emulates opening it
 */
- (RTEmulatedTCPConnection *)openedAccessoryRTSPControlPipe;

@end
