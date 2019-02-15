//
//  iAP2LinkSynchronizationEvent.h
//  ATSMacApp
//
//  Created by Joe Basu on 8/20/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <CaptureKit/iAP2Event.h>

@class iAP2SessionIDStore;

@interface iAP2LinkSynchronizationEvent : iAP2Event

@property (nonatomic, retain) iAP2SessionIDStore *sessionIDStore;

@end
