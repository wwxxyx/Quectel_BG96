//
//  IDBusTrafficViewController.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 7/19/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/TrafficViewController.h>

@class ATSCaptureProtocol;

@interface IDBusTrafficViewController : TrafficViewController

/**
 Initialize a Traffic view controller for IDBus.
 
 @param idbusCaptureProtocol Capture protocol associated with the traffic view.
 */
- (instancetype)initWithCaptureProtocol:(ATSCaptureProtocol *)idbusCaptureProtocol;

@end
