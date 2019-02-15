//
//  BSUSBEthernetNetworkingFunctionalDescriptor.h
//  BusSniffer
//
//  Created by Zachary Church on 7/28/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import "BSUSBDescriptor.h"

@interface BSUSBEthernetNetworkingFunctionalDescriptor : BSUSBDescriptor

- (UInt8)macAddressIndex;
- (UInt32)bmEthernetStatistics;
- (UInt16)maxSegmentSize;
- (UInt16)numberMCFilters;
- (UInt8)numberPowerFilters;

@end
