#if !defined(AMRESTORE_H)
#define AMRESTORE_H 1

#include <CoreFoundation/CoreFoundation.h>

#if TARGET_OS_IPHONE || TARGET_OS_EMBEDDED || defined(__WIN32__)
#include <AMRestoreOptions.h>
#include <AMAuthInstall.h>
#else
#include <amd/AMRestoreOptions.h>
#include <amd/AMAuthInstall.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif
/*
 *  AMRestoreSetLocationIDFilter
 *    Limits notifications to a specific locationID
 *    Useful for multi-process automation, does not work on windows
 */
void AMRestoreSetLocationIDFilter(uint32_t locationID);
    
/*
 *  AMRestoreGetSupportedPayloadVersion
 *    Returns a value indicating the payload version this library supports.
 *    Clients can use this value to request the appropriate payload.  This value
 *    will only change when the payload changes in such a way to require a new
 *    version of the restore library.
 */
uint32_t AMRestoreGetSupportedPayloadVersion(void);

/*
 *  AMDFUModeDeviceRef
 *    This is the type that represents a device in DFU.
 */
typedef struct __AMDFUModeDevice *AMDFUModeDeviceRef;

/*
 *  AMRecoveryModeDeviceRef
 *    This is the type that represents a device in recovery mode (iBoot).
 */
typedef struct __AMRecoveryModeDevice *AMRecoveryModeDeviceRef;

/*
 *  AMRestoreModeDeviceRef
 *    This is the type that represents a device booted into the restore
 *    environment.
 */
typedef struct __AMRestoreModeDevice *AMRestoreModeDeviceRef;

/*
 *  AMDFUModeDeviceGetTypeID
 *  AMRecoveryModeDeviceGetTypeID
 *  AMRestoreModeDeviceGetTypeID
 *    Return the type identifier for each of the above three CF types.
 */
CFTypeID AMDFUModeDeviceGetTypeID(void);
CFTypeID AMRecoveryModeDeviceGetTypeID(void);
CFTypeID AMRestoreModeDeviceGetTypeID(void);

/*
 *  AMDFUModeDeviceGetLocationID
 *  AMRecoveryModeDeviceGetLocationID
 *  AMRestoreModeDeviceGetLocationID
 *    Return the USB location id for the device.
 */
uint32_t AMDFUModeDeviceGetLocationID(AMDFUModeDeviceRef device);
uint32_t AMRecoveryModeDeviceGetLocationID(AMRecoveryModeDeviceRef device);
uint32_t AMRestoreModeDeviceGetLocationID(AMRestoreModeDeviceRef device);

/*
 *  AMDFUModeDeviceGetProductID
 *  AMRecoveryModeDeviceGetProductID
 *    Return the USB product id for the device.
 */
uint16_t AMDFUModeDeviceGetProductID(AMDFUModeDeviceRef device);
uint16_t AMRecoveryModeDeviceGetProductID(AMRecoveryModeDeviceRef device);

/*
 *  AMDFUModeDeviceGetProductType
 *  AMRecoveryModeDeviceGetProductType
 *    These functions return the "product type" of a device in DFU mode or
 *    recovery mode.  The "product type" is a 32 bit value that identifies the
 *    hardware of the device.
 *    Note that the same device will likely have a different product type in 
 *    DFU and recovery mode.
 */
uint32_t AMDFUModeDeviceGetProductType(AMDFUModeDeviceRef device);
uint32_t AMRecoveryModeDeviceGetProductType(AMRecoveryModeDeviceRef device);

/*
 *  AMDFUModeDeviceGetECID
 *  AMRecoveryModeDeviceGetECID
 *    These functions return the ECID of the device.  The ECID is a 64 bit
 *    unique identifier.
 *    This function returns 0 if an error occurs reading the ECID from the
 *    device, or if the device does not expose an ECID.
 */
uint64_t AMDFUModeDeviceGetECID(AMDFUModeDeviceRef device);
uint64_t AMRecoveryModeDeviceGetECID(AMRecoveryModeDeviceRef device);

/*
 *  AMDFUModeDeviceGetChipID
 *  AMRecoveryModeDeviceGetChipID
 *    These functions return the chip id of the device. The chip id is a
 *    32 bit identifier that identifies the SoC.
 */
uint32_t AMDFUModeDeviceGetChipID(AMDFUModeDeviceRef device);
uint32_t AMRecoveryModeDeviceGetChipID(AMRecoveryModeDeviceRef device);

/*
 *  AMDFUModeDeviceGetBoardID
 *  AMRecoveryModeDeviceGetBoardID
 *    These functions return the board id of the device. The board id is a
 *    32 bit identifier that identifies the board revision of the device.
 */
uint32_t AMDFUModeDeviceGetBoardID(AMDFUModeDeviceRef device);
uint32_t AMRecoveryModeDeviceGetBoardID(AMRecoveryModeDeviceRef device);

/*
 *  AMDFUModeDeviceGetSecurityDomain
 *  AMRecoveryModeDeviceGetSecurityDomain
 *    These functions return the security domain of the device. The security domain is a
 *    32 bit identifier that identifies the security domain the device uses.
 */
uint32_t AMDFUModeDeviceGetSecurityDomain( AMDFUModeDeviceRef device );
uint32_t AMRecoveryModeDeviceGetSecurityDomain( AMRecoveryModeDeviceRef device );

/*
 * AMDFUModeDeviceGetOperation
 * AMRecoveryModeDeviceGetOperation
 * AMRestoreModeDeviceGetOperation
 *    These functions return the current restore operation. The operation is
 *    an enum. The values are defined in AMRestore.h
 */
uint32_t AMDFUModeDeviceGetOperation( AMDFUModeDeviceRef device );
uint32_t AMRecoveryModeDeviceGetOperation( AMRecoveryModeDeviceRef device );
uint32_t AMRestoreModeDeviceGetOperation( AMRestoreModeDeviceRef device );

/*
 *  AMDFUModeDeviceGetProductionMode
 *  AMDFUModeDeviceGetEffectiveProductionMode
 *  AMRecoveryModeDeviceGetProductionMode
 *  AMRecoveryModeDeviceGetEffectiveProductionMode
 *  AMRestoreModeDeviceGetProductionMode
 *    These functions return the production mode of the device. The production mode is a
 *    boolean that identifies the if the device is production fused. The effective variants
 *    reflect the demotion state of the device.
 */
Boolean AMDFUModeDeviceGetProductionMode( AMDFUModeDeviceRef device );
Boolean AMDFUModeDeviceGetEffectiveProductionMode( AMDFUModeDeviceRef device );

Boolean AMRecoveryModeDeviceGetProductionMode( AMRecoveryModeDeviceRef device );
Boolean AMRecoveryModeDeviceGetEffectiveProductionMode( AMRecoveryModeDeviceRef device );

