//
//  NSArray+DSAdditions.h
//  Grapher
//
//  Created by David Silver on 8/20/10.
//  Copyright 2010 Apple, Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface NSMutableArray (DSAdditions)

/* Moves an object. The destination refers to the position at which you would want to insert the object if you were adding it, and not moving it. Thus,
 * the destinationIndex is the index of the item that will be after the moved item after the move. To move an item to the end of the array, specify one more 
 * than the index of the current last element.
 * Returns YES of the object is actually moved. If the object at the origin index is already at the destination index, returns NO. This may be possible
 * even if originIndex and destinationIndex are not equal, due to objects shifting during a move. */
- (BOOL)moveObjectAtIndex:(NSUInteger)originIndex toIndex:(NSUInteger)destinationIndex;

@end
