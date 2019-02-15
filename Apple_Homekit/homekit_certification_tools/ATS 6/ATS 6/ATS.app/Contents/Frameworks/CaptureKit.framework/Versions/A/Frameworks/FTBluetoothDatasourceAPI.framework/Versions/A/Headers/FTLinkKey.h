//
//  FTLinkKey.h
//  FTBluetoothDatasourceAPI
//
//  Created by Michael Lance on 2/6/12.
//  Copyright 2012 - 2016 Frontline Test Equipment, Inc. All rights reserved.
//  Copyright 2017 Teledyne LeCroy, Inc. All rights reserved.
//

#pragma once

/**
 @file FTLinkKey.h
*/
/**
 FTLinkKey
 
 A class that represents a link key for devices using Secure Simple Pairing.
 */
@interface FTLinkKey : NSObject <NSCopying>

/**
 The contents of the link key.
 */
@property (readonly) NSData* linkKey;

/**
 Initialize an empty link key.
 
 @return An initialized empty link key.
 */
- (id)init;

/**
 Initialize a link key with the specified link key.
 
 @return An initialized link key.
 */
- (id)initWithLinkKey:(FTLinkKey*)lk;

/**
 Copy a link key.
 
 @param zone The zone identifies an area of memory from which to allocate for the new instance. If zone is NULL, the new instance is allocated from the default zone, which is returned from the function NSDefaultMallocZone.
 @return A copy of the initiating link key.
 */
- (id)copyWithZone:(NSZone*)zone;

/**
 Deallocate a link key.
 */
- (void)dealloc;

/**
 Set a link key from a link key.
 
 @param newLinkKey A link key.
 @param reverseByteOrder An indication of whether the Endianness of the input should be reversed.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return A status code indicating the result of the operation. The possible return codes are: ERR_OK if the operation succeeds; ERR_INVALID_ARGUMENT if newLinkKey is nil; ERR_INVALID_LENGTH_ODD if newLinkKey is an odd number of nibbles; ERR_INVALID_CHARACTERS if newLinkKey contains non-hexadecimal characters; ERR_INVALID_LENGTH_TOO_SHORT or ERR_INVALID_LENGTH_TOO_LONG if newLinkKey is not 16 bytes (32 characters). @see Status code documentation.
 */
- (UInt16)setFromLinkKey:(FTLinkKey*)newLinkKey reverseByteOrder:(BOOL)reversebyteOrder error:(NSError**)errorPtr;

/**
 Set a link key from a string.
 
 @param newLinkKey A string that represents the new contents of the link key.
 @param reverseByteOrder An indication of whether the Endianness of the input should be reversed.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return A status code indicating the result of the operation. The possible return codes are: ERR_OK if the operation succeeds; ERR_INVALID_LENGTH_ODD if newLinkKey is an odd number of nibbles; ERR_INVALID_CHARACTERS if newLinkKey contains non-hexadecimal characters; ERR_INVALID_LENGTH_TOO_SHORT or ERR_INVALID_LENGTH_TOO_LONG if newLinkKey is not 16 bytes (32 characters). @see Status code documentation.
 */
- (UInt16)setFromString:(NSString*)newLinkKey reverseByteOrder:(BOOL)reversebyteOrder error:(NSError**)errorPtr;

/**
 Set a link key from an array of Byte.
 
 @param newLinkKey An array of Byte that represents the new contents of the link key.
 @param reverseByteOrder An indication of whether the Endianness of the input should be reversed.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return A status code indicating the result of the operation. The possible return codes are: ERR_OK if the operation succeeds; ERR_INVALID_LENGTH_ODD if newLinkKey is an odd number of nibbles; ERR_INVALID_CHARACTERS if newLinkKey contains non-hexadecimal characters; ERR_INVALID_LENGTH_TOO_SHORT or ERR_INVALID_LENGTH_TOO_LONG if newLinkKey is not 16 bytes (32 characters). @see Status code documentation.
 */
- (UInt16)setFromData:(NSData*)newLinkKey reverseByteOrder:(BOOL)reversebyteOrder error:(NSError**)errorPtr;

/**
 Set a link key from a Byte pointer.
 
 @param newLinkKey A pointer to Byte that represents the new contents of the link key.
 @param length Length of the link key - should be 16.
 @param reverseByteOrder An indication of whether the Endianness of the input should be reversed.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return A status code indicating the result of the operation. The possible return codes are: ERR_OK if the operation succeeds; ERR_INVALID_ARGUMENT inf newLinkKey is NULL; ERR_INVALID_LENGTH_ODD if newLinkKey is an odd number of nibbles; ERR_INVALID_CHARACTERS if newLinkKey contains non-hexadecimal characters; ERR_INVALID_LENGTH_TOO_SHORT or ERR_INVALID_LENGTH_TOO_LONG if newLinkKey is not 16 bytes (32 characters). @see Status code documentation.
 */
- (UInt16)setFromDataBytes:(const Byte*)newLinkKey ofLength:(NSUInteger)length reverseByteOrder:(BOOL)reversebyteOrder error:(NSError**)errorPtr;

/**
 Get a link key as a string.
 
 @param linkKeyString A mutable string to hold the returned link key.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return TRUE if the retrieval succeeded.
 */
- (BOOL)getLinkKeyAsString:(NSMutableString*)linkKeyString error:(NSError**)errorPtr;

/**
 Get a link key as a wrapped Byte buffer.
 
 @param linkKeyData A mutable data object to hold the returned link key.
 @param reverse Set TRUE to reverse output byte order.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return TRUE if the retrieval succeeded.
 */
- (BOOL)getData:(NSMutableData*)linkKeyData reverse:(BOOL)reverse error:(NSError**)errorPtr;

/**
 Clear the contents of a link key.
 */
- (void)clear;

/**
 Test the existence of contents of a link key.
 
 @return TRUE if the link key is empty.
 */
- (BOOL)isEmpty;
@end

/**
 FTLinkKeyPatterns
 
 A class that represents a utility function for generating link key patterns.
 */
@interface FTLinkKeyPatterns : NSObject

/**
 An enumeration of link key patterns.
 */typedef enum LinkKeyPattern
{
  LKP_ORIGINAL, /**< Original link key. */
  LKP_REVERSED,  /**< Reversed bytes. */
  LKP_MIRROR_WORDS,  /**< Reverse byte order for each pair of bytes. */
  LKP_MIRROR_WORDS_REVERSED, /**< Reverse byte order for each pair of bytes, then reverse overall byte order.*/
  LKP_MIRROR_DWORDS, /**< Reverse byte order for each group of 4 bytes. */
  LKP_MIRROR_DWORDS_REVERSED, /**< Reverse byte order for each group of 4 bytes, then reverse overall byte order. */
} LinkKeyPattern;

/**
 Generate a collection of link key patterns based on the input data.
 
 @param linkKeyData A data object that contains the proposed link key.
 @param patterns The collection of patterns where the key is LinkKeyPattern (@see LinkKeyPattern) and the value is a mutable dictionary that contains the key "linkkey" with a value representing the link key data for the pattern.
 @return TRUE if pattern generation succeeded.
 */
+ (BOOL)generateLinkKeyPatternsFor:(NSData*)linkKeyData patterns:(NSMutableDictionary*)patterns;

/**
 Generate a readable version of the specified pattern enum.
 @param readablePattern Mutable string to return readable pattern.
 @param linkKeyPattern Pattern to generate from.
 @return TRUE if valid link key pattern.
 */
+ (BOOL)readablePattern:(NSMutableString*)readablePattern forPattern:(LinkKeyPattern)linkKeyPattern;
@end
