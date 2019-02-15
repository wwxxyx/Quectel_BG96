//
//  LKTMutableOrderedDictionary.h
//  LingoKit
//
//  Created by Joel Levin on 9/23/09.
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <Foundation/Foundation.h>
#import "LKTOrderedDictionary.h"


@interface LKTOrderedDictionary (Mutability)

- (id)initWithCapacity:(NSUInteger)capacity;

- (void)addObject:(id)anObject forKey:(id)aKey;
- (void)insertObject:(id)anObject forKey:(id)aKey atIndex:(NSUInteger)anIndex;
- (void)setObject:(id)anObject forKey:(id)aKey;
- (void)removeObjectForKey:(id)aKey;

@end


@interface LKTMutableOrderedDictionary : LKTOrderedDictionary
{

}

@end
