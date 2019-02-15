//
//  HTTPIncompleteMessageEvent.h
//  ATSMacApp
//
//  Created by Cody Brimhall on 3/2/15.
//  Copyright (c) 2015 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureEvent.h>

/** HTTPIncompleteMessageEvent represents a partially-received HTTP request or response. */
@interface HTTPIncompleteMessageEvent : CaptureEvent

/** A buffer containing the partial HTTP message. */
@property (copy) NSData *data;

@end
