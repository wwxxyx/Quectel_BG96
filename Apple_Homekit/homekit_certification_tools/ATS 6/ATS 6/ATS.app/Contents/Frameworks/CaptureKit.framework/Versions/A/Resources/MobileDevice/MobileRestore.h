//
//  MobileRestore.h
//  MobileDevice
//
//  Created by Jason Ederle on 4/26/11.
//  Copyright 2011 Apple Inc. All rights reserved.
//

#ifndef __MOBILERESTORE_H
#define __MOBILERESTORE_H

#include <CoreFoundation/CoreFoundation.h>
#include <MobileDevice/MobileDevice.h>
#include <MobileDevice/AMRestoreOptions.h>

//annotate returns retained if using clang compiler
#ifndef __has_feature
#define __has_feature(x) 0
#endif

#ifndef CF_RETURNS_RETAINED
#if __has_feature(attribute_cf_returns_retained)
#define CF_RETURNS_RETAINED __attribute__((cf_returns_retained))
#else
#define CF_RETURNS_RETAINED
#endif
#endif

#if defined(__cplusplus)
#define	__AMR_BEGIN_DECLS	extern "C" {
#define	__AMR_END_DECLS	}
#else
#define	__AMR_BEGIN_DECLS
#define	__AMR_END_DECLS
#endif

__AMR_BEGIN_DECLS
    
/*!
 @header MobileRestore.h
 
 Restore an iOS device
 
 The MobileRestore API facilitates discovering devices and
 allowing an easy way to restore builds onto them. In order to use the API
 properly your application should follow this generic workflow.
 
 Register your application for device discovery and disappeared events with
 AMRestorableDeviceRegisterForNotifications(). You will receive "discovery" events
 when a device has been connected and "disappeared" events when a device is
 unexpectedly disconnected. During the course of a restore, you will NOT receive
 any events for discovered/disappeared since the state transitions are
 expected. If during a restore a device transition lasts longer then expected,
 a device disappeared event will be sent to your handler and the restore
 will be terminated. You will likely also want to keep track of your
 AMRestorableDeviceRef objects so that you can restore them later.
 
 When you are ready to restore a device you can do that by calling
 AMRestorableDeviceRestore(). You will receive callbacks during the course of
 the restore so that your application can follow the progress. After invoking a restore
 you should setup a CFRunloop (if you're a GUI app you'll already have one) since an
 AppleConnect window may need to open for internal install
 personalization. The runloop is also necessary so that you can efficiently wait for
 our restore progress callbacks and handle them if desired. Events will be
 dispatched from a MobileRestore internal thread and NOT from your applications threads.
 
 When you are done with the MobileRestore API you can unregister your callbacks
 with AMRestorableDeviceUnregisterForNotifications().
 
 */

typedef struct __AMRestorableDevice* AMRestorableDeviceRef;
typedef int AMRestorableClientID;

typedef struct __AMRestorableBuild* AMRestorableBuildRef;

typedef struct _AMRestorablePersonalized* AMRestorablePersonalizedRef;

/*! @const kAMRestorableInvalidClientID value of an invalid client ID
 */
extern const AMRestorableClientID kAMRestorableInvalidClientID;

/*!
 @enum Device connection event flags passed to AMRestorableDeviceEventCallback
 @const kAMRestorableDeviceEventDiscovered Device has been connected and is restorable
 @const kAMRestorableDeviceEventDisappeared Device was detached unexpectedly
 */
typedef enum {
    kAMRestorableDeviceEventDiscovered,
    kAMRestorableDeviceEventDisappeared
} AMRestorableDeviceEvent;

/*!
 @enum Device state flags returned from AMRestorableDeviceGetState
 @const kAMRestorableDeviceStateDFU device is in DFU mode
 @const kAMRestorableDeviceStateRecovery device is in recovery mode
 @const kAMRestorableDeviceStateRestoreOS device is booted into restore OS
 @const kAMRestorableDeviceStateBootedOS device is booted into OS
 */
typedef enum {
    kAMRestorableDeviceStateUnknown,
    kAMRestorableDeviceStateDFU,
    kAMRestorableDeviceStateRecovery,
    kAMRestorableDeviceStateRestoreOS,
    kAMRestorableDeviceStateBootedOS
} AMRestorableDeviceState;

/*!
 @enum Device fusing values
 @const kAMRestorableDeviceFusingDevelopment device is development fused
 @const kAMRestorableDeviceFusingProduction device is production fused
 @const kAMRestorableDeviceFusingInsecure device is insecure fused
 */
typedef enum {
    kAMRestorableDeviceFusingUnknown,
    kAMRestorableDeviceFusingDevelopment,
    kAMRestorableDeviceFusingProduction,
    kAMRestorableDeviceFusingInsecure,
} AMRestorableDeviceFusing;
    
