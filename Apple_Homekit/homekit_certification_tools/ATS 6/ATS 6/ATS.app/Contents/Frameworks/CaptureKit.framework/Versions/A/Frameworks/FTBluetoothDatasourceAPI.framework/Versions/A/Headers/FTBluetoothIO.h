//
//  FTBluetoothIO.h
//  FTBluetoothDatasourceAPI
//
//  Created by Michael Lance on 8/16/11.
//  Copyright 2011 Frontline Test Equipment, Inc. All rights reserved.
//

#pragma once

#import <Cocoa/Cocoa.h>
#import <CoreFoundation/CoreFoundation.h>
#import <Foundation/Foundation.h>
#import "FTErrorCodes.h"
#import "FTBdAddress.h"
#import "FTLinkKey.h"
#import "FTPinCode.h"
#import "FTEventDataDeque.h"

#define PASSKEY_CHARACTERS 6
#define OOB_TK_CHARACTERS 32

/**
 An enumeration of ComProbe types.
 */
typedef enum ComProbeType
{
  CPT_NONE= -1,
  CPT_BLUETOOTH_EDR,
  CPT_BPA_LOW_ENERGY,
  CPT_BPA_100,
  CPT_BPA_500,
  CPT_BPA_600
} ComProbeType;

/**
 An enumeration of pairing modes.
 */
typedef enum PairingMode
{
  PAIRING_NONE, /**< No pairing. */
  PAIRING_LEGACY, /**< Legacy pairing - can use PIN code or link key. */
  PAIRING_SSP, /**< Secure Simple Pairing - generates a link key for use in later connections. */
} PairingMode;

/**
 An enumeration of capture modes.
 */
typedef enum CaptureMode
{
  CM_CLASSIC_ONLY, /**< Capture only Classic Bluetooth data. */
  CM_LOW_ENERGY_ONLY, /**< Capture only low energy Bluetooth data. */
  CM_BOTH /**< Capture Classic and low energy Bluetooth data. */
} CaptureMode;

/**
 An enumeration of low energy sync modes.
 */
typedef enum LeSyncMode
{
  LSM_UNKNOWN, /** Unknown synchronization mode. */
  LSM_DEFAULT, /**< Synchronize with first master or a Classic device, depending on Capture Mode. */
  LSM_ADVERTISER_ADDRESS, /**< Synchronize with the specified advertiser address. */
  LSM_INITIATOR_ADDRESS /**< Synchronize with the specified initiator address. */
} LeSyncMode;

/**
 An enumeration of low energy address types.
 */
typedef enum LeAddressType
{
  LAT_UNKNOWN, /** Unknown address type. */
  LAT_INITIATOR, /**< Device represents an initiator. */
  LAT_ADVERTISER /**< Device represents an advertiser. */
} LeAddressType;

/**
 An enumeration of low energy pairing data types. The values
 represent required string lenghts for each type.
 */
typedef enum LePairingDataType
{
  LPDT_UNKNOWN, /** Unknown low energy pairing data type. */
  LPDT_DEC_PIN, /** Low energy pairing data represent a decimal PIN. */
  LPDT_HEX_PIN, /** Low energy pairing data represent a hexadecimal PIN. */
  LPDT_OOB_TK /** Low energy pairing date represent a temporary key. */
} LePairingDataType;

/**
 An enumeration of low energy pairing advertising channels.
 */
typedef enum LeAdvertisingChannel
{
  LE_ADV_37= 37, /** Channel 37. */
  LE_ADV_38= 38, /** Channel 38. */
  LE_ADV_39= 39 /** Channel 39. */
} LeAdvertisingChannel;

/**
 @file FTBluetoothIO.h
 */
/**
 FTBluetoothCaptureSessionConfiguration
 
 A class that represents a capture session configuration.
 */
@interface FTBluetoothCaptureSessionConfiguration : NSObject
{
@private
  PairingMode pairingMode;  // Mode selected for pairing.
                            // PAIRING_NONE - Pairing with no authentication.
                            // PAIRING_LEGACY - Can use PIN Code or link key.
                            // PAIRING_SSP - Must use link key.
  BOOL legacyUsesLinkKey;  // User has selected link key for legacy pairing.

  FTLinkKey* linkKey;  // Link key for paired devices.
  
  FTPinCode* pinCode;  // Depends on encryption method:
                       // PIN_None - Empty
                       // PIN_ASCII - 16 character ASCII string.
                       // PIN_Hex - 32 digit hex number.
        
  FTBdAddress* startSniffingMasterAddress; // Master bdaddr - default empty.
  
  FTBdAddress* startSniffingSlaveAddress;  // Slave bdaddr - default empty.
  
  // Sniffer Features
  BOOL filterIdPackets;  // Filter out all ID packets. Default FALSE.
  
