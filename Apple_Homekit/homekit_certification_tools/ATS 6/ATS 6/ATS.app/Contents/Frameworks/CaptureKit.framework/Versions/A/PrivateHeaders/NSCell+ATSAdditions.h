//
//  NSCell+ATSAdditions.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 7/23/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface NSCell (ATSAdditions)

/*!
 @method    ceilingCellSizeForBounds:
 @abstract  Size for bounds, passing the dimensions first through the ceiling function
 */
- (NSSize)ceilingCellSizeForBounds:(NSRect)bounds;

@end