/*!
 @enum Error codes used in CFError
 */
typedef enum {
    kAMRestorableErrorNone = 0,
    
    kAMRestorableErrorGenericError = 4000,
    kAMRestorableErrorBadArgument = 4001,
    kAMRestorableErrorAllocationFailed = 4002,
    kAMRestorableErrorInitializationFailed = 4003,
    kAMRestorableErrorDeviceIsBusyRestoring = 4004,
    kAMRestorableErrorDeviceDisappeared = 4005,
    kAMRestorableErrorFileDoesNotExist = 4006,
    kAMRestorableErrorFailedToReadFile = 4007,
    kAMRestorableErrorUnexpectedType = 4008,
    kAMRestorableErrorUnsupportedRestoreDocumentType = 4009,
    
    kAMRestorableErrorStateMachineGenericError = 4010,
    kAMRestorableErrorStateMachineNoMoreStates = 4011,
    kAMRestorableErrorStateMachineUnexpectedUnknownState = 4012,
    kAMRestorableErrorStateMachineUnexpectedRecoveryState = 4013,
    kAMRestorableErrorStateMachineUnexpectedDFUState = 4014,
    kAMRestorableErrorStateMachineUnexpectedRestoreOSState = 4015,
    kAMRestorableErrorStateMachineUnexpectedBootedOSState = 4016,
    kAMRestorableErrorFailedToCreateStateMachine = 4017,
    kAMRestorableErrorInvalidFinishStageOverride = 4018,
    kAMRestorableErrorFailedToGetSSOTicket = 4019,
    
    kAMRestorableErrorHTTPDownloadFailed = 4020,
    kAMRestorableErrorFailedToPrepareRestoreOptions = 4021,
    kAMRestorableErrorFailedToCreateObject = 4022,
    kAMRestorableErrorFailedToAccessBuildMetadata = 4023,
    kAMRestorableErrorFailedToGetTemporaryDirectory = 4024,
    kAMRestorableErrorUnsupportedURLScheme = 4025,
    kAMRestorableErrorNotEnoughDiskSpace = 4026,
    kAMRestorableErrorFailedToPreflight = 4027,
    kAMRestorableErrorFailedToPersonalize = 4028,
    kAMRestorableErrorFailedToCreateAuthInstallObject = 4029,
    
    kAMRestorableErrorFailedToSendFile = 4030,
    kAMRestorableErrorAccessDenied = 4031,
    
} AMRestorableError;

/*!
 @enum Mobile Restore operation codes
 */
typedef enum {
    kAMRestorableOperationUnknown = -1,
    kAMRestorableOperationDownloadingRestoreBundle = 200,
    kAMRestorableOperationUnzippingIPSW,
    kAMRestorableOperationMountingDMG,
    kAMRestorableOperationPreflighting,
    kAMRestorableOperationDFUModeDeviceRebooting,
    kAMRestorableOperationRecoveryModeDeviceRebooting,
    kAMRestorableOperationRestoreOSModeDeviceRebooting,
    kAMRestorableOperationBootedOSModeDeviceRebooting,
    kAMRestorableOperationQueuePositionDidChange,
    kAMRestorableOperationSendingFDRUserInfo,
    kAMRestorableOperationNotifyiTunesShowUnlockDevice,
    kAMRestorableOperationNotifyiTunesHideUnlockDevice
} AMRestorableOperation;

//Info dictionary keys passed to restore progress callback
#define kAMRestorableDeviceInfoKeyStatus CFSTR("Status")                        //Value is CFStringRef, see status values below
#define kAMRestorableDeviceInfoKeyProgress CFSTR("Progress")                    //Value is CFNumberRef between -1 to 100, kCFNumberIntType
#define kAMRestorableDeviceInfoKeyOperation CFSTR("Operation")                  //Value is CFNumberRef of current operation, kCFNumberIntType
#define kAMRestorableDeviceInfoKeyError CFSTR("Error")                          //Value is CFError if restore failed
#define kAMRestorableDeviceInfoKeyLogDataType CFSTR("LogDataType")              //Value is CFStringRef of log type, see types below
#define kAMRestorableDeviceInfoKeyLogData CFSTR("LogData")                      //Value is CFDataRef of log message content
#define kAMRestorableDeviceInfoKeyQueuePosition CFSTR("QueuePosition")          //Value is CFNumber of pending restores in front of this restore
#define kAMRestorableDeviceInfoKeyPersonalizedObject CFSTR("PersonalizedObj")   //Value is AMRestorablePersonalizedRef
#define kAMRestorableDeviceLastFailureKeyLog CFSTR("LastFailureLog")            //Value is CFStringRef
#define kAMRestorableDeviceLastFailureKeyType CFSTR("LastFailureType")          //Value is CFStringRef, see restore types
#define kAMRestorableDeviceLastFailureKeyInfo CFSTR("LastFailureInfo")          //Value is CFDictionary of failure metadata
#define kAMRestorableDeviceInfoKeyFDRUserInfo CFSTR("FDRUserInfo")              //Value is CFDictionary of missing or unverified FDR data

