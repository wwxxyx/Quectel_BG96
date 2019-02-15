//
//  ATSFixedSizeLinkedArray.h
//  ATSMacApp
//
//  Created by Carlos Frederico Azevedo on 8/24/15.
//  Copyright (c) 2015 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

/*!
 Partial implementation of a linked array with a fixed number of links.
 Unfilled links have nil value.
 */
@interface ATSFixedSizeLinkedArray : NSObject

/*!
 Number of available links in the ATSFixedSizeLinkedArray
 */
@property (assign) NSUInteger links;

/*!
 Current index in the linkedList. If set the value assumed is value%links.
 */
@property (nonatomic, assign) NSUInteger currentIndex;

/*!
 Creates a ATSFixedSizeLinkedArray object with a linkedList of capacity links.
 @param links The capacity of the new ATSFixedSizeLinkedArray object.
 @return A ATSFixedSizeLinkedArray of capacity links.
 */
+ (instancetype)ATSFixedSizeLinkedArrayWithNumberOfLinks:(NSUInteger)links;

/*!
 Returns the next value in the linked list from the current index.
 If no object has been placed there yet, returns nil.
 */
- (id)next;

/*!
 Returns the previous value in the linked list form the current index.
 If no object has been placed there yet, returns nil.
 */
- (id)previous;

/*!
 Returns the value in the linked list for the current index.
 If the linked list is empty, returns nil.
 */
- (id)current;

/*!
Inserts the value object into the next position in the linked array. Replaces anything that was there before.
@param value object to be inserted.
 */
- (void)insertValue:(id)value;

@end
