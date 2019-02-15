//
//  ATFSerialDataBufferEvent.h
//  ATSMacApp
//
//  Created by Joe Basu on 9/20/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <CaptureKit/ATFEvent.h>

@interface ATFSerialDataBufferEvent : ATFEvent

- (NSData *)serialData;

@end