//Status values for kAMRestorableDeviceInfoKeyStatus
#define kAMRestorableDeviceStatusRestoring CFSTR("Restoring")               //Device is restoring
#define kAMRestorableDeviceStatusSuccessful CFSTR("Successful")             //Device successfully restored
#define kAMRestorableDeviceStatusFailed CFSTR("Failed")                     //Device failed to restore

//Log types
#define kAMRestorableDeviceLogTypeSerial CFSTR("SerialLogType")             //Serial log type
#define kAMRestorableDeviceLogTypeHost CFSTR("HostLogType")                 //Host log messages from restore libraries
#define kAMRestorableDeviceLogTypeDevice CFSTR("DeviceLogType")             //Device restore stream messages sent from Restore OS

//Restore types
#define kAMRestorableDeviceRestoreTypeRestore CFSTR("RestoreTypeRestore")   //Device was restored
#define kAMRestorableDeviceRestoreTypeOTA CFSTR("RestoreTypeOTA")           //Device was OTA updated

/* Additional restore options supported by MobileRestore only */

/*!
 @discussion Override the state machines finishing state to create a partial restore
 that will be considered complete. This is useful for stopping after a certain restore stage
 for engineering reasons, or for stopping after sending a ramdisk that will never exit with a status.
 
 Value must be a CFStringRef from this list:
 kAMRestorableRestoreOptionStageNameDFU
 kAMRestorableRestoreOptionStageNameRecovery
 kAMRestorableRestoreOptionStageNameRestoreOS
 kAMRestorableRestoreOptionStageNameBootedOS
 */
#define kAMRestorableRestoreOptionOverrideFinishStage CFSTR("OverrideFinishStage")
#define kAMRestorableRestoreOptionStageNameDFU CFSTR("DFU")
#define kAMRestorableRestoreOptionStageNameRecovery CFSTR("Recovery")
#define kAMRestorableRestoreOptionStageNameRestoreOS CFSTR("RestoreOS")
#define kAMRestorableRestoreOptionStageNameBootedOS CFSTR("BootedOS")

/*!
 @discussion Override when the last restore stage should send a restore complete event.
 If this is true, the state machine waits for the device to connect back to the host in any device state
 before sending a restore complete event. If this is false, the state machine will consider the restore 
 complete immediately following the last state machine restore operation. The value must be a CFBooleanRef,
 and the default value is kCFBooleanTrue if not specified.
 */
#define kAMRestorableRestoreOptionWaitForDeviceConnectionToFinishStateMachine CFSTR("WaitForDeviceConnectionToFinishStateMachine")

/*!
 @discussion Specify a custom state machine to use during a restore. 
 The state machine verifies that a device shows up in the expected
 state before advancing to the next stage in the state machine.
 
 kAMRestorableRestoreOptionStateMachine:
 Value is a CFArrayRef of CFDictionaryRef's
 Each dictionary defines an expected state in the restore sequence.
 
 kAMRestorableRestoreOptionStateMachineEntryState:
 Value is a CFStringRef of the name of the restore step.
 The supported state strings are:
 kAMRestorableRestoreOptionStageNameDFU
 kAMRestorableRestoreOptionStageNameRecovery
 kAMRestorableRestoreOptionStageNameRestoreOS
 kAMRestorableRestoreOptionStageNameBootedOS
 
 kAMRestorableRestoreOptionStateMachineEntryMaxLoops:
 Value is a CFNumber of kCFNumberIntType that specifies
 how many times a stage may be repeated. For example
 passing through recovery mode twice can be expressed as 1
 dictionary entry in the state machine with MaxLoops set to 2.
 
 kAMRestorableRestoreOptionStateMachineEntryStrictMaxLoops:
 Value is CFBooleanRef and it determines if the device must
 enter a state MaxLoop's times before it can advanced to the next state.
 Some devices may differ in the number of times a state is entered
 and this key determines if MaxLoops is a firm required value or a loose max
 upper bound.
 
 Example, this is the state machine for a restore starting from BootedOS:
<key>StateMachine</key>
<array>
    <dict>
        <key>State</key>
        <string>BootedOS</string>
        <key>MaxLoops</key>
        <integer>1</integer>
        <key>StrictMaxLoops</key>
        <true />
    </dict>
    <dict>
        <key>State</key>
        <string>Recovery</string>
        <key>MaxLoops</key>
        <integer>2</integer>
        <key>StrictMaxLoops</key>
        <false />
    </dict>
    <dict>
        <key>State</key>
        <string>RestoreOS</string>
        <key>MaxLoops</key>
        <integer>1</integer>
        <key>StrictMaxLoops</key>
        <true />
    </dict>
</array>
 */
