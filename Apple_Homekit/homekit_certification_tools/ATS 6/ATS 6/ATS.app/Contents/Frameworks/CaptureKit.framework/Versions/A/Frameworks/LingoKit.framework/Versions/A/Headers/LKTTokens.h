//
//  LKTTokens.h
//  LingoKit
//
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <LingoKit/LKTInstance.h>

@class LKTTokensDefinition;

@interface LKTTokens : LKTInstance <NSCopying, NSCoding> {
    NSString *_key;
    NSArray *_tokens;
}

@property (nonatomic, copy) NSString *key;
@property (nonatomic, retain) NSArray *tokens;

@end
