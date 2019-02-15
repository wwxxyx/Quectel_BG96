//
//  RawBytesDetailsViewSection.h
//  ATS
//
//  Created by Joe Basu on 6/1/10.
//  Copyright © 2010 Apple Inc. All Rights Reserved.
//

#import <CaptureKit/DetailsViewSection.h>

@interface RawBytesDetailsViewSection : DetailsViewSection {
    NSData *_data;
    NSUInteger _caretLocation;
}

@property (nonatomic, copy) NSData *data;
@property (nonatomic) NSUInteger caretLocation;

/*!
 @method    initWithTitle:identifier:highlightLeafRanges:
 @abstract  Init with option to highlight ranges which leaves notify as needing highlighting
 */
- (id)initWithTitle:(NSString *)title identifier:(NSString *)identifier highlightLeafRanges:(BOOL)highlightsLeafRanges;

/*!
 @method    addHighlightRange:color:identifier
 @abstract  Highlights the specified range using the specified color (which is an html string, so #aabbcc or color name, 'red'), using the optional identifier "identifier"
 */
- (void)addHighlightRange:(NSRange)range color:(NSString *)htmlColor identifier:(id)identifier;

/*!
 @method    removeHighlightWithIdentifier:
 @abstract  Removes the highlight with the given identifier
 */
- (void)removeHighlightWithIdentifier:(id)identifier;

@end
