//
//  NSView+APTS.h
//  APTS
//
//  Created by Mark Hamlin on 9/7/12.
//  Copyright (c) 2012 Apple. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface NSView (APTS)

/*!
 @method    addFullSizedSubview:
 @abstract  Sets the subview's frame size to our own, and adds
 */
- (void)addFullSizedSubview:(NSView *)subview;

@end
