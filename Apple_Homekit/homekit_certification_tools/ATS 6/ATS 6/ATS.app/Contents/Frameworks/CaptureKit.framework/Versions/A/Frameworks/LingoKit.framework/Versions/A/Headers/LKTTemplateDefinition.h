//
//  LKTTemplateDefinition.h
//  LingoKit
//
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <LingoKit/LKTDefinition.h>


@interface LKTTemplateDefinition : LKTDefinition <NSCopying, NSCoding> {
    NSString *_value;
}

@property (nonatomic, readonly, copy) NSString *value;

- (id)initWithValue:(NSString *)value;

@end
