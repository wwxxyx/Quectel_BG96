//
//  LKTToken.h
//  LingoKit
//
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <LingoKit/LKTFieldContainer.h>

@class LKTOrderedDictionary;

@interface LKTToken : LKTFieldContainer <NSCopying, NSCoding> {
    NSString *_name;
    uint8_t _fidType;
    uint8_t _fidSubtype;
}

@property (nonatomic, copy) NSString *name;
@property (nonatomic) uint8_t fidType;
@property (nonatomic) uint8_t fidSubtype;

@end
