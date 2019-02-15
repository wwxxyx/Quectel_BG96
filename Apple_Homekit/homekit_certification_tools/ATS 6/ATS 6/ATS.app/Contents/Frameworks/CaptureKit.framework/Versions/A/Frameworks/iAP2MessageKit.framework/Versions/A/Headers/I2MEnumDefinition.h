//
//  I2MEnumDefinition.h
//  iAP2MessageKit
//
//  Created by Joe Basu on 4/27/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <iAP2MessageKit/I2MDefinition.h>

/*!
 @class     I2MEnumDefinition
 @abstract  Class for definitions of enum values
 */

@interface I2MEnumDefinition : I2MDefinition <NSCoding, NSCopying>

/*!
 @property value
 @abstract The enum value (an unsigned 8-bit integer)
 */
@property (nonatomic, readonly) UInt8 value;

/*!
 @property valueDescription
 @abstract A string description of the enum value
 */
@property (nonatomic, readonly) NSString *valueDescription;

/*!
 @method +enumDefinitionWithValue:valueDescription:
 @abstract Creates and returns an enum definition
 @param value            The enum value (an unsigned 8-bit integer)
 @param valueDescription A string description of the enum value
 @return The newly created and initialized enum definition
 */
+ (instancetype)enumDefinitionWithValue:(UInt8)value
                       valueDescription:(NSString *)valueDescription;

/*!
 @method -initWithValue:valueDescription:
 @abstract Initializes an enum definition
 @param value            The enum value (an unsigned 8-bit integer)
 @param valueDescription A string description of the enum value
 @return The initialized enum definition
 */
- (instancetype)initWithValue:(UInt8)value
             valueDescription:(NSString *)valueDescription;


@end
