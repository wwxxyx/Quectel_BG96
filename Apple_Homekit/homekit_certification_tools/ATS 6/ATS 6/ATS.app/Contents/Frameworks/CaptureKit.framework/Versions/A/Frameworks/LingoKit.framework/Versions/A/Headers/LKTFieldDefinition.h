//
//  LKTFieldDefinition.h
//  LingoKit
//
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <LingoKit/LKTBaseFieldDefinition.h>


@interface LKTFieldDefinition : LKTBaseFieldDefinition <NSCopying, NSCoding> {
    NSString *_length;
    NSNumber *_maxLength;
    NSNumber *_minValue;
    NSNumber *_maxValue;
    NSString *_units;
    LKTPreferredBase _preferredBase;
}

@property (nonatomic, readonly, copy) NSString *length;
@property (nonatomic, readonly, copy) NSNumber *maxLength;
@property (nonatomic, readonly, copy) NSNumber *minValue;
@property (nonatomic, readonly, copy) NSNumber *maxValue;
@property (nonatomic, readonly, copy) NSString *units;
@property (nonatomic, readonly) LKTPreferredBase preferredBase;


- (id)initWithKey:(NSString *)key
             desc:(NSString *)desc
             type:(LKTFieldType)type
           length:(NSString *)length
        maxLength:(NSNumber *)maxLength
         minValue:(NSNumber *)minValue
         maxValue:(NSNumber *)maxValue
            units:(NSString *)units
    preferredBase:(LKTPreferredBase)preferredBase;


@end
