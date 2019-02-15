//
//  LKTTokenDefinition.h
//  LingoKit
//
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <LingoKit/LKTDefinition.h>


@interface LKTTokenDefinition : LKTDefinition <NSCopying, NSCoding> {
    NSString *_name;
    uint8_t _fidType;
    uint8_t _fidSubtype;
    BOOL _isDeprecated;
}

@property (nonatomic, readonly, copy) NSString *name;
@property (nonatomic, readonly) uint8_t fidType;
@property (nonatomic, readonly) uint8_t fidSubtype;
@property (nonatomic, readonly) BOOL isDeprecated;

- (id)initWithName:(NSString *)name
           fidType:(uint8_t)fidType
        fidSubtype:(uint8_t)fidSubtype
          children:(NSArray *)children
      isDeprecated:(BOOL)isDeprecated;

@end
