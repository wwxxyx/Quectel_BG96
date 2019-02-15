//
//  SKSession.h
//  SelfCertKit
//
//  Created by Zachary Church on 9/10/15.
//  Copyright © 2015 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "SKSessionEnvironment.h"

@class SKAuthenticationViewController;
@class SKProductPlan;
@class SKSubmission;
@class SKSession;
@class SKSessionConfiguration;

extern NSString * const SKErrorDomain;


@protocol SKSessionDelegate <NSObject>
#pragma mark Required
@required
/**
 Tells the delegate to present a view controller that contains a login view.
 
 @param session SKSession object.
 @param viewController Authentication view controller that presents login view.
 */
- (void)session:(SKSession *)session presentAuthenticationViewController:(SKAuthenticationViewController *)viewController;

/**
 Tells the delegate to dismiss a view controller previously presented in session:(SKSession *)presentAuthenticationViewController:(SKAuthenticationViewController *).
 This is called when either authentication is complete with success or failure.
 
 @param session SKSession object.
 @param viewController Authentication view controller to dismiss.
 */
- (void)session:(SKSession *)session hideAuthenticationViewController:(SKAuthenticationViewController *)viewController;

#pragma mark Optional
@optional
#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
/**
 Tells the delegate whether the user agreed to use Touch ID for subsequent authentications. 
 Use touchIDEnabled to implement a in-app settings preference switch.
 This delegate method is only required if SKSession is configured with requestUseTouchID = YES.

 @param session SKSession object.
 @param touchIDEnabled Boolean that tells whether the Touch ID is enabled.
 */
- (void)session:(SKSession *)session userAuthenticatedWithTouchIDEnabled:(BOOL)touchIDEnabled;
#elif TARGET_OS_MAC
/**
 Tells the delegate that the user clicked on the help button in the login sheet.
 
 @param session SKSession object.
 */
- (void)sessionShowAuthenticationHelp:(SKSession *)session;
#endif

@end


@interface SKSession : NSObject

/**
 Session delegate reference.
 */
@property (weak, nonatomic) id<SKSessionDelegate> delegate;

/**
 Configuration object used to  initialized a session.
 */
@property (nonatomic, readonly, copy) SKSessionConfiguration *configuration;

#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
/**
 Returns whether Touch ID is enabled and being used for authentication.

 @Note Touch ID flow:
 
 - Any user who is logging in for the first time gets a prompt, once, to know whether or not he wants to use the touch ID.
 - The delegate is notified with the touch ID enrollment status for it keep track of the same and for an opportunity to use that to implement its own settings switch `Use Touch ID`.
 - On subsequent requests, the preference is remembered and the user is not prompted.
 - When a different user tries to log in, the flow restarts where gets a prompt, once, to know whether or not he wants to use the touch ID.
 - If a user turns off the Touch ID request, the flow is reset.
 
 */
@property (nonatomic, readonly) BOOL isTouchIDEnabled;
#endif

/**
 Factory method to get SKSession.
 
 @param configuration Configuration used to setup SKSession. Can not be nil.
 @param delegate Session delegate(Refer SKSessionDelegate for more details). Can not be nil.

 @return An instance of SKSession
 */
+ (instancetype)sessionWithConfiguration:(SKSessionConfiguration *)configuration
                             delegate:(id<SKSessionDelegate>)delegate;

/**
 Method to show progress sheet.
 */
- (void)showProgressView;

/**
 Request SelfCert to authenticate a user. presentAuthenticationViewController is called when presenting the login view.
 hideAuthenticationViewController is called when authentication is complete.
 
 @param completionHandler Completion handler is called after authentication is complete. error is nil in case of successful authentication.
 */
- (void)authenticateWithCompletionHandler:(void (^)(NSError *error))completionHandler;

/**
 Request SelfCert to log out a user.
 
 @param completionHandler Completion handler is called after logging out is complete. error is nil in case of successful logout. Only send new requests after the logout request is complete.
 */

- (void)logoutWithCompletionHandler:(void (^)(NSError *error))completionHandler;

/**
 Finds all product plans approved for a user from the MFi portal.
 
 @param completionHandler Completion handler returns an array of product plans. error is nil in case of successful fetch.
 */
- (void)fetchProductPlansWithCompletionHandler:(void (^)(NSArray *productPlans, NSError *error))completionHandler;