Boolean AMRestoreModeDeviceGetProductionMode( AMRestoreModeDeviceRef device );

/*
 *  AMDFUModeDeviceGetSecurityMode
 *  AMDFUModeDeviceGetEffectiveSecurityMode
 *  AMRecoveryModeDeviceGetSecurityMode
 *  AMRecoveryModeDeviceGetEffectiveSecurityMode
 *
 *    These functions return the security mode of the device. The security mode is a
 *    boolean that identifies the if security is enbabled on the device. The effective variants
 *    reflect the demotion state of the device.
 */
Boolean AMDFUModeDeviceGetSecurityMode( AMDFUModeDeviceRef device );
Boolean AMDFUModeDeviceGetEffectiveSecurityMode( AMDFUModeDeviceRef device );

Boolean AMRecoveryModeDeviceGetSecurityMode( AMRecoveryModeDeviceRef device );
Boolean AMRecoveryModeDeviceGetEffectiveSecurityMode( AMRecoveryModeDeviceRef device );

/*
 *  AMDFUModeDeviceGetImage4Aware
 *  AMRecoveryModeDeviceGetImage4Aware
 *
 *    These functions returns whether the device supports the image4 format. 
 */
Boolean AMDFUModeDeviceGetImage4Aware( AMDFUModeDeviceRef device );
Boolean AMRecoveryModeDeviceGetImage4Aware( AMRecoveryModeDeviceRef device );

/*
 *  AMDFUModeDeviceIsBootstrapOnly
 *  AMRecoveryModeDeviceIsBootstrapOnly
 *    These functions return true if the connected device has the BootstrapOnly
 *    flag set to true in the Device Map.
 */
Boolean AMDFUModeDeviceIsBootstrapOnly(AMDFUModeDeviceRef device, CFDictionaryRef options);
Boolean AMRecoveryModeDeviceIsBootstrapOnly(AMRecoveryModeDeviceRef device, CFDictionaryRef options);

/*
 *  AMDFUModeDeviceResetAuthInstallSettings
 *    This function clears the libauthintall information associated with a device.
 */
void AMDFUModeDeviceResetAuthInstallSettings( AMDFUModeDeviceRef device );
void AMRecoveryModeDeviceResetAuthInstallSettings( AMRecoveryModeDeviceRef device );
void AMRestoreModeDeviceResetAuthInstallSettings( AMRestoreModeDeviceRef device );
    
/*
 * AMRAuthInstallCopyURLForKey
 *  Get the URL or path to a resource in the personalized bundle. Personalization must be enabled
 *  and there must be a variant in the options.
 */
int
AMRAuthInstallCopyURLForKey(
                            AMAuthInstallRef amai,
                            CFDictionaryRef options,
                            CFStringRef aKey,
                            CFURLRef *outURL );
    
/*
 *  AMRecoveryModeGetSoftwareBuildVersion
 *    Returns the software build version from the device.  If the device
 *    does not have a build version, 0 is returned.  -1 is returned if the
 *    version cannot be retrieved.
 */
int AMRecoveryModeGetSoftwareBuildVersion(AMRecoveryModeDeviceRef device);

/*
 *  AMRestoreModeDeviceCopyRestoreLog
 *    Attempts to contact the device and fetch a log of the restore from it.
 *    For this to work, the device needs to have restored running so this is 
 *    most useful after a failed restore to attempt to fetch information from
 *    the device before it is restarted.
 */
int AMRestoreModeDeviceCopyRestoreLog(AMRestoreModeDeviceRef device, CFStringRef *log);

/*
 *  AMRestoreEnableFileLogging
 *    Enable logging to the specified file.
 */
int AMRestoreEnableFileLogging(const char *logFile);

/*
 *  AMRestoreSetLogLevel
 *    Set the log level - all logs lower (more verbose) than the specified 
 *    level are suppressed.  The log level should be specified in terms of
 *    a syslog-style constant
 */
void AMRestoreSetLogLevel(int logLevel);
    
/*
 *  AMRestoreDisableFileLogging
 *    Stop logging to a file.
 */
void AMRestoreDisableFileLogging(void);

/*
 * AMRestoreUseFactoryLoggingFormat
 * This tells AMRLogv to to use a different logging format than usual.
 */
void AMRestoreUseFactoryLoggingFormat(int flag);
    
/*
 * AMRestoreLogFilePaths
 * If set to 1, log full file paths to console when AMRLogWithFilePath(...) is called. By default
 * this is set to 0 (disabled).
 */
void AMRestoreLogFilePaths(int flag);

/*
 *AMRestoreScrubLogs
 *Scrub logs to remove sensitive customer information. Value of 1 enables scrubbing, 0 disables it. 
 *Scrubbing is enabled by default.
 */
void AMRestoreScrubLogs(int flag);

/*AMRestoreIsScrubbingEnabled
 *Return the flag that determines if scrubbing logs will occur. 
 *Return value of 1 means scrubbing is enabled, 0 means disabled.
 */
int AMRestoreIsScrubbingEnabled();
    
/*
 *  AMRLog
 *  AMRLogv
 *  Log functions.
 */
void AMRLog(int level, CFStringRef format, ...);
void AMRLogv(int level, CFStringRef format, va_list args);
  
/*
 * AMRLogWithFilePath
 *   Log a message related to a file. If AMRestoreLogFilePaths(1) is set
 *   then the full path will be logged following the message for diagnostics.
 */
void AMRLogWithFilePath(int level, CFStringRef filePath, CFStringRef format, ...); 

/*
 * AMRLogSetCallbackHandler
 *   Set a callback handler to received messages set to AMRLog(..) function.
 *   The handler is invoked after logging to all other facilities and only
 *   if the message is logged within the current logging mask.
 */
typedef void (*logCallbackHandler) ( CFStringRef logMessage, int logLevel );
void AMRLogSetCallbackHandler( logCallbackHandler handler );

/*
 *  AMRestoreCreateBootArgsByAddingArg
 *  AMRestoreCreateBootArgsByRemovingArg
 *    These are convenience functions for the manipulation of boot-args
 *    strings.  They allow you to add and remove arguments (optionally with
 *    values) to a string that represents boot-args.
 *    These functions do not actual change boot-args anywhere, they are merely
 *    string manipulation functions.
 *    These functions return a new string.
 */
CFStringRef AMRestoreCreateBootArgsByAddingArg(CFStringRef bootArgs, CFStringRef arg, CFStringRef value);
CFStringRef AMRestoreCreateBootArgsByRemovingArg(CFStringRef bootArgs, CFStringRef arg);

/*
 *  AMRestoreModeDeviceCreate
 *    Create a new object to represent a device booted into the restore
 *    environment.
 *    deviceID is the USB mux device identifier
 *    locationID is the USB location ID for the device (queried from usbmuxd)
 */
