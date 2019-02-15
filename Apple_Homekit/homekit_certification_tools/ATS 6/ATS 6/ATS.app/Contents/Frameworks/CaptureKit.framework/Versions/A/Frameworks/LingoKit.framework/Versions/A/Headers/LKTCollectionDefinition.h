//
//  LKTCollectionDefinition.h
//  LingoKit
//
//  Created by Joe Basu on 1/20/10.
//  Copyright © 2010 Apple Inc. All Rights Reserved.
//

#import <LingoKit/LKTDefinition.h>


@interface LKTCollectionDefinition : LKTDefinition <NSCopying, NSCoding> {
    NSString *_key;
    NSString *_desc;
    NSString *_count; // this is a string because it can be a key for an field with a number value containing the count
}

@property (nonatomic, readonly, copy) NSString *key;
@property (nonatomic, readonly, copy) NSString *desc;
@property (nonatomic, readonly, copy) NSString *count;

- (id)initWithKey:(NSString *)key
             desc:(NSString *)desc
            count:(NSString *)count
         children:(NSArray *)children;

@end
