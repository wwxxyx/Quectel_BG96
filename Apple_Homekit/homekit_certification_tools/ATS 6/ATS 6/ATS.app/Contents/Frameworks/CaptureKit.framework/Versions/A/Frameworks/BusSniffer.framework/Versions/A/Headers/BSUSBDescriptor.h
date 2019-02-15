//
//  BSUSBDescriptor.h
//  BusSniffer
//
//  Created by Joe Basu on 5/10/09.
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <Foundation/Foundation.h>
#import <BusSniffer/BSUSBTypes.h>


@protocol BSUSBDescriptor <NSObject>

@property (nonatomic, readonly, copy) NSData *descriptorData;
@property (nonatomic, readonly) UInt8 length;
@property (nonatomic, readonly) BSUSBDescriptorType type;
@property (nonatomic, readonly) BSUSBDescriptorSubtype subtype;

@end

@interface BSUSBDescriptor : NSObject <BSUSBDescriptor> {
	NSData *_descriptorData;
}

+ (id)descriptorWithDescriptorData:(NSData *)descriptorData error:(NSError **)error;
+ (NSString *)title;
- (id)initWithDescriptorData:(NSData *)descriptorData error:(NSError **)error;

- (NSArray *)fields;
- (NSString *)title;

@end
