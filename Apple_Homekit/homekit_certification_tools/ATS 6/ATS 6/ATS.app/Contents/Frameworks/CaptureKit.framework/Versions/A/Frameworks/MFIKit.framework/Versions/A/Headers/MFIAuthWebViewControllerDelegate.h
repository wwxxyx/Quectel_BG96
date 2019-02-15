//
//  MFIAuthWebViewControllerDelegate.h
//  MFIKit
//
//  Created by Carlos Frederico Azevedo on 7/23/15.
//  Copyright (c) 2015 Apple Inc. All rights reserved.
//

#ifndef MFIKit_MFIAuthWebViewControllerDelegate_h
#define MFIKit_MFIAuthWebViewControllerDelegate_h

@class MFIAuthWebViewController;
/**
 This protocol declares the methods whose execution may involve the dismissal of the authentication window.
 Classes that utilize MFIAuthWebViewController should also implement these methods and assign themselves as the value of the delegate property.
 */
@protocol MFIAuthWebViewControllerDelegate <NSObject>

/*!
 Call back for succesful authentication.
 @param sender The WebView used for authenticatoin
 */
- (void)authWebViewControllerDidAuthenticate:(MFIAuthWebViewController *)sender;

/*!
 Call back for failed authentication.
 @param sender The WebView used for authenticatoin
 @param error  The MFISessionErrorDomain error which caused the user to fail authentication.
 */
- (void)authWebViewController:(MFIAuthWebViewController *)sender didFailWithError:(NSError *)error;

@end

#endif
