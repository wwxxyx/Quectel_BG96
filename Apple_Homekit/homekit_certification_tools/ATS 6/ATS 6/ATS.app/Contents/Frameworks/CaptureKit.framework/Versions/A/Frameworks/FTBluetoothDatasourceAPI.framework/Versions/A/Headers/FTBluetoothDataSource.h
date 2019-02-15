//
//  FTBluetoothDataSource.h
//  FTBluetoothDatasourceAPI
//
//  Created by Michael Lance on 9/23/11.
//  Copyright 2011 Frontline Test Equipment, Inc. All rights reserved.
//

#pragma once

#import <Foundation/Foundation.h>
#import "FTBluetoothIO.h"
#import "FTDataFrame.h"

@class FTCrossTransportKeyDatabase;
@class FTLEConnectionManager;

/**
 @file FTBluetoothDataSource.h
*/

/**
 An enumeration of chip sniffing states.
 */
typedef enum ChipSniffingState
{
  CSS_IDLE, /**< Chip is idle. */
  CSS_INITIALIZED, /**< Chip is initialized for sniffing. */
  CSS_LOCKED, /**< Chip is locked onto a connection. */
  CSS_CANCEL_SUCCEEDED, /**< Chip sniffing successfully canceled. */
  CSS_CANCEL_FAILED /**< Chip sniffing cancel failed. */
} ChipSniffingState;

/**
 Set of selectors called by the datasource in response to USB bus events associated with a ComProbe.
 */
@protocol FTBluetoothComProbeAttached <NSObject>

/**
 A ComProbe was plugged in but an error occurred.
 
 @param comprobeAddedData A dictionary containing the serial number of added ComProbe for key "serialnumber", an NSNumber object that contains the ComProbe type for key "comprobetype" - @see ComProbeType, and an error object that contains information related to a configuration failure for the ComProbe for key "error" If there was no configuration failure, there will be no key
   "error"
 */
- (void)comprobeAddedWithConfigurationError:(NSDictionary*)comprobeAddedData;

/**
 An invalid ComProbe was plugged in.
 
 @param invalidComprobeAddedData A dictionary containing the serial number of added ComProbe for key "serialnumber", an NSNumber object that contains the ComProbe type for key "comprobetype" - @see ComProbeType, and an error object that contains information about the invalid ComProbe for key "error"
 */
- (void)invalidComprobeAdded:(NSDictionary *)invalidComprobeAddedData;

/**
 A ComProbe was unplugged.
 
 @param comprobeRememovedData A dictionary containing the serial number of removed ComProbe for key "serialnumber' and the capture state of the ComProbe, a Boolean, in an NSNumber for key "iscapturing"
 @param isCapturing Capture state of ComProbe at removal.
 */
- (void)comprobeRemoved:(NSDictionary*)comprobeRemovedData;

/**
 The status of the ComProbe firmware.
 
 @param firmwareStatus NSNumber initialized with an unsigned integer that represents the status of the ComProbe firmware: ERR_OK for a ComProbe with the correct version. ERR_INVALID_CHIP_FIRMWARE_VERSION for a ComProbe that requires a firmware update. ERR_READ_CHIP_FIRMWARE_VERSION_FAILED for a ComProbe for which the version number could not be read.
 */
- (void)comprobeFirmwareStatus:(NSNumber*)comprobeFirmwareStatus;

@optional
/**
 The ComProbe was queried for chip firmware versions.
 
 @param firmwareVersionResults NSDictionary of results of chip version query: Key "chipfirmwareversionstatus" has a dictionary of chip status values where the keys are chip serial numbers, and the values are NSNumber representation of query status values where the possible values are ERR_NO_COMPROBES, ERR_ALLOCATION_FAILURE, ERR_START_CHIP_FIRMWARE_READ_FAILED, ERR_READ_CHIP_FIRMWARE_VERSION_FAILED, ERR_INVALID_CHIP_FIRMWARE_VERSION or ERR_OK. There are keys for each chip identifier where the values are chip status messages. There will also be a key "requiredfirmwareversion" that will have an NSNumber that contains the required firmware version for the selected ComProbe, or -1 if there is no selected ComProbe.
 */
- (void)comprobeFirmwareVersion:(NSDictionary*)firmwareVersionResults;

@end

/**
 Set of selectors called by the datasource in response to ComProbe inquiry events.
 */
@protocol FTBluetoothComProbeDeviceInquiry <NSObject>

/**
 The inquiry started.
 */
- (void)inquiryStarted;

