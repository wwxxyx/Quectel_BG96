//
//  LKTDefinition.h
//  LingoKit
//
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <Foundation/Foundation.h>

@class LKTOrderedDictionary;

@interface LKTDefinition : NSObject <NSCopying, NSCoding> {
    NSArray *_children;
}

@property (nonatomic, readonly) NSArray *children;

- (id)initWithChildren:(NSArray *)children;

- (NSString *)humanDescription;

@end
