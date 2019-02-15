//
//  LKTBit.h
//  LingoKit
//
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <LingoKit/LKTInstance.h>


@interface LKTBit : LKTInstance <NSCopying, NSCoding> {
    NSUInteger _index;
    BOOL _isDeprecatedValue;
}

@property (nonatomic) NSUInteger index;
@property (nonatomic) BOOL isDeprecatedValue;

@end