  BOOL filterOutNullsPolls;  // Default TRUE.
  
  BOOL filterOutSco;  // Default FALSE.
  
  // low energy parameters.
  BOOL filterOutAdv;  // Filter out advertising packets for non-configured devices.
  LeSyncMode leSyncMode;
  LeAddressType leAddressType;
  FTBdAddress* leDefaultAddress;
  FTBdAddress* leAdvertiserAddress;
  FTBdAddress* leInitiatorAddress;
  BOOL ltkSet;
  NSMutableData* longTermKey;
  BOOL userLtkSet;
  NSMutableData* userLongTermKey;
  LePairingDataType lePairingDataType;
  BOOL tkSet;
  NSMutableData* temporaryKey;
}

/**
 Global mode of data capture.
 */
@property (readonly) CaptureMode captureMode;

/**
 Filter out ID packets.
 */
@property BOOL filterIdPackets;

/**
 Filter out Null and Poll packets.
 */
@property BOOL filterOutNullsPolls;

/**
 Filter out SCO and eSCO packets.
 */
@property BOOL filterOutSco;

/**
 The link key for SSP connections.
 */
@property (readonly) FTLinkKey* linkKey;

/**
 The master device-under-test BD_ADDR.
 */
@property (readonly) FTBdAddress* startSniffingMasterAddress;

/**
 The pairing mode.
 */
@property (readonly) PairingMode pairingMode;

/**
 Flag set TRUE when legacy pairing uses link key instead of PIN code.
 */
@property BOOL legacyUsesLinkKey;

/**
 Legacy pairing PIN code.
 */
@property (readonly) FTPinCode* pinCode;

/**
 The slave device-under-test BD_ADDR.
 */
@property (readonly) FTBdAddress* startSniffingSlaveAddress;

/**
 Filter out advertising packets for non-configured low energy devices.
 */
@property BOOL filterOutAdv;

/**
 The low energy syncronization mode.
 */
@property (readonly) LeSyncMode leSyncMode;

/**
 The low energy device role.
 */
@property (readonly) LeAddressType leAddressType;

/**
 The low energy device-under-test BD_ADDR for LeSyncMode LSM_DEFAULT.
 */
@property (readonly) FTBdAddress* leDefaultAddress;

/**
 The low energy advertiser device-under-test BD_ADDR for LeSyncMode LSM_ADVERTISER_ADDRESS.
 */
@property (readonly) FTBdAddress* leAdvertiserAddress;

/**
 The low energy initiator device-under-test BD_ADDR for LeSyncMode LSM_INITIATOR_ADDRESS.
 */
@property (readonly) FTBdAddress* leInitiatorAddress;

/**
 The long term key has been set.
 */
@property (readonly) BOOL ltkSet;

/**
 The long term key.
 */
@property (readonly) NSData* longTermKey;

/**
 The user long term key has been set.
 */
@property (readonly) BOOL userLtkSet;

/**
 The user-entered long term key.
 */
@property (readonly) NSData* userLongTermKey;

/**
 The type of pairing data specified.
 */
@property (readonly) LePairingDataType lePairingDataType;

/**
 The temporary key has been set.
 */
@property (readonly) BOOL tkSet;

/**
 The temporary key.
 */
@property (readonly) NSData* temporaryKey;

/**
 Parse the input full string for the field value for the specfied field key.
 
 @param field A mutable string to receive the field value.
 @param fullString A string that may or may not contain the field key/value pair.
 @param fieldKey Value of the key for the field.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return TRUE if the field value is retrieved.
 */
+ (BOOL)field:(NSMutableString*)field fromString:(NSString*)fullString forKey:(NSString*)fieldKey error:(NSError**)errorPtr;

/**
 Is the specified capture mode valid.
 
 @param mode Input CaptureMode value - see @CaptureMode.
 @return TRUE if the specified mode is valid.
 */
+ (BOOL)validCaptureMode:(CaptureMode)mode;

/**
 Return a readable capture mode for an input capture mode.
 
 @param readableCaptureMode A mutable string to receive the readable mode or an error message - may be nil.
 @param modeString A mutable string to receive the mode - "CM_CLASSIC_ONLY", "CM_LOW_ENERGY_ONLY" or "CM_BOTH" - may be nil.
 @param mode Capture mode - @see CaptureMode.
 @return TRUE if the specified mode is valid.
 */
+ (BOOL)readableCaptureMode:(NSMutableString*)readableCaptureMode modeString:(NSMutableString*)modeString forMode:(CaptureMode)mode;

/**
 Return capture mode for an input readable capture mode.
 
 @param modePtr Pointer to a CaptureMode - @see CaptureMode.
 @param readableMode A string to representing a readable capture mode - "CM_CLASSIC_ONLY", "CM_LOW_ENERGY_ONLY" or "CM_BOTH"
 @return TRUE if the specified readable mode is valid.
 */