AMRestoreModeDeviceRef AMRestoreModeDeviceCreate(CFAllocatorRef allocator, uint32_t deviceID, uint32_t locationID);

/*
 *  AMRestoreModeDeviceGetDeviceID
 *    Return the USB mux identifier for the device.
 */
uint32_t AMRestoreModeDeviceGetDeviceID(AMRestoreModeDeviceRef device);

/*
 *  AMRestoreModeDeviceGetFDRUserInfo
 *    Return a dictionary containing missing or unverified FDR data
 */
CFDictionaryRef AMRestoreModeDeviceGetFDRUserInfo(AMRestoreModeDeviceRef device);

/*
 *  AMRestoreModeDeviceSetFDRUserInfo
 *    Sets the user info dictionary on the device ref
 */
void AMRestoreModeDeviceSetFDRUserInfo(AMRestoreModeDeviceRef device, CFDictionaryRef userInfo);

/*
 *  AMRecoveryModeDeviceCopySerialNumber
 *  AMRestoreModeDeviceCopySerialNumber
 *    Return a copy of the device serial number.  The returned value must be
 *    released.  Note that the serial number may be invalid if it was not 
 *    written to NOR prior to boot.
 */
CFStringRef AMRecoveryModeDeviceCopySerialNumber(AMRecoveryModeDeviceRef device);
CFStringRef AMRestoreModeDeviceCopySerialNumber(AMRestoreModeDeviceRef device);


/*
 *  AMRecoveryModeDeviceCopyIMEI
 *  AMRestoreModeDeviceCopyIMEI
 *    Return a copy of the device IMEI.  Note that the IMEI does not exist
 *    on all hardware (specifically, it will not be available on non-iPhone
 *    devices).  Further, this API requires device-side support to work, so
 *    when restoring software older than LittleBear (4AXX), this function will
 *    return NULL regardless of the type of device used.
 */
CFStringRef AMRecoveryModeDeviceCopyIMEI(AMRecoveryModeDeviceRef device);
CFStringRef AMRestoreModeDeviceCopyIMEI(AMRestoreModeDeviceRef device);

/*
 *  AMDFUModeDeviceCopyAPNonce
 *  AMRecoveryModeDeviceCopyAPNonce
 *    Copy the AP nonce for DFU or recovery mode devices. Returns
 *    kAMRestoreNonceUnsupportedError if the device does not support nonce
 *    generation.
 */
int AMDFUModeDeviceCopyAPNonce(AMDFUModeDeviceRef device, CFStringRef *nonce);
int AMRecoveryModeDeviceCopyAPNonce(AMRecoveryModeDeviceRef device, CFStringRef *nonce);

/*
 *  AMDFUModeDeviceCopySEPNonce
 *  AMRecoveryModeDeviceCopySEPNonce
 *    Copy the SEP nonce for DFU or recovery mode devices. Returns
 *    kAMRestoreNonceUnsupportedError if the device does not support nonce
 *    generation.
 */
int AMDFUModeDeviceCopySEPNonce(AMDFUModeDeviceRef device, CFStringRef *nonce);
int AMRecoveryModeDeviceCopySEPNonce(AMRecoveryModeDeviceRef device, CFStringRef *nonce);

/*
 *  AMDFUModeDeviceGetSecurityEpoch
 *  AMRecoveryModeDeviceGetSecurityEpoch
 *    Return the current security epoch of the device, or -1 on error.
 *    Note that the DFU version of this function may return bogus results on
 *    a device in chip DFU on some hardware.
 */
int AMDFUModeDeviceGetSecurityEpoch(AMDFUModeDeviceRef device);
int AMRecoveryModeDeviceGetSecurityEpoch(AMRecoveryModeDeviceRef device);

/*
 *  AMRecoveryModeDeviceSetAutoBoot
 *    Enables or disables auto-boot on the device
 */
int AMRecoveryModeDeviceSetAutoBoot(AMRecoveryModeDeviceRef device, Boolean enable);

/*
 *  AMRecoveryModeDeviceReboot
 *    Issue a command to the device to trigger a reboot.  The state of the
 *    device after this call may vary.  For instance, if the contents of NOR
 *    are not valid, the device may end up in DFU.  If the contents are valid
 *    and auto-boot is true, the device will attempt to boot into the OS.
 */
int AMRecoveryModeDeviceReboot(AMRecoveryModeDeviceRef device);

/*
 *  AMRestoreModeDeviceReboot
 *    Request the device to reboot.  This will not toggle auto-boot, so if it
 *    is off, the device will end up in recovery mode.  If it is on, it will
 *    attempt to boot into the OS.
 */
int AMRestoreModeDeviceReboot(AMRestoreModeDeviceRef device);

/*
 * AMRestoreEnableExtraDFUDevices
 *   By default, the restore library knows about (and will notify clients of)
 *   a set of devices with a specific set of USB product IDs.  This API allows
 *   a client to expand the list of recognized PIDs for devices in DFU mode.
 *   The keys in the device ID dictionary are:
 *     kAMRDFUDeviceInfoPID - the USB PID for the device in DFU mode (assumes
 *      the vendor is Apple)
 *
 *   Note that this function must be called before the first call to
 *   AMRestoreRegisterForDeviceNotifications
 */
#define kAMRDFUDeviceInfoPID CFSTR("PID")
int AMRestoreEnableExtraDFUDevices(CFArrayRef dfuInfo);

/*
 *  AMDFUModeDeviceConnectedCallback
 *  AMRecoveryModeDeviceConnectedCallback
 *    Type for the callback when a recovery mode device is connected.
 *    device is the newly connected device.
 *    userInfo is the data provided by the client in the call to
 *      AMRestoreRegisterForDeviceNotifications (see below).
 */
typedef void (*AMDFUModeDeviceConnectedCallback)(AMDFUModeDeviceRef device, void *userInfo);
typedef void (*AMRecoveryModeDeviceConnectedCallback)(AMRecoveryModeDeviceRef device, void *userInfo);

/*
 *  AMDFUModeDeviceDisconnectedCallback
 *  AMRecoveryModeDeviceDisconnectedCallback
 *    Type for the callback when a recovery mode device is disconnected.
 *    device is the device that has been disconnected.
 *    userInfo is the data provided by the client in the call to
 *      AMRestoreRegisterForDeviceNotifications (see below).
 */
typedef void (*AMDFUModeDeviceDisconnectedCallback)(AMDFUModeDeviceRef device, void *userInfo);
typedef void (*AMRecoveryModeDeviceDisconnectedCallback)(AMRecoveryModeDeviceRef device, void *userInfo);

