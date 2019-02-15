//
//  BSUSBDeviceDescriptor.h
//  BusSniffer
//
//  Created by Joe Basu on 3/26/09.
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <BusSniffer/BSUSBDescriptor.h>


@class BSUSBConfigurationDescriptor;

@protocol BSUSBDeviceDescriptor <BSUSBDescriptor>

@property (nonatomic, readonly, copy) NSArray *classSpecificDeviceDescriptors;
@property (nonatomic, readonly, copy) NSArray *configurationDescriptors;

@property (nonatomic, readonly) UInt8 maxPacketSizeForEndpointZero;
@property (nonatomic, readonly) UInt16 vendorID;
@property (nonatomic, readonly) UInt16 productID;
@property (nonatomic, readonly) UInt8 deviceClass;

@end

@interface BSUSBDeviceDescriptor : BSUSBDescriptor <BSUSBDeviceDescriptor> {
	NSArray *_classSpecificDeviceDescriptors;
	NSArray *_configurationDescriptors;
}

- (void)addClassSpecificDeviceDescriptor:(BSUSBDescriptor *)classSpecificDeviceDescriptor;
- (void)addConfigurationDescriptor:(BSUSBConfigurationDescriptor *)configurationDescriptor;
- (BSUSBConfigurationDescriptor *)configurationDescriptorForValue:(UInt8)value;

@end