+ (BOOL)captureMode:(CaptureMode*)modePtr forReadableMode:(NSString*)readableMode;

/**
 Initialize a default capture session configuration object.
 
 @return A capture session configuration.
 */
- (id)init;

/**
 Initialize a capture session configuration object with another capture session configuration object.
 
 @return A capture session configuration initialized with the specified capture session configuration or nil if the supplied object is of the wrong type.
 */
- (id)initWithCaptureSessionConfiguration:(FTBluetoothCaptureSessionConfiguration*)configuration;

/**
 Deallocate a capture session configuration.
 */
- (void)dealloc;

/**
 Get a capture session configuration as a string in the format "key:value|key:value...".
 
 @param keysAndValues A mutable string to hold the capture session constituent data.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return TRUE if operation succeeded.
 */
- (BOOL)getAsKeyValueString:(NSMutableString*)keysAndValues error:(NSError**)errorPtr;

/**
 Set a capture session configuration from a string in the format "key:value|key:value...".
 
 @param keysAndValues A string that holds the capture session constituent data.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return TRUE if operation succeeded.
 */
- (BOOL)setFromKeyValueString:(NSString*)keysAndValues error:(NSError**)errorPtr;

/**
 Set the pairing mode if the specified value is valid.
 
 @param aPairingMode The specified pairing mode.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return TRUE if operation succeeded.
 */

- (BOOL)setAndValidatePairingMode:(PairingMode)aPairingMode error:(NSError**)errorPtr;

/**
 Set the capture mode if the specified value is valid. Capture mode set to Classic Only if specified value is not valid.
 
 @param mode The specified capture mode.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return TRUE if operation succeeded.
 */
- (BOOL)setAndValidateCaptureMode:(CaptureMode)mode error:(NSError**)errorPtr;

/**
 Does the capture mode support Classic Bluetooth data capture.
 
 @return TRUE if capture mode is CM_CLASSIC_ONLY or CM_BOTH.
 */
- (BOOL)captureModeSupportsClassic;

/**
 Does the capture mode support low energy Bluetooth data capture.
 
 @return TRUE if capture mode is CM_LOW_ENERGY_ONLY or CM_BOTH.
 */
- (BOOL)captureModeSupportsLowEnergy;

/**
 Set the low energy sync mode if the specified value is valid.
 
 @param syncMode The specified low energy sync mode.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return TRUE if operation succeeded.
 */
- (BOOL)setAndValidateLeSyncMode:(LeSyncMode)syncMode error:(NSError**)errorPtr;

/**
 Set the low energy pairing data type if the specified value is valid.
 
 @param pairingType The specified low energy pairing data type.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return TRUE if operation succeeded.
 */
- (BOOL)setAndValidateLePairingDataType:(LePairingDataType)pairingType error:(NSError**)errorPtr;

/**
 Set the low energy address role if the specified value is valid.
 
 @param role The specified low energy address role.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return TRUE if operation succeeded.
 */
- (BOOL)setAndValidateLeAddressRole:(LeAddressType)role error:(NSError**)errorPtr;

/**
 Set the low energy advertiser device address.
 
 @param address The specified low energy device BD_ADDR.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return TRUE if operation succeeded.
 */
- (BOOL)setLowEnergyAdvertiserAddress:(FTBdAddress*)address error:(NSError**)errorPtr;

/**
 Set the low energy initiator device address.
 
 @param address The specified low energy device BD_ADDR.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return TRUE if operation succeeded.
 */
- (BOOL)setLowEnergyInitiatorAddress:(FTBdAddress*)address error:(NSError**)errorPtr;

/**
 Set the long term key.
 
 @param ltk Long term key data - must be 16 bytes, not all zero, for long term key to be marked as "set"
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return TRUE if operation succeeded.
 */
- (BOOL)setLongTermKey:(NSData*)ltk error:(NSError**)errorPtr;

/**
 Set the user-entered long term key.
 
 @param ltk User-entered long term key data - must be 16 bytes, not all zero, for long term key to be marked as "set"
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return TRUE if operation succeeded.
 */
- (BOOL)setUserLongTermKey:(NSData*)ltk error:(NSError**)errorPtr;

/**
 Set the user-entered long term key from string.
 
 @param ltk User-entered long term key string - must be 32 hexadecimal characters, not all zero, for long term key to be marked as "set"
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return TRUE if operation succeeded.
 */
- (BOOL)setUserLongTermKeyString:(NSString*)ltk error:(NSError**)errorPtr;

/**
 Set the temporary key and pairing data type.
 
 @param tk Temporary key data - must be 6 or 16 bytes, depending on pairing data type, not all zero, for temporary key to be marked as "set"
 @param pairingDataType Pairing data type - @see LePairingDataType.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return TRUE if operation succeeded.
 */
