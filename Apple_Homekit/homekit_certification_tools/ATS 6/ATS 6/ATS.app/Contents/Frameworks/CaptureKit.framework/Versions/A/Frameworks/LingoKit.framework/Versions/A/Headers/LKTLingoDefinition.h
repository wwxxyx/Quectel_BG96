//
//  LKTLingoDefinition.h
//  LingoKit
//
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <LingoKit/LKTDefinition.h>

@class LKTCommandDefinition;

@interface LKTLingoDefinition : LKTDefinition <NSCopying, NSCoding> {
    NSString *_lingoName;
    uint8_t _lingoID;
    NSArray *_commands;
    BOOL _isDeprecated;
}

@property (nonatomic, readonly, copy) NSString *lingoName;
@property (nonatomic, readonly) uint8_t lingoID;
@property (nonatomic, readonly) NSArray *commands;
@property (nonatomic, readonly) BOOL isDeprecated;

- (id)initWithLingoID:(uint8_t)lingoID 
                 name:(NSString *)lingoName 
             commands:(NSArray *)commands
         isDeprecated:(BOOL)isDeprecated;

- (LKTCommandDefinition *)commandForID:(uint16_t)commandID;

@end
