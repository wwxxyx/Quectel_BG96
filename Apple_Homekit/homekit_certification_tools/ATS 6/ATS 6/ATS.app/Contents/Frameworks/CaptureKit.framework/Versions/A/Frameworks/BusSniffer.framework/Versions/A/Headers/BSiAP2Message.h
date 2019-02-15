//
//  BSiAP2Message.h
//  BusSniffer
//
//  Created by Joe Basu on 4/12/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <BusSniffer/BSiAPTypes.h>
#import <BusSniffer/BSiAP2LinkTypes.h>

@protocol BSiAP2Message <NSObject>

@property (nonatomic, readonly) BSiAPSource source;
@property (nonatomic, readonly) BSiAPTransport transport;
@property (nonatomic, readonly) UInt64 timestamp;
@property (nonatomic, readonly) NSData *data;

@end

@interface BSiAP2Message : NSObject <BSiAP2Message> {
    BSiAPSource _source;
	BSiAPTransport _transport;
	UInt64 _timestamp;
    NSData *_data;
}

+ (id)messageWithTimestamp:(UInt64)timestamp
                    source:(BSiAPSource)source
                 transport:(BSiAPTransport)transport
                      data:(NSData *)data;

- (id)initWithTimestamp:(UInt64)timestamp
                 source:(BSiAPSource)source
              transport:(BSiAPTransport)transport
                   data:(NSData *)data;

@end
