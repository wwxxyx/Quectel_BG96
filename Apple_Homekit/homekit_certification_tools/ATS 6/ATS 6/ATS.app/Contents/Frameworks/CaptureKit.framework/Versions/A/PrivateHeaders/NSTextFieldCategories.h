//
//  NSTextFieldCategories.h
//  BBSniff
//
//  Created by Marin Balde on 6/18/10.
//  Copyright 2010 Apple. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface NSTextField (Categories)

// setting maxLength = 0 removes the max length limit
- (void)setMaxLength:(NSUInteger)maxLength;

@end
