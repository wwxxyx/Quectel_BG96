//
//  LKTEnumRangeDefinition.h
//  LingoKit
//
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <LingoKit/LKTDefinition.h>


@interface LKTEnumRangeDefinition : LKTDefinition <NSCopying, NSCoding> {
    NSNumber *_min;
    NSNumber *_max;
    NSString *_desc;
    BOOL _isDeprecated;
}

@property (nonatomic, readonly, copy) NSNumber *min;
@property (nonatomic, readonly, copy) NSNumber *max;
@property (nonatomic, readonly, copy) NSString *desc;
@property (nonatomic, readonly) BOOL isDeprecated;

- (id)initWithMin:(NSNumber *)min
              max:(NSNumber *)max
             desc:(NSString *)desc
     isDeprecated:(BOOL)isDeprecated;

@end
