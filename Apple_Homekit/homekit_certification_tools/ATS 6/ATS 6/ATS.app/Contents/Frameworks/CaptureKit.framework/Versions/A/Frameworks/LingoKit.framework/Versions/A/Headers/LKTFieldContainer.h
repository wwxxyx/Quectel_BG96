//
//  LKTFieldContainer.h
//  LingoKit
//
//  Created by Joe Basu on 1/21/10.
//  Copyright © 2010 Apple Inc. All Rights Reserved.
//

#import <LingoKit/LKTInstance.h>
#import <LingoKit/LKTTypeUtils.h>

@class LKTOrderedDictionary;
@class LKTProtocolDefinition;


@interface LKTFieldContainer : LKTInstance <NSCopying, NSCoding> {
    LKTSource _source;
    LKTOrderedDictionary *_fields;
    NSString *_template;
}

@property (nonatomic) LKTSource source;
@property (nonatomic, retain) LKTOrderedDictionary *fields;
@property (nonatomic, readonly) NSArray *fieldKeys;
@property (nonatomic, copy) NSString *template;
@property (nonatomic, readonly) BOOL hasTemplate;

- (void)addField:(LKTInstance *)field forKey:(id)key;
- (LKTInstance *)fieldForKey:(NSString *)key;
- (void)removeFieldForKey:(NSString *)key;
- (BOOL)hasFieldForKey:(NSString *)key;
- (UInt8)uInt8ForKey:(NSString *)key;
- (UInt16)uInt16ForKey:(NSString *)key;
- (UInt32)uInt32ForKey:(NSString *)key;
- (UInt64)uInt64ForKey:(NSString *)key;

@end
