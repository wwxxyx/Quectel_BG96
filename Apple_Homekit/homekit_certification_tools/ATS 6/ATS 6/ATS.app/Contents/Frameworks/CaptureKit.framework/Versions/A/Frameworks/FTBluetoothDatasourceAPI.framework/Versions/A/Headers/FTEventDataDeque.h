//
//  FTEventDataDeque.h
//  FTBluetoothDatasourceAPI
//
//  Created by Michael Lance on 4/12/12.
//  Copyright (c) 2012 Frontline Test Equipment, Inc. All rights reserved.
//

#pragma once

@class CHListDeque;

/**
 @file FTEventDataDeque.h
 */
/**
 FTThreadSafeEventDataDeque
 
 A class that represents a lock-protected deque.
 */
@interface FTThreadSafeEventDataDeque : NSObject
{
@private
  NSLock* lock;
  CHListDeque* deque;
}

/**
 Initialize a deque.
 
 @return An initialized empty deque.
 */
- (id)init;

/**
 Initialize a deque with another deque.
 
 @return A deque initialized with the contents of the specified deque.
 */
- (id)initWithThreadSafeEventDataDeque:(FTThreadSafeEventDataDeque*)threadSafeDeque;

/**
 Deallocate a deque.
 */
- (void)dealloc;

/**
 Remove all objects from the deque.
 */
- (void)removeAllObjects;

/**
 Copy all objects in the deque to an array, then remove them.
 
 @param objects Mutable array to receive the objects.
 */
- (void)removeAllObjectsToArray:(NSMutableArray*)objects;

/**
 Append an item to the end of the deque.
 
 @param item Item to append.
 @return TRUE if operation succeeded.
 */
- (BOOL)appendItem:(id)item;  // Puts a buffer at the bottom of the deque.

/**
 Prepend an item to the beginning of the deque.
 
 @param item Item to prepend.
 @return TRUE if operation succeeded.
 */
- (BOOL)prependItem:(id)item;  // Puts a buffer at the top of the deque.

/**
 Get the number of items in the deque.
 
 @return Number of items in the deque.
 */
- (NSUInteger)count;

/**
 Check to see if the deque is empty.
 
 @return TRUE if no items in deque.
 */
- (BOOL)empty;

/**
 Get a pointer to the top item in the deque.
 
 @param itemPtr Pointer to receive the address of the top item in the deque.
 @return TRUE if operation succeeded.
 */
- (BOOL)referenceTopEvent:(id*)itemPtr;

/**
 Get a pointer to the bottom item in the deque.
 
 @param itemPtr Pointer to receive the address of the bottom item in the deque.
 @return TRUE if operation succeeded.
 */
- (BOOL)referenceBottomEvent:(id*)itemPtr;

/**
 Remove the top item in the deque and return a pointer to it.
 
 @param itemPtr Pointer to receive the address of the top item in the deque.
 @return TRUE if operation succeeded.
 */
- (BOOL)popTopEvent:(id*)itemPtr;  // Gets buffer at top of deque and releases it.

/**
 Remove the bottom item in the deque and return a pointer to it.
 
 @param itemPtr Pointer to receive the address of the bottom item in the deque.
 @return TRUE if operation succeeded.
 */
- (BOOL)popBottomEvent:(id*)itemPtr;  // Gets buffer at bottom of deque and releases it.

/**
 Access the item at the specified index in the deque.
 
 @param index Index of item to access.
 @return The requested item or nil if the index is out of range.
 */
- (id)objectAtIndex:(NSUInteger)index;  // Gets buffer at index in deque.

/**
 Copy the contents of the deque to an array.
 
 @param array Mutable array to receive items.
 @return TRUE if operation succeeded.
 */
- (BOOL)copyObjectsToArray:(NSMutableArray*)array;
@end