/**
 One or more Bluetooth devices were found.
 
 @param deviceData A dictionary of device parameters created by calling getAsKeysAndValues:error: for the inquiry result. @see FTInquiryResult#getAsKeysAndValues:error:.
 */
- (void)inquiryDeviceFound:(NSDictionary*)deviceData;

/**
 The inqiry was canceled.
 */
- (void)inquiryCanceled;

/**
 The inquiry ran to completion.
 */
- (void)inquiryComplete;

/**
 The inquiry failed.
 
 @param errorData A dictionary of data to describe the error
 
 There may be a key "error" with a corresponding NSError object describing the failure.
 There will be a key "command" with a corresponding NSNumber holding an NSUInteger that represents the command the chip was executing at failure.
 There may be a key "parentcommand", with a corresponding NSNumber carrying an NSUInteger, to describe what the datasource was doing at failure.
 There will be a key "chipserialnumber" with a corresponding NSString to represent the chip that failed.
 There will be a key "chipindex" with a corresponding NSNumber carrying an NSUInteger that is the index of the chip that failed.
 */
- (void)inquiryFailed:(NSDictionary*)errorData;

@end

/**
 Set of selectors called by the datasource in response to ComProbe data capture events.
 */
@protocol FTBluetoothComProbeDataCapture <NSObject>

/**
 The command failed.
 
 @param errorData A dictionary of data to describe the error
 
 There may be a key "error" with a corresponding NSError object describing the failure.
 There will be a key "command" with a corresponding NSNumber holding an NSUInteger that represents the command the chip was executing at failure.
 There may be a key "parentcommand", with a corresponding NSNumber carrying an NSUInteger, to describe what the datasource was doing at failure.
 There will be a key "chipserialnumber" with a corresponding NSString to represent the chip that failed.
 There will be a key "chipindex" with a corresponding NSNumber carrying an NSUInteger that is the index of the chip that failed.
 */
- (void)commandFailed:(NSDictionary*)errorData;

/**
 The ComProbe was successfully initialized.
 
 @param chipSerialNumber The serial number of the chip that completed initialization.
 */
- (void)snifferInitializationComplete:(NSString*)chipSerialNumber;

/**
 The ComProbe locked onto a Bluetooth connection.
 
 @param lockedChipInformation A dictionary of information about the chip that synchronized with the connection.
 
 There will be a key "chipserialnumber" with an NSString that contains the locked chip serial number.
 There will be a key "iosettings" with an FTBluetoothCaptureSessionConfiguration that describes the configuration of the locked chip.
 */
- (void)snifferInLock:(NSDictionary*)lockedChipInformation;

/**
 The ComProbe fell out of lock on the connection.
 
 @param comprobeStatus A dictionary of data to identify the chip that disconnected
 
 There will be a key "chipserialnumber" with a corresponding NSString to represent the chip identifier.
 There will be a key "chipindex" with a corresponding NSNumber carrying an NSUInteger that is the index of the chip.
 */
- (void)snifferOutOfLock:(NSDictionary*)comprobeStatus;

/**
 The sniffer cancel operation status.
 
 @param comprobeCancelStatus A dictionary of data to indicate the status of each ComProbe chip after a cancel command has been issued:
 
 There will be a key for each chip serial number for which sniffing was started. The value for eacn key will be an NSNumber initialized with an unsigned integer representing a @see ChipSniffingState member.
 
 If the value for each chip is CSS_CANCEL_SUCCEEDED, the cancel operation completed successfully.
 */
- (void)snifferCancelStatus:(NSDictionary*)comprobeCancelStatus;

/**
 A command was canceled.
 
 @param command The command that was canceled.
 */
- (void)commandCanceled:(NSNumber*)command;

/**
 */

/**
 Optional selectors.
 */
@optional
/**
 A data frame was acquired.
 
 @param dataFrame The data frame that was acquired.
 */
- (void)dataFrameAcquired:(FTDataFrame*)dataFrame;

/**
 A status message from the ComProbe.
 
 @param statusData A dictionary with a key "statusmessage" with an NSString for the message.
 */
- (void)displayStatus:(NSDictionary*)statusData;

/**
 A message from the ComProbe.
 
 @param message An NSString for the message.
 */
- (void)displayMessage:(NSString*)message;

/**
 A new link key was generated as part of the SSP debug pairing process or a link key
 was validated as part of the authentication process.
 
 @param linkKeyData A dictionary to describe the link key.
 
 There will be a key "linkkey" with a value that is an NSData that contains the new link key data.
 There will be a key "chipserialnumber" with a corresponding NSString to represent the chip that computed the link key.
 There will be a key "chipindex" with a corresponding NSNumber carrying an NSUInteger that is the index of the chip that computed the link key.
 */