#define kAMRestorableRestoreOptionStateMachine CFSTR("StateMachine")
#define kAMRestorableRestoreOptionStateMachineEntryState CFSTR("State")
#define kAMRestorableRestoreOptionStateMachineEntryMaxLoops CFSTR("MaxLoops") 
#define kAMRestorableRestoreOptionStateMachineEntryStrictMaxLoops CFSTR("StrictMaxLoops")

#if 0
#pragma mark AMRestorableDevice API
#endif

/*!
 @function AMRestorableDeviceGetTypeID
 @discussion Get the CFRuntime type ID for the AMRestorableDevice type.
 */
CFTypeID AMD_API AMRestorableDeviceGetTypeID( void );

/*!
 @discussion Callback for when devices are discovered or disappear unexpectedly.
 This callback will NOT fire during the course of a restore during expected device transitions.
 
 @param device The AMRestorableDeviceRef this connection event is referring to
 @param event The type of event that has occurred of type AMRestorableDeviceEvent
 @param context User data given at time of notification registration.
 */
typedef void (*AMRestorableDeviceEventCallback)( AMRestorableDeviceRef device, AMRestorableDeviceEvent event, void *context );

/*!
 @discussion Callback sent to update client about restore progress and operations.
 
 @param device The AMRestorableDeviceRef that this progress is about
 @param info Dictionary containing progress, state, and current operation information, see keys in kAMRestorableDeviceInfo…
 @param context User data given when starting the restore
 */
typedef void (*AMRestorableDeviceProgressUpdateCallback)( AMRestorableDeviceRef device, CFDictionaryRef info, void *context );

/*!
 @function AMRestorableDeviceRegisterForNotifications
 @discussion Register for restorable device event notifications. You need to register before the rest of the API is available.
 
 @param eventHandler Callback for device discovery and disappearance of a AMRecoveryDeviceRef
 @param context Any user provided data that will be passed to eventHandler
 @param error An output CFError that will be set if an error is encountered
 
 @return On success returns a valid client ID, on failure returns kAMRestorableInvalidClientID
 */
AMRestorableClientID AMD_API AMRestorableDeviceRegisterForNotifications( AMRestorableDeviceEventCallback eventHandler,
                                                                         void *context,
                                                                         CFErrorRef *error );

/*!
 @function AMRestorableDeviceUnregisterForNotifications
 @discussion Unregister your callback handlers from receiving AMRestorableDeviceEvent's.
 
 @param clientID The clientID returned from a successful event registration.
 
 @return Returns true if we could unregister clientID, false otherwise.
 */
bool AMD_API AMRestorableDeviceUnregisterForNotifications( AMRestorableClientID clientID );

/*!
 @function AMRestorableDeviceRestore
 @discussion Kickoff a restore, and recieve callbacks on the progress. This
 function is asynchronous, and the final restore status is delievered to your
 progress callback handler in the info dictionary under the key kAMRestorableDeviceInfoKeyStatus.
 
 @param device The AMRestorableDeviceRef you would like to restore
 @param options Dictionary of restore options
 @param progressHandler Callback for progress, status, and completion events.
 @param context Any user provided data that will be passed to progressHandler as argument
 */
void AMD_API AMRestorableDeviceRestore( AMRestorableDeviceRef device,
                                        CFDictionaryRef options,
                                        AMRestorableDeviceProgressUpdateCallback progressHandler,
                                        void *context );

/*!
 @function AMRestorableDeviceRestoreWithError
 @discussion Kickoff a restore, and recieve callbacks on the progress. This
 function is asynchronous, and the final restore status is delievered to your
 progress callback handler in the info dictionary under the key kAMRestorableDeviceInfoKeyStatus.

 @param device The AMRestorableDeviceRef you would like to restore
 @param options Dictionary of restore options
 @param progressHandler Callback for progress, status, and completion events.
 @param error CFErrorRef holding any errors we hit when a progressHandler is not available.
 @param context Any user provided data that will be passed to progressHandler as argument

 @return Returns true if the restore operation was successful
 */
Boolean AMD_API AMRestorableDeviceRestoreWithError( AMRestorableDeviceRef device,
                                                    CFDictionaryRef options,
                                                    AMRestorableDeviceProgressUpdateCallback progressHandler,
                                                    CFErrorRef *error,
                                                    void *context );

