//
//  I2MParameter.h
//  iAP2MessageKit
//
//  Created by Joe Basu on 3/20/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <iAP2MessageKit/I2MTypes.h>

/*!
 @class     I2MParameter
 @abstract  A flexible record-style class representing an instance of an iAP2 parameter
 */

@interface I2MParameter : NSObject

/*!
 @property length
 @abstract The parameter length as defined in the Accessory Interface Specification
 @discussion This property is set, not computed, and may not reflect the actual amount of data available.
 */
@property (nonatomic) UInt16 length;

/*!
 @property identifier
 @abstract The parameter ID
 */
@property (nonatomic) UInt16 identifier;

/*!
 @property name
 @abstract The parameter name
 */
@property (nonatomic, copy) NSString *name;

/*!
 @property type
 @abstract The parameter type
 */
@property (nonatomic) I2MParameterType type;

/*!
 @property countExpression
 @abstract The parameter's count expression as an I2MCountExpression
 */
@property (nonatomic) I2MCountExpression countExpression;

/*!
 @property data
 @abstract The parameter payload data (not including the ID and length bytes)
 */
@property (nonatomic, copy) NSData *data;

/*!
 @property  objectValue
 @abstract  An object representing the parameter's data.
 @discussion This property is set by the I2MRichParser's dataSource during
             the parsing phase.
 */
@property (nonatomic, strong) id objectValue;

/*!
 @property  messageBytesRange
 @abstract  The range of bytes this parameter was parsed from in the message raw bytes, including length and identifier
 */
@property (nonatomic) NSRange messageBytesRange;

#pragma mark - Values

/*!
 @property int8Value
 @abstract Returns the value of the data interpreted as a SInt8.
 @discussion The parameter type must be int8, otherwise an exception will be thrown.
 @throws When the parameter type is not int8
 @return A SInt8 value
 */
@property (readonly) SInt8 int8Value;

/*!
 @property int16Value
 @abstract Returns the value of the data interpreted as a SInt16.
 @discussion The parameter type must be int16, otherwise an exception will be thrown.
 @throws When the parameter type is not int16
 @return A SInt16 value
 */
@property (readonly) SInt16 int16Value;

/*!
 @property int32Value
 @abstract Returns the value of the data interpreted as a SInt32.
 @discussion The parameter type must be int32, otherwise an exception will be thrown.
 @throws When the parameter type is not int32
 @return A SInt32 value
 */
@property (readonly) SInt32 int32Value;

/*!
 @property int64Value
 @abstract Returns the value of the data interpreted as a SInt64.
 @discussion The parameter type must be int64, otherwise an exception will be thrown.
 @throws When the parameter type is not int64
 @return A SInt64 value
 */
@property (readonly) SInt64 int64Value;

/*!
 @property uint8Value
 @abstract Returns the value of the data interpreted as a UInt8.
 @discussion The parameter type must be uint8, otherwise an exception will be thrown.
 @throws When the parameter type is not uint8
 @return A UInt8 value
 */
@property (readonly) UInt8 uint8Value;

/*!
 @property uint16Value
 @abstract Returns the value of the data interpreted as a UInt16.
 @discussion The parameter type must be uint16, otherwise an exception will be thrown.
 @throws When the parameter type is not uint16
 @return A UInt16 value
 */
@property (readonly) UInt16 uint16Value;

/*!
 @property uint32Value
 @abstract Returns the value of the data interpreted as a UInt32.
 @discussion The parameter type must be uint32, otherwise an exception will be thrown.
 @throws When the parameter type is not uint32
 @return A UInt32 value
 */
@property (readonly) UInt32 uint32Value;

/*!
 @property uint64Value
 @abstract Returns the value of the data interpreted as a UInt64.
 @discussion The parameter type must be uint64, otherwise an exception will be thrown.
 @throws When the parameter type is not uint64
 @return A UInt64 value
 */
@property (readonly) UInt64 uint64Value;

/*!
 @property enumValue
 @abstract Returns the value of the data interpreted as a enum (UInt8).
 @discussion The parameter type must be enum, otherwise an exception will be thrown.
 @throws When the parameter type is not enum
 @return A UInt8 value
 */
@property (readonly) UInt8 enumValue;

/*!
 @property boolValue
 @abstract Returns the value of the data interpreted as a BOOL.
 @discussion The parameter type must be bool, otherwise an exception will be thrown.
 @throws When the parameter type is not bool
 @return A BOOL value
 */
@property (readonly) BOOL boolValue;

/*!
 @property utf8Value
 @abstract Returns the value of the data interpreted as a string.
 @discussion The parameter type must be utf8, otherwise an exception will be thrown.
 @throws When the parameter type is not utf8
 @return A string value
 */
@property (readonly) NSString *utf8Value;

/*!
 @property blobValue
 @abstract Returns the data directly.
 @discussion The parameter type must be blob, otherwise an exception will be thrown.
 @throws When the parameter type is not blob
 @return The parameter data
 */
@property (readonly) NSData *blobValue;

/*!
 @property serialize:
 @abstract This method uses the ID and data properties to compose iAP parameter data suitable for sending over the wire
 @return iAP parameter data
 */
@property (readonly) NSData *serialize;

/*!
 @method -initWithID:type:data:name:
 @abstract Initializes a new iAP2 parameter
 @param ID   The parameter ID
 @param type The parameter type
 @param data The parameter data
 @param name The parameter name. Can be nil if the name is not available.
 @return The initialized parameter
 */