- (void)newLinkKeyGenerated:(NSDictionary*)linkKeyData;

/**
 A bad link key was detected as part of the SSP debug pairing process.
 */
- (void)badLinkKeyDetected;

/**
 A new long term key was set or computed and stored in the capture session configuration. 
 */
- (void)updateLongTermKey;

/**
 A new timestamp reference interval was computed. This NSTimeInterval is the basis for all data frame timestamp computation and is relative to 01 January 2001 GMT.
 
 @param chipReferenceInterval An NSNumber containing a double that represents the seconds since 01 January 2001 GMT when the microsecond clocks of the chips were read.
 */
- (void)startCaptureReferenceInterval:(NSNumber*)startCaptureReferenceInterval;

/**
 State of the Bluetooth data debug collection flag for a chip.
 
 @param chipBluetoothDataDebugCollectionStates An dictionary of string, keyed on chip serial numbers, to describe the current state for each chip.
 */
- (void)saveCollectedFramesState:(NSDictionary*)chipBluetoothDataDebugCollectionStates;

/**
 The current set of paths for collection of Bluetooth debug data frames.
 
 @param bluetothDataCollectionPaths A dictionary of paths where the keys are the serial numbers of the sniffing chips.
 */
- (void)bluetoothDataCollectionPathsSet:(NSDictionary*)bluetoothDataCollectionPaths;

@end

@class FTUtility;

/**
 FTBluetoothDataSource
 
 A class that represents a source of Bluetooth data.
 */

@interface FTBluetoothDataSource : NSObject

/**
 The capture session configuration object.
 */
@property (readonly) FTBluetoothCaptureSessionConfiguration* captureSessionConfiguration;

/**
 The delegate to perform required-protocol selectors on.
 */
@property (readonly) id delegate;

/**
 The current command being executed.
 */
@property (readonly) NSNumber* currentOperation;

/**
 A utility that can perform debug operations.
 */
@property (readonly) FTUtility* debugUtility;

/**
 A database that contains addresses and keys that define a connection.
 */
@property (readonly) FTCrossTransportKeyDatabase* cxTransportKeyDatabase;

/**
 The ComProbeType of the selected ComProbe - @see ComProbeType.
 */
@property (readonly) ComProbeType selectedComProbeType;

/**
 The required firmware version of the selected ComProbe.
 */
@property (readonly) NSUInteger requiredFirmwareVersion;

/**
 Initialize a Bluetooth datasource with a delegate and thread to perform protocol selectors on.
 
 @param comprobeDelegate The delegate to perform protocol selectors on.
 @param callbackThread The thread context for delegate selector calls.
 */
- (id)initWithDelegate:(id)comprobeDelegate andCallbackThread:(NSThread*)callbackThread;

/**
 Set the delegate.
 
 @param comprobeDelegate The delegate to perform protocol selectors on.
 */
- (void)setDelegate:(id)comprobeDelegate;

/**
 Deallocate a Bluetooth datasource.
 */
- (void)dealloc;

/**
 Set the selected ComProbe.
 
 @param comprobeSerialNumber NSString containing the serial number of the desired ComProbe. If nil, the current ComProbe selected is stopped and the selection is cleared.
 */
- (BOOL)selectComProbe:(NSString*)comprobeSerialNumber error:(NSError**)errorPtr;

/**
 Stop all operations on ComProbe chips.
 */
- (void)stopChipActivities;

/**
 Register for ComProbe USB bus activity and check for ComProbes.
 */
- (BOOL)setupDeviceNotification:(NSError**)errorPtr;

/**
 A check for known ComProbes.
 
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return TRUE if ComProbes are known.
 */
- (BOOL)haveComProbes:(NSError**)errorPtr;

/**
 Get the serial numbers of known ComProbes.
 
 @param serialNumbers A mutable array to hold returned serial numbers.
 @param comprobeSerialNumber The serial number of the containing ComProbe.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return TRUE if operation succeeded.
 */
- (BOOL)getComProbeSerialNumbers:(NSMutableArray*)serialNumbers forComProbe:(NSString*)comprobeSerialNumber error:(NSError**)errorPtr;

/**
 A callback used by device notification handling to indicate a ComProbe has left the USB bus.
 
 @param chipSerialNumber The serial number of the initiating device, a ComProbe chip.
 */
- (void)chipRemoved:(NSString*)chipSerialNumber;

/**
 Get the chip serial numbers for the selected ComProbe.
 
 @param chipSerialNumbers A mutable array to hold the returned chip serial numbers.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return TRUE if operation succeeded.
 */
