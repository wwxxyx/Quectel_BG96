//
//  I2MTypeUtils.h
//  iAP2MessageKit
//
//  Created by Joe Basu on 5/21/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <iAP2MessageKit/I2MTypes.h>

/** A class with utility methods for dealing with iAP2MessageKit types. */
@interface I2MTypeUtils : NSObject

/**
 Returns the string description for an I2MSource value.
 
 @param source The source value.
 @return The string description of the source value.
 */
+ (NSString *)descriptionForSource:(I2MSource)source;

/**
 Returns the I2MSourceValue corresponding to the given description string.
 
 @param description The description string to map to a source value.
 @return The I2MSource value that the string maps to, or I2MSourceUnknown if there is no mapping.
 */
+ (I2MSource)sourceForDescription:(NSString *)description;

/**
 Returns the string description for an I2MParameterType value.
 
 @param parameterType The I2MParameterType value.
 @return The string description of the I2MParameterType value.
 */
+ (NSString *)descriptionForParameterType:(I2MParameterType)parameterType;

/**
 Returns the I2MParameterType value corresponding to the given description string.
 
 @param description The description string to map to a parameter type.
 @return The I2MParameterType value that the string maps to, or I2MParameterTypeUnknown if there is no mapping.
 */
+ (I2MParameterType)parameterTypeForDescription:(NSString *)description;

/**
 Returns the string description for an I2MCountExpression value.
 
 @param countExpression The count expression value.
 @return The string description of the count expression value.
 */
+ (NSString *)descriptionForCountExpression:(I2MCountExpression)countExpression;

/**
 Returns the I2MCountExpression value corresponding to the given description string.
 
 @param description The description string to map to a count expression value.
 @return The I2MCountExpression value that the string maps to, or I2MCountExpressionUnkown if there is no mapping.
 */
+ (I2MCountExpression)countExpressionForDescription:(NSString *)description;

/**
 Returns the number of data bytes is expected for the given parameter type.
 
 @param type The parameter type
 @return The length (in bytes) that the data payload should be. Nil if undefined
 */
+ (NSNumber *)byteCountForType:(I2MParameterType)type;

/**
 Use to know if the parameter is a simple array, meaning that it contains integers and such.
 
 @param type The parameter type
 @return Whether the type is a simple array.
 */
+ (BOOL)isSimpleArrayType:(I2MParameterType)type;

/**
 Returns the element type of the given simple array. An exception is thrown if the passed in type is no a simple array.
 
 @param type The parameter type
 @return The element type of the array.
 */
+ (I2MParameterType)elementTypeForSimpleArrayType:(I2MParameterType)type;

@end