/*!
 @function AMRestorableDeviceGetState
 @discussion Get the state of a device
 
 @param aDevice The restorable device to get state about.
 
 @return Returns the state as AMRestorableDeviceState type
 */
AMRestorableDeviceState AMD_API AMRestorableDeviceGetState( AMRestorableDeviceRef aDevice );

/*!
 @function AMRestorableDeviceGetECID
 @description Copy the ECID for a restorable device
 
 @param aDevice The device to get the ECID for
 
 @return The ECID of the device
 */
uint64_t AMD_API AMRestorableDeviceGetECID( AMRestorableDeviceRef aDevice );

/*!
 @function AMRestorableDeviceGetBoardID
 @description Copy the board ID for a restorable device

 @param aDevice The device to get the board ID for

 @return The board ID of the device
 */
uint32_t AMD_API AMRestorableDeviceGetBoardID( AMRestorableDeviceRef aDevice );

/*!
 @function AMRestorableDeviceGetChipID
 @description Copy the chip ID for a restorable device

 @param aDevice The device to get the chip ID for

 @return The chip ID of the device
 */
uint32_t AMD_API AMRestorableDeviceGetChipID( AMRestorableDeviceRef aDevice );

/*!
 @function AMRestorableDeviceGetLocationID
 @description Copy the location ID for a device. This function will always return 0 on
 Windows.
 
 @param aDevice The restorable device to get the location ID of
 
 @return The location ID
 */
uint32_t AMD_API AMRestorableDeviceGetLocationID( AMRestorableDeviceRef aDevice );

/*!
 @function AMRestorableDeviceCopyBoardConfig
 @description Copy the board config for a device.
 
 @param aDevice The device to get the board config for
 @param deviceMap A dictionary loaded from a device map plist file that has information about aDevice
 
 @return The board config for the device
 */
CFStringRef AMD_API AMRestorableDeviceCopyBoardConfig( AMRestorableDeviceRef aDevice, CFDictionaryRef deviceMap ) CF_RETURNS_RETAINED;

/*!
 @function AMRestorableDeviceCopyAMDevice
 @description Copy the underlying AMDeviceRef for a AMRestorableDeviceRef. The AMRestorableDeviceRef
 must be for a device in state kAMRestorableDeviceStateBootedOS or else this function will return NULL.
 
 @param aDevice The restorable device to get the AMDeviceRef from
 
 @return an AMDeviceRef that references the same device as the AMRestorableDeviceRef
 */
AMDeviceRef AMD_API AMRestorableDeviceCopyAMDevice( AMRestorableDeviceRef aDevice ) CF_RETURNS_RETAINED;

/*!
 @function AMRestorableDeviceCopyRestoreOptionsFromDocument
 @description Get restore options from a document. We currently only support PurpleRestore
 ".pr" or ".pr2" files. We recommend using .pr documents since .pr2 is deprecated.
 
 @param documentURL The URL to a supported restore document
 @param error CFError if restore options can not be copied from the document URL. 
 
 @return restore options that can be passed to AMRestorableDeviceRestore(...)
 */
CFDictionaryRef AMD_API AMRestorableDeviceCopyRestoreOptionsFromDocument( CFURLRef documentURL, CFErrorRef *error ) CF_RETURNS_RETAINED;

/*!
 @function AMRestorableSetLogLevel
 @description Adjust the logging level that will be sent to syslog. Any
 log files specified through AMRestorableDeviceSetLogFileURL(...) or
 AMRestorableSetGlobalLogFileURL(...) will not be filtered and will
 receive all logging messages.
 
 @param logLevel Log level flags that are compatible with syslog
 */
void AMD_API AMRestorableSetLogLevel( int logLevel );

/*!
 @function AMRestorableDeviceSetLogFileURL
 @description Optional URL to write logging data to during a restore. If this is not set
 then the default behavior is for nothing to get logged. This logging data is only 
 written to the file over the duration of a restore started by AMRestorableDeviceRestore(...),
 and the log file will be closed at the end of the restore.
 
 @param aDevice that you want to log restore log messages for
 @param fileURL a URL to append log messages to
 @param logType the type of log data that should be written into fileURL (see log types above)

 @return returns true if the device was successfully setup to write logType data into fileURL
 */
bool AMD_API AMRestorableDeviceSetLogFileURL( AMRestorableDeviceRef aDevice, CFURLRef fileURL, CFStringRef logType );

/*!
 @function AMRestorableSetGlobalLogFileURL
 @description Set a log file URL which will receive all log messages for all log levels.
 This function can be used to capture logging data outside the context of a restore which
 is the difference from using AMRestorableDeviceSetLogFileURL(...) which only captures
 log data during a restore. This function appends all log messages regardless of the device
 into the specified log fileURL. The default behavior is to not log anything, and NULL
 can be passed to unset a global log fileURL.
 
 @param fileURL to write all log messages into
 @return returns true if we could successfully open the file for writing
 */
