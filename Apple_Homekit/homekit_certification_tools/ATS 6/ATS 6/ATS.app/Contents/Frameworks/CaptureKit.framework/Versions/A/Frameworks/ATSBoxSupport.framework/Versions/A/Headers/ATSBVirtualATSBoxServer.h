//
//  ATSBVirtualATSBoxServer.h
//  ATSBoxSupport
//
//  Created by Joe Basu on 5/14/10.
//  Copyright © 2010 Apple Inc. All Rights Reserved.
//

#import <Foundation/Foundation.h>

@protocol ATSBVirtualATSBoxServer <NSObject>

- (void)registerClient:(id)sender;
- (void)unregisterClient:(id)sender;

@end

@interface ATSBVirtualATSBoxServer : NSObject {
	NSString *_boxIdentifier;
	NSString *_portName;
	NSConnection *_connection;
	NSMutableArray *_clients;
}

- (id)init;
- (id)initWithIdentifier:(NSString *)identifier;

- (void)shutdown;

- (void)broadcastATFData:(NSData *)atfData;
- (void)registerClient:(id)client;
- (void)unregisterClient:(id)client;


@end
