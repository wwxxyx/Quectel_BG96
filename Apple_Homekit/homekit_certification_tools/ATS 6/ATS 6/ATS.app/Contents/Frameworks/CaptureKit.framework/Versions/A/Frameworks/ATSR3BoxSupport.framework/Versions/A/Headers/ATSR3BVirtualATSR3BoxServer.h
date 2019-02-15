//
//  ATSR3BVirtualATSR3BoxServer.h
//  ATSR3BoxSupport
//
//  Created by Marin Balde on 4/16/12.
//  Copyright © 2012 Apple Inc. All Rights Reserved.
//

#import <Foundation/Foundation.h>

@protocol ATSR3BVirtualATSR3BoxServer <NSObject>

- (void)registerClient:(id)sender;
- (void)unregisterClient:(id)sender;

@end

@interface ATSR3BVirtualATSR3BoxServer : NSObject {
    NSString *_boxIdentifier;
    NSString *_portName;
    NSConnection *_connection;
    NSMutableArray *_clients;
}

- (id)init;
- (id)initWithIdentifier:(NSString *)identifier;

- (void)shutdown;

- (void)broadcastATFR3Data:(NSData *)atfR3Data;
- (void)registerClient:(id)client;
- (void)unregisterClient:(id)client;


@end
