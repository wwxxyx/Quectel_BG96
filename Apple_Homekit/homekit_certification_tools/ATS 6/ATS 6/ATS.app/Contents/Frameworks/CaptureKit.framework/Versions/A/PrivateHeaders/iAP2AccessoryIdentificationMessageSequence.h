//
//  iAP2AccessoryIdentificationMessageSequence.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 7/30/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <CaptureKit/iAP2MessageSequence.h>

enum {
    iAP2AccessoryIdentificationNoneState = 0, // Default
    iAP2AccessoryIdentificationInvalidState,  // Invalid state (bad message sequence)
    iAP2AccessoryIdentificationStartState,
    iAP2AccessoryIdentificationInformationState,
    iAP2AccessoryIdentificationAcceptedState,
    iAP2AccessoryIdentificationRejectedState,
    iAP2AccessoryIdentificationTimedOutState,
    iAP2AccessoryIdentificationCancelState,
};

@interface iAP2AccessoryIdentificationMessageSequence : iAP2MessageSequence

@end