- (BOOL)getChipSerialNumbersForSelectedComProbe:(NSMutableArray*)chipSerialNumbers error:(NSError**)errorPtr;

/**
 Get the type of the specified ComProbe.
 
 @param comprobeSerialNumber ComProbe serial number.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return Type of ComProbe if operation succeeded - see @ComProbeType.
 */
- (ComProbeType)comprobeTypeForSerialNumber:(NSString*)comprobeSerialNumber error:(NSError**)errorPtr;

/**
 Update the Classic Bluetooth capture session configuration in use for the ComProbe. @see FTBluetoothCaptureSessionConfiguration.
 
 @param pairingMode The new pairing mode. @see PairingMode.
 @param legacyUsesLinkKey A flag to indicate whether legacy pairing uses a link key or PIN code.
 @param masterAddress New master Bluetooth device BD_ADDR as a string.
 @param slaveAddress New slave Bluetooth device BD_ADDR as a string.
 @param linkKey New link key as a string.
 @param pinCode New PIN code as a string.
 @param pinCodeType Type of new PIN code if needed. @see PinCodeType.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return TRUE if operation succeeded.
 */
- (BOOL)updateClassicBluetoothCaptureSessionConfiguration:(PairingMode)pairingMode legacyUsesLinkKey:(BOOL)legacyUsesLinkKey masterAddress:(NSString*)masterAddress slaveAddress:(NSString*)slaveAddress linkKey:(NSString*)linkKey pinCode:(NSString*)pinCode pinCodeType:(PinCodeType)pinCodeType error:(NSError**)errorPtr;

/**
 Update the low energy Bluetooth capture session configuration in use for the ComProbe. @see FTBluetoothCaptureSessionConfiguration.
 
 @param syncMode The low energy synchronization mode. @see LeSyncMode.
 @param addressType The device role if synchronization mode is LSM_ADVERTISER_ADDRESS or LSM_INITIATOR_ADDRESS. @see LeAddressType.
 @param advertiserAddress New low energy Bluetooth advertiser device BD_ADDR as a string - ignored if synchronization role is not LSM_ADVERTISER_ADDRESS or LSM_INITIATOR_ADDRESS.
 @param initiatorAddress New low energy Bluetooth initator device BD_ADDR as a string - ignored if synchronization role is not LSM_ADVERTISER_ADDRESS or LSM_INITIATOR_ADDRESS.
 @param validLtkEntered TRUE if a valid long term key has been set.
 @param ltk New long term key as a string.
 @param temporaryKeySet TRUE if long term key has been set.
 @param tk New temporary key as a string.
 @param temporaryKeyType Type of pairing data represented by temporary key.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return TRUE if operation succeeded.
 */
- (BOOL)updateLowEnergyBluetoothCaptureSessionConfiguration:(LeSyncMode)syncMode leAddressType:(LeAddressType)addressType leAdvertiserAddress:(NSString*)advertiserAddress leInitiatorAddress:(NSString*)initiatorAddress validLtkEntered:(BOOL)validLtkEntered enteredLongTermKey:(NSString*)enteredLtk tkSet:(BOOL)temporaryKeySet temporaryKey:(NSString*)tk temporaryKeyType:(LePairingDataType)temporaryKeyType error:(NSError**)errorPtr;

/**
 Update the capture session configuration in use for the ComProbe with a new capture session configuration. @see FTBluetoothCaptureSessionConfiguration.
 
 @param newConfiguration The new capture session configuration.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return TRUE if operation succeeded.
 */
- (BOOL)updateBluetoothCaptureSessionConfigurationWithConfiguration:(FTBluetoothCaptureSessionConfiguration*)newConfiguration error:(NSError**)errorPtr;

/**
 Update the capture session configuration low energy temporary key. @see FTBluetoothCaptureSessionConfiguration.
 
 @param temporaryKey The new temporary key.
 @param lePairingDataType The type of key - @see LePairingDataType.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return TRUE if operation succeeded.
 */
- (BOOL)updateBluetoothCaptureSessionConfigurationTemporaryKey:(NSString*)temporaryKey ofType:(LePairingDataType)lePairingDataType error:(NSError**)errorPtr;

/**
 Perform a device inquiry using the selected ComProbe.
 
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return TRUE if inquiry started.
 */
- (BOOL)deviceInquiry:(NSError**)errorPtr;

/**
 Cancel device inquiry.
 
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return TRUE if inquiry canceled.
 */
 - (BOOL)cancelDeviceInquiry:(NSError**)errorPtr;
 
