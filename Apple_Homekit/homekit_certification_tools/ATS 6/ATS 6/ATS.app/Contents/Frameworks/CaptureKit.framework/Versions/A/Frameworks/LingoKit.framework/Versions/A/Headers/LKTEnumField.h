//
//  LKTEnumField.h
//  LingoKit
//
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <LingoKit/LKTInstance.h>
#import <LingoKit/LKTTypeUtils.h>


@interface LKTEnumField : LKTInstance <NSCopying, NSCoding> {
    NSString *_key;
    LKTFieldType _type;
    uint64_t _value;
    NSString *_valueDesc;
    BOOL _isDeprecatedValue;
}

@property (nonatomic, copy) NSString *key;
@property (nonatomic) LKTFieldType type;
@property (nonatomic) uint64_t value;
@property (nonatomic, copy) NSString *valueDesc;
@property (nonatomic, readonly) BOOL isReserved;
@property (nonatomic) BOOL isDeprecatedValue;

@end
