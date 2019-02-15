//
//  LKTField.h
//  LingoKit
//
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <LingoKit/LKTInstance.h>
#import <LingoKit/LKTTypeUtils.h>


@interface LKTField : LKTInstance <NSCopying, NSCoding> {
    NSString *_key;
    NSObject *_value;
    LKTFieldType _type;
    NSString *_length;
    NSString *_units;
    LKTPreferredBase _preferredBase;
}

@property (nonatomic, copy) NSString *key;
@property (nonatomic, retain) NSObject *value;
@property (nonatomic) LKTFieldType type;
@property (nonatomic, copy) NSString *length;
@property (nonatomic, copy) NSString *units;
@property (nonatomic) LKTPreferredBase preferredBase;

@end