/**
 Finds a detailed product plan associated with a product plan ID (PPID) from the MFi portal.
 
 @param completionHandler Completion handler returns the product plan. error is nil in case of successful fetch.
 */
- (void)fetchProductPlanWithPPID:(NSString *)ppid
            andCompletionHandler:(void (^)(SKProductPlan *productPlan, NSError *error))completionHandler;

/**
 Create a new submission object that can be used to submit MFi self-certification artifacts like traces, result, etc.
 
 @param completionHandler Completion handler returns a submission object. Use attachFile:toSubmission:withProgressHandler:andCompletionHandler to enqueue submissions.
                          error is nil in case of successful creation.
 */
- (void)createSubmissionForPPID:(NSString *)ppid
          withCompletionHandler:(void (^)(SKSubmission *submission, NSError *error))completionHandler;

/**
 Enqueue MFi self-certification artifacts that need to be submitted to SelfCert server. Submission object is obtained by calling createSubmissionForPPID:withCompletionHandler.
 
 @param submission Submission object obtained by calling createSubmissionForPPID:withCompletionHandler.
 @param progressHandler Shows the progress percentage of attach process. Range from 0 to 100.
 @param completionHandler Completion handler is called after attachment of the file is complete. error is nil in case of successful attachment.
 */
- (void)attachFile:(NSFileWrapper *)file
      toSubmission:(SKSubmission *)submission
withProgressHandler:(void (^)(NSUInteger progress))progressHandler
andCompletionHandler:(void (^)(NSError *error))completionHandler;


/**
 Complete a submission of MFi self-certification artifacts like traces, result, etc.
 
 @param completionHandler Completion handler is called after submission of the enqueued files are processed. error is nil in case of successful completion.
 */
- (void)completeSubmission:(SKSubmission *)submission
    withCompletionHandler:(void (^)(NSError *error))completionHandler;

/**
 Method to hide progress sheet.
 */
- (void)hideProgressView;

@end


/**
 Configuration options for an SKSession.  When a session is created, a copy of the configuration object is made 
 - you cannot modify the configuration of a session after it has been created.
 */
@interface SKSessionConfiguration : NSObject <NSCopying>

/**
 Environment that this session is configured. Staging or Production.
 */
@property (nonatomic) SKSessionEnvironment environment;

/**
 Application ID of the application that is using SKSession. This property is required.
 */
@property (nonatomic) NSInteger applicationID;

/**
  Application ID key of the application that is using SKSession. It's the responsibility of the Application to provide a matching Staging / Production key for the input SKSessionEnvironment. This property is required.
 */
@property (nonatomic, copy) NSString *applicationIDKey;

/**
 String used by SelfCert server to identify which service to handle the request for. This property is required.
 */
@property (nonatomic, copy) NSString *stepIdentifier;

/**
 Application name using SelfCert. Can be nil.
 */
@property (nonatomic, copy) NSString *hostApplicationName;

/**
 Application Logo Image path, used to create an image that is displayed in the login window. Can be nil.
 */
@property (nonatomic, copy) NSString *hostApplicationLogoPath;

#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
/**
 Request SKSession to use Touch ID during authentication. Actual state whether Touch ID is enabled can be tracked with isTouchIDEnabled property on SKSession.
 If Touch ID is requested, delegate method session:(SKSession *)userAuthenticatedWithTouchIDEnabled:(BOOL) has to be implemented to get whether user opted in for Touch ID.
 To persist the user's touch ID preference, all subsequent requests should this property to the value received from the delegate method.
 */
@property (nonatomic, getter=isTouchIDRequested) BOOL requestUseTouchID;
#endif

/**
 Factory method to get SKSessionConfiguration.

 @param stepIndentifier  String used by SelfCert server to identify which service to handle the request for. Should not be nil.
 @param applicationID  Application ID of the application that is using SKSession. Should be a valid Application ID.
 @param applicationIDKey   Application ID key of the application that is using SKSession. It's the responsibility of the Application to provide a matching Staging / Production key for the input SKSessionEnvironment. Should not be nil.
 */
+ (instancetype)sessionConfigurationWithStepIdentifier:(NSString *)stepIdentifier applicationID:(NSInteger)applicationID applicationIDKey:(NSString *)applicationIDKey;

@end
