//
//  MFIRPCSession.h
//  MFIKit
//
//  Created by Cody Brimhall on 11/19/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import "MFISession.h"

/** The domain for errors encountered during the lifecycle of the session. */
FOUNDATION_EXPORT NSString *const MFIRPCSessionErrorDomain;

/** The enumeration of errors that can be encountered during the lifecycle of the session. */
typedef NS_ENUM(NSInteger, MFIRPCSessionError) {
    /** The response could not be parsed. */
    MFIRPCSessionErrorFault = 1,

    /** The response was an HTTP error. */
    MFIRPCSessionErrorHTTPStatus = 2,

    /** A URL connection error occurred. */
    MFIRPCSessionErrorURLConnection = 3,

    /** An XML-RPC error occurred. */
    MFIRPCSessionErrorXMLRPC = 4,
    
    /** The operation could not be completed. */
    MFIRPCSessionOperationCouldNotBeCompleted = 111,

};

/** MFIRPCSession implements a client-side interface for ATS Server's legacy XML-RPC API.  */
@interface MFIRPCSession : MFISession

/**
 Initiates a request for the latest client version information.
 
 @param appLocation A string identifying the location within the application from which the request was generated (e.g., menu, launch).
 @param versionIndex The version index of the currently running instance of ATS.
 @param projectIdentifier The project identifier, which distinguishes different specialized versions of ATS (e.g., main, CarPlay, etc.).
 @param completionHandler A block that yields the latest available version index or an error object. The version index value is only valid if the error object is nil.
 @return A unique identifier for the connection created to handle the API request. The identifier can be used to cancel the request.
 */
- (NSString *)getLatestClientVersionUsingAppLocation:(NSString *)appLocation versionIndex:(NSInteger)versionIndex projectIdentifier:(NSString *)projectIdentifier completionHandler:(void (^)(NSInteger latestVersionIndex, NSError *error))completionHandler;

/**
 Initiates a request to authenticate the current user.
 
 @param completionHandler A block that yields a BOOL indicating whether or not the user succesfully authenticated, or an error object. the authentication flag is only valid if the error object is nil.
 @return A unique identifier for the connection created to handle the API request. The identifier can be used to cancel the request.
 */
- (NSString *)authenticateCompletionHandler:(void (^)(BOOL authenticated, NSError *error))completionHandler;

/**
 Initiates a request for a list of product plans belonging to the current user.
 
 Only product plans in the testing or self-cert stages will be returned.
`
 @param completionHandler A block that yields an array of product plan descriptions or an error object. The product plan list is invalid unless the error object is nil.
 @return A unique identifier for the connection created to handle the API request. The identifier can be used to cancel the request.
 */
- (NSString *)getAccessoryProductPlansForUsernameCompletionHandler:(void (^)(NSArray *productPlans, NSError *error))completionHandler;

/**
 Initiates a request to validate the given product plan selections and return a set of test parameters based on them.
 
 @param productPlanSelections A dictionary of product plan selections representing the user's choices in the Certification Assistant.
 @param completionHandler A block that yields a dictionary of test parameters or an error object. The test parameters dictionary is only valid if the error object is nil.
 @return A unique identifier for the connection created to handle the API request. The identifier can be used to cancel the request.
 */
- (NSString *)convertProductPlanSelectionsToTestParameters:(NSDictionary *)productPlanSelections completionHandler:(void (^)(NSDictionary *testParameters, NSError *error))completionHandler;

/**
 Initiates a request to register a new submission with the given product plan and file ID, and the current ATS version string.
 
 The file ID is obtained by first uploading the archived certification results.
 
 @param productID The product plan ID.
 @param fileID The file ID obtained by uploading archived certification results.
 @param clientVersion The current ATS version string.
 @param completionHandler A block that yields a submission receipt code string or an error object. The recepit code string is only valid if the error object is nil.
 @return A unique identifier for the connection created to handle the API request. The identifier can be used to cancel the request.
 */
- (NSString *)submitCertification:(NSString *)productID fileID:(NSString *)fileID clientVersion:(NSString *)clientVersion completionHandler:(void (^)(NSString *receiptCode, NSError *error))completionHandler;

@end