bool AMD_API AMRestorableSetGlobalLogFileURL( CFURLRef fileURL );

/*!
 @function AMRestorableEnableLogStreaming
 @description Stream log events for all log types to the progress handler specified
 in AMRestorableDeviceRestore(…). Log messages are only delivered during the duration
 of a restore. By default log streaming is disabled.
 
 @param shouldStreamLogs Flag to enable or disable log streaming for all devices.
 */
void AMD_API AMRestorableEnableLogStreaming( bool shouldStreamLogs );

/*!
 @function AMRestorableDeviceCopySerialDevicePath
 @description Copy the serial device path if serial is available
 for the connection to a device.
 
 @param device The device you want the serial dev node path for
 @return path to serial device (ex: /dev/cu.usbserial-A400fwXA)
 */
CFStringRef AMD_API AMRestorableDeviceCopySerialDevicePath( AMRestorableDeviceRef device ) CF_RETURNS_RETAINED;

/*!
 @function AMRestorableDeviceGetFusingInfo
 @description Check how a device is fused (Production, Development, etc)
 
 @param aDevice The device to get fusing status
 @return Enum value indicating the fusing status of the device.
 */
AMRestorableDeviceFusing AMD_API AMRestorableDeviceGetFusingInfo( AMRestorableDeviceRef aDevice );
    
/*!
 @function AMRestorableDeviceCopyDefaultRestoreOptions
 @description Copy the default restore options that can be used with AMRestorableDeviceRestore(...)
 
 The restore options have the following behavior:
    - Sets appropriate boot-args for booting the restore
    - Erases the device and creates 2 new partitions
    - Flashes the NOR with production NOR
    - Updates the baseband
    - Installs the release kernel cache
    - Uses the user system image/ram disk when used in conjunction with kAMRestoreOptionsRestoreBundlePath
 
 @return restore options dictionary
 */
CFMutableDictionaryRef AMD_API AMRestorableDeviceCopyDefaultRestoreOptions( void ) CF_RETURNS_RETAINED;

/*!
 @discussion
 Callback that is invoked when and HTTP proxy fails to authenticate due to missing
 or invalid credentials. You set the callback with AMRestorableDeviceSetProxyCredentialsCallback(...).
 
 @param device AMRestorableDevice that the credentials callback is for
 @param proxyURL URL of the proxy requiring credentials
 @param outUsername is a copy of the username provided by the application
 @param outPassword is a copy of the password provided by the application
 @param outSecureAuthentication indicates whether the credentials will be sent in a
        secure manner (e.g., digest authentication)
 @param outSaveInKeychain indicates whether the user wishes to have valid credentials
        stored in the keychain.  Invalid credentials will not be saved regardless
        of this value.
 @param context is the pointer that was registered with the callback
 */
typedef bool (*AMRestorableDeviceProxyCredentialsCallback)( AMRestorableDeviceRef device,
                                                            CFURLRef proxyURL,
                                                            bool secureAuthentication,
                                                            CFStringRef *outUsername,
                                                            CFStringRef *outPassword,
                                                            bool *outSaveInKeychain,
                                                            void *context );

/*!
 @function AMRestorableDeviceSetProxyCredentialsCallback
 @description Set a callback to handle failed HTTP proxy authentication. This function must be set on 
 an AMRestorableDeviceRef before a restore begins, and this callback will be cleared after each restore completes.
 
 @param aDevice the device you would like to set the proxy callback for
 @param proxyCallback the callback handler to use on aDevice for the next restore
 */
void AMD_API AMRestorableDeviceSetProxyCredentialsCallback( AMRestorableDeviceRef aDevice,
                                                            AMRestorableDeviceProxyCredentialsCallback proxyCallback,
                                                            void *context );

/*!
 @function AMRestorableDeviceCopyAuthInstallPreflightOptions
 @description Copy the authinstall preflight options. This function works for all device states
 except kAMRestorableDeviceStateRestoreOS. One use of this function is to test that the personalization server
 is up and answering requests before kicking off a restore.
 
 @param aDevice the device you want to personalize the restoreOptions options with
 @param restoreOptions to use when personalizing
 @return preflighted restore options
 */
CFDictionaryRef AMD_API AMRestorableDeviceCopyAuthInstallPreflightOptions( AMRestorableDeviceRef aDevice, CFDictionaryRef restoreOptions ) CF_RETURNS_RETAINED;

/*!
 @function AMRestorableDeviceGetProductID
 @description Get the product ID for a device. This function can only be called with the device is
 in state kAMRestorableDeviceStateDFU or kAMRestorableDeviceStateRecovery.
 
 @param aDevice The device to get the product ID for
 @return returns product ID
 */
