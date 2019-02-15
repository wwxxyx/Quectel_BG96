//
//  LKTBitDefinition.h
//  LingoKit
//
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <LingoKit/LKTDefinition.h>


@interface LKTBitDefinition : LKTDefinition <NSCopying, NSCoding> {
    NSNumber *_index;
    NSString *_desc;
    NSString *_off;
    BOOL _required;
    BOOL _isDeprecated;
}

@property (nonatomic, readonly, copy) NSNumber *index;
@property (nonatomic, readonly, copy) NSString *desc;
@property (nonatomic, readonly, copy) NSString *off;
@property (nonatomic, readonly) BOOL required;
@property (nonatomic, readonly) BOOL isDeprecated;

- (id)initWithIndex:(NSNumber *)index
               desc:(NSString *)desc
                off:(NSString *)off
           required:(BOOL)required
       isDeprecated:(BOOL)isDeprecated;

@end
