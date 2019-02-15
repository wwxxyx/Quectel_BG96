//
//  LKTBaseFieldDefinition.h
//  LingoKit
//
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <LingoKit/LKTDefinition.h>
#import <LingoKit/LKTTypeUtils.h>


@interface LKTBaseFieldDefinition : LKTDefinition <NSCopying, NSCoding> {
    NSString *_key;
    NSString *_desc;
    LKTFieldType _type;
}

@property (nonatomic, readonly, copy) NSString *key;
@property (nonatomic, readonly, copy) NSString *desc;
@property (nonatomic, readonly) LKTFieldType type;

- (id)initWithKey:(NSString *)key
             desc:(NSString *)desc
             type:(LKTFieldType)type
         children:(NSArray *)children;

@end