/**
 Start sniffing a Bluetooth connection using the selected ComProbe.
 
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return TRUE if sniffing started.
 */
- (BOOL)startSniffing:(NSError**)errorPtr;

/**
 Stop sniffing a Bluetooth connection.
 
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return TRUE if sniffing stopped.
 */
- (BOOL)stopSniffing:(NSMutableArray*)errors;

/**
 Save updated pairing data to specified connections.
 
 @param connections Connections to write new data to - key is chip serial number.
 @param errors Mutable array to hold per-connection error data. You may specify nil for this parameter if you do not want the error information.
 @return TRUE if no errors.
 */
- (BOOL)writePairingDataOnConnections:(NSDictionary*)connections errors:(NSMutableArray*)errors;

/**
 Get firmware status for all chips.
 
 @param firwareVersions Mutable dictionary to receive firmware version information. There will be a key for each chip identifier. Each key will have a value that is an NSNumber that contains an NSUInteger that is the firware version.
 @param errors Mutable dictionary to receive chip errors. There may be a key for each chip serial number with a value that is an NSError.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return TRUE if no errors.
 */
- (BOOL)getAllFirmwareStatus:(NSMutableDictionary*)firmwareVersions errors:(NSMutableDictionary*)errors error:(NSError**)errorPtr;

/**
 Read local version information for each chip of the specified ComProbe and report the status in delegate method comprobeFirmwareStatus:.
 
 @param comprobeSerialNumber Optional serial number of ComProbe to read from. If not
   specified, the selected ComProbe will be queried. If there is no selected ComProbe,
   an error will be returned.
 @param firwareVersions Optional mutable dictionary to receive firmware version information. There will be a key for each chip serial number. Each key will have a value that is an NSNumber that contains an NSUInteger that is the firware version.
 @param prettyKeys Optional mutable dictionary to receive "pretty" keys. There will be a key for each chip serial numbe. Each key will have a value that is a key in the form "ComProbe <ComProbe Serial Number> Chip <chip>" like "ComProbe 21036 Chip G"
 @param errors Optional mutable dictionary to receive general and chip errors. There may be a key "general" for a non-chip error. There may be a key for each chip identifier. Each key will have a value that is an NSError.
 @param notifyDelegate Boolean set to "TRUE" if delegate is to be notified of results.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return TRUE if no errors.
 */
- (BOOL)readAllFirmwareStatusForComProbe:(NSString*)comprobeSerialNumber firmwareVersions:(NSMutableDictionary*)firmwareVersions prettyKeys:(NSMutableDictionary*)prettyKeys errors:(NSMutableDictionary*)errors notifyDelegateOfResults:(BOOL)notifyDelegate error:(NSError**)errorPtr;

/**
 Are there data frames available.
 
 @return TRUE if frames are available.
 */
- (BOOL)dataFramesAvailable;

/**
 Enable debug Bluetooth frame collection prior to LMP processing. These data along with the appropriate capture configuration data will be saved to a file during the capture session. Changes made to this flag will not take effect during a capture or inquiry session.
 
 @param enable True to enable collection.
 */
- (void)enableSaveCollectedBluetoothFrames:(BOOL)enableSaveCollectedFrames;

/**
 Enable debug frame collection of all HCI events, HCI commands and sniffer data that have been processed for LMP information. These data will be dumped upon request.
 
 @param enable True to enable collection.
 */
- (void)enableDebug:(BOOL)enable;

/**
 Dump frames collected for debug to the specified path. HCI and ACL records will be printed to the file, organized by the chip that captured the data.
 
 @param outputPath A string that contains the path to dump frame records to. There will be one file per chip, and the chip serial number will make up part of the resulting file name.
 @param errorPtr On input, a pointer to an error object. If an error occurs, this pointer is set to an actual error object containing the error information. You may specify nil for this parameter if you do not want the error information.
 @return TRUE if frames printed.
 */
- (BOOL)dumpDebugDataToPath:(NSString*)outputPath error:(NSError**)errorPtr;

/**
 Are debug frames available.
 
 @param chipsWithDebugFrames A mutable dictionary with a key for each chip with a value that is an NSNumber representing number of frames.
 @return TRUE if frames are available.
 */
- (BOOL)hasDebugFrames:(NSMutableArray*)chipsWithDebugFrames;

// Test funtions:
+ (BOOL)decodeFrameDataFromPath:(NSString*)inputDataPath toPath:(NSString*)outputDataPath error:(NSError**)errorPtr;

- (void)processFramesFromPath:(NSString*)path callbackThread:(NSThread*)callbackThread;
@end
