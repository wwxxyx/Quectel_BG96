//
//  BSUSBNCMFunctionalDescriptor.h
//  BusSniffer
//
//  Created by Zachary Church on 7/29/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import "BSUSBDescriptor.h"

@interface BSUSBNCMFunctionalDescriptor : BSUSBDescriptor

- (UInt16)ncmVersion;
- (UInt8)networkCapabilities;

@end
