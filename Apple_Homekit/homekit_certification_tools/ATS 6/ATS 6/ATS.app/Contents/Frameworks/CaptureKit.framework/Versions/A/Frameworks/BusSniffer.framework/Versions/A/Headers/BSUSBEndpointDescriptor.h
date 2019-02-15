//
//  BSUSBEndpointDescriptor.h
//  BusSniffer
//
//  Created by Joe Basu on 2/27/09.
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <BusSniffer/BSUSBDescriptor.h>


@protocol BSUSBEndpointDescriptor <BSUSBDescriptor>

@property (nonatomic, readonly, copy) NSArray *classSpecificEndpointDescriptors;
@property (nonatomic, readonly) UInt8 endpointAddress;
@property (nonatomic, readonly) UInt8 endpointNumber;
@property (nonatomic, readonly) BSUSBEndpointType endpointType;
@property (nonatomic, readonly) BSUSBEndpointDirection endpointDirection; // not valid for control endpoints
@property (nonatomic, readonly) UInt16 maxPacketSize;

@end

@interface BSUSBEndpointDescriptor : BSUSBDescriptor <BSUSBEndpointDescriptor> {
	NSArray *_classSpecificEndpointDescriptors;
}

- (void)addClassSpecificEndpointDescriptor:(BSUSBDescriptor *)classSpecificEndpointDescriptors;

@end
