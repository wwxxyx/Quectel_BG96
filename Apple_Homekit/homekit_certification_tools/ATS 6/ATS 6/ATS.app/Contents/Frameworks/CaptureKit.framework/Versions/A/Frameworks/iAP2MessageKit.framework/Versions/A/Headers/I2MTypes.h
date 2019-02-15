//
//  I2MTypes.h
//  iAP2MessageKit
//
//  Created by Joe Basu on 3/20/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

typedef NS_ENUM(UInt8, I2MSpecialByte) {
    I2MSpecialByteStartOfMessage = 0x40,
};

typedef NS_ENUM(UInt8, I2MSource) {
    I2MSourceAccessory   = 0x00,
    I2MSourceAppleDevice = 0x01,
    I2MSourceUnknown     = 0xFF,
};

/** The display name for the accessory as the message source. */
extern NSString * const I2MSourceAccessoryName;

/** The display name for the Apple device as the message source. */
extern NSString * const I2MSourceAppleDeviceName;

/** The display name for any unknown message source. */
extern NSString * const I2MSourceUnknownName;

typedef NS_ENUM(UInt8, I2MParameterType) {
    I2MParameterTypeInt8 = 0x00,
    I2MParameterTypeInt16,
    I2MParameterTypeInt32,
    I2MParameterTypeInt64,
    I2MParameterTypeUInt8,
    I2MParameterTypeUInt16,
    I2MParameterTypeUInt32,
    I2MParameterTypeUInt64,
    I2MParameterTypeEnum,
    I2MParameterTypeEnumArray,
    I2MParameterTypeBool,
    I2MParameterTypeUTF8,
    I2MParameterTypeBlob,
    I2MParameterTypeNone,
    I2MParameterTypeGroup,
    I2MParameterTypeSecs16,
    I2MParameterTypeSecs32,
    I2MParameterTypeSecs64,
    I2MParameterTypeMSecs16,
    I2MParameterTypeMSecs32,
    I2MParameterTypeMSecs64,
    I2MParameterTypeUSecs16,
    I2MParameterTypeUSecs32,
    I2MParameterTypeUSecs64,
    I2MParameterTypeRat16,
    I2MParameterTypeRat32,
    I2MParameterTypeApex,
    I2MParameterTypeURat16,
    I2MParameterTypeURat32,
    I2MParameterTypeUApex,
    I2MParameterTypeInt8Array,
    I2MParameterTypeInt16Array,
    I2MParameterTypeInt32Array,
    I2MParameterTypeInt64Array,
    I2MParameterTypeUInt8Array,
    I2MParameterTypeUInt16Array,
    I2MParameterTypeUInt32Array,
    I2MParameterTypeUInt64Array,
    I2MParameterTypeRat16Array,
    I2MParameterTypeRat32Array,
    I2MParameterTypeApexArray,
    I2MParameterTypeURat16Array,
    I2MParameterTypeURat32Array,
    I2MParameterTypeUApexArray,
    I2MParameterTypeUnknown = 0xFF,
};

/** The display name of the int8 type. */
extern NSString * const I2MParameterTypeInt8Name;

/** The display name of the int16 type. */
extern NSString * const I2MParameterTypeInt16Name;

/** The display name of the int32 type. */
extern NSString * const I2MParameterTypeInt32Name;

/** The display name of the int64 type. */
extern NSString * const I2MParameterTypeInt64Name;

/** The display name of the uint8 type. */
extern NSString * const I2MParameterTypeUInt8Name;

/** The display name of the uint16 type. */
extern NSString * const I2MParameterTypeUInt16Name;

/** The display name of the uint32 type. */
extern NSString * const I2MParameterTypeUInt32Name;

/** The display name of the uint64 type. */
extern NSString * const I2MParameterTypeUInt64Name;

/** The display name of the enum type. */
extern NSString * const I2MParameterTypeEnumName;

/** The display name of the enum[] type. */
extern NSString * const I2MParameterTypeEnumArrayName;

/** The display name of the bool type. */
extern NSString * const I2MParameterTypeBoolName;

/** The display name of the utf8 type. */
extern NSString * const I2MParameterTypeUTF8Name;

