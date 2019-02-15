//
//  NSTextViewCategories.h
//  BBSniff
//
//  Created by Marin Balde on 1/7/10.
//  Copyright 2010 Apple. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface NSTextView (Categories)

- (void)removeEnclosingScrollView;

// return YES if successful or NO otherwise
- (BOOL)makeTextLinkForSpecifier:(NSString *)specifier toURL:(NSString *)urlStr withOptionalLinkName:(NSString *)name;

@end
