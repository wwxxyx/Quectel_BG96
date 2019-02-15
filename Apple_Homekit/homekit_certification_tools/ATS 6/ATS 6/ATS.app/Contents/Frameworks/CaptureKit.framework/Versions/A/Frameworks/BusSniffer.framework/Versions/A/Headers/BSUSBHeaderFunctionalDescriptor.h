//
//  BSUSBHeaderFunctionalDescriptor.h
//  BusSniffer
//
//  Created by Zachary Church on 7/28/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import "BSUSBDescriptor.h"

@interface BSUSBHeaderFunctionalDescriptor : BSUSBDescriptor

- (UInt16) bcdCDC;

@end