/** The display name of the blob type. */
extern NSString * const I2MParameterTypeBlobName;

/** The display name of the none type. */
extern NSString * const I2MParameterTypeNoneName;

/** The display name of the group type. */
extern NSString * const I2MParameterTypeGroupName;

/** The display name of the secs16 type. */
extern NSString * const I2MParameterTypeSecs16Name;

/** The display name of the secs32 type. */
extern NSString * const I2MParameterTypeSecs32Name;

/** The display name of the secs64 type. */
extern NSString * const I2MParameterTypeSecs64Name;

/** The display name of the msecs16 type. */
extern NSString * const I2MParameterTypeMSecs16Name;

/** The display name of the msecs32 type. */
extern NSString * const I2MParameterTypeMSecs32Name;

/** The display name of the msecs64 type. */
extern NSString * const I2MParameterTypeMSecs64Name;

/** The display name of the usecs16 type. */
extern NSString * const I2MParameterTypeUSecs16Name;

/** The display name of the usecs32 type. */
extern NSString * const I2MParameterTypeUSecs32Name;

/** The display name of the usecs64 type. */
extern NSString * const I2MParameterTypeUSecs64Name;

/** The display name of the rat16 type. */
extern NSString * const I2MParameterTypeRat16Name;

/** The display name of the rat32 type. */
extern NSString * const I2MParameterTypeRat32Name;

/** The display name of the apex type. */
extern NSString * const I2MParameterTypeApexName;

/** The display name of the urat16 type. */
extern NSString * const I2MParameterTypeURat16Name;

/** The display name of the urat32 type. */
extern NSString * const I2MParameterTypeURat32Name;

/** The display name of the uapex type. */
extern NSString * const I2MParameterTypeUApexName;

/** The display name of the int8[] type. */
extern NSString * const I2MParameterTypeInt8ArrayName;

/** The display name of the int16[] type. */
extern NSString * const I2MParameterTypeInt16ArrayName;

/** The display name of the int32[] type. */
extern NSString * const I2MParameterTypeInt32ArrayName;

/** The display name of the int64[] type. */
extern NSString * const I2MParameterTypeInt64ArrayName;

/** The display name of the uint8[] type. */
extern NSString * const I2MParameterTypeUInt8ArrayName;

/** The display name of the uint16[] type. */
extern NSString * const I2MParameterTypeUInt16ArrayName;

/** The display name of the uint32[] type. */
extern NSString * const I2MParameterTypeUInt32ArrayName;

/** The display name of the uint64[] type. */
extern NSString * const I2MParameterTypeUInt64ArrayName;

/** The display name of the rat16[] type. */
extern NSString * const I2MParameterTypeRat16ArrayName;

/** The display name of the rat32[] type. */
extern NSString * const I2MParameterTypeRat32ArrayName;

/** The display name of the apex[] type. */
extern NSString * const I2MParameterTypeApexArrayName;

/** The display name of the urat16[] type. */
extern NSString * const I2MParameterTypeURat16ArrayName;

/** The display name of the urat32[] type. */
extern NSString * const I2MParameterTypeURat32ArrayName;

/** The display name of the uapex[] type. */
extern NSString * const I2MParameterTypeUApexArrayName;

/** The display name for any unknown type. */
extern NSString * const I2MParameterTypeUnknownName;

typedef NS_ENUM(UInt8, I2MCountExpression) {
    I2MCountExpression0Plus   = 0x00,
    I2MCountExpression1       = 0x01,
    I2MCountExpression1Plus   = 0x02,
    I2MCountExpression0Or1    = 0x03,
    I2MCountExpressionUnknown = 0xFF,
};

/** The display string for the zero-or-more count indicator. */
extern NSString * const I2MCountExpression0PlusName;

/** The display string for the exactly-one count indicator. */
extern NSString * const I2MCountExpression1Name;

/** The display string for the one-or-more count indicator. */
extern NSString * const I2MCountExpression1PlusName;

/** The display string for the zero-or-one count indicator. */
extern NSString * const I2MCountExpression0Or1Name;

/** The display string for the unknown count indicator. */
extern NSString * const I2MCountExpressionUnknownName;