/*
 *  AMDeviceErrorCallback
 *    Type for the callback when a recovery mode device is detected but an
 *    error occurs setting it up for use.
 *    error is an errno value that indicates the problem.
 *    userInfo is the data provided by the client in the call to
 *      AMRestoreRegisterForDeviceNotifications (see below).
 */
typedef void (*AMDeviceErrorCallback)(int error, void *userInfo);


/* 
 *  AMRestoreRegisterForDeviceNotifications
 *    Register for notifications about devices in DFU and recovery modes.  
 *    Below, "connected" and "disconnected" do not necessarily refer to a change
 *    in the physical connectedness.  For example, a device will receive a 
 *    disconnect callback when a device leaves DFU mode to enter recovery mode
 *    (it will also receive the corresponding connect callback when it appears
 *    in recovery mode)
 *    
 *    The dfuConnect and recoveryModeConnect callbacks are made when a new 
 *    device in DFU or recovery mode is found, identified and opened.
 *    The dfuDisconnect and recoveryModeDisconnect callbacks are made when
 *    an attached USB device in DFU mode or recovery mode is disconnected
 *    The error callback is made when a USB device that we might be
 *    interested in is discovered but something goes wrong in the initial
 *    handling of the device (we can't identify it, we can't open it for 
 *    exclusive access, etc)
 *    The userInfo parameter is passed to each of the callbacks.
 *
 *    Note that if there are devices in DFU/recovery mode connected when this
 *    call is made, the corresponding connected callbacks may be made 
 *    before this function returns
 *    Also note that a process can only have a single registered client for
 *    these callbacks.  Calling this function multiple times will update that
 *    information but only the callbacks/user info provied in the last call 
 *    will be used.
 *
 *    All callback parameters are required.
 *
 *    This function returns 0 on success.  In the case of failure, it returns
 *    an errno describing the failure or -1 if that information is unavailable.
 */
int AMRestoreRegisterForDeviceNotifications(AMDFUModeDeviceConnectedCallback dfuConnect,
                                            AMRecoveryModeDeviceConnectedCallback recoveryModeConnect,
                                            AMDFUModeDeviceDisconnectedCallback dfuDisconnect,
                                            AMRecoveryModeDeviceDisconnectedCallback recoveryModeDisconnect,
                                            AMDeviceErrorCallback error,
                                            void *userInfo);

/* 
 *  AMRestoreUnregisterForDeviceNotifications
 *    Reverses the work done in AMRestoreRegisterForDeviceNotifications.
 *    Callbacks regestered in AMRestoreRegisterForDeviceNotifications will no
 *    be called after this function has been called. It is safe to call 
 *    AMRestoreRegisterForDeviceNotifications again after calling this function.
 */
int AMRestoreUnregisterForDeviceNotifications();

/*
 *  Restore operation constants
 *    Operation values used in progress callbacks to indicate what the restore
 *    is actually doing.
 *    Nothing should be assumed based on the order of these operations in the
 *    enum, or their integer values.  It says nothing about the order they
 *    will actually be generated in.  Furthermore, a restore may not necessarily
 *    expect to see all these operations.
 */
enum {
    kAMRestoreNoOperation,
    kAMRestoreOperationPurgingOldDevice, /* No longer used */
    kAMRestoreOperationDFUDownload,
    kAMRestoreOperationConnectingToDevice,
    kAMRestoreOperationConnectionEstablished,
    kAMRestoreOperationSendingBootImage,
    kAMRestoreOperationSendingDeviceTree,
    kAMRestoreOperationSendingKernelCache,
    kAMRestoreOperationSettingRestoreBootArgs,
    kAMRestoreOperationBooting,
    
    kAMRestoreOperationFlashingSysCfg, /* 10 */
    kAMRestoreOperationCreatingFilesystemPartitions,
    kAMRestoreOperationCreatingFilesystem,
    kAMRestoreOperationRestoringImage,
    kAMRestoreOperationVerifyingRestore,
    kAMRestoreOperationCheckingFilesystem,
    kAMRestoreOperationMountingFilesystem,
    kAMRestoreOperationFixingUpVar,
    kAMRestoreOperationFlashingNOR,
    kAMRestoreOperationUpdatingBaseband,
    
    kAMRestoreOperationSettingBootStage, /* 20 */ /* No longer used */
    kAMRestoreOperationRebootingDevice,
    kAMRestoreOperationShuttingDownDevice,
    kAMRestoreOperationEnablingAccessoryPower,
    kAMRestoreOperationClearingPersistentBootArgs,
    kAMRestoreOperationModifyingPersistentBootArgs,
    kAMRestoreOperationInstallingRoot,
    kAMRestoreOperationInstallingKernelCache,
    kAMRestoreOperationWaitingForStorageDevice,
    kAMRestoreOperationUnmountingFilesystem,
    
    kAMRestoreOperationSettingTimeOnDevice, /* 30 */
    kAMRestoreOperationBootstrappingSecureBoot,
    kAMRestoreOperationFinalizingNANDEpochUpdate,
    kAMRestoreOperationCheckingForBootablePartitions,
    kAMRestoreOperationCreatingFactoryRestoreMarker,
    kAMRestoreOperationLoadingNORData,
    kAMRestoreOperationRemovingActivationRecord, /* No longer used */
    kAMRestoreOperationCheckingBatteryVoltage,
    kAMRestoreOperationWaitingForBatteryToCharge,
    kAMRestoreOperationClosingModemTickets,

    kAMRestoreOperationMigratingData, /* 40 */
    kAMRestoreOperationWipingStorageDevice,
    kAMRestoreOperationSendingBootGraphic,
    kAMRestoreOperationCheckingForUncollectedLogs,
    kAMRestoreOperationPersonalizingRestoreBundle,
    kAMRestoreOperationClearingNVRAM,
    kAMRestoreOperationUpdatingGasGaugeSoftware,
    kAMRestoreOperationPreparingForBasebandUpdate,
    kAMRestoreOperationBootingBaseband,
    kAMRestoreOperationCreatingSystemKeyBag,

    kAMRestoreOperationUpdatingIRMCU, /* 50 */
    kAMRestoreOperationResizingSystemPartition,
    kAMRestoreOperationCollectingBasebandUpdaterOutput,
    kAMRestoreOperationPairingStockholm,
    kAMRestoreOperationUpdatingStockholm,
    kAMRestoreOperationUpdatingSWDHID,
    kAMRestoreOperationCertifySEP,
    
    kAMRestoreNumOperations,
};


/*
 *  AMDFUModeDeviceGetProgress
 *  AMRecoveryModeDeviceGetProgress
 *  AMRestoreModeDeviceGetProgress 
 *    Return the current operation and progress.
 */
void AMDFUModeDeviceGetProgress(AMDFUModeDeviceRef device, int *operation, int *progress);
void AMRecoveryModeDeviceGetProgress(AMRecoveryModeDeviceRef device, int *operation, int *progress);
void AMRestoreModeDeviceGetProgress(AMRestoreModeDeviceRef device, int *operation, int *progress);


