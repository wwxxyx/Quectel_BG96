//
//  NSColor+ATSAdditions.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 8/14/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface NSColor (ATSAdditions)

/*!
 @method        htmlColorString
 @abstract      Returns a formatted hex representation of this color, suitable for a css color style
 @discussion    For example, +blackColor returns @"#000000"
 */
- (NSString *)htmlColorString;

@end
