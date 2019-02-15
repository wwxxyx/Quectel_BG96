//
//  BSiAPVPort.h
//  BusSniffer
//
//  Created by Joe Basu on 1/16/10.
//  Copyright © 2010 Apple Inc. All Rights Reserved.
//

#import <Foundation/Foundation.h>
#import <BusSniffer/BSiAPTypes.h>

@protocol BSiAPVPort <NSObject>

@property (nonatomic, readonly) UInt8 vPortID;
@property (nonatomic, readonly) UInt8 vPortTransport;

@end

@interface BSiAPVPort : NSObject <BSiAPVPort> {
	UInt8 _vPortID;
	BSiAPTransport _vPortTransport;
}

+ (id)iAPVPortWithVPortID:(UInt8)vPortID vPortTransport:(BSiAPTransport)vPortTransport;

- (id)initWithVPortID:(UInt8)vPortID vPortTransport:(BSiAPTransport)vPortTransport;

@end
