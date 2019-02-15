//
//  I2MEnumParameterDefinition.h
//  iAP2MessageKit
//
//  Created by Joe Basu on 5/10/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <iAP2MessageKit/I2MParameterDefinition.h>

/*!
 @class     I2MEnumParameterDefinition
 @abstract  Class for definitions of iAP2 enum parameters
 */

@interface I2MEnumParameterDefinition : I2MParameterDefinition <NSCoding, NSCopying>

/*!
 @property enumDefinitions
 @abstract An array of I2MEnumDefinitions for all of the enum values in the enumeration
 */
@property (nonatomic, readonly) NSArray *enumDefinitions;

/*!
 @property enumName
 @abstract The parameter enum name if it's an enum
 */
@property (nonatomic, readonly) NSString *enumName;

/*!
 @method +enumParameterDefinitionWithID:name:countExpression:enumDefinitions:note:
 @abstract Creates and returns an enum parameter definition.
 @param ID                  The parameter ID
 @param name                The parameter name
 @param enumName            The parameter enum name if it's an enum
 @param countExpression     The parameter's count expression as an I2MCountExpression
 @param enumDefinitions     An array of I2MEnumDefinitions for all of the enum values in the enumeration
 @param note                Miscellaneous notes about this parameter
 @param isArray             Whether or not the enum is an array of enums
 @return The newly created and initialized enum parameter definition
 */
+ (instancetype)enumParameterDefinitionWithID:(UInt16)ID
                                         name:(NSString *)name
                                     enumName:(NSString *)enumName
                              countExpression:(I2MCountExpression)countExpression
                              enumDefinitions:(NSArray *)enumDefinitions
                                         note:(NSString *)note
                                      isArray:(BOOL)isArray;

/*!
 @method -initWithID:name:countExpression:enumDefinitions:note:
 @abstract Initializes an enum parameter definition.
 @param ID                  The parameter ID
 @param name                The parameter name
 @param enumName            The parameter enum name if it's an enum

 @param countExpression     The parameter's count expression as an I2MCountExpression
 @param enumDefinitions     An array of I2MEnumDefinitions for all of the enum values in the enumeration
 @param note                Miscellaneous notes about this parameter
 @param isArray             Whether or not the enum is an array of enums
 @return The initialized enum parameter definition
 */
- (instancetype)initWithID:(UInt16)ID
                      name:(NSString *)name
                  enumName:(NSString *)enumName
           countExpression:(I2MCountExpression)countExpression
           enumDefinitions:(NSArray *)enumDefinitions
                      note:(NSString *)note
                   isArray:(BOOL)isArray;

@end
