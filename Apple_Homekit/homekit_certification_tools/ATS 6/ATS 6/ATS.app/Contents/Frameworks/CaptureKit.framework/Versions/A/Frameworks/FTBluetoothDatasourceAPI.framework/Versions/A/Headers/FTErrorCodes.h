//
//  FTErrorCodes.h
//  FTBluetoothDatasourceAPI
//
//  Created by Michael Lance on 9/27/11.
//  Copyright 2011 Frontline Test Equipment, Inc. All rights reserved.
//

/**
 @file FTErrorCodes.h
 */

#ifndef BtComProbe_ErrorCodes_h
#define BtComProbe_ErrorCodes_h

/**
 An enumeration of error codes used by the Bluetooth Datasource API.
 */
typedef enum FtErrorCode
{
  ERR_OK, /**< OK. */
  ERR_EMPTY, /**< Input argument empty. */
  ERR_INVALID_POINTER, /**< Invalid pointer. */
  ERR_INVALID_ARGUMENT, /**< Invalid argument - NULL or empty. */
  ERR_ALLOCATION_FAILURE, /**< Memory allocation failure. */
  ERR_NOT_FOUND, /**< Not found. */
  ERR_INVALID_INTERFACE, /**< Interface is not valid. */
  ERR_READWRITE_NOT_RUNNING, /**< Read/Write threads not running.*/
  ERR_NO_COMMAND, /**< No command specified - NULL or empty. */
  ERR_INVALID_COMMAND, /**< Invalid command specified. */
  ERR_BCCMD_FAILURE, /**< BCCMD command status was not good. */
  ERR_NO_WRITE_FOR_READ, /**< Attempted to match an HCI event when there was no HCI command data. */
  ERR_HCI_COMMAND_EVENT_MISMATCH, /**< HCI command does not correspond to specified HCI event. */
  ERR_BCCMD_PAYLOAD_DESCRIPTOR_MISMATCH, /**< BCCMD command payload descriptor mismatch. */
  ERR_MISMATCHING_VENDOR_ID, /**< Device vendor ID does not match. */
  ERR_MISMATCHING_PRODUCT_ID, /**< Device product ID does not match. */
  ERR_NO_COMPROBES, /**< No known Bluetooth ComProbes. */
  ERR_NO_MATCHING_COMPROBE, /**< Specified serial number or identifier does not match a known Bluetooth ComProbe. */
  ERR_NOT_A_SELECTED_COMPROBE, /**< Specified serial number does not match a selected Bluetooth ComProbe. */
  ERR_INVALID_CHIP_INDEX, /**< Specified chip index is out of range for known Bluetooth ComProbes. */
  ERR_NO_COMPROBE_IN_USE, /**< There is currently no ComProbe. */
  ERR_SIZE_HIGH, /**< Size of resulting data set is too high. */
  ERR_NO_SERIAL_NUMBER, /**< No serial number specified - NULL or empty. */
  ERR_NO_ADDRESS, /**< No Bluetooth ComProbe address specified - NULL or empty. */
  ERR_NO_DATA, /**< No data in buffer. */
  ERR_NO_NAME, /**< No Bluetooth device name. */
  ERR_INVALID_DEVICE_ADDRESS, /**< No known Bluetooth ComProbe with specified address. */
  ERR_NO_SLAVE_ADDRESS, /**< No slave device address specified - NULL or empty. */
  ERR_NO_MASTER_ADDRESS, /**< No master device address specified - NULL or empty. */
  ERR_MASTER_EQUAL_SLAVE, /**< Master and slave device addresses are equal. */
  ERR_EMPTY_PINCODE, /**< No PIN code specified - NULL or empty. */
  ERR_INVALID_PINCODE_LENGTH, /**< ASCII PIN code cannot exceed 16 characters; hexadecimal PIN code cannot exceed 128 bits. */
  ERR_INVALID_PINCODE_CONTENT, /**< Hexadecimal PIN code can only contain hexadecimal characters. */
  ERR_INVALID_LINKKEY, /**< Specified link key is invalid. */
  ERR_INVALID_INQUIRY_RESULT_TYPE, /**< The specified inquiry result type is not valid. */
  ERR_INVALID_LENGTH, /**< The specified string length is not valid. */
  ERR_INVALID_LENGTH_ODD, /**< The specified string length is odd. */
  ERR_INVALID_LENGTH_TOO_SHORT, /**< The specified string is too short. */
  ERR_INVALID_LENGTH_TOO_LONG, /**< The specified string is too long. */
  ERR_INVALID_CHARACTERS, /**< Specified character or characters not allowed. */
  ERR_OPERATION_CANCELED, /**< The operation was canceled. */
  ERR_OPERATION_TIMED_OUT, /**< The operation timed out. */
  ERR_OPERATION_FAILED, /**< The operation failed. */
  ERR_COMPROBE_BUSY, /**< The ComProbe is in a blocking operation. */
  ERR_NO_COMPROBE_RESPONSE, /**< The ComProbe did not deliver the expected response. */
  ERR_NO_COMPROBE_RESPONSE_COMMAND_COMPLETE, /**< The ComProbe did not deliver the command-complete response. */
  ERR_NO_COMPROBE_RESPONSE_COMMAND_STATUS, /**< The ComProbe did not deliver the command-status response. */
  ERR_NO_COMPROBE_RESPONSE_INQUIRY, /**< The ComProbe did not deliver an inquiry response. */
  ERR_NO_EVENTS, /**< The receive buffer has no events. */
  ERR_HCI, /**< HCI error. */
  ERR_CANCEL_SNIFFING_FAILED, /**< Cancel sniffing operation failed. */
  ERR_DID_NOT_READ_ALL_CLOCKS, /**< Read microsecond clock operation failed for one or more ComProbe chips. */
  ERR_NO_FIRMWARE_DATA, /**< There is no firmware information for the current ComProbe. */
  ERR_START_CHIP_FIRMWARE_READ_FAILED, /**< Failed to start the operation to read ComProbe chip firmware version. */
  ERR_READ_CHIP_FIRMWARE_VERSION_FAILED, /**< Read local version failed for a ComProbe chip. */
  ERR_INVALID_CHIP_FIRMWARE_VERSION, /**< The version of the firmware on a ComProbe chip is invalid. */
  ERR_MISSING_CHIP_FIRMWARE_VERSION, /**< The version of the firmware on a ComProbe chip is unknown. */
  ERR_INVALID_VALUE, /**< The value of a member is invalid. */
  ERR_UNKNOWN, /**< Error unknown. */
  ERR_NOT_SET= 0xff /**< Error not set. */
} FtErrorCode;

#endif
