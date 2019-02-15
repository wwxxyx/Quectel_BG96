//
//  LKTPredicateDefinition.h
//  LingoKit
//
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <LingoKit/LKTDefinition.h>


@interface LKTPredicateDefinition : LKTDefinition <NSCopying, NSCoding> {
    NSString *_predicateString;
    NSPredicate *_predicate;
}

@property (nonatomic, readonly, copy) NSString *predicateString;
@property (nonatomic, readonly) NSPredicate *predicate;

- (id)initWithPredicateString:(NSString *)predicateString
                     children:(NSArray *)children;

@end