uint16_t AMD_API AMRestorableDeviceGetProductID( AMRestorableDeviceRef aDevice );

/*!
 @function AMRestorableDeviceGetProductType
 @description Get the product type for a device. This function can only be called with the device is
 in state kAMRestorableDeviceStateDFU or kAMRestorableDeviceStateRecovery.
 
 @param aDevice The device you want the product ID for
 @return returns product type
 */
uint32_t AMD_API AMRestorableDeviceGetProductType( AMRestorableDeviceRef aDevice );

/*!
 @function AMRestorableDeviceCopySerialNumber
 @description Copy the serial number for a device. The device cannot be in state
 kAMRestorableDeviceStateDFU since that state does not report a serial number.
 
 @param aDevice The device you want the serial number for
 @return returns the serial number
 */
CFStringRef AMD_API AMRestorableDeviceCopySerialNumber( AMRestorableDeviceRef aDevice );

/*!
 @function AMRestorableDeviceSendFile
 @description Send a file to iBoot on the device. This function can only be called when the device is
 in state kAMRestorableDeviceStateRecovery.
 
 @param aDevice a device in recovery mode
 @param aFile file to send to iBoot
 @return successful if we were able to send the file
 */
bool AMD_API AMRestorableDeviceSendFile( AMRestorableDeviceRef aDevice, CFStringRef aFile );

/*!
 @function AMRestorableDeviceSendCommand
 @description Send an iBoot command to the device
 
 @param aDevice a device in recovery mode
 @param aCommand the command to send to iBoot
 @return successful if we were able to send the command
 */
bool AMD_API AMRestorableDeviceSendCommand( AMRestorableDeviceRef aDevice, CFStringRef aCommand );

/*!
 @function AMRestorableDeviceSendBlindCommand
 @description Send an iBoot command blindly to the device. The command is sent
 without verifying that iBoot received the command.
 
 @param aDevice a device in recovery mode
 @param aCommand the command to send to iBoot
 @return successful if we were able to send the command
 */
bool AMD_API AMRestorableDeviceSendBlindCommand( AMRestorableDeviceRef aDevice, CFStringRef aCommand );

/*!
 @function AMRestorableDeviceCopyEnvironmentVariable
 @description Copy an environment variable from iBoot. This function can only be called when the device is
 in state kAMRestorableDeviceStateRecovery.
 
 @param aDevice a device in recovery mode
 @param aVariable the variable to copy
 @return value of the environment variable
 */
CFStringRef AMD_API AMRestorableDeviceCopyEnvironmentVariable( AMRestorableDeviceRef aDevice, CFStringRef aVariable ) CF_RETURNS_RETAINED;

/*!
 @function AMRestorableGetIdentifyingErrorCode
 @description Get the deepest most identifying error code from a
 CFError returned from a failed restore callback.
 
 @param aError the failed restore error
 @return error code identifying failure in aError
 */
CFIndex AMD_API AMRestorableGetIdentifyingErrorCode( CFErrorRef aError );

/*!
 @function AMRestorableDeviceEnableExtraDFUDevices
 @description Enable MobileRestore to notify you of additional DFU device PID's. MobileRestore
 will notify you of these additional devices when connection/disconnect events happen but makes
 no gaurantee that we can actually perform restores on them since they are not default supported
 devices. This function must be called before registering for device notifications. The dictionary
 for each entry in the array must have the DFU device's PID for key kAMRestorableDFUDeviceInfoPID.
 
 @param dfuInfo an array of dictionaries with additional DFU device info
 @return successful if we could register for additional DFU devices
 */
#define kAMRestorableDFUDeviceInfoPID CFSTR("PID")
bool AMD_API AMRestorableDeviceEnableExtraDFUDevices( CFArrayRef dfuInfo );

#if 0 
#pragma mark AMRestorableBuild API
#endif

/*!
 @function AMRestorableBuildCreate
 @description Create an AMRestorableBuildRef object that can be used to extract
 information about a build. The buildURL is a URL to the root of a restore bundle
 that should contain one of the following metadata files; Index.plist, BuildManifest.plist, 
 or Restore/BuildManiest.plist. We will choose first build metadata file found in the previously
 listed order.
 */
AMRestorableBuildRef AMD_API AMRestorableBuildCreate( CFURLRef buildURL,
                                                      CFErrorRef *error ) CF_RETURNS_RETAINED;

/*!
 @function AMRestorableBuildCopySupportedVariants
 @description Copy an array of variants supported by a build for a specific board config. The value 
 of each array entry is a CFStringRef of the name of the variant.

 @param build object with information about the build
 @param boardConfig the name of the device you want the variants for
 @param error set if we can't return the supported variants
 @return array of CFStringRef variant names
 */
