//
//  NSControlCategories.h
//  BBSniff
//
//  Created by Marin Balde on 1/22/10.
//  Copyright 2010 Apple. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface NSControl (Categories)

// will safely set the string value to @"" if aString == nil
- (void)setStringValueSafe:(NSString *)aString;

@end
