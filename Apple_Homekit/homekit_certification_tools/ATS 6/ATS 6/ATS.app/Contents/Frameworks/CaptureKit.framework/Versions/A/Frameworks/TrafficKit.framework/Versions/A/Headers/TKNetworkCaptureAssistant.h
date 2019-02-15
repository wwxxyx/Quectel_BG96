//
//  TKNetworkCaptureAssistant.h
//  TrafficKit
//
//  Created by Mark Hamlin on 5/16/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface TKNetworkCaptureAssistant : NSObject

/*!
 Returns the shared assistant
 */
+ (TKNetworkCaptureAssistant *)sharedAssistant;

/*!
 Installs the tool and runs it with test input to see if it has proper privileges
 @return    Whether or not this succeeded
 */
- (BOOL)installAndVerifyTool;

/*!
 Attempts to give read acces to /dev/bpf* to the currently logged in user
 */
- (BOOL)grantBPFAccess;

@end
