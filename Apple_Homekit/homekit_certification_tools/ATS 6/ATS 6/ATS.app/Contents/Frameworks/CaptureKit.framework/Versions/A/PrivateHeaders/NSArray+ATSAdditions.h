//
//  NSArray+ATSAdditions.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 9/14/11.
//  Copyright (c) 2011 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <TrafficKit/NSArray+TrafficKit.h>

@interface NSArray (ATSAdditions)

// Similar to isEqual:, but ignores order
- (BOOL)isEqualIgnoringOrder:(id)other;

/*!
 @method    firstObjectOfClass:
 @abstract  Returns the first object found of the passed class
 */
- (id)firstObjectOfClass:(Class)aClass;

/*!
 @method    firstObjectOfClass:predicate:
 @abstract  Returns a the first object found of the passed class which also passes the predicate
 */
- (id)firstObjectOfClass:(Class)aClass predicate:(BOOL (^)(id))block;

/*!
 @method    objectsOfClass:
 @abstract  Returns a subarray of objects of the passed class
 */
- (NSArray *)objectsOfClass:(Class)aClass;

/*!
 @method    objectsOfClass:predicate:
 @abstract  Returns a subarray of objects of the passed class which also return true for the predicate
 */
- (NSArray *)objectsOfClass:(Class)aClass predicate:(BOOL (^)(id))block;

@end
