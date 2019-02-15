//
//  iAP2LinkLayerInitialization.h
//  ATSMacApp
//
//  Created by Natália Cabral on 06/08/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <CaptureKit/iAP2LinkLayerSequence.h>

/**
 States for Link Layer initialization.
 */
typedef NS_ENUM(NSUInteger, iAP2LinkLayerInitializationState) {
    iAP2LinkLayerInitializationNotStarted = 0,
    iAP2LinkLayerInitializationStarted,
    iAP2LinkLayerInitializationBlocked,
    iAP2LinkLayerInitializationFinished,
};

@interface iAP2LinkLayerInitialization : iAP2LinkLayerSequence

@end
