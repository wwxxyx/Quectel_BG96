//
//  SKViewController.h
//  SelfCertKit
//
//  Created by Vinod Madigeri on 11/18/15.
//  Copyright © 2015 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "SKSessionEnvironment.h"

#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
#import <UIKit/UIKit.h>
#elif TARGET_OS_MAC
#import <AppKit/AppKit.h>
#endif

@class SKSession;

@interface SKAuthenticationViewController
#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
: UIViewController
#elif TARGET_OS_MAC
: NSViewController
#endif

/**
 Reference to session that created the authentication controller.
 */
@property (weak, nonatomic) SKSession *session;

/**
 Last known authentication error.
 */
@property (nonatomic, readonly) NSError *authenticationError;

@end
