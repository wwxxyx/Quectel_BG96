//
//  MFIURLTask.h
//  MFIKit
//
//  Created by Sergio Sette on 12/23/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

/** The type of block used as a completion handler for all delegated API requests. The first parameter is the connection identifier associated with the delegate. The second parameter is a dictionary containing data from the response body. The third parameter is an error object describing a failure. The response object is only valid if the error object is nil. */
typedef void (^MFIURLTaskCompletionHandler)(NSNumber *connectionIdentifier, NSDictionary *collection, NSInteger statusCode, NSError *error);

/** The type of block used as a progress handler for all delegated API requests that needs to track the upload file progress.*/
typedef void (^MFIURLTaskProgressHandler)(int64_t bytesSent, int64_t totalBytesSent, int64_t totalBytesExpectedToSend);

@interface MFIURLTask : NSObject

/** The NSURLSessionTask instance */
@property (nonatomic, strong) NSURLSessionTask* task;

/** The completoin handler for this task */
@property (nonatomic, strong) MFIURLTaskCompletionHandler completionHandler;

/** The progress handler for this task */
@property (nonatomic, strong) MFIURLTaskProgressHandler progressHandler;

/**
 Initializes and returns a new MFIURLTask.
 
 This is the designated initializer.

 @param task The NSURLSessionTask associated with this task
 @param progressHandler The block to execute when the task needs to track the partial status of an upload.
 @param completionHandler The block to execute when the task is complete.
 @return The new connection delegate.
 */
- (instancetype)initWithTask:(NSURLSessionTask*) task completionHandler:(MFIURLTaskCompletionHandler)completionHandler progressHandler:(MFIURLTaskProgressHandler)progressHandler;

@end
