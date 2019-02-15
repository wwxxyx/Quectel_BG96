//
//  LKTBitgroup.h
//  LingoKit
//
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <LingoKit/LKTInstance.h>


@interface LKTBitgroup : LKTInstance <NSCopying, NSCoding> {
    NSString *_key;
    NSUInteger _lower;
    NSUInteger _upper;
    uint64_t _value;
    NSString *_valueDesc;
    BOOL _isDeprecatedValue;
}

@property (nonatomic, copy) NSString *key;
@property (nonatomic) NSUInteger lower;
@property (nonatomic) NSUInteger upper;
@property (nonatomic) uint64_t value;
@property (nonatomic, copy) NSString *valueDesc;
@property (nonatomic) BOOL isDeprecatedValue;


@end