- (instancetype)initWithID:(UInt16)ID type:(UInt16)type data:(NSData *)data name:(NSString *)name;

/*!
 @method +int8ParameterWithID:int8:name:
 @abstract Creates and returns an int8 parameter from a SInt8 value
 @param ID   The parameter ID
 @param int8 A SInt8 value
 @param name The parameter name. Can be nil if the name is not available.
 @return The newly created and initialized int8 parameter
 */
+ (instancetype)int8ParameterWithID:(UInt16)ID
                               int8:(SInt8)int8
                               name:(NSString *)name;

/*!
 @method +int16ParameterWithID:int16:name:
 @abstract Creates and returns an int16 parameter from a SInt16 value
 @param ID    The parameter ID
 @param int16 A SInt16 value
 @param name  The parameter name. Can be nil if the name is not available.
 @return The newly created and initialized int16 parameter
 */
+ (instancetype)int16ParameterWithID:(UInt16)ID
                               int16:(SInt16)int16
                                name:(NSString *)name;

/*!
 @method +int32ParameterWithID:int32:name:
 @abstract Creates and returns an int32 parameter from a SInt32 value
 @param ID    The parameter ID
 @param int32 A SInt32 value
 @param name  The parameter name. Can be nil if the name is not available.
 @return The newly created and initialized int32 parameter
 */
+ (instancetype)int32ParameterWithID:(UInt16)ID
                               int32:(SInt32)int32
                                name:(NSString *)name;

/*!
 @method +int64ParameterWithID:int64:name:
 @abstract Creates and returns an int64 parameter from a SInt64 value
 @param ID    The parameter ID
 @param int64 A Int64 value
 @param name  The parameter name. Can be nil if the name is not available.
 @return The newly created and initialized int64 parameter
 */
+ (instancetype)int64ParameterWithID:(UInt16)ID
                               int64:(SInt64)int64
                                name:(NSString *)name;

/*!
 @method +uint8ParameterWithID:uint8:name:
 @abstract Creates and returns a uint8 parameter from a UInt8 value
 @param ID    The parameter ID
 @param uint8 A UInt8 value
 @param name  The parameter name. Can be nil if the name is not available.
 @return The newly created and initialized uint8 parameter
 */
+ (instancetype)uint8ParameterWithID:(UInt16)ID
                               uint8:(UInt8)uint8
                                name:(NSString *)name;

/*!
 @method +uint16ParameterWithID:uint16:name:
 @abstract Creates and returns a uint16 parameter from a UInt16 value
 @param ID     The parameter ID
 @param uint16 A UInt16 value
 @param name   The parameter name. Can be nil if the name is not available.
 @return The newly created and initialized uint16 parameter
 */
+ (instancetype)uint16ParameterWithID:(UInt16)ID
                               uint16:(UInt16)uint16
                                 name:(NSString *)name;

/*!
 @method +uint32ParameterWithID:uint32:name:
 @abstract Creates and returns a uint32 parameter from a UInt32 value
 @param ID     The parameter ID
 @param uint32 A UInt32 value
 @param name   The parameter name. Can be nil if the name is not available.
 @return The newly created and initialized uint32 parameter
 */
+ (instancetype)uint32ParameterWithID:(UInt16)ID
                               uint32:(UInt32)uint32
                                 name:(NSString *)name;

/*!
 @method +uint64ParameterWithID:uint64:name:
 @abstract Creates and returns a uint64 parameter from a UInt64 value
 @param ID     The parameter ID
 @param uint64 A UInt64 value
 @param name   The parameter name. Can be nil if the name is not available.
 @return The newly created and initialized uint64 parameter
 */
+ (instancetype)uint64ParameterWithID:(UInt16)ID
                               uint64:(UInt64)uint64
                                 name:(NSString *)name;

/*!
 @method +boolParameterWithID:bool:name:
 @abstract Creates and returns a bool parameter from a BOOL value
 @param ID      The parameter ID
 @param boolean A BOOL value
 @param name    The parameter name. Can be nil if the name is not available.
 @return The newly created and initialized bool parameter
 */
+ (instancetype)boolParameterWithID:(UInt16)ID
                               bool:(BOOL)boolean
                               name:(NSString *)name;

/*!
 @method +utf8ParameterWithID:string:name:
 @abstract Creates and returns a utf8 parameter from a string value
 @param ID     The parameter ID
 @param string A string value
 @param name   The parameter name. Can be nil if the name is not available.
 @return The newly created and initialized utf8 parameter
 */
+ (instancetype)utf8ParameterWithID:(UInt16)ID
                             string:(NSString *)string
                               name:(NSString *)name;

/*!
 @method +blobParameterWithID:data:name:
 @abstract Creates and returns a blob parameter from a string value
 @param ID   The parameter ID
 @param data A data value
 @param name The parameter name. Can be nil if the name is not available.
 @return The newly created and initialized blob parameter
 */
+ (instancetype)blobParameterWithID:(UInt16)ID
                               data:(NSData *)data
                               name:(NSString *)name;

/*!
 @method +noneParameterWithID:name:
 @abstract Creates and returns a none parameter
 @param ID   The parameter ID
 @param name The parameter name. Can be nil if the name is not available.
 @return The newly created and initialized none parameter
 */
+ (instancetype)noneParameterWithID:(UInt16)ID
                               name:(NSString *)name;

@end
