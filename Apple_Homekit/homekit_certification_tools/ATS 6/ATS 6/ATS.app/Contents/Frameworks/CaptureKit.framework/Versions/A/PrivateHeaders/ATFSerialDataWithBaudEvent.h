//
//  ATFSerialDataWithBaudEvent.h
//  ATSMacApp
//
//  Created by Joe Basu on 9/20/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <CaptureKit/ATFEvent.h>

@interface ATFSerialDataWithBaudEvent : ATFEvent

@property (nonatomic) UInt32 baudRate;

@end
