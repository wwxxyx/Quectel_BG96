//
//  FTPinCode.h
//  FTBluetoothDatasourceAPI
//
//  Created by Michael Lance on 2/13/12.
//  Copyright 2012 Frontline Test Equipment, Inc. All rights reserved.
//

/**
 @file FTPinCode.h
*/
/**
 An enumeration of PIN code types.
*/
typedef enum PinCodeType
{
  PIN_NONE, /**< No PIN code. */
  PIN_ASCII, /**< ASCII PIN code. */
  PIN_HEX /**< Hex PIN code. */
} PinCodeType;

/**
 FTPinCode
 
 A class that represents a PIN code for devices using Legacy Pairing.
 */
@interface FTPinCode : NSObject <NSCopying>

/**
 The length of the PIN code.
 */
@property (readonly) UInt16 pinCodeLength;

/**
 The PIN code type.
 */
@property PinCodeType pinCodeType;


/**
 The data that make up the PIN code.
 */
@property (readonly) NSData* pinCodeData;

/**
 Initialize an empty PIN code.
 
 @return An initialized empty PIN code.
 */
- (id)init;

/**
 Initialize an PIN code with a valid PIN code.
 
 @return An initialized PIN code.
 */
- (id)initWithPinCode:(FTPinCode*)pinCode;

/**
 Copy a PIN code.
 
 @param zone The zone identifies an area of memory from which to allocate for the new instance. If zone is NULL, the new instance is allocated from the default zone, which is returned from the function NSDefaultMallocZone.
 @return A copy of the initiating PIN code.
 */
- (id)copyWithZone:(NSZone*)zone;

/**
 Deallocate a PIN code.
 */
- (void)dealloc;

/**
 Set a PIN code from a string.
 
 @param newPinCode A string that represents the new contents of the link key.
 @param thePinCodeType The format of the PIN code string - hexadecimal or ASCII.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return A status code indicating the result of the operation. The possible return codes are: ERR_OK if the operation succeeds; ERR_INVALID_PINCODE_LENGTH if newPinCode too long; ERR_INVALID_PINCODE_CONTENT if ASCII string encoding of newPinCode fails. @see Status code documentation.
 */
- (UInt16)setFromString:(NSString*)newPinCode hexOrAscii:(PinCodeType)thePinCodeType error:(NSError**)errorPtr;

/**
 Get a PIN code as a string.
 
 @param pinCodeString A mutable string to hold the returned PIN code.
 @param pinCodeTypeString A mutable string to hold the type of the returned PIN code.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return TRUE if the retrieval succeeded.
 */
- (BOOL)getPinCodeAsString:(NSMutableString*)pinCodeString type:(NSMutableString*)pinCodeTypeString error:(NSError**)errorPtr;

/**
 Clear the contents of a PIN code.
 */
- (void)clear;

/**
 Test the existence of contents of a PIN code.
 
 @return TRUE if the PIN code is empty.
 */
- (BOOL)isEmpty;
@end
