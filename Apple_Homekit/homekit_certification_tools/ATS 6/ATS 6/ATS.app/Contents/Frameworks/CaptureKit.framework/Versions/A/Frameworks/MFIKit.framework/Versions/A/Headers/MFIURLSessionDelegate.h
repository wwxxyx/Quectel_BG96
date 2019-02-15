//
//  MFIURLSessionDelegate.h
//  MFIKit
//
//  Created by Carlos Frederico Azevedo on 6/18/15.
//  Copyright (c) 2015 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MFIURLTask.h"
@class MFIURLSession;

@interface MFIURLSessionDelegate : NSObject <NSURLSessionDataDelegate, NSURLSessionTaskDelegate, NSURLSessionDelegate>

/*!
    The MFIURLSession for which this delegate contains the NSURLConnection logic.
 */
@property (weak) MFIURLSession *mfiURLSession;

/*!
 The active NSURLSession in use by tasks for this delegate.
 */
@property (strong, readonly) NSURLSession *URLSession;

+ (id)sessionDelegateWithMFIURLSession:(MFIURLSession *)session;

/*!
    Begins a MFIURLTask for the desired request using this delegate's NSURLSession and the passed parameters.
    @param request the NSMutableURLRequest used to generate the task.
    @param completionHandler the created task's completionHandler
    @param progressHandler a progressHandler for the created task.
    @return The identifier for the created task.
 */
- (NSNumber *)taskForRequest:(NSMutableURLRequest *)request completionHandler:(MFIURLTaskCompletionHandler)completionHandler progressHandler:(MFIURLTaskProgressHandler)progressHandler;

/*!
    Cancels the activeTask with an identifier equal to the passed parameter.
    @param identifier An identifier to find and cancel a task.
 */
- (void)cancelTaskWithIdentifier:(NSNumber *)identifier;
@end