/*
 * AMRestoreModeDeviceGetRestoreOSCompleted
 *   Returns if restored has completed the restore process
 */
Boolean AMRestoreModeDeviceGetRestoreOSCompleted(AMRestoreModeDeviceRef device);

/*
 * Error domain for AMRestore CFError's.
 */
#define kAMRestoreErrorDomain CFSTR("AMRestoreErrorDomain")

/*
 *  Restore result constants
 *    Return values from restore API to indicate different types of errors.
 */
enum {
    kAMRestoreErrorGenericError = -1,
    kAMRestoreNoError = 0,
    kAMRestoreErrorBadArgument = 1,
    kAMRestoreErrorConnectionFailed = 2,
    kAMRestoreErrorNotRestoreDevice = 3,
    kAMRestoreErrorUnsupportedDeviceSoftware = 4,
    kAMRestoreErrorUnsupportedDeviceHardware = 5,
    kAMRestoreErrorOperationFailed = 6,
    kAMRestoreErrorBadAck = 7,
    kAMRestoreErrorGotNAck = 8,
    kAMRestoreErrorCommunicationError = 9,
    kAMRestoreErrorProtocolError = 10,
    kAMRestoreErrorFileError = 11,
    kAMRestoreErrorFileOpenError = 12,
    kAMRestoreErrorIOError = 13,
    kAMRestoreErrorASRError = 14,
    kAMRestoreErrorBomError = 15,
    kAMRestoreErrorPermissionDenied = 16,
    kAMRestoreErrorPathDoesNotExist = 17,
    kAMRestoreErrorMemoryAllocationError = 18,
    kAMRestoreErrorBadPropertyList = 19,
    kAMRestoreErrorEpochFail = 20,
    kAMRestoreCommandExecutionError = 21,
    kAMRestoreNotImplementedError = 22,
    kAMRestoreRecoveryModeRadioError = 23,
    kAMRestoreServiceNotFoundError = 24,
    kAMRestoreUncollectedLogsError = 25,
    kAMRestoreFilesystemCheckError = 26,
    kAMRestoreFilesystemMountError = 27,
    kAMRestoreNANDTimeoutError = 28,
    kAMRestoreGasGaugeUpdateError = 29,
    kAMRestoreDataJournalingError = 30,
    kAMRestoreErrorConversion = 31,
    kAMRestoreErrorHash = 32,
    kAMRestoreErrorCrypto = 33,
    kAMRestoreKeyBagCreationError = 34,
    kAMRestoreNANDCleaningError = 35,
    kAMRestoreEffaceableStorageError = 36,
    kAMRestoreFirmwareUpdateError = 37,
    kAMRestoreIRMCUUpdateError = 38,
    kAMRestoreSystemPartitionTooSmallError = 39,
    kAMRestoreNANDDriverInitError = 40,
    kAMRestorePartitionLookupError = 41,
    kAMRestoreLwVMFormattingError = 42,
    kAMRestoreErrorNetwork = 43,
    kAMRestoreErrorBadResponse = 44,
    kAMRestoreSystemPartitionTooLargeError = 45,
    kAMRestorePartitioningError = 46,
    kAMRestoreFilesystemFormattingError = 47,
    kAMRestoreNonceUnsupportedError = 48,
    kAMRestoreAppleConnectNotFoundError = 49,
    kAMRestoreErrorUnableToBootSEP = 50,
    kAMRestoreErrorMissingSEPFirmware = 51,
    kAMRestoreErrorFDRNoFilesystem = 52,
    kAMRestoreErrorFDRFailure = 53,
    kAMRestoreErrorSEPEEPROMTestFailure = 54,
    kAMRestoreErrorStockholmPairingFailure = 55,
    kAMRestoreErrorStockholmUpdateFailure = 56,
    kAMRestoreErrorSWDUpdateFailure = 57,
    kAMRestoreErrorSEPCertificationFailure = 58,

    // The following errors are all specific to baseband updates
    kAMRestoreErrorBasebandGenericUserError = 1001,
    kAMRestoreErrorBasebandProtocolError = 1002,
    kAMRestoreErrorBasebandHostWrongEndian = 1003,
    kAMRestoreErrorBasebandBadArguments = 1004,
    kAMRestoreErrorBasebandInvalidBootloader = 1005,
    kAMRestoreErrorBasebandFileSizeMismatch = 1006,
    kAMRestoreErrorBasebandInvalidFLS = 1007,
    kAMRestoreErrorBasebandFLSFileError = 1008,
    kAMRestoreErrorBasebandUnsignedFLS = 1009,
    kAMRestoreErrorBasebandCommLayerInitError = 1010,
    kAMRestoreErrorBasebandOutOfDateBootloader = 1011,
    kAMRestoreErrorBasebandCannotVerifyFLS = 1012,
    kAMRestoreErrorBasebandUnknownError = 1013,
    kAMRestoreErrorBasebandSettingIMEISVFailed = 1014,
    kAMRestoreErrorBasebandUnableToRollBack = 1015,
    kAMRestoreErrorBasebandBadStaticEEP = 1016,
    kAMRestoreErrorBasebandFusingFailed = 1017,
    kAMRestoreErrorBasebandUnpersonalizedFirmwareRejected = 1018,
    kAMRestoreErrorBasebandProvisioningFailed = 1019,
    kAMRestoreErrorBasebandSystemFilesystemNotMounted = 1020,
    kAMRestoreErrorBasebandProvisioningFDRUploadFailed = 1021,

    // USB errors
    kAMRestoreErrorUSBDeviceOpenFailed = 2001,
    kAMRestoreErrorUSBInterfaceOpenFailed = 2002,
    kAMRestoreErrorUSBPipeWriteError = 2003,
    kAMRestoreErrorUSBPipeReadError = 2004,
    kAMRestoreErrorDFUError = 2005,
    kAMRestoreErrorUSBDeviceNotResponding = 2006,
    kAMRestoreErrorUSBDeviceCreateFailed = 2007,
    kAMRestoreUSBPipeStalledError = 2008,
    kAMRestoreUSBDeviceRequestError = 2009,
    kAMRestoreUSBGenericError = 2010,
    kAMRestoreUSBNotEnoughPowerError = 2011,

