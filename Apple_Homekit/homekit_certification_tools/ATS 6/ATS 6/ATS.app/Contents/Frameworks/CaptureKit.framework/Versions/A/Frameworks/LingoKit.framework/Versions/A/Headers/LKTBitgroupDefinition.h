//
//  LKTBitgroupDefinition.h
//  LingoKit
//
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <LingoKit/LKTDefinition.h>


@interface LKTBitgroupDefinition : LKTDefinition <NSCopying, NSCoding> {
    NSString *_key;
    NSString *_desc;
    NSNumber *_lower;
    NSNumber *_upper;
}

@property (nonatomic, readonly, copy) NSString *key;
@property (nonatomic, readonly, copy) NSString *desc;
@property (nonatomic, readonly, copy) NSNumber *lower;
@property (nonatomic, readonly, copy) NSNumber *upper;

- (id)initWithKey:(NSString *)key
             desc:(NSString *)desc
            lower:(NSNumber *)lower
            upper:(NSNumber *)upper
         children:(NSArray *)children;

@end
