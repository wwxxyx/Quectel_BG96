//
//  MFIViewControllerFactory.h
//  MFIKit
//
//  Created by Natália Cabral on 2/19/15.
//  Copyright (c) 2015 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
#import "MFIAuthWebiOSViewController.h"

#elif TARGET_OS_MAC
#import "MFIAuthWebViewController.h"
#endif

@class MFIURLSession;

@interface MFIAuthWebViewControllerFactory : NSObject

#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR

+ (MFIAuthWebiOSViewController *)viewControllerWithData:(NSData *)data expectedURL:(NSURL *)expectedURL baseURL:(NSURL *)baseURL session:(MFIURLSession *)session completionHandler:(void (^)(NSData *data, NSURLResponse *response, NSError *error))completionHandler;

#elif TARGET_OS_MAC
+ (MFIAuthWebViewController *)viewControllerWithData:(NSData *)data expectedURL:(NSURL *)expectedURL baseURL:(NSURL *)baseURL session:(MFIURLSession *)session completionHandler:(void (^)(NSData *data, NSURLResponse *response, NSError *error))completionHandler;
#endif

@end
