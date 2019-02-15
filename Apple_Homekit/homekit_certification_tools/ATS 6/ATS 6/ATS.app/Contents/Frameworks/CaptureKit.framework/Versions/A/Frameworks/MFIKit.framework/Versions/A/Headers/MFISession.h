//
//  MFISession.h
//  MFIKit
//
//  Created by Cody Brimhall on 11/30/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@class MFISession;
@class MFIAuthWebViewController;
@class MFIAuthWebiOSViewController;

/** The MFISessionDelegate protocol defines the methods that MFISession delegate objects must implement. Delegates are notified of important session events, such as when a request is redirected to a login webpage and user interaction is required. */
@protocol MFISessionDelegate <NSObject>

#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
/**
 Asks the delegate to present a view controller in order to allow the user to enter their web service credentials into a login form.
 
 @param viewController The view controller to present to the user.
 */
- (void)presentAuthenticationViewController:(MFIAuthWebiOSViewController *)viewController;

#elif TARGET_OS_MAC

/**
 Asks the delegate to present a view controller in order to allow the user to enter their web service credentials into a login form.
 
 @param viewController The view controller to present to the user.
 */
- (void)presentAuthenticationViewController:(MFIAuthWebViewController *)viewController;

#endif

@optional

/**
 Notifies the delegate that the authentication was concluded.
 
 @param authenticated YES if the authentication suceeded, NO otherwise.
 */
- (void)authenticationSuceeded:(BOOL)authenticated;

@end

/** MFISession is the base class for all MFi authentication session interfaces. */
@interface MFISession : NSObject

/** The MFI Session delegate used to receive events from the MFI Session **/
@property (weak) id<MFISessionDelegate> delegate;

/** The base URL from which absolute request URLs are generated. */
@property (readonly) NSURL *baseURL;

/**
 Initializes and returns a new session object using the given base URL.
 
 This is the designated initializer.
 
 @param baseURL The base URL to use when constructing API requests.
 @return The new session object.
 */
- (instancetype)initWithBaseURL:(NSURL *)baseURL delegate:(id<MFISessionDelegate>)delegate;

/**
    Removes the cookies that store the user session
 */
- (void)logout;

@end
