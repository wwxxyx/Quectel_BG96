//
//  BSUSBConfigurationDescriptor.h
//  BusSniffer
//
//  Created by Joe Basu on 3/26/09.
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <BusSniffer/BSUSBDescriptor.h>


@protocol BSUSBConfigurationDescriptor <BSUSBDescriptor>

@property (nonatomic, readonly, copy) NSArray *classSpecificConfigurationDescriptors;
@property (nonatomic, readonly, copy) NSArray *interfaceDescriptors;
@property (nonatomic, readonly) UInt8 configurationValue;

@end

@class BSUSBInterfaceDescriptor;

@interface BSUSBConfigurationDescriptor : BSUSBDescriptor <BSUSBConfigurationDescriptor> {
	NSArray *_classSpecificConfigurationDescriptors;
	NSArray *_interfaceDescriptors;
}

+ (BSUSBConfigurationDescriptor *)analyzeConfigurationDescriptorHierarchyRawData:(NSData *)configData error:(NSError **)error;

- (void)addClassSpecificConfigurationDescriptor:(BSUSBDescriptor *)classSpecificConfigurationDescriptor;
- (void)addInterfaceDescriptor:(BSUSBInterfaceDescriptor *)interfaceDescriptor;

@end
