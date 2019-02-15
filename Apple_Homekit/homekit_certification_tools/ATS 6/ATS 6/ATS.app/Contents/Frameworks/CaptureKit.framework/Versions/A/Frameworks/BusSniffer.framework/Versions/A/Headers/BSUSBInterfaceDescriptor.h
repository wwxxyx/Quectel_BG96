//
//  BSUSBInterfaceDescriptor.h
//  BusSniffer
//
//  Created by Joe Basu on 3/26/09.
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <BusSniffer/BSUSBDescriptor.h>


@protocol BSUSBInterfaceDescriptor <BSUSBDescriptor>

@property (nonatomic, readonly, copy) NSArray *classSpecificInterfaceDescriptors;
@property (nonatomic, readonly, copy) NSArray *endpointDescriptors;

@property (nonatomic, readonly) UInt8 interfaceNumber;
@property (nonatomic, readonly) UInt8 alternateSetting;
@property (nonatomic, readonly) UInt8 endpointCount;
@property (nonatomic, readonly) BSUSBInterfaceClass interfaceClass;
@property (nonatomic, readonly) BSUSBInterfaceSubClass interfaceSubclass;
@property (nonatomic, readonly) UInt8 interfaceProtocol;
@property (nonatomic, readonly) UInt8 stringDescriptorIndex;

@end

@class BSUSBEndpointDescriptor;

@interface BSUSBInterfaceDescriptor : BSUSBDescriptor <BSUSBInterfaceDescriptor> {
	NSArray *_classSpecificInterfaceDescriptors;
	NSArray *_endpointDescriptors;
}

- (void)addClassSpecificInterfaceDescriptor:(BSUSBDescriptor *)classInterfaceDescriptors;
- (void)addEndpointDescriptor:(BSUSBEndpointDescriptor *)endpointDescriptors;

@end
