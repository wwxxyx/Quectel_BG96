//
//  MFIAuthWebViewController.h
//  MFIKit
//
//  Created by Cody Brimhall on 12/2/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <WebKit/WebKit.h>
#import "MFIAuthWebViewControllerDelegate.h"

@class MFIURLSession;

/** MFIAuthWebViewController manages a web view that presents a login form for authentication ATS web service requests using DS Auth Web. */
@interface MFIAuthWebViewController : NSViewController

- (instancetype)initWithData:(NSData *)data expectedURL:(NSURL *)expectedURL baseURL:(NSURL *)baseURL session:(MFIURLSession *)session completionHandler:(void (^)(NSData *data, NSURLResponse *response, NSError *error))completionHandler;
@property (weak) IBOutlet WebView *authWebView;

@property (weak) IBOutlet NSProgressIndicator *progressIndicator;

/*
 Delegate implementing the methods through which this view controller can expect to be dismissed.
 */
@property (weak) id <MFIAuthWebViewControllerDelegate> delegate;
@end

