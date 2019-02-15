//
//  MFIURLSession.h
//  MFIKit
//
//  Created by Cody Brimhall on 11/19/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import "MFISession.h"
#import "ATSResponse.h"
#import "MFIURLTask.h"
#import "MFIURLSessionDelegate.h"

extern NSString *const ATSErrorResponseKey;
extern NSString *const MFIURLLoginFailed;

// MFISession.h

/** The domain for errors that MFISession generates while performing operations. */
extern NSString * const MFISessionErrorDomain;

/** An enumeration of error conditions encountered while using MFISession. */
typedef NS_ENUM(NSInteger, MFISessionError) {
    /** No error occurred. */
//    MFISessionErrorNone = 0,
    
    /** Authentication failed (invalid or unauthorized username/password combination) */
    MFISessionErrorAuthenticationFailed = 1,
    
    /** The server sent an unexpected response. */
    MFISessionErrorUnexpectedResponse = 2,
    
    /** The request failed due to a client-side problem. */
    MFISessionErrorRequestFailed = 3,
};

/** MFIURLSession implements the client-side interface for the RESTful ATS server API. */
@interface MFIURLSession : MFISession

/**
 Get an error object from ATS server failures.

 @param atsResponse A response object received from ATS server.
 @param statusCode  A status code received from ATS server.
 @param inError     An error object passed through a completionHandler from remote calls to ATS server.
 @return            An error object filled in with appropriate status.
 */
+ (NSError *)getErrorFromAtsResponse:(ATSResponse *)atsResponse statusCode:(NSInteger)statusCode inError:(NSError *)inError;

/*!
 Returns an array of all URLs that belong to the ATS Server AuthenticateController.
 */
+ (NSArray *)arrayOfAuthenticateControllerURLs;

/**
 Initiates a request for the latest client version information.
 
 @param appLocation A string identifying the location within the application from which the request was generated (e.g., menu, launch).
 @param versionIndex The version index of the currently running instance of ATS.
 @param projectIdentifier The project identifier, which distinguishes different specialized versions of ATS (e.g., main, CarPlay, etc.).
 @param completionHandler A block that yields the latest available version index or an error object. The version index value is only valid if the error object is nil.
 @return A unique identifier for the connection created to handle the API request. The identifier can be used to cancel the request.
 */
- (NSNumber *)getLatestClientVersionUsingAppLocation:(NSString *)appLocation versionIndex:(NSInteger)versionIndex projectIdentifier:(NSString *)projectIdentifier completionHandler:(void (^)(NSString *url, NSString *versionIndex, NSString *packageMD5, NSError *error))completionHandler;

/**
 Initiates a request to authenticate the current user.
 
 @param completionHandler A block that yields a BOOL indicating whether or not the user succesfully authenticated, or an error object. the authentication flag is only valid if the error object is nil.
 @return A unique identifier for the connection created to handle the API request. The identifier can be used to cancel the request.
 */
- (NSNumber *)authenticateCompletionHandler:(void (^)(BOOL authenticated, NSError *error))completionHandler;

/**
 Initiates a request for a list of product plans belonging to the current user.
 
 Only product plans in the testing or self-cert stages will be returned.
 `
 @param completionHandler A block that yields an array of product plan descriptions or an error object. The product plan list is invalid unless the error object is nil.
 @return A unique identifier for the connection created to handle the API request. The identifier can be used to cancel the request.
 */
- (NSNumber *)getAccessoryProductPlansForUsernameCompletionHandler:(void (^)(NSArray *productPlans, NSError *error))completionHandler;

/**
 Initiates a request to register a new submission with the given product plan and file ID, and the current ATS version string.
 
 The file ID is obtained by first uploading the archived certification results.
 
 @param productID The product plan ID.
 @param fileID The file ID obtained by uploading archived certification results.
 @param clientVersion The current ATS version string.
 @param completionHandler A block that yields a submission receipt code string or an error object. The recepit code string is only valid if the error object is nil.
 @return A unique identifier for the connection created to handle the API request. The identifier can be used to cancel the request.
 */
- (NSNumber *)submitCertification:(NSString *)productID fileID:(NSString *)fileID clientVersion:(NSString *)clientVersion completionHandler:(void (^)(NSString *receiptNumber, NSError *error))completionHandler;

/**
 Initiates a request to upload a new cert report file
 
 @param fileURL The file URL
 @param completionHandler A block that yields a submission receipt code string or an error object. The recepit code string is only valid if the error object is nil.
 @return A unique identifier for the connection created to handle the API request. The identifier can be used to cancel the request.
 */
- (NSNumber *)uploadCertReportFileWithURL:(NSURL *)fileURL completionHandler:(void (^)(NSString *fileID, NSError *error))completionHandler progressHandler:(void (^)(int64_t bytesSent, int64_t totalBytesSent, int64_t totalBytesExpectedToSend))progressHandler;

- (NSNumber *)getProductPlanDetailsForPPID:(NSString *)productID completionHandler:(void (^)(NSDictionary *details, NSError *error))completionhandler;

/**
 Cancel a task identified by its task id.
 
 @param taskID ID that identifies the task to be cancelled
 */
- (void)cancelTaskWithID:(NSNumber *)taskID;

/**
 Performs the given URL request and passes the HTTP response status code to the completion handler.
 
 @param request The URL request to perform.
 @param handler The completion handler to invoke once the URL has been loaded.
 */
- (void)evaluateHTTPStatusCodeForRequest:(NSURLRequest *)request completionHandler:(void (^)(NSInteger))handler;

@end
