//
//  USBConfigurationChangeEvent.h
//  ATSMacApp
//
//  Created by Joe Basu on 8/10/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <CaptureKit/USBEvent.h>
#import <BusSniffer/BusSniffer.h>

@interface USBConfigurationChangeEvent : USBEvent

@property (nonatomic, retain) BSUSBConfigurationDescriptor *configurationDescriptor;

@end
