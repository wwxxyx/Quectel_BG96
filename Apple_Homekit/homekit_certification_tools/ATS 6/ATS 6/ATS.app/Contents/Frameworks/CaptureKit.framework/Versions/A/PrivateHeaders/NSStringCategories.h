//
//  NSStringCategories.h
//  BBSniff
//
//  Created by Marin Balde on 8/2/10.
//  Copyright 2010 Apple. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface NSString (Categories)

- (NSString *)stringByEscapingAllCharactersWithBackslash;
- (NSString *)md5Checksum;
- (NSString *)stringForCSVElement;

@end

@interface NSMutableString (Categories)

- (void)escapeAllCharactersWithBackslash;

@end
