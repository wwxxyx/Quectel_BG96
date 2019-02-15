//
//  RuntimeUtilities.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 4/25/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface RuntimeUtilities : NSObject

/*!
 @method    enumerateClassesWithBlock:
 @abstract  Enumerates all classes in the runtime with the given block
 */
+ (void)enumerateClassesWithBlock:(void (^)(Class))block;

/*!
 @method    doesClass:inheritFromClass:
 @abstract  Whether or not 'givenClass' is, or inherits from, 'parentClass'
 */
+ (BOOL)doesClass:(Class)givenClass inheritFromClass:(Class)parentClass;

/*!
 @method    allSubclassesOfClass:
 @abstract  Returns an array of all subclasses of the passed class, including the class itself
 */
+ (NSArray *)allSubclassesOfClass:(Class)aClass;

+ (void)performViewDidLoadIfMavericksOrOlderOnViewController:(NSViewController *)viewController;

+ (BOOL)isYosemiteOrLater;

@end