    // AuthInstall errors
    kAMRestoreErrorAuthInstallBadBuildIdentity = 3000,
    kAMRestoreErrorAuthInstallFileNotFound = 3001,                      /* deprecated */
    kAMRestoreErrorAuthInstallEntryNotFound = 3002,
    kAMRestoreErrorAuthInstallNotImplemented = 3003,
    kAMRestoreErrorAuthInstallServerNotReachable = 3004,
    kAMRestoreErrorAuthInstallInvalidImg3Object = 3005,
    kAMRestoreErrorAuthInstallMissingParameter = 3006,                  /* deprecated */
    kAMRestoreErrorAuthInstallOutOfMemory = 3007,                       /* deprecated */
    kAMRestoreErrorAuthInstallHash = 3008,                              /* deprecated */
    kAMRestoreErrorAuthInstallNullParameter = 3009,                     /* deprecated */
    kAMRestoreErrorAuthInstallNullSession = 3010,                       /* deprecated */
    kAMRestoreErrorAuthInstallNullValue = 3011,                         /* deprecated */
    kAMRestoreErrorAuthInstallEmptyRequest = 3012,                      /* deprecated */
    kAMRestoreErrorAuthInstallServerTimedOut = 3013,                    /* deprecated */
    kAMRestoreErrorAuthInstallInternal = 3014,
    kAMRestoreErrorAuthInstallGeneric = 3015,
    kAMRestoreErrorAuthInstallInvalidFlsObject = 3016,
    kAMRestoreErrorAuthInstallAPProvisioningFailed = 3017,
    kAMRestoreErrorAuthInstallAPAlreadyProvisioned = 3018,
    kAMRestoreErrorAuthInstallCantReachServer = 3020,                   /* deprecated */
    kAMRestoreErrorAuthInstallServerInternal_begin = 3100,
    kAMRestoreErrorAuthInstallServerInternal_end = 3999
};


/*
 *  AMRestoreCreateDefaultOptions
 *    Create a mutable dictionary of default options for a restore.  This
 *    dictionary is suitable for passing in to any of the three restore
 *    functions.  Clients of this API are responsible for at a minimum adding
 *    the appropriate keys to direct the restore functions at what to 
 *    restore (for instance, kAMRestoreOptionsSystemImage).
 *    The behavior of this dictionary:
 *    - Sets appropriate boot-args for booting the restore
 *    - Erases the device and creates 2 new partitions
 *    - Flashes the NOR with production NOR
 *    - Updates the baseband
 *    - Installs the release kernel cache
 *    - Uses the user system image/ram disk when used in conjunction with
 *      kAMRestoreOptionsRestoreBundlePath
 */
CFMutableDictionaryRef AMRestoreCreateDefaultOptions(CFAllocatorRef allocator);


/*
 *  AMRestoreCopySystemRestoreImageTypesInBundleForTarget
 *    For a given target (ie, 'm68'), return a CFArray containing the
 *    supported system image types.
 */
CFArrayRef AMRestoreCopySystemRestoreImageTypesInBundleForTarget(CFStringRef bundlePath, CFStringRef target);


/*
 *  AMRestoreCopySystemRestoreImagePath
 *    Return the path to the system restore image of the given type suitable
 *    the given target.  If no image of that type exists for the provided
 *    target, then NULL is returned.  If target is NULL, then the per-target
 *    system images are ignored, and the old default images are used
 */
CFStringRef AMRestoreCopySystemRestoreImagePath(CFStringRef bundlePath, CFStringRef systemImageType, CFStringRef target);


/*
 *  AMDFUModeRestoreProgressCallback
 *    Type for the callback used during the actual restore to indicate
 *    progress.
 *    device is the device for which the callback is being generated.
 *    operation is the operation for which progress is being reported.
 *    progress is an integer progress value.  It will either have a value in
 *    the range 0-100 to indicate progress or -1 if the operation is
 *    indeterminate.
 *    userInfo is the pointer provided by the client.
 */
typedef void (*AMDFUModeRestoreProgressCallback)(AMDFUModeDeviceRef device,
                                                 int operation,
                                                 int progress,
                                                 void *userInfo);


/*
 *  AMRestorePerformDFURestore
 *    Perform the DFU portion of a restore by sending firmware to the device
 *    for it to execute.
 *    device is a reference to the device on which to perform the restore.
 *    options is a dictionary of options controlling the restore (see above
 *    for a description of the keys and their expected values)
 *    callback is a callback made for progress at different points of the
 *    restore.
 *    userInfo is a pointer that will be included with the progress callbacks
 *
 *    Options requirements:
 *    * kAMRestoreOptionsDFUFile or kAMRestoreOptionsFirmwareDirectory must be
 *      specified.  If they are both present, kAMRestoreOptionsDFUFile is used. 
 *    * If kAMRestoreOptionsOnlyPersonalizeDFUFile is present and has a value of
 *      CFBooleanTrue the DFU file will be personalized via the Authorized
 *      Installation mechanism. Only the file specified by the
 *      kAMRestoreOptionsDFUFile key will be personalized. Any restore bundles
 *      will be ignored. If kAMRestoreOptionsAuthInstallEnableAppleConnectSingleSignOn
 *      is also present, UserAuthlisting will be used. Otherwise, personalization
 *      will take place via the DeviceAuthlisting method.
 */
int AMRestorePerformDFURestore(AMDFUModeDeviceRef device, 
                               CFDictionaryRef options, 
                               AMDFUModeRestoreProgressCallback callback,
                               void *userInfo);


/*
 *  AMRecoveryModeRestoreProgressCallback
 *    Type for the callback used during the actual restore to indicate
 *    progress.
 *    device is the device for which the callback is being generated.
 *    operation is the operation for which progress is being reported.
 *    progress is an integer progress value.  It will either have a value in
 *    the range 0-100 to indicate progress or -1 if the operation is
 *    indeterminate.
 *    userInfo is the pointer provided by the client.
 */
typedef void (*AMRecoveryModeRestoreProgressCallback)(AMRecoveryModeDeviceRef device,
                                                      int operation,
                                                      int progress,
                                                      void *userInfo);


/*
 *  AMRestorePerformRecoveryModeRestore
 *    Perform the recovery mode portion of a restore by sending the boot
 *    pieces to the device (kernelcache, boot image, device tree, etc.)
 *    device is a reference to the device on which to perform the restore.
 *    options is a dictionary of options controlling the restore (see above
 *    for a description of the keys and their expected values) 
 *    callback is a callback made for progress at different points of the
 *    restore.
 *    userInfo is a pointer that will be included with the progress callbacks
 */
int AMRestorePerformRecoveryModeRestore(AMRecoveryModeDeviceRef device, 
                                        CFDictionaryRef options, 
                                        AMRecoveryModeRestoreProgressCallback callback,
                                        void *userInfo);


/*
 *  AMRestoreModeRestoreProgressCallback
 *    Type for the callback used during the actual restore to indicate
 *    progress.
 *    device is the device for which the callback is being generated.
 *    operation is the operation for which progress is being reported.
 *    progress is an integer progress value.  It will either have a value in
 *    the range 0-100 to indicate progress or -1 if the operation is
 *    indeterminate.
 *    userInfo is the pointer provided by the client.
 */
