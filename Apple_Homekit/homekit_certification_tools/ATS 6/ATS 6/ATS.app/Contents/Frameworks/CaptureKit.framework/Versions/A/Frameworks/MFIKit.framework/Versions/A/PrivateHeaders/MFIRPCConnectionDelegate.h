//
//  MFIRPCConnectionDelegate.h
//  MFIKit
//
//  Created by Cody Brimhall on 11/19/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <XMLRPC/XMLRPCConnectionDelegate.h>
#import "MFIRPCSession.h"

/** The type of block used as a completion handler for all delegated API requests. The first parameter is the connection identifier associated with the delegate. The second parameter is a dictionary or array containing data from the response body. The third parameter is an error object describing a failure. The response object is only valid if the error object is nil. */
typedef void (^MFIRPCConnectionDelegateHandler)(NSString *connectionIdentifier, id collection, NSError *error);

/** MFIRPCConnectionDelegate is an XML-RPC delegate that implements support for DS Auth Web, the authentication system used when communicating with the MFi Portal. */
@interface MFIRPCConnectionDelegate : NSObject <XMLRPCConnectionDelegate>

/** The MFi session whose delegate is responsible for presenting the web view. */
@property (copy) MFIRPCSession *mfiSession;

/** The block to execute when the XML-RPC request has completed or resulted in an error. */
@property (copy) MFIRPCConnectionDelegateHandler completionHandler;

/** The identifier for the connection that is servicing the API request. */
@property (copy) NSString *connectionIdentifier;

/**
 Initializes and returns a new XML-RPC connection delegate that will execute the given block when its connection completes.
 
 @param completionHandler The block to execute when the connection is complete.
 @return The new connection delegate.
 */
+ (instancetype)connectionDelegateWithMFiSession:(MFIRPCSession *)mfiSession completionHandler:(MFIRPCConnectionDelegateHandler)completionHandler ;

/**
 Initializes and returns a new XML-RPC connection delegate that will execute the given block when its connection completes.
 
 This is the designated initializer.
 
 @param completionHandler The block to execute when the connection is complete.
 @return The new connection delegate.
 */
- (instancetype)initWithMFiSession:(MFIRPCSession *)mfiSession completionHandler:(MFIRPCConnectionDelegateHandler)completionHandler ;

@end
