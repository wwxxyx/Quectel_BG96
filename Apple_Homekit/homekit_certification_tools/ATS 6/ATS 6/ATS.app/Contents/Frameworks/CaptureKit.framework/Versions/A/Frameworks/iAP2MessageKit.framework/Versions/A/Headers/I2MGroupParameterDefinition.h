//
//  I2MGroupParameterDefinition.h
//  iAP2MessageKit
//
//  Created by Joe Basu on 4/26/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <iAP2MessageKit/I2MParameterDefinition.h>

/*!
 @class     I2MGroupParameterDefinition
 @abstract  Class for definitions of iAP2 group parameters
 */

@interface I2MGroupParameterDefinition : I2MParameterDefinition <NSCoding, NSCopying>

/*!
 @property parameterDefinitions
 @abstract An array of I2MParameterDefintions representing the subparameter defintions of this group parameter
 */
@property (nonatomic, readonly) NSArray *parameterDefinitions;

/*!
 @method +groupParameterDefinitionWithID:name:countExpression:parameterDefinitions:note:
 @abstract Creates and returns a group parameter definition.
 @param ID                   The parameter ID
 @param name                 The parameter name
 @param countExpression      The parameter's count expression as an I2MCountExpression
 @param parameterDefinitions An array of I2MParameterDefintions representing the subparameter defintions of this group parameter
 @param note                 Miscellaneous notes about this parameter
 @return The newly created and initialized group parameter definition
 */
+ (instancetype)groupParameterDefinitionWithID:(UInt16)ID
                                          name:(NSString *)name
                               countExpression:(I2MCountExpression)countExpression
                          parameterDefinitions:(NSArray *)parameterDefinitions
                                          note:(NSString *)note;

/*!
 @method +initWithID:name:countExpression:parameterDefinitions:note:
 @abstract Creates and returns a group parameter definition.
 @param ID                   The parameter ID
 @param name                 The parameter name
 @param countExpression      The parameter's count expression as an I2MCountExpression
 @param parameterDefinitions An array of I2MParameterDefintions representing the subparameter defintions of this group parameter
 @param note                 Miscellaneous notes about this parameter
 @return The The initialized group parameter definition
 */
- (instancetype)initWithID:(UInt16)ID
                      name:(NSString *)name
           countExpression:(I2MCountExpression)countExpression
      parameterDefinitions:(NSArray *)parameterDefinitions
                      note:(NSString *)note;


@end