typedef void (*AMRestoreModeRestoreProgressCallback)(AMRestoreModeDeviceRef device,
                                                     int operation,
                                                     int progress,
                                                     void *userInfo);


/*
 *  AMRestorePerformRestoreModeRestore
 *    Perform the bulk of a restore on a device that is booted into the 
 *    restore environment.
 *    device is a reference to the device on which to perform the restore.
 *    options is a dictionary of options controlling the restore (see above
 *    for a description of the keys and their expected values) 
 *    callback is a callback made for progress at different points of the
 *    restore.
 *    userInfo is a pointer that will be included with the progress callbacks
 */
Boolean AMRestorePerformRestoreModeRestoreWithError(AMRestoreModeDeviceRef device,
                                                    CFDictionaryRef opts, 
                                                    AMRestoreModeRestoreProgressCallback callback,
                                                    void *userInfo,
                                                    CFErrorRef *error);

int AMRestorePerformRestoreModeRestore(AMRestoreModeDeviceRef device,
                                   CFDictionaryRef options, 
                                   AMRestoreModeRestoreProgressCallback callback,
                                   void *userInfo);

    
    
/*
 *  AMRestoreType
 *    represents the type of restore.
 *    AMRestoreNone: No restore or no failure took place.
 *    AMRestoreRestore: The restore was an Update or Erase install.
 *    AMRestoreOTAUpdate The restore was an OTA Software Update.
 */
typedef enum {
    AMRestoreNone,
    AMRestoreRestore,
    AMRestoreOTAUpdate
} AMRestoreType;

/*
 * Keys returned in restoreMetadata by AMRestoreModeGetLastFailureLog
 *
 *  kAMRestoreFailureBaseVersion: version of iOS the restore started from. CFStringRef
 *  kAMRestoreFailureTargetVersion: version of iOS restore was attempting to install. CFStringRef
 *  kAMRestoreFailureRestoreError: reason for the restore failure. CFStringRef
 */
#define kAMRestoreFailureBaseVersion CFSTR("BaseVersion")
#define kAMRestoreFailureTargetVersion CFSTR("TargetVersion")
#define kAMRestoreFailureRestoreError CFSTR("RestoreError")

/*
 *  AMRestoreModeGetLastFailureLog
 *    Ask the restore library if there was a log from a previous restore failure
 *    on the device. If there was no previous log lastFailureLog will be set to NULL
 *    and restoreType will be set to AMRestoreNone. If a previous restore log exists
 *    lastFailureLog will contain the log and restoreType will indicate the type
 *    of the restore that failed. restoreMetadata will contain metadata about the failed
 *    restore.
 */
int AMRestoreModeGetLastFailureLog(AMRestoreModeDeviceRef device, 
                        CFStringRef *lastFailureLog, 
                        AMRestoreType *restoreType,
                        CFDictionaryRef *restoreMetadata);

/*
 *  AMRestoreAcquireDeviceLock
 *    Attempts to acquire an exclusive lock for the device at locationID 
 *    Returns true if the lock was acquired.  False otherwise.
 */
Boolean AMRestoreAcquireDeviceLock(uint32_t locationID);

/*
 *  AMRestoreReleaseDeviceLock
 *    Releases the exclusive lock on the device at locationID.
 *    This function should be called if the lock was successfully acquired.
 */
Boolean AMRestoreReleaseDeviceLock(uint32_t locationID);

/*
 *  AMRAuthInstallCopyLocalizedStringForServerError
 *    Given a status code in the range of 
 *    kAMRestoreErrorAuthInstallServerInternal_begin and 
 *    kAMRestoreErrorAuthInstallServerInternal_end (inclusive), return a 
 *    corresponding localized error string.  If no localized string exists for the
 *    error code, NULL is returned.
 */
CFStringRef AMRAuthInstallCopyLocalizedStringForServerError(int status);

/*
 *  AMRAuthInstallPreflight
 *    Given a dictonary containing AuthInstall parameters, confirm that it's safe
 *    to proceed with the restore.  This includes verifying connectivity to the
 *    signing server.
 *
 *    This function is deprecated.  Use one of the device-specific preflight 
 *    functions instead.
 */ 
Boolean AMRAuthInstallPreflight(CFDictionaryRef options);

/*
 *  AMRAuthInstallCopyPreflightOptions
 *    Given the specified parameters copy and modify the baseOptions so they are suitable for a preflighted restore.
 */
int AMRAuthInstallCopyPreflightOptions(CFMutableDictionaryRef apPreflightDictionary,
                                       CFDictionaryRef basebandPreflightDictionary,
                                       CFDictionaryRef baseOptions,
                                       CFMutableDictionaryRef *outPreflightOptions );

/*
 *  AMRecoveryModeDeviceCopyAuthInstallPreflightOptions
 *    Given an AMRecoveryModeDeviceRef and an options dictionary, attempt to contact
 *    the signing server and personalize the bundle.  On success, the outPreflightOptions 
 *    dictionary will be non-NULL and its contents will contain the original restore 
 *    options modified to use the newly-personalized restore bundle.  On error, the 
 *    outPreflightOptions dictionary will be NULL.
 */
int AMRecoveryModeDeviceCopyAuthInstallPreflightOptions(AMRecoveryModeDeviceRef device,
                                                        CFDictionaryRef baseOptions,
                                                        CFDictionaryRef *outPreflightOptions);

/*
 *  AMDFUModeDeviceCopyAuthInstallPreflightOptions
 *    Given an AMDFUModeDeviceRef and an options dictionary, attempt to contact
 *    the signing server and personalize the bundle.  On success, the outPreflightOptions 
 *    dictionary will be non-NULL and its contents will contain
 *    the original restore options modified to use the newly-personalized restore 
 *    bundle.  On error, the outPreflightOptions dictionary will be NULL.
 */
int AMDFUModeDeviceCopyAuthInstallPreflightOptions(AMDFUModeDeviceRef device,
                                                   CFDictionaryRef baseOptions,
                                                   CFDictionaryRef *outPreflightOptions);

/*
 *  AMRecoveryModeDeviceRequestAbbreviatedSendSync
 *    Send an abbreviated request to the personalization server from recovery
 */
int AMRecoveryModeDeviceRequestAbbreviatedSendSync(
    AMRecoveryModeDeviceRef device,
    CFDataRef uniqueBuildID,
    CFDictionaryRef *outResponse);

/*
 *  AMDFUModeDeviceRequestAbbreviatedSendSync
 *    Send an abbreviated request to the personalization server from DFU
 */
int AMDFUModeDeviceRequestAbbreviatedSendSync(
    AMDFUModeDeviceRef device,
    CFDataRef uniqueBuildID,
    CFDictionaryRef *outResponse);

