//
//  USBCompleteStringDescriptorEvent.h
//  ATSMacApp
//
//  Created by Joe Basu on 8/27/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <CaptureKit/USBEvent.h>

@interface USBCompleteStringDescriptorEvent : USBEvent

@property (nonatomic, assign) UInt8 index;
@property (nonatomic, copy) NSString *string;

@end
