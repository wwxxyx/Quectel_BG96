//
//  BSUSBUnionInterfaceFunctionalDescriptor.h
//  BusSniffer
//
//  Created by Zachary Church on 7/15/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import "BSUSBDescriptor.h"

@interface BSUSBUnionFunctionalDescriptor : BSUSBDescriptor

@property (nonatomic, readonly) UInt8 controlInterface;
@property (nonatomic, readonly) NSArray* subordindateInterfaces;

@end
