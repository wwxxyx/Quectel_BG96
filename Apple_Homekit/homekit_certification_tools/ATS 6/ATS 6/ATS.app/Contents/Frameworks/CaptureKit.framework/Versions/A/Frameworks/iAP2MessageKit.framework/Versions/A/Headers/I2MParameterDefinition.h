//
//  I2MParameterDefinition.h
//  iAP2MessageKit
//
//  Created by Joe Basu on 4/26/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <iAP2MessageKit/I2MDefinition.h>
#import <iAP2MessageKit/I2MTypes.h>

/*!
 @class     I2MParameterDefinition
 @abstract  Class for definitions of iAP2 parameters
 */

@interface I2MParameterDefinition : I2MDefinition <NSCoding, NSCopying>

/*!
 @property identifier
 @abstract The parameter ID
 */
@property (nonatomic, readonly) UInt16 identifier;

/*!
 @property name
 @abstract The parameter name
 */
@property (nonatomic, readonly) NSString *name;

/*!
 @property countExpression
 @abstract The parameter's count expression as an I2MCountExpression
 */
@property (nonatomic, readonly) I2MCountExpression countExpression;

/*!
 @property type
 @abstract The parameter type (i.e. I2MParameterTypeUInt8)
 */
@property (nonatomic, readonly) I2MParameterType type;

/*!
 @property note
 @abstract Miscellaneous notes about this parameter
 */
@property (nonatomic, readonly) NSString *note;

/*!
 @method +parameterDefinitionWithID:name:countExpression:type:note:
 @abstract Creates and returns a parameter definition.
 @param ID                  The parameter ID
 @param name                The parameter name
 @param countExpression     The parameter's count expression as an I2MCountExpression
 @param type                The parameter type (i.e. I2MParameterTypeUInt8)
 @param note                Miscellaneous notes about this parameter
 @return The newly created and initialized parameter definition
 */
+ (instancetype)parameterDefinitionWithID:(UInt16)ID
                                     name:(NSString *)name
                          countExpression:(I2MCountExpression)countExpression
                                     type:(I2MParameterType)type
                                     note:(NSString *)note;

/*!
 @method -initWithID:name:countExpression:type:note:
 @abstract Initializes a parameter definition.
 @param ID                  The parameter ID
 @param name                The parameter name
 @param countExpression     The parameter's count expression as an I2MCountExpression
 @param type                The parameter type (i.e. I2MParameterTypeUInt8)
 @param note                Miscellaneous notes about this parameter
 @return The initialized parameter definition
 */
- (instancetype)initWithID:(UInt16)ID
                      name:(NSString *)name
           countExpression:(I2MCountExpression)countExpression
                      type:(I2MParameterType)type
                      note:(NSString *)note;
@end
