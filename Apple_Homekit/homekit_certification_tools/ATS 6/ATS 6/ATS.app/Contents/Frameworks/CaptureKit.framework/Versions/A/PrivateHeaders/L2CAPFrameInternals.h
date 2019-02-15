//
//  L2CAPFrameInternals.h
//  Header for Bluetooth L2CAP Internals
//
//  Created by Edgard Lima on 08/13/18.
//  Copyright (c) 2018 Apple. All rights reserved.
//

#import <Foundation/Foundation.h>

#define kL2CAPMinimalPsmSize 2

#pragma pack(push,1)
#ifdef __LITTLE_ENDIAN__  // Usually bit and byte endieness are the same

typedef struct {
    /// The length of the payload (excluding this header)
    UInt16 length;
    /// See L2CAPChannelId.
    UInt16 channelId;
} RawL2CAPHeader;

/// Determine the maximum number of bytes that a ISO3309 number can use.
typedef UInt32 ISO3309ExtentionType;

@interface L2CAPFrameInternals : NSObject

/**
 Extract and return the value stored in ISO3309 extension format.
 The maximum number of bytes is defined by ISO3309ExtentionType type.
 
 @param valueFirstBytePtr the pointer to the byte array where the number is stored. Actually it is the pointer to the first less signinificant byte of the value into the byte array.
 @param errorDescription in case of and error extracting the value, a error description will be stored.
 @param valueSizeResult How many bytes was used by the stored/extracted value.
 
 @return the ISO3309 value stored/extracted from valueFirstBytePtr.
 
 @note If errorDescription is returned not nil, i.e. an error description has been inserted on it, the returned value and valueSizeResult must be ignored.
 */
+ (ISO3309ExtentionType)iso3309extensionFromFirstByte:(const BytePtr)valueFirstBytePtr errorDescriptionResult:(NSString **)errorDescription valueSize:(UInt16*)valueSizeResult;

@end

#else
#error "redefine RawL2CAPHeader stucture for big-endian"
#endif
#pragma pack(pop) 
