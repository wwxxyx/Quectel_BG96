//
//  TKAttributedString.h
//  Colors
//
//  Created by Mark Hamlin on 5/25/13.
//  Copyright (c) 2013 Apple. All rights reserved.
//

#import <Cocoa/Cocoa.h>

typedef enum : int {
    TKFormatTokenString,            // Next token is a string to append
    TKFormatTokenColorAttribute,    // Next token is an NSColor to apply
    TKFormatTokenColorPointerAttribute, // Next token is a pointer to an NSColor *, so that colors can be hot-swapped
    TKFormatTokenStrikeAttribute,   // Enable strikethrough
    TKFormatTokenPopLastAttribute,  // Token indicating that the previously applied attribute should be removed
    TKFormatTokenEndOfList,         // There is no next token, this is the end of the list
} TKFormatToken;

#define TK_ATTRIBUTED_STRING(first, ...) [TKAttributedString stringWithComponents:@[first, ##__VA_ARGS__]]

#define TK_STRING(string) (string) 

#define TK_STRING_FMT(string, ...) [NSString stringWithFormat:string, ##__VA_ARGS__]

#define TK_COLOR(color, string) [[NSAttributedString alloc] initWithString:string attributes:@{ NSForegroundColorAttributeName: color }]

#define TK_COLOR_POINTER(color, string) [[NSAttributedString alloc] initWithString:string attributes:@{ TKColorPointerAttributeName: [NSValue valueWithPointer:color] }]

#define TKStrikethroughStyle @1
#define TK_STRIKE(string) [[NSAttributedString alloc] initWithString:string attributes:@{ NSStrikethroughAttributeName: TKStrikethroughStyle }]

#define TKColorPointerAttributeName @"TKColorPointerAttribute"

@interface TKAttributedString : NSObject

/*!
 @property  cacheRevision
 @abstract  An optional cache revision may be assigned by the owner, to track cache versions
 */
@property (nonatomic, assign) unsigned int cacheRevision;

/*!
 @property  isSelected
 @abstract  if YES, cached string is recreated with no colors
 */
@property (nonatomic, assign) BOOL isSelected;

/*!
 @method    stringWithComponents:
 @abstract  Creates a TKAttributedString with the given list of NSAttributedString components.
 */
+ (TKAttributedString *)stringWithComponents:(NSArray *)components;

/*!
 @method    attributedStringWithComponents:
 @abstract  Creates an NSAttributedString with the given list of NSAttributedString components.
 */
+ (NSAttributedString *)attributedStringWithComponents:(NSArray *)components;

/*!
 @method    appendComponents:
 @abstract  Appends a list of components to the existing one
 */
- (void)appendComponents:(NSArray *)components;

/*!
 @method    appendComponent:
 @abstract  Appends a single component (NSString or NSAttributedString) to the existing list
 */
- (void)appendComponent:(id)component;
    
/*!
 @method    attributedString
 @abstract  Returns the attributed string which should be drawn
 */
- (NSAttributedString *)attributedString;

/*!
 @method    nonInteractiveAttributedString
 @abstract  Returns the attributed string which should be drawn, with no links
 */
- (NSAttributedString *)nonInteractiveAttributedString;

/*!
 @method    string
 @abstract  The plain NSString value
 */
- (NSString *)string;

/*!
 @method    stringWithoutColorizedText
 @method    Returns an NSString which is all the string contents where there's no color attribute set
 */
- (NSString *)stringWithoutColorizedText;

/*!
 @method        clearCache
 @abstract      Clears the cached NSAttributedString
 @discussion    This is useful if the color changes, because the string will be re-generated on demand
 */
- (void)clearCache;

@end