- (BOOL)setTemporaryKey:(NSData*)tk ofType:(LePairingDataType)pairingDataType error:(NSError**)errorPtr;

/**
 Set the temporary key from string for the specified pairing data type.
 
 @param tk Temporary key data - must be 6 decimal or hexadecimal, or 32 hexadecimal, characters, depending on pairing data type, not all zero, for temporary key to be marked as "set"
 @param pairingDataType Pairing data type - @see LePairingDataType.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return TRUE if operation succeeded.
 */
- (BOOL)setTemporaryKeyOfType:(LePairingDataType)pairingDataType fromString:(NSString*)tk error:(NSError**)errorPtr;
@end

#define NO_COMMAND 0xff

/**  
 A class that represents an inquiry result.
 */
@interface FTInquiryResult : NSObject

/**
 The type.
 */
@property (readonly) Byte type;

/**
 The BD_ADDR.
 */
@property (readonly) FTBdAddress* bdAddr;

/**
 The page-scan repetition mode.
 */
@property (readonly) Byte pageScanRepetitionMode;

/**
 The class of device.
 */
@property (readonly) UInt32 classOfDevice;

/**
 The clock offset.
 */
@property (readonly) UInt16 clockOffset;

/**
 The RSSI value.
 */
@property (readonly) SInt8 rssi;

/**
 The extended inquiry response (EIR) data.
 */
@property (readonly) NSData* eirData;

/**
 The device name.
 */
@property (retain) NSMutableString* deviceName;

/**
 Get the name specified in the EIR data.
 
 @param localName Mutable string to hold returned name.
 @param theEirData Array of EIR data.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return TRUE if operation succeeded.
 */
+ (BOOL)getLocalName:(NSMutableString*)localName fromEirData:(NSData*)theEirData error:(NSError**)errorPtr;

/**
 Initialize an inquiry result of a given type with a BD_ADDR, page-scan repetition mode, class of device, clock offset, RSSI and EIR data.
 
 @param aType Type of inquiry response.
 @param anAddress BD_ADDR to set.
 @param pageScanRepetitionMode Page-scan repetition mode to set.
 @param aClass Class of device to set.
 @param offset Clock offset to set.
 @param theRssi RSSI to set.
 @param data EIR data to set.
 @return An initialized inquiry response or nil if the specified parameters are not valid.
 */
- (id)initWith:(Byte)aType bluetoothAddress:(FTBdAddress*)anAddress pageScanRepetitionMode:(Byte)aMode classOfDevice:(UInt32)aClass clockOffset:(UInt16)offset rssi:(SInt8)theRssi eirData:(NSData*)data;

/**
 Initialize an inquiry result from a string of the format "key:value|key:value..."
 
 @param inquiryResultString String representation of inquiry response.
 @return An inquiry result with values set from the string.
 */
- (id)initWithSerializedString:(NSString*)inquiryResultString;

/**
 Initialize an inquiry result from a dictionary of key/value pairs.
 
 @param inquiryResultKeysValues Dictionary of key/value pairs that represent an inquiry response.
 @return An inquiry result with values set from the dictionary.
 */
- (id)initWithKeysAndValues:(NSDictionary*)inquiryResultKeysValues;

/**
 Initialize an inquiry result with the specified BD_ADDR and name.
 
 @param bdAddress BD_ADDR to set.
 @param name Name to set.
 @return An inquiry result with the specified values.
 */
- (id)initWithAddress:(NSString*)bdAddress andName:(NSString*)name;

/**
 Initialize an inquiry result with another inquiry result.
 
 @param inquiryResult Inquiry result to assign members from.
 @return An inquiry result initialized from the specifed inquiry result.
*/ 
- (id)initWithInquiryResult:(FTInquiryResult*)inquiryResult;

/**
 Deallocate an inquiry result.
 */
- (void)dealloc;

/**
 Get a description of the inquiry result.
 
 @param description Mutable string to hold returned description.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return TRUE if operation succeeded.
 */
- (BOOL)getDescription:(NSMutableString*)description error:(NSError**)errorPtr;

/**
 Get an inquiry result as a key/value string in the format "key:value|key:value..."
 
 @param keysAndValues Mutable string to hold returned key/value string.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return TRUE if operation succeeded.
 */
- (BOOL)getAsKeyValueString:(NSMutableString*)keysAndValues error:(NSError**)errorPtr;

/**
 Get an inquiry result as a key/value collection."
 
 @param keysAndValues Mutable dictionary to hold returned key/value pairs.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return TRUE if operation succeeded.
 */
- (BOOL)getAsKeysAndValues:(NSMutableDictionary*)keysAndValues error:(NSError**)errorPtr;
@end

