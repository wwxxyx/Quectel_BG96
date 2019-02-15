//
//  NSArray+TrafficKit.h
//  TrafficKit
//
//  Created by Mark Hamlin on 6/7/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface NSArray (TrafficKit)

/*!
 @method        componentsJoinedByCommaSeparatedList
 @abstract      Uses english grammar rules to turn the string elements of self into a comma-separated list with joining word "and"
 @return        Correctly formatted string of items. For example. @[ @"Hello", @"World", @"Foo" ] => @"Hello, World, and Foo"
 */
- (NSString *)componentsJoinedByCommaSeparatedList;

/*!
 @method        componentsJoinedBySemicolonSeparatedList
 @abstract      Uses english grammar rules to turn the string elements of self into a semicolon-separated list with joining word "and"
 @return        Correctly formatted string of items. For example. @[ @"Hello", @"World", @"Foo" ] => @"Hello; World; and Foo"
 */
- (NSString *)componentsJoinedBySemicolonSeparatedList;

/*!
 @method        componentsJoinedByCommaSeparatedListWithJoiningWordOr
 @abstract      Uses english grammar rules to turn the string elements of self into a comma-separated list with joining word "or"
 @return        Correctly formatted string of items. For example. @[ @"Hello", @"World", @"Foo" ] => @"Hello, World, or Foo"
 */
- (NSString *)componentsJoinedByCommaSeparatedListWithJoiningWordOr;

/*!
 Uses english grammar rules to turn the string elements of self into a semicolon-separated list with joining word "and". However, it will also include the separator when joining two string elements.
 @return Correctly formatted string of items. For example. @[ @"Hello", @"World", @"Foo" ] => @"Hello; World; and Foo". If the string of items contains two elements, the result will include the separator as well. For example. @[ @"Hello", @"World" ] => @"Hello; and World"
 */
- (NSString *)componentsJoinedBySemicolonSeparatedListWithForcedSeparator;

/*!
 @method        componentsJoinedByListWithSeparator:
 @abstract      Uses English grammar rules to turn the string elements of self into a list seperated with the separator string and joining word.
                The forceSeparator can be set to force the use of the separator string when joining two string elements
 @return        Correctly formatted string of items. For example. @[ @"Hello", @"World", @"Foo" ] => @"Hello<separator> World<separator> <joiningWord> Foo".
                If forceSeparator is true, the formatted string of two items will include the separator. For example. @[ @"Hello", @"World" ] => @"Hello<separator> <joiningWord> World"
 */
- (NSString *)componentsJoinedByListWithSeparator:(NSString *)separator joiningWord:(NSString *)joiningWord forceSeparator:(BOOL)forceSeparator;

- (NSArray *)map:(id (^)(id object, NSUInteger index))block;

@end
