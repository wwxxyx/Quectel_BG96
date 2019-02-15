//
//  FTBdAddress.h
//  FTBluetoothDatasourceAPI
//
//  Created by Michael Lance on 11/4/11.
//  Copyright 2011 Frontline Test Equipment, Inc. All rights reserved.
//

#pragma once

/**
 @file FTBdAddress.h
 */

#define BD_ADDR_SIZE 6

/**
 An enumeration of address types.
 */
typedef enum BDAddrTypeId
{
  BDAddrTypePublic, /**< Public address. */
  BDAddrTypeRandom, /**< Random address. */
} BDAddrTypeId;

/**
 FTBdAddress
 
 A class that represents a BD_ADDR for Bluetooth devices.
 */
@interface FTBdAddress : NSObject <NSCopying>

/**
 Has the BD_ADDR been set.
 */
@property (readonly) BOOL bdAddrSet;

/**
 The BD_ADDR data.
 */
@property (readonly) NSData* bdAddressData;

/**
 The BD_ADDR type.
 */
@property (readonly) BDAddrTypeId addressType;

/**
 Indicator of how the address was initialized - TRUE if a reverse initializer was used.
 */
@property (readonly) BOOL reverseInitialized;

/**
 Initialize an empty BD_ADDR.
 
 @return An initialized empty BD_ADDR.
 */
- (id)init;

/**
 Initialize a BD_ADDR with another BD_ADDR.
 
 @param bdAddress BD_ADDR to assign members from.
 @return A BD_ADDR initialized with the specified BD_ADDR.
 */
- (id)initWithAddress:(FTBdAddress*)bdAddress;

/**
 Initialize a BD_ADDR with an array of 6 Byte.
 
 @param bdAddress BD_ADDR to assign members from.
 @return A BD_ADDR initialized with the specified BD_ADDR.
 */
- (id)initWithAddressData:(NSData*)bdAddress;

/**
 Initialize a BD_ADDR with an array of 6 Byte but reverse byte order.
 
 @param bdAddress BD_ADDR to assign members from.
 @return A BD_ADDR initialized with the specified BD_ADDR.
 */
- (id)initWithAddressDataReverse:(NSData*)bdAddress;

/**
 Initialize a BD_ADDR with a Byte pointer with length of 6.
 
 @param bdAddress BD_ADDR to assign members from.
 @return A BD_ADDR initialized with the specified BD_ADDR.
 */
- (id)initWithAddressBytes:(const Byte*)bdAddress ofLength:(NSUInteger)length;

/**
 Initialize a BD_ADDR with a Byte pointer with length of 6 but reverse byte order.
 
 @param bdAddress BD_ADDR to assign members from.
 @return A BD_ADDR initialized with the specified BD_ADDR.
 */
- (id)initWithAddressBytesReverse:(const Byte*)bdAddress ofLength:(NSUInteger)length;

/**
 Initialize a BD_ADDR with a uin64_t.
 
 @param bdAddress BD_ADDR to assign members from.
 @return A BD_ADDR initialized with the specified BD_ADDR.
 */
- (id)initWithAddressValue:(uint64_t)bdAddress;

/**
 Initialize a BD_ADDR from a string.
 
 @param bdAddress BD_ADDR to assign members from.
 @param type Type of address - @see BDAddrTypeId.
 @return A BD_ADDR initialized with the specified BD_ADDR.
 */
- (id)initWithAddressString:(NSString*)bdAddress andType:(BDAddrTypeId)type;

/**
 Initialize a BD_ADDR with an array of 6 Byte.
 
 @param bdAddress BD_ADDR to assign members from.
 @param type Type of address - @see BDAddrTypeId.
 @return A BD_ADDR initialized with the specified BD_ADDR.
 */
- (id)initWithAddressData:(NSData*)bdAddress andType:(BDAddrTypeId)type;

/**
 Initialize a BD_ADDR with an array of 6 Byte but reverse byte order.
 
 @param bdAddress BD_ADDR to assign members from.
 @param type Type of address - @see BDAddrTypeId.
 @return A BD_ADDR initialized with the specified BD_ADDR.
 */
- (id)initWithAddressDataReverse:(NSData*)bdAddress andType:(BDAddrTypeId)type;

/**
 Initialize a BD_ADDR with a Byte pointer with length of 6.
 
 @param bdAddress BD_ADDR to assign members from.
 @param length Length of the Byte array.
 @param type Type of address - @see BDAddrTypeId.
 @return A BD_ADDR initialized with the specified BD_ADDR.
 */
- (id)initWithAddressBytes:(const Byte*)bdAddress ofLength:(NSUInteger)length andType:(BDAddrTypeId)type;

/**
 Initialize a BD_ADDR with a Byte pointer with length of 6 but reverse byte order.
 
 @param bdAddress BD_ADDR to assign members from.
 @param length Length of the Byte array.
 @param type Type of address - @see BDAddrTypeId.
 @return A BD_ADDR initialized with the specified BD_ADDR in reverse.
 */
- (id)initWithAddressBytesReverse:(const Byte*)bdAddress ofLength:(NSUInteger)length andType:(BDAddrTypeId)type;

/**
 Initialize a BD_ADDR with a uin64_t.
 
 @param bdAddress BD_ADDR to assign members from.
 @param type Type of address - @see BDAddrTypeId.
 @return A BD_ADDR initialized with the specified BD_ADDR.
 */