/*
 * AMRestoreModeDeviceCopyEcid
 * Get the ECID of a device in restore mode.
 */
CFNumberRef AMRestoreModeDeviceCopyEcid(AMRestoreModeDeviceRef device);

/*
 * AMRestoreModeDeviceCopyChipID
 * Get the chip id of a device in restore mode.
 */
CFNumberRef AMRestoreModeDeviceCopyChipID( AMRestoreModeDeviceRef device );

/*
 * AMRestoreModeDeviceCopyBoardID
 * Get the board id of a device in restore mode.
 */
CFNumberRef AMRestoreModeDeviceCopyBoardID( AMRestoreModeDeviceRef device );

/*
 * AMRestoreModeDeviceCopyProductionMode
 * Get the production mode of a device in restore mode.
 */
CFBooleanRef AMRestoreModeDeviceCopyProductionMode( AMRestoreModeDeviceRef device );

/*
 * AMRestoreModeDeviceCopySecurityMode
 * Get the  security mode of a device in restore mode.
 */
CFBooleanRef AMRestoreModeDeviceCopySecurityMode( AMRestoreModeDeviceRef device );

/*
 * AMRestoreModeDeviceCopyEffectiveProductionMode
 * Get the effective production mode of a device in restore mode.
 */
CFBooleanRef AMRestoreModeDeviceCopyEffectiveProductionMode( AMRestoreModeDeviceRef device );

/*
 * AMRestoreModeDeviceCopyEffectiveSecurityMode
 * Get the effective security mode of a device in restore mode.
 */
CFBooleanRef AMRestoreModeDeviceCopyEffectiveSecurityMode( AMRestoreModeDeviceRef device );

/*
 * AMRestoreModeDeviceCopyImage4Aware
 * Get the device's ability to support image4 in restore mode.
 */
CFBooleanRef AMRestoreModeDeviceCopyImage4Aware( AMRestoreModeDeviceRef device );

/*
 * AMRecoveryModeDeviceCopyBoardConfig
 * Get board config string of a device in recovery mode.
 */
CFStringRef AMRecoveryModeDeviceCopyBoardConfig( AMRecoveryModeDeviceRef device, CFDictionaryRef deviceMap );

/*
 * AMDFUModeDeviceCopyBoardConfig
 * Get board config string of a device in DFU mode.
 */
CFStringRef AMDFUModeDeviceCopyBoardConfig( AMDFUModeDeviceRef device, CFDictionaryRef deviceMap );

/*
 * AMRestoreModeDeviceCopyBoardConfig
 * Get board config string of a device in restore mode.
 */
CFStringRef AMRestoreModeDeviceCopyBoardConfig( AMRestoreModeDeviceRef device );

/*
 *  AMRAuthInstallProxyCredentialsCallback
 *    Type for the callback used when an attempt to authenticate with an HTTP proxy 
 *    fails due to invalid or missing credentials.  This callback is issued on every 
 *    failed attempt to authenticate with the proxy; the application is responsible 
 *    for implementing any policy limiting the number of retries.
 *
 *    If the application maintains a cache of credentials, it should first look in its 
 *    cache and return any credentials appropriate for the given proxy.  If not, or if 
 *    the callback is issued again indicating that the cached credentials failed, the 
 *    application should prompt the user to manually enter his credentials.
 *
 *    proxyURL is the URL of the proxy requiring credentials
 *    outUsername is a copy of the username provided by the application
 *    outPassword is a copy of the password provided by the application
 *    outSecureAuthentication indicates whether the credentials will be sent in a
 *        secure manner (e.g., digest authentication)
 *    outSaveInKeychain indicates whether the user wishes to have valid credentials
 *        stored in the keychain.  Invalid credentials will not be saved regardless
 *        of this value.
 *    userInfo is the pointer that was registered with the callback
 *
 *    The callback should return TRUE if credentials are provided or FALSE if the user
 *    chooses to not provide them.
 */
typedef Boolean (*AMRAuthInstallProxyCredentialsCallback)(CFURLRef proxyURL,
                                                          Boolean secureAuthentication,
                                                          CFStringRef *outUsername,
                                                          CFStringRef *outPassword,
                                                          Boolean *outSaveInKeychain,
                                                          void *userInfo);

/*
 *  AMRAuthInstallRegisterProxyCredentialsCallback
 *    Registers a callback for prompting the user for proxy credentials on
 *    authentication failure.  If no callback is registered, proxy authentication
 *    failures are treated as fatal.
 *
 *    This should be called once during initialization, before any other AuthInstall 
 *    APIs are called.
 */
void AMRAuthInstallRegisterProxyCredentialsCallback(AMRAuthInstallProxyCredentialsCallback callback,
                                                    void *userInfo);

/*
 *  AMRAuthInstallFinalize
 *    Given the options dictionary used to drive the restore, perform any final
 *    post-restore cleanup, including removal of tmp directories created during
 *    the restore.
 */
void AMRAuthInstallFinalize(CFDictionaryRef options);

/*
 * AMRestoreCopyPublishedVariantsArrayInBundle
 *  Copy the available variants in a restore bundle.
 */
CFArrayRef AMRAuthInstallCopyPublishedVariantsArrayInBundle( CFStringRef bundlePath );

/*
 * AMRestoreCopyVariantNamesForBoardConfig
 *  Copy the list of variants that are available for a certain board config.
 */
CFArrayRef AMRAuthInstallCopyVariantNamesForBoardConfig( CFStringRef boardConfig, CFArrayRef allVariants );

/*
 * AMRestoreRequestAbbreviatedSendSync
 * Send an abbreviated request to the personalization server.
 */ 
int
AMRestoreRequestAbbreviatedSendSync(
    uint64_t ecID,
    uint32_t chipID,
    uint32_t boardID,
    uint32_t securityDomain,
    Boolean isProductionMode,
    CFDataRef uniqueBuildID,
    CFDictionaryRef *outResponse );

/*
 * AMRestoreGetTransformedDFUType
 *  Transform DFU value found in Build Manifest or Device Map to value found in libusbrestore options.
 */ 
CFStringRef
AMRestoreGetTransformedDFUType(
    CFStringRef value);

/*
 * AMRestoreGetTransformedFirmwareTypeValue
 *  Transform firmware type value found in Build Manifest or Device Map to value found in libusbrestore options.
 */ 
CFStringRef
AMRestoreGetTransformedFirmwareTypeValue(
    CFStringRef value);

/*
 * AMRestoreOptionsAreLegacy
 *   Tells you if the restore options format is legacy or not. If the IsLegacy key is not set in the restore options,
 *   isLegacy is determined by the absence of a restore variant.
 */
Boolean
AMRestoreOptionsAreLegacy(
    CFDictionaryRef options );
    
#ifdef __cplusplus
}
#endif

#endif
