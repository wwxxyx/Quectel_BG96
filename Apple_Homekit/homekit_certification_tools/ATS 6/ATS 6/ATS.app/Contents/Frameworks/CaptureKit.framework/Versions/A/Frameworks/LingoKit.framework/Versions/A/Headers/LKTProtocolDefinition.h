//
//  LKTProtocolDefinition.h
//  LingoKit
//
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <LingoKit/LKTDefinition.h>

@class LKTLingoDefinition;
@class LKTCommandDefinition;

@interface LKTProtocolDefinition : LKTDefinition <NSCopying, NSCoding> {
    NSArray *_lingoes;
}

@property (nonatomic, readonly) NSArray *lingoes;

- (id)initWithLingoes:(NSArray *)lingoes;
- (LKTLingoDefinition *)lingoForID:(uint8_t)lingoID;
- (NSString *)lingoNameForLingoID:(uint8_t)lingoID;
- (LKTCommandDefinition *)commandDefinitionForLingoID:(uint8_t)lingoID commandID:(uint16_t)commandID;

@end
