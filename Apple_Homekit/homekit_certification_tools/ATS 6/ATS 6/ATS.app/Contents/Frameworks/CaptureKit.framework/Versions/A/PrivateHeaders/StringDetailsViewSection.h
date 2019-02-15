//
//  StringDetailsViewSection.h
//  ATS
//
//  Created by Joe Basu on 6/1/10.
//  Copyright © 2010 Apple Inc. All Rights Reserved.
//

#import <CaptureKit/DetailsViewSection.h>

@interface StringDetailsViewSection : DetailsViewSection {
    NSString *_string;
}

@property (nonatomic, copy) NSString *string;

/*!
 @method    +stringDetailsViewSectionWithString:title:identifier
 @abstract  Creates and returns a StringDetailsViewSection with the given parameters.
 @param     string The string to display.
 @param     title The title of the section.
 @param     identifier The identifier for the section.
 */
+ (instancetype)stringDetailsViewSectionWithString:(NSString *)string title:(NSString *)title identifier:(NSString *)identifier;

/*!
 @method    +initWithString:title:identifier:
 @abstract  Inits a StringDetailsViewSection with the given parameters.
 @param     string The string to display.
 @param     title The title of the section.
 @param     identifier The identifier for the section.
 */
- (instancetype)initWithString:(NSString *)string title:(NSString *)title identifier:(NSString *)identifier;

@end