CFArrayRef AMD_API AMRestorableBuildCopySupportedVariants( AMRestorableBuildRef build,
                                                           CFStringRef boardConfig,
                                                           CFErrorRef *error ) CF_RETURNS_RETAINED;

/*!
 @function AMRestorableBuildCopySupportedDevices
 @description Copy an array of board configs that are supported by a build. The value of
 each array entry is a CFStringRef.
 
 @param build object with information about the build
 @param error set if we can't return the supported board configs
 @return array of CFStringRef board config names
 */
CFArrayRef AMD_API AMRestorableBuildCopySupportedBoardConfigs( AMRestorableBuildRef build,
                                                               CFErrorRef *error ) CF_RETURNS_RETAINED;

/*!
 @function AMRestorableBuildCopyRestoreBundleURL
 @description Copy the location to a restore bundle by evaluating build metadata with device and 
 variant information.
 
 @param build object with information about the build
 @param boardConfig to get restore bundle URL for
 @param variantName you want to restore
 @param error set if we can't return the restore bundle error
 @return URL to restore bundle that should be used to restore with
 */
CFURLRef AMD_API AMRestorableBuildCopyRestoreBundleURL( AMRestorableBuildRef build,
                                                        CFStringRef boardConfig,
                                                        CFStringRef variantName,
                                                        CFErrorRef *error ) CF_RETURNS_RETAINED;

/*!
 @function AMRestorableSetGlobalLocationIDFilter
 @description Registers location ID for filtering.   Will only send notifications for this device.
 This function MUST be called first (before setting up notifications.)   This is really only 
 useful in Multi-Process based Automation to reduce the number of processes talking to each 
 device

 @param locationID LocationID of device you want to pay attention to
 @return successful if we can register a locationID
*/
bool AMD_API AMRestorableSetGlobalLocationIDFilter( uint32_t locationID );


/*!
 @function AMRestorablePersonalizeBundle
 @description Personalize a bundle for a given device. The resulting AMRestorablePersonalizedRef
 can be used later to send files over to the device.

 @param device to personalized the bundle for
 @param options to use when personalizing
 @param error set if we can't personalize bundle
 @return AMRestorablePersonalizedRef caching the personalization state
 */
AMRestorablePersonalizedRef AMD_API AMRestorablePersonalizeBundle( AMRestorableDeviceRef device,
                                                                   CFDictionaryRef options,
                                                                   CFErrorRef *error ) CF_RETURNS_RETAINED;

/*!
 @function AMRestorablePersonalizeSendFile
 @description Send a personalized file from a perviously personalized bundle or 
 single file personalization call.

 @param personlizedObj is the cached state of the personalization
 @param manifestTag to retrieve the corresponding image to send
 @param command the command to send
 @param blind dictates sending a blind command or not
 @param error set if the operation fails
 @return successful if the device accepts the image and the command
 */
bool AMD_API AMRestorablePersonalizeSendFile( AMRestorablePersonalizedRef personlizedObj,
                                              CFStringRef manifestTag,
                                              CFStringRef command,
                                              bool blind,
                                              CFErrorRef *error );

/*!
 @function AMRestorablePersonalizeCopyManifestTag
 @description Copy the manifest tag used while personalizing from an file URL.

 @param fileURL URL of the file to lookup the manifest tag for
 @return value of the manifest tag
 */
CFStringRef AMD_API AMRestorablePersonalizeCopyManifestTag( CFURLRef fileURL ) CF_RETURNS_RETAINED;

/*!
 @function AMRLocalizedCopyStringForAMROperation
 @description Copies the localized string for a restore opertaion.

 @param operation restore operation to look up
 @return string description of the restore operation
 */
CFStringRef AMD_API AMRLocalizedCopyStringForAMROperation(int operation) CF_RETURNS_RETAINED;

/*!
 @function AMRLocalizedCopyStringForAMRError
 @description Copies the localized string for a restore error.

 @param error restore error to look up
 @return string description of the restore error
 */
CFStringRef AMD_API AMRLocalizedCopyStringForAMRError(int error) CF_RETURNS_RETAINED;

/*!
 @function AMRestorableCommitStashBag
 @description Attempts to commit a stash bag to prevent Preboard UI from being shown

 @param device to commit stash bag to
 @param progressHandler Callback for progress, status, and completion events.
 @param context Any user provided data that will be passed to progressHandler as argument
 @return successfull if the stash bag has been created
 */
bool AMD_API AMRestorableCommitStashBag( AMRestorableDeviceRef device,
                                         AMRestorableDeviceProgressUpdateCallback progressHandler,
                                         void *context );

__AMR_END_DECLS

#endif
