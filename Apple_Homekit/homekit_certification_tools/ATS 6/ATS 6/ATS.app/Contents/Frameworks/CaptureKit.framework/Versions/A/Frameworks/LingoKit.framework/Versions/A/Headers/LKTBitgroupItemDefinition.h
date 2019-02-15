//
//  LKTBitgroupItemDefinition.h
//  LingoKit
//
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <LingoKit/LKTDefinition.h>


@interface LKTBitgroupItemDefinition : LKTDefinition <NSCopying, NSCoding> {
    NSNumber *_value;
    NSString *_desc;
    BOOL _isDeprecated;
}

@property (nonatomic, readonly, copy) NSNumber *value;
@property (nonatomic, readonly, copy) NSString *desc;
@property (nonatomic, readonly) BOOL isDeprecated;

- (id)initWithValue:(NSNumber *)value
               desc:(NSString *)desc
       isDeprecated:(BOOL)isDeprecated;

@end
