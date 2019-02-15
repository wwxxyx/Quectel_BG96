//
//  OrderedDictionary.h
//  OrderedDictionary
//
//  Created by Matt Gallagher on 19/12/08.
//  Copyright 2008 Matt Gallagher. All rights reserved.
//
//  Permission is given to use this source code file without charge in any
//  project, commercial or otherwise, entirely at your risk, with the condition
//  that any redistribution (in part or whole) of source code must retain
//  this copyright and permission notice. Attribution in compiled projects is
//  appreciated but not required.
//

#import <Foundation/Foundation.h>


@interface LKTOrderedDictionary : NSObject <NSCopying, NSCoding, NSFastEnumeration>
{
    NSMutableDictionary *_dictionary;
    NSMutableArray *_array;
}

@property (nonatomic, readonly) NSDictionary *dictionary;
@property (nonatomic, readonly) NSArray *array;

+ (id)orderedDictionary;
+ (id)dictionaryWithObjectsAndKeys:(id)firstObject, ...;

- (id)initWithObjectsAndKeys:(id)firstObject, ...;
- (id)initWithObject:(id)object forKey:(id)key;

- (NSUInteger)count;
- (id)objectForKey:(id)aKey;

- (id)objectAtIndex:(NSUInteger)anIndex;
- (id)keyAtIndex:(NSUInteger)anIndex;

@end
