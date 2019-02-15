//
//  LKTBitfield.h
//  LingoKit
//
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <LingoKit/LKTInstance.h>
#import <LingoKit/LKTTypeUtils.h>
#import <LingoKit/LKTBitgroup.h>


@interface LKTBitfield : LKTInstance <NSCopying, NSCoding> {
    NSString *_key;
    LKTFieldType _type;
    NSArray *_components;
    uint64_t _rawValue;
}

@property (nonatomic, copy) NSString *key;
@property (nonatomic) LKTFieldType type;
@property (nonatomic, copy) NSArray *components;
@property (nonatomic) uint64_t rawValue;

- (LKTBitgroup *)bitgroupForKey:(NSString *)key;

@end