- (id)initWithAddressValue:(uint64_t)bdAddress andType:(BDAddrTypeId)type;

/**
 Copy a BD_ADDR.
 
 @param zone The zone identifies an area of memory from which to allocate for the new instance. If zone is NULL, the new instance is allocated from the default zone, which is returned from the function NSDefaultMallocZone.
 @return A copy of the initiating BD_ADDR.
 */
- (id)copyWithZone:(NSZone*)zone;

/**
 Deallocate a BD_ADDR.
 */
- (void)dealloc;

/**
 Set a BD_ADDR from a BD_ADDR.
 
 @param newAddress An FTBdAddress.
 @param reverseByteOrder An indication of whether the Endianness of the input should be reversed.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return A status code indicating the result of the operation. The possible return codes are: ERR_OK if the operation succeeds; ERR_INVALID_ARGUMENT if newAddresss is not 6 bytes; @see Status code documentation.
 */
- (UInt16)setAddressFromAddress:(FTBdAddress*)newAddress reverseByteOrder:(BOOL)reversebyteOrder error:(NSError**)errorPtr;

/**
 Set a BD_ADDR from an array of Byte.
 
 @param newAddress An array of Byte that represents the new contents of the BD_ADDR.
 @param reverseByteOrder An indication of whether the Endianness of the input should be reversed.
 @param type The address type - @see BDAddrTypeId.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return A status code indicating the result of the operation. The possible return codes are: ERR_OK if the operation succeeds; ERR_INVALID_ARGUMENT if newAddresss is not 6 bytes; @see Status code documentation.
 */
- (UInt16)setAddressFromData:(NSData*)newAddress reverseByteOrder:(BOOL)reversebyteOrder type:(BDAddrTypeId)type error:(NSError**)errorPtr;

/**
 Set a BD_ADDR from a pointer to Byte.
 
 @param newAddress An pointer to Byte that represents the new contents of the BD_ADDR.
 @param length Length of pointer to Byte.
 @param reverseByteOrder An indication of whether the Endianness of the input should be reversed.
 @param type The address type - @see BDAddrTypeId.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return A status code indicating the result of the operation. The possible return codes are: ERR_OK if the operation succeeds; ERR_INVALID_ARGUMENT if newAddresss is not 6 bytes; @see Status code documentation.
 */
- (UInt16)setAddressFromBytes:(const Byte*)newAddress length:(NSUInteger)length reverseByteOrder:(BOOL)reverseByteOrder type:(BDAddrTypeId)type error:(NSError**)errorPtr;

/**
 Set a BD_ADDR from a string.
 
 @param newAddress A string representation of the BD_ADDR.
 @param type The address type - @see BDAddrTypeId.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return A status code indicating the result of the operation. The possible return codes are: ERR_OK if the operation succeeds; ERR_INVALID_LENGTH_ODD if newAddress is an odd number of nibbles; ERR_INVALID_CHARACTERS if newAddress contains non-hexadecimal characters; ERR_INVALID_LENGTH_TOO_SHORT or ERR_INVALID_LENGTH_TOO_LONG if newAddress is not 6 bytes (12 characters). @see Status code documentation.
 */
- (BOOL)setAddressFromString:(NSString*)newAddress type:(BDAddrTypeId)type error:(NSError**)errorPtr;

/**
 Set a BD_ADDR using the component fields.
 
 @param lap Lower address part.
 @param uap Upper address part.
 @param nap Non-significant address part.
 @param type The address type - @see BDAddrTypeId.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return TRUE if the address was set.
 */
- (BOOL)setAddressFromLap:(UInt32)lap uap:(UInt8)uap nap:(UInt16)nap type:(BDAddrTypeId)type error:(NSError**)errorPtr;

/**
 Set a BD_ADDR from a value.
 
 @param newAddress A 64-bit representation of the BD_ADDR.
 @param type The address type - @see BDAddrTypeId.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return TRUE if the address was set.
 */
- (BOOL)setAddressFromValue:(uint64_t)newAddress type:(BDAddrTypeId)type error:(NSError**)errorPtr;

/**
 Get a BD_ADDR as the component fields.
 
 @param pLap Pointer to hold LAP return value.
 @param pUap Pointer to hold UAP return value.
 @param pNap Pointer to hold NAP return value.
 */
- (void)getLap:(UInt32*)pLap uap:(UInt8*)pUap nap:(UInt16*)pNap;

/**
 Get a BD_ADDR as a representative string.
 
 @param theAddress Mutable string to hold return value.
 @param statusPtr Pointer to hold status of returned BD_ADDR.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return TRUE if operation succeeded.
 */
- (BOOL)getAddressAsString:(NSMutableString*)theAddress status:(UInt16*)statusPtr error:(NSError**)errorPtr;

/**
 Get a BD_ADDR as a representative Byte array.
 
 @param addressData Mutable data to hold return value.
 @param reverse Set TRUE to reverse output byte order.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return TRUE if operation succeeded.
 */
- (BOOL)getAddressData:(NSMutableData*)addressData reverse:(BOOL)reverse error:(NSError**)errorPtr;

/**
 Perform a byte-wise comparison of BD_ADDRs.
 
 @param compareAddress BD_ADDR to compare.
 @return TRUE if addresses equal.
 */
- (BOOL)isAddressEqual:(FTBdAddress*)compareAddress;

@end
