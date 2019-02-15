//
//  IndexOffsetValueTransformer.h
//  ATSMacApp
//
//  Created by Cody Brimhall on 10/4/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

/*!
 @header     IndexOffsetValueTransformer
 @abstract   This header defines a value transformer that can be used to apply
             offsets to index values.
 */

#import <Foundation/Foundation.h>

/*!
 @class      IndexOffsetValueTransformer
 @abstract   A value transformer designed to apply offsets to index values. This
             is useful for establishing bindings between lists that have
             different starting index values.
 */
@interface IndexOffsetValueTransformer : NSValueTransformer

/*!
 @property   offset
 @abstract   The offset to apply to the value being transformed.
 @discussion In reverse transformations, the offset is negated before being 
             applied.
 */
@property (nonatomic, readonly) NSInteger offset;

/*!
 @method     valueTransformerWithOffset:
 @abstract   Returns a new, autoreleased value transformer initialized with the 
             given offset.
 @param      offset
             The offset to apply to the value being transformed.
 */
+ (id)valueTransformerWithOffset:(NSInteger)offset;

/*!
 @method     initWithOffset:
 @abstract   Returns a new value transformer initialized with the given offset.
 @param      offset
             The offset to apply to the value being transformed.
 @discussion This is the designated initializer.
 */
- (id)initWithOffset:(NSInteger)offset;

@end
