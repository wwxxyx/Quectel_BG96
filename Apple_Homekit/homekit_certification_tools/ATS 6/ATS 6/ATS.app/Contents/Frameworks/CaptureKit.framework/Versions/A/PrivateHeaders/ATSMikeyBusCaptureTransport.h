//
//  ATSMikeyBusCaptureTransport.h
//  ATSMacApp
//
//  Created by Wesley McCloy on 12/7/15.
//  Copyright © 2015 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureTransport.h>

@class ATSMikeyBusCaptureProtocol;
@class PhysicalATSLightningAudioBoxCaptureDevice;

/**
 ATSMikeyBusCaptureTransport manages the state of the MikeyBus traffic analyzer and sniffs traffic as it becomes available.
*/
@interface ATSMikeyBusCaptureTransport : CaptureTransport

/**
 This is where the parser is essentially held.
 */
@property (nonatomic, retain) ATSMikeyBusCaptureProtocol *mikeyBusProtocol;

/**
 Reference to the ATS Lightning Audio Box capture device instance this capture transport is configured to sniff data from.
 */
@property (nonatomic, readwrite, retain) PhysicalATSLightningAudioBoxCaptureDevice *captureDevice;

@end
