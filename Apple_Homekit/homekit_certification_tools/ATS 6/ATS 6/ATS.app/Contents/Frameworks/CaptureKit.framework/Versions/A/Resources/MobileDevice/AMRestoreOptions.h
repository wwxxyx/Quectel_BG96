//
//  AMRestoreOptions.h
//  PurpleRestore
//
//  Created by Jason Ederle on 10/19/12.
//
//

#ifndef PurpleRestore_AMRestoreOptions_h
#define PurpleRestore_AMRestoreOptions_h

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  Restore options constants
 *    The set of constants that is accepted by the restore mechanism.  Each one
 *    is described below, including the expected type of the corresponding
 *    value, as well as where it is expected (denoted as DFU, RM, and Muxed to
 *    cover DFU restores, recovery mode restores, and muxed restores
 *    respectively).
 *    Unexpected keys are ignored.
 *
 *  kAMRestoreOptionsBoardConfig
 *    Overrides detection of the device type by explicitly specifying the type.
 *    The value is a CFStringRef containing the target/config of the device,
 *    such as 'm68ap' or 'n45dev'.  [DFU, RM]
 *
 *  kAMRestoreOptionsRestoreBundlePath
 *    Provide a path to a restore bundle.  This bundle is supposed to be
 *    a directory with a restore.plist at its root that indicates what
 *    is contained within it.  Other kAMRestoreOptions can be used to override
 *    individual aspects of the plist.  For instance, specifying this option
 *    with kAMRestoreOptionsSystemImage will result in the image referenced by
 *    kAMRestoreOptionsSystemImage being restored, regardless of the contents
 *    of the plist. [DFU, RM, Muxed]
 *
 *  kAMRestoreOptionsSystemImageType
 *    Determines which type of system image to restore.  Currently the options
 *    are user installs and internal installs.  The value should be specified as
 *    one of the kAMRestoreImageType constants below. [Muxed]
 *
 *  kAMRestoreOptionsBootImageType
 *    Determines which type of boot image to use.  Currently the options
 *    are user, internal or update.  The user image limits functionality to
 *    that which is available to customers.  The update image is similar to the
 *    user image but the intention is perform an update that preserves data on
 *    the device.  The value should be specified as one of the
 *    kAMRestoreImageType constants below. [RM]
 *
 *  kAMRestoreOptionsDFUFile
 *    Explicitly specify the DFU file to send to the device, overriding any
 *    logic that might attempt to choose a file based on the type of device
 *    connected.  The value is a CFStringRef containing the path to the file.
 *    [DFU]
 *
 *  kAMRestoreOptionsDFUFileType
 *    Specify the type of DFU file to use.  Currently there are debug,
 *    development, and release versions of the DFU files.  To preserve existing
 *    behavior, if this key is not present, any attempts to perform a DFU
 *    restore will use the development DFU files.  The value for this
 *    key should be one of the kAMRestoreDFU constants below. [DFU]
 *
 *  kAMRestoreOptionsOnlyPersonalizeDFUFile
 *    This option takes a CF boolean value. When true, the DFU file will be
 *    personalized via the authorized installation mechanism. If personalization
 *    fails the restore operation also fails. [DFU]
 *
 *  kAMRestoreOptionsDeviceTreeFile
 *    Explicitly specify the device tree file to send to the device when
 *    booting (this may be different than the device tree that actually gets
 *    written to NOR later).  The value is a CFStringRef containing the path to
 *    the file.  [RM]
 *
 *  kAMRestoreOptionsFirmwareDirectory
 *    Specify the path to a firmware directory in which various
 *    firmware-related pieces may be found (DFU files, all_flash images, etc).
 *    The value is a CFStringRef containing the path to a directory.
 *    [DFU, RM, Muxed]
 *
 *  kAMRestoreOptionsBootImageFile
 *    Specifies the path to the disk image to boot off of (the ramdisk).  The
 *    value is a CFStringRef containing the path to the disk image file. [RM]
 *
 *  kAMRestoreOptionsRestoreKernelCacheFile
 *    Specifies the path to the kernel cache to use at restore time.
 *    The value is a CFStringRef containing the path to the
 *    kernel cache. [DFU, RM, Muxed]
 *
 *  kAMRestoreOptionsKernelCacheFile
 *    Specifies the path to the kernel cache to use to install to disk.
 *    The value is a CFStringRef containing the path to the
 *    kernel cache. [DFU, RM, Muxed]
 *
 *  kAMRestoreOptionsNORImagePath
 *    Explicitly set the path to a NOR image to use.  This image is flashed if
 *    that behavior is requested, and may also be used to find the device tree
 *    used at boot time. The value is a CFStringRef containing the path to the
 *    NOR image. [RM, Muxed]
 *
 *  kAMRestoreOptionsNORImageType
 *    Specifies a type of NOR image to flash.  The library will find the
 *    appropriate image to flash based on the board config.  This option
 *    depends on kAMRestoreOptionsFirmwareDirectory being specified as well.
 *    The value for this should be one of the kAMRestoreNORImageXXX constants
 *    provided below.  [RM, Muxed]
 *
 *  kAMRestoreOptionsBackgroundColor
 *    Specifies the background color of the display for the course of the
 *    restore.  If not specified, the color is unchanged (black).  The value
 *    is a CFArrayRef with 3 elements.  Index 0 is the red component, index 1
 *    is the green component, and index 2 is the blue component.  Each component
 *    is a CFNumberRef holding an integer value in the range 0 to 255. [RM]
 *
 *  kAMRestoreOptionsRestoreBootArgs
 *    Specifies the boot-args value to use when booting into the restore
 *    environment (this is not saved anywhere and does not impact anything but
 *    that one boot).  If this key/value is not provided, a minimal set of
 *    boot-args will be used.  The value is a CFStringRef containing the
 *    boot-args. [RM]
 *
 *  kAMRestoreOptionsSetRecoveryModeOutput
 *    Controls the enabling of output in recovery mode.  If the value is a
 *    CFBooleanRef with a true value, output is enabled.  If the value is a
 *    CFBooleanRef with a false value, output is disabled.  If the key is not
 *    present in the options dictionary, the existing settings are preserved.
 *    [RM]
 *
 *  kAMRestoreOptionsWaitForStorageDevice
 *    Controls whether the restore waits for the storage device /dev/disk0 to
 *    be available before the restore is initiated.  The value is a
 *    CFBooleanRef.  If this key is not present, the default behavior is to
 *    wait for the device to become available. [Muxed]
 *
 *  kAMRestoreOptionsCreateFilesystemPartitions
 *    Controls whether a new partition map is created on the device.  The
 *    value is a CFBooleanRef.  The default behavior is to create a new
 *    map. [Muxed]
 *
 *  kAMRestoreOptionsEraseDataPartition
 *    Specifies whether or not the data partition should be erased during
 *    the restore. By default, the data partition is not erased.  The value
 *    is a CFBooleanRef.  This option is deprecated and
 *    kAMRestoreOptionsCreateFilesystemPartitions should be used instead.  New
 *    versions of the OS may ignore this option entirely. [Muxed]
 *
 *  kAMRestoreOptionsSystemImage
 *    This option controls the image that will be restored to the System.
 * If the key is absent, no restore is performed on
 *    the filesystem.  The value is a CFDictionaryRef.  The
 *    dictionary must include the kAMRestoreOptionsImageFile key, which
 *    provides the path to the image to restore.  If the image is encrypted
 *    and will need a password to restore, the password is specified with the
 *    kAMRestoreOptionsImagePassword key.  Both values in that dictionary are
 *    CFStringRef's.  [Muxed]
 *
 *  kAMRestoreOptionsFlashNOR
 *    Controls whether a new NOR image is flashed during the restore.  The
 *    value of this key is a CFBooleanRef; if it is not present the default
 *    behavior is to not flash. [Muxed]
 *
 *  kAMRestoreOptionsUpdateBaseband
 *    Controls whether the baseband and baseband bootloader are updated as
 *    part of the restore.  The value of this key is a CFBooleanRef.  The
 *    baseband and baseband updater are not updated if this key is not
 *    present. [Muxed]
 *
 *  kAMRestoreOptionsAutoPersonalizeBaseband
 *    Controls whether the the restore library will automatically enable/disable
 *    personalization for the baseband firmware.
 *    The value of this key is a CFBooleanRef.  If this key is not
 *    present or set to false the restore library will personalize the baseband if AuthInstallVariant
 *    or kAMRestoreOptionsAuthInstallRestoreBehavior are set. [Muxed]
 *
 *  kAMRestoreOptionsPostRestoreAction
 *    This option determines what happens at the end of the restore.  The
 *    possible values for this key are:
 *    kAMRestorePostRestoreReboot - reboot the device
 *    kAMRestorePostRestoreShutdown - shut down the device
 *    kAMRestorePostRestoreEnableAccessoryPower - turn on accessory power
 *    The default behavior if this key is nor present is to reboot the device.
 *    [Muxed]
 *
 *  kAMRestoreOptionsFailureAction
 *    This option determines what happens when the restore fails and the
 *    UncollectedLogsAction is not "reboot".
 *    The possible values for this key are:
 *    kAMRestoreFailureActionWaitForDisconnect - wait for USB disconnect
 *    kAMRestoreFailureActionWaitForButton - wait for button press
 *    The default behavior if this key is nor present is to wait for disconnect
 *    [Muxed]
 *
 *  kAMRestoreOptionsClearPersistentBootArgs
 *    If this option is specified with a true value, any persistant boot-args
 *    saved in NVRAM are cleared to their default values.  The value for this
 *    key is a CFBooleanRef.  The default behavior is to not clear boot-args.
 *    [Muxed]
 *
 *  kAMRestoreOptionsPersistentBootArgModifications
 *    Apply modifications to the persistant boot-args.  This allows the
 *    client to add or remove individual variables to the boot-args string.
 *    Note that this occurs after kAMRestoreOptionsClearPersistentBootArgs so
 *    the two can be used in conjunction to set the boot-args.
 *    The value is an array of arrays with each of the individual arrays
 *    referring to one 'modification operation'.  Index 0 of the array is
 *    either kAMRestoreBootArgsAdd or kAMRestoreBootArgsRemove indicating the
 *    type of operation.  Index 1 is the variable name.  Index 2 is optional
 *    for kAMRestoreBootArgsAdd and provides a value to the variable. [Muxed]
 *
 *  kAMRestoreOptionsSkipGrapeFWUpdate
 *    This option allows the user to skip installation of grape firmware.
 *    This option is a CFBoolean, and is false by default.
 *
 *  kAMRestoreOptionsGrapeFW
 *    This option specifies the grape FW file that will be sent to the device
 *    as an override. The value is s a CFStringRef containing the path to the
 *    file.
 *
 *  kAMRestoreOptionsSkipOpalFWUpdate
 *    This option allows the user to skip installation of opal firmware.
 *    This option is a CFBoolean, and is false by default.
 *
 *  kAMRestoreOptionsOpalFW
 *    This option specifies the opal FW file that will be sent to the device
 *    as an override. The value is s a CFStringRef containing the path to the
 *    file.
 *
 *  kAMRestoreOptionsStockholmPostflight
 *    This option specifies the file that will be run postflight by
 *    the stockholm updater. The value is a CFStringRef containing the
 *    path to the file.
 *
 *  kAMRestoreOptionsStockholmJCOPType
 *    This option specifies the JCOP type to use for firmware installation
 *    The possible values are Default, Development, and Production. The 
 *    value is a CFStringRef. 
 *
 *  kAMRestoreOptionsRootToInstall
 *    This options specifies a root to install on the device after the
 *    restore.  The value for this key is a CFStringRef containing the path
 *    to the root.  The root can either be a directory hierarchy or a
 *    CPIO/CPGZ archive. [Muxed]
 *
 *  kAMRestoreOptionsKernelCacheType
 *    This option controls the kernel cache that gets installed on the
 *    device.  The possible values for this option are:
 *    kAMRestoreKernelCacheRelease - install the release kernel cache
 *    kAMRestoreKernelCacheDevelopment - install the development kernel cache
 *    [Muxed]
 *
 *  kAMRestoreOptionsSetBootPartition
 *    This option sets the partition the device will boot off.  The value is
 *    a CFNumberRef indicating the partition number to boot off.  Currently
 *    the only supported values are -1 and 0.  -1 means don't change the
 *    current setting and 0 means the System partition. [Muxed]
 *
 *  kAMRestoreOptionsAutoBootDelay
 *    This option specifies the auto-boot behavior of the device.  The value
 *    is a CFNumberRef that indicates how long the auto-boot delay is.  If
 *    the value is -1, auto-boot is disabled.  If this key is not present,
 *    auto-boot is enabled with a delay of 0. [Muxed]
 *
 *  kAMRestoreOptionsRamdiskDelay
 *    This option specifies the delay before sending the ramdisk to device.  The value
 *    is a CFNumberRef that indicates how long the ramdisk delay is.
 *    If this key is not present, ramdisk delay has a value of 1000000 usec. [RM]
 *
 *  kAMRestoreOptionsReadOnlyRootFilesystem
 *    This option specifies whether the device should have a read-only or
 *    read-write root filesystem (the System filesystem).  The value is a
 *    CFBooleanRef.  The default behavior is to create a read-only root
 *    filesystem. [Muxed]
 *
 *  kAMRestoreOptionsPersistentBootArgs
 *    This option specifies the boot-args that should be set (persistently)
 *    on the device as part of the restore.  The value is a CFStringRef
 *    containing the new boot-args. [Muxed]
 *
 *  kAMRestoreOptionsSystemPartitionSize
 *    Specifies the size (in megabytes) that is desired for the system
 *    partition.  Because the partition size can only be changed when creating
 *    a new partition map, this option is only relevant when used in
 *    conjunction with kAMRestoreOptionsCreateFilesystemPartitions.  The value
 *    is a CFNumberRef holding the size in megabytes of the new partition.  If
 *    the value is 0, the restore library will attempt to use the minimum
 *    appropriate for the bundle.
 *
 *  kAMRestoreOptionsSetTimeOnDevice
 *    Set the time on the device to some specified time.  The time is specified
 *    as a CFNumberRef containing the number of seconds since the start of 1970.
 *    If this option is not provided, the device's time is not changed.
 *
 *  kAMRestoreOptionsCreateFactoryRestoreMarker
 *    If this option is set, then the restore will create a marker on the
 *    that can be used at runtime to distinguish between a restore that was
 *    done by iTunes and once that came out of the factory.  The value is a
 *    CFBooleanRef. [Muxed]
 *  
 *  kAMRestoreOptionsASRConnectionAttempts
 *    This value is the maximum number of times we try to connect to ASR.
 *    The default number of attempts is 5. This values is a CFNumber
 *
 *  kAMRestoreOptionsMinimumBatteryVoltage
 *    This value controls the minimum charge level at which the restore will
 *    be allowed to continue.  Below this threshold, we either wait to charge
 *    (if we're charging) or fail (if we're not charging).  If this option is
 *    not specified, a default value is used (currently 3.8V).  Setting this
 *    option to 0 bypasses all battery level checks.  This value is a
 *    CFNumberRef.  [Muxed]
 *
 *  kAMRestoreOptionsLetBatteryCharge
 *    If the current voltage is below the minimum level (either the level
 *    requested by kAMRestoreOptionsMinimumBatteryVoltage or built in), then
 *    the default behavior is to let the device charge and then continue.  This
 *    option overrides that behavior when given a false CFBooleanRef.  [Muxed]
 *
 *  kAMRestoreOptionsCloseModemTickets
 *    Ensure that all tickets associated with the baseband modem are closed by
 *    power cycling the modem a number of times.  This option is a CFBooleanRef
 *    indicating whether that power cycling should be performed.  The default
 *    behavior is to not perform that operation. [Muxed]
 *
 *  kAMRestoreOptionsForceBasebandUpdate
 *    The baseband update is skipped when the existing firmware matches the
 *    available firmware.  But in some cases, it is desirable to force the
 *    firmware update to occur, regardless of what is currently on there.  This
 *    option is a CFBooleanRef that when set to true, forces the update to
 *    be attempted. [Muxed]
 *
 *  kAMRestoreOptionsIgnoreBadStaticEEPBackup
 *    This option allows the baseband update to continue even if the updating
 *    tool detects a bad static EEP.  The value of this option is a
 *    CFBooleanRef.  If it is not present, the restore behaves as if were false.
 *    [Muxed]
 *
 *  kAMRestoreOptionsUpdateStaticEEPOnly
 *    This option tells the baseband update that it should skip the code section
 *    of the automatic update and only update the static EEP.  The value of this
 *    option is a CFBooleanRef.  If the option is not specified, the restore
 *    behaves as if it were false. [Muxed]
 *
 *  kAMRestoreOptionsVerifyStaticEEP
 *    This option tells the baseband update that prior to performing the update
 *    it should verify that the static EEP matches its backup.  The value of
 *    this option is a CFBooleanRef.  If the option is not specified, the
 *    restore behaves as if it were false. [Muxed]
 *
 *  kAMRestoreOptionsAllowBasebandRollbacks
 *    This option tells the baseband update that it should allow the update to
 *    attempt to rollback the baseband if the firmware provided is older than
 *    what the baseband is currently running.  This option is ignored by ICE3
 *    and newer basebands.  The value of this option is a CFBooleanRef.  If the
 *    option is not specified, the restore behaves as if it were false.
 *    Note that this will fail without an unlocked baseband. [Muxed]
 *
 *  kAMRestoreOptionsAllowUntetheredRestore
 *    Permit the restore to run untethered (not connected to a host).  The
 *    result of specifying this option when the restore needs data from the
 *    host (for instance, when flashing NOR) is undefined (but probably bad).
 *    If this option is specified and the device remains tethered, things
 *    should proceed as usual. [Muxed]
 *
 *  kAMRestoreOptionsFakeCCCC
 *    If this value is set, for the purposes of provisioning the restore will
 *    pretend that the last four digits of the serial number are as specified.
 *    By default, the real serial number will be used.
 *
 *  kAMRestoreOptionsMarketingPartNumber
 *    If this value is set, the marketing part number on the device will be
 *    added or updated.  By default, this option is not set and syscfg is not
 *    changed.
 *
 *  kAMRestoreOptionsRegionCode
 *    If this value is set, the region code on the device will be added or
 *    updated.  By default, this option is not set and syscfg is not changed.
 *
 *  kAMRestoreOptionsSoftwareBehavior
 *    If this value is set, the software behavior on the device will be added or
 *    updated.  By default, this option is not set and syscfg is not changed.
 *
 *  kAMRestoreOptionsAuthInstallVariant
 *    If this value is set, the AuthInstall portion of the library is enabled
 *    with the specified variant.
 *
 *  kAMRestoreOptionsAuthInstallRequestTicketWithoutNonce
 *    This option is a CFBooleanRef that when set to true, forces tells libauthinstall
 *    request an ap ticket even when no ap nonce is present.
 *
 *  kAMRestoreOptionsBasebandAntennaType
 *    If this option is specified the baseband antenna type will be sent to the server
 *    along with the personalization request.
 *    The value is a CFStringRef
 *
 *  kAMRestoreOptionsBasebandBehavior
 *    If this option is specified the baseband behavior will be sent to the server
 *    along with the personalization request.
 *    The value is a CFStringRef
 *
 *  kAMRestoreOptionsIsLegacy
 *    This option can be used to change the code path that is used in libusbrestore
 *    regarding personalization and file path locations. If IsLegacy is missing from
 *    the restore options its value will default to false if a personalization variant 
 *    or auth install behavior exists in the restore options, true otherwise. For all 
 *    unpersonalized restores IsLegacy must be set to true. This key was introduced to 
 *    support PR3 file override features without breaking or altering the logic of 
 *    PR2.5 restore options. Legacy restore tools, like PR2.5, should set IsLegacy to 
 *    true to maintain legacy code path. The value is a CFBooleanRef.
 *
 *  kAMRestoreOptionsAuthInstallRestoreBehavior
 *    If this value is set, the AuthInstall portion of the library is enabled,
 *    and the first matching variant that satisfies the specifed behavior is
 *    used.  If a value for kAMRestoreOptionsAuthInstallVariant is also set, it
 *    overrides restore behavior.
 *
 *  kAMRestoreOptionsAuthInstallSigningServerURL
 *    A string URL of the personalized install signing server.  If not set, a
 *    default (http://gs.apple.com:80) is used.
 *
 *  kAMRestoreOptionsAuthInstallEnableAppleConnectSingleSignOn
 *    A Boolean value specifying whether Apple Connect sso should be used.
 *
 *  kAMRestoreOptionsAuthInstallAppleConnectData
 *    Single sign on data generated by AppleConnect's API SSOGetServiceTicket. An 
 *    Apple Connect username should be specified as well.
 *
 *  kAMRestoreOptionsAuthInstallAppleConnectUser
 *    A string representing the AppleConnect username is used in addition to the 
 *    sso data.
 *
 *  kAMRestoreOptionsUserLocale
 *    If specified, provides a hint as to what locale to use for displayed text
 *    if localized externally (such as the case with AuthInstall, whose error strings
 *    are localized by the signing server).  The format of this value should be the
 *    ISO 639-1 language code followed by, if necessary, an underscore and the
 *    ISO 3166 country code.  Some examples:
 *        pt_PT -- Portuguese/Portugal (as opposed to Brazilian Portuguese)
 *        en_US -- English/US
 *        fr -- French
 *
 *  kAMRestoreOptionsEncryptDataPartition
 *    Controls whether restored attempts to create an encrypted data partition
 *    if the hardware supports it.  If this option is not specified, the restore
 *    will use encrypted data partitions when appropriate.  The value is a
 *    CFBooleanRef.
 *    This option has no effect unless kAMRestoreOptionsCreateFilesystemPartitions
 *    is specified and true (encryption must be set up at the time the
 *    partition is created).
 *
 *  kAMRestoreOptionsUpdateGasGaugeSoftware
 *    Controls whether restored attempts to software update the gas gauge (on
 *    devices with that hardware).  The value of the option is a CFBooleanRef.
 *    If this options is not specified, the restore will behave as if it were
 *    specified with a value of kCFBooleanTrue
 *
 *  kAMRestoreOptionsFormatForLwVM
 *    Controls whether the restore will format the storage for use with LwVM
 *    (LightweightVolumeManager).  The value of the option is a CFBooleanRef.
 *    If this options is not specified, the restore will behave as if it were
 *    specified with a value of kCFBooleanTrue
 *    Note that this option is only respected on an erase install (see
 *    kAMRestoreOptionsCreateFilesystemPartitions)
 *
 *  kAMRestoreOptionsBasebandNVRestoreItems
 *    Controls whether the Baseband NV items are reset and to which set.
 *    The value of the option is a CFNumberRef.
 *    If this options is not specified, the restore will not reset Baseband NV items
 
 *  kAMRestoreOptionsBasebandEnableFactoryDebug
 *    This option has been deprecated. Use kAMRestoreOptionsEntitlements instead.
 *    Controls whether the Enable Factory Debug entitlement is requested.
 *    The value of the option is a CFBooleanRef.
 *    If this options is not specified, the restore will behave as if it were
 *    specified with a value of kCFBooleanFalse
 *
 *  kAMRestoreOptionsBasebandEnableActivationBypass
 *    This option has been deprecated. Use kAMRestoreOptionsEntitlements instead.
 *    Controls whether the Enable Activation Bypass entitlement is requested.
 *    The value of the option is a CFBooleanRef.
 *    If this options is not specified, the restore will behave as if it were
 *    specified with a value of kCFBooleanFalse
 *
 *  kAMRestoreOptionsBasebandUpdaterOptions
 *    Provides a dictionary of options to be passed directly to libbbupdater.
 *    If this option is not specified, the restore will behave as if an empty
 *    dictionary was provided.
 *
 *  kAMRestoreOptionsBasebandJtagEnable
 *    This option has been deprecated. Use kAMRestoreOptionsEntitlements instead.
 *    Controls whether the BbJtagEnable entitlement is set in the baseband root
 *    manifest. If the option is not specified, the restore operates as if it
 *    was specified as kCFBooleanFalse.
 *
 *  kAMRestoreOptionsBasebandEnableProvisioning
 *    Controls whether Provisioning is attempted.
 *    The value of the option is a CFBooleanRef.
 *    If this options is not specified, the restore will behave as if it were
 *    specified with a value of kCFBooleanFalse
 *
 *  kAMRestoreOptionsForceServerProvisioning
 *    Controls whether to force a server based provisioning attempt even if the device is already provisioned.
 *    The value of the option is a CFBooleanRef.
 *    If this options is not specified, the restore will behave as if it were
 *    specified with a value of kCFBooleanFalse
 *
 *  kAMRestoreOptionsAuthInstallProvisioningServerURL
 *    A string URL of the provisioning server.  If not set, provisioning may
 *    not be possible.
 *
 *  kAMRestoreOptionsBasebandProvisioningLineType
 *    A string identifying which station on the line this provisioning attempt
 *    is being made from. If not set no value will be set in libauthinstall or reported to server.
 *
 *  kAMRestoreOptionsSkipPreflightPersonalization
 *    Controls if a preflighted restore should be attempted.
 *    The value of the option is a CFBooleanRef.
 *    If this options is not specified, the restore will behave as if it were
 *    specified with a value of kCFBooleanFalse
 *
 *  kAMRestoreOptionsPersonalizedDuringPreflight
 *    Indicates whether the personalization was done during preflight.
 *    The value of this option is a CFBooleanRef
 *    If this option is not specified, the restore will behave as if it were
 *    specified with a value of kCFBooleanFalse
 *
 *  kAMRestoreOptionsBasebandFactoryActivationKeyID
 *    Controls if a factory activation key hash is included in server request.
 *    The value of the option is a CFStringRef.
 *    If this option is not specified no factory activation key hash will be included in the server request.
 *
 *  kAMRestoreOptionsSystemPartitionPadding
 *    Controls the amount padding to add to the system partition for filesystem performance.
 *    The value of the option is a CFDictionaryRef.
 *    This value is a dictionary key'ed on device capacity (ex: 32 for 32GB device) and
 *    each key's value is the size of the padding for that device in megabytes.
 *    If this option is not specific no system partition padding will be added.
 *
 *  kAMRestoreOptionsAddSystemPartitionPadding
 *    Controls if a device specific padding value should be added into the system partition size.
 *    The value of the option is a CFBooleanRef.
 *    If this option is not specified and kAMRestoreOptionsSystemPartitionPadding is a valid padding dictionary,
 *    then the default restore behavior will default to true.
 *
 *  kAMRestoreOptionsFitSystemPartitionToContent
 *    Controls if the system partition should be resized to fit it's content after fixing up var.
 *    The value of the option is a CFBooleanRef.
 *    If this option is not specified the default restore behavior is true.
 *
 *  kAMRestoreOptionsEntitlements
 *    Controls which entitlements are granted.
 *    The value of the option is a CFDictionaryRef.
 *    If this option is not specified all entitlements will be disabled.
 *
 *  kAMRestoreOptionsRestoreBootstrapOnly
 *    Controls if this restore should ignore the device_map.plist and treat the device as bootstrap only.
 *    The value of the option is a CFBooleanRef.
 *    If this option is not specified the value from device_map.plist will be used. If device_map.plist cannot be found
 *    or the value is not in device_map.plist then the default is kCFBooleanFalse.
 *
 *  kAMRestoreOptionsFDRCAURL
 *    Controls the FDR CA URL to use.
 *    The value of the option is a CFURLRef.
 *    If this option is not specified a default URL will be used.
 *
 *  kAMRestoreOptionsFDRDataStoreURL
 *    Controls the FDR DataStore URL to use.
 *    The value of the option is a CFURLRef.
 *    If this option is not specified a default URL will be used.
 *
 *  kAMRestoreOptionsFDRSealingURL
 *    Controls the FDR Sealing Service URL to use.
 *    The value of the option is a CFURLRef.
 *    If this option is not specified a default URL will be used.
 *
 *  kAMRestoreOptionsFDRSkipSealing
 *    Controls if FDR data is sealed in the restore process. By default FDR data is sealed.
 *    If specified with value kCFBooleanTrue, libusbrestore will request the unsealed FDR data
 *    entitlement when personalizing.
 *    The value of the option is a CFBooleanRef.
 *    If this option is not specified data will be sealed.
 *
 *  kAMRestoreOptionsFDRAllowUnsealedData
 *    Controls whether an entitlement will be requested from the personalization server
 *    to allow unsealed FDR data. This entitlement effectively implies that incomplete data 
 *    (see below) is allowed.
 *    The value is a CFBoolean.
 *    If not specified, the entitlement will not be requested and sealed data will be required.
 *
 *  kAMRestoreOptionsFDRAllowIncompleteData
 *    Controls whether an entitlement will be requested from the personalization server
 *    to allow incomplete FDR data. Any data that is present must still be signed and sealed.
 *    The value is a CFBoolean.
 *    If not specified, the entitlement will not be requested and complete data will be required.
 *
 *  kAMRestoreOptionsFDRTrustObjectURL
 *    The URL to be used by FDR when fetching the FDR trust object.
 *    The value is expected as a CFStringRef.
 *
 *  kAMRestoreOptionsFDRTrustObjectFile
 *    A file path or a remote url to a custom FDR trust object to be included in the personalization ticket request.
 *    The path can be a local file or an http one.
 *    The value is expected as a CFStringRef.
 *
 *  kAMRestoreOptionsFDRDisableSSLValidation
 *    Controls whether FDR will perform SSL certificate validation for HTTPS
 *    URLs. If this option is not specified, validation will be performed. This
 *    option is only used internally. Customer restores will always perform SSL
 *    server certificate validation.
 *    The value is CFBooleanRef.
 *
 *  kAMRestoreOptionsFDRServerTimeout
 *    Controls the number of seconds to wait before timing out an FDR data fetch request.
 *    The value is CFNumberRef.
 *
 *  kAMRestoreOptionsFDRSkipDataPreservation
 *    Controls if FDR data should be preserved across the restore. If present and set to true
 *    all FDR data will be fetched from the server. Otherwise restored will attempt to preserve
 *    all valid FDR data.
 *    The value is CFBooleanRef.
 *
 *  kAMRestoreOptionsFDRSkipProvisioningSubmission
 *    Controls if when provisioning the baseband bbpv data should be submitted to FDR.
 *    If present and set to true provisioning data submission will be skipped.
 *    The value is CFBooleanRef.
 *
 *  kAMRestoreOptionsShouldRestoreSystemImage
 *    Restore option indicating if a system image will be restored.
 *    The value is CFBooleanRef.
 *
 *  kAMRestoreOptionsPersonalizedRestoreBundlePath
 *    Path to the personalized restore bundle.
 *    Value is a CFStringRef.
 *
 *  kAMRestoreOptionsBundleOverrides
 *    Dictionary of libauthinstall file overrides to be used instead of variant files.
 *    The key for each file override is the libauthinstall key name, and the value
 *    value is a CFStringRef URL, example CFSTR("file://localhost/myfile.dfu")
 *
 *  kAMRestoreOptionsSEPEEPROMTestDelay
 *    Setting for number of seconds to wait between loading the SEP OS and starting the 
 *    SEP EEPROM test if run.  This only has an effect if the SEP EEPROM test is run.
 *    This is needed because we can't run the test right away after booting the SEP OS
 *    See Radar: <rdar://problem/13783020> AP panic if SEP EEPROM test is run too soon after SEP OS is booted
 *    Defaults to 15s if not specified.
 *
 *  kAMRestoreOptionsSEPEEPROMTestMinimumGoodBlocks
 *    Indicates that the SEP EEPROM test should run with the specified minimum number of good blocks to be considered a pass.
 *    If the test returns fewer than the specified number of good blocks, the restore will fail and the error
 *    kAMRestoreErrorSEPEEPROMTestFailure will be returned.  If it is set to 0, the test will not be run.
 *    The type is a CFNumberRef.  The default is 0. 
 *
 *  kAMRestoreOptionsProvisionMesa
 *    Setting to trigger Mesa provisioning on devices that have it.  This should only function for audiences
 *    that are allowed to perform this operation (i.e. Factory)
 *    The value is a CFBooleanRef and defaults to FALSE
 *
 *  kAMRestoreOptionsAdditionalNVRAMVariables
 *    One or more key-value pairs to be written to NVRAM after the restore.  The
 *    pairs will be added to any existing values in NVRAM, overwriting existing
 *    values if any key already exists.  This option is ignored on customer
 *    restores.
 *    The value is a CFDictionaryRef of key-value pairs.  The values must be
 *    CFStringRefs.
 *
 *  kAMRestoreOptionsLogPartitionSize
 *    If set with a non-zero value, restored will create a log partition on the 
 *    device during the restore with the specified size.  This partition will be
 *    created with content protection disabled.
 *    The value is a CFNumberRef representing the partition size in megabytes.
 *    The default value is 0.
 *
 *  kAMRestoreOptionsBasebandDataPartitionSize
 *    Controls the size of the baseband data partition, if one is created in the
 *    course of the restore.  Has no effect unless the device being restored
 *    requires a baseband data partition.
 *    The value is a CFNumberRef representing the partition size in megabytes.
 *    The default value is specified by the baseband updater.
 *
 *  kAMRestoreOptionsBasebandUpdaterOutputPath
 *    Controls the path on the host to which any files created on the device by
 *    the baseband updater in the course of a baseband update will be written.
 *    The baseband updater output from each restore will be written to a
 *    directory under this path named "baseband_updater_output-<UUID>", where
 *    the UUID will be the one generated to uniquely identify the restore event,
 *    if any (kAMRestoreUUID) or a randomly generated one.
 *    The value is a CFStringRef of the absolute path to be used. The default
 *    value is "/tmp".
 *    This option is only supported in the OS X version of the library. On other
 *    platforms, this key is ignored and baseband updater output will not be
 *    copied to the host.
 *
 *  kAMRestoreOptionsDFUITRequestOverrides
 *    This is only used when personalizing a single DFU file. All items in the dictionary 
 *    will be included in the image request and override any existing property. 
 *
 *  kAMRestoreOptionsDFUITTrustedImage
 *    This is only used when personalizing a single DFU file. This option sets the EKEY flag
 *    in the tatsu request.
 *
 *  kAMRestoreOptionsRequestRestoreRulesDeviceMap
 *    This is only used when personalizing a single file. The device map used
 *    to process the restore request rules.
 *
 *  kAMRestoreOptionsRegulatoryModelNumber
 *    If this value is set, the regulatory model number on the device will be
 *    added or updated.  By default, this option is not set and syscfg is not
 *    changed.  The value must be at most 16 characters long.
 *    The value is a CFStringRef.
 *
 *  kAMRestoreOptionsCheckForPendingFDRSubmissions
 *    If this value is set, restored will check the data partition for an FDR
 *    data store containing pending FDR submissions.  If it finds any, the data
 *    will be verified and then submitted to FDR.
 *    The value is a CFBoolean. The default value is kCFBooleanFalse.
 *    This option is only honored by the internal version of restored.
 *
 *  kAMRestoreOptionsApRequestEntries
 *    A dictionary of key value pairs that will be forwarded to the Ap personalization request.
 *    The expected value is CFDictionaryRef.
 *
 *  kAMRestoreOptionsBbRequestEntries
 *    A dictionary of key value pairs that will be forwarded to the Bb personalization request.
 *    The expected value is CFDictionaryRef.
 *
 *  kAMRestoreOptionsObjectRequestEntries
 *    A dictionary containing personalization objects with properties to be included 
 *    in the personalization request. Each object is expected to be of type CFDictionaryRef containing
 *    key value paris of properties to be forwarded to the personalization request. 
 *    Ex: The dictionary RestoreOptions.ObjectRequestEntries.RestoreSEP would contain ESEC:YES to
 *    set the object property ESEC for the RestoreSEP entry.
 *
 *  kAMRestoreOptionsPairStockholm
 *    If this value is set, restored will attempt SE/SEP pairing.
 *    The value is a CFBoolean. The default value is kCFBooleanFalse.
 *    This option is only honored by the internal version of restored.
 *
 *  kAMRestoreOptionsStockholmPairingServerURL
 *    A string URL of the personalized install signing server.  If not set
 *    SE/SEP pairing may not be possible.
 *    This option is only honored by the internal version of restored.
 *
 *  kAMRestoreOptionsCertifySEP
 *    If this value is set, restored will attempt SEP device certification.
 *    The value is a CFBoolean. The default value is kCFBooleanFalse.
 *    This option is only honored by the internal version of restored.
 *
 *  kAMRestoreOptionsEnablePreboardService
 *    If this value is set, PreboardService will be launched and a stash bag will be created.
 *    The value is a CFBoolean. The default value is kCFBooleanFalse.
 *
 *  kAMRestoreOptionsSystemPartitionCushionSize
 *    Specifies the extra system partition size (in megabytes) in addition to the minimum 
 *    system partition size. Additional space will only be added on erase installs. The value
 *    is a CFNumber containing the number of megabytes to be added to the minimum system 
 *    partition size. If the value is missing or 0, no extra space will be added.
 */

/*
 *  Note: The following need to be #define's instead of
 *  'extern CFStringRef const' for Windows compatibility
 */
#define kAMRestoreOptionsBoardConfig CFSTR("BoardConfig")
#define kAMRestoreOptionsRestoreBundlePath CFSTR("RestoreBundlePath")
#define kAMRestoreOptionsSystemImageType CFSTR("SystemImageType")
#define kAMRestoreOptionsBootImageType CFSTR("BootImageType")
#define kAMRestoreOptionsDFUFile CFSTR("DFUFile")
#define kAMRestoreOptionsDFUFileType CFSTR("DFUFileType")
#define kAMRestoreOptionsOnlyPersonalizeDFUFile CFSTR("OnlyPersonalizeDFUFile")
#define kAMRestoreOptionsDeviceTreeFile CFSTR("DeviceTreeFile")
#define kAMRestoreOptionsFirmwareDirectory CFSTR("FirmwareDirectory")
#define kAMRestoreOptionsBootImageFile CFSTR("BootImageFile")
#define kAMRestoreOptionsRestoreKernelCacheFile CFSTR("RestoreKernelCacheFile")
#define kAMRestoreOptionsKernelCacheFile CFSTR("KernelCacheFile")
#define kAMRestoreOptionsNORImagePath CFSTR("NORImagePath")
#define kAMRestoreOptionsNORImageType CFSTR("NORImageType")
#define kAMRestoreOptionsBackgroundColor CFSTR("BackgroundColor")
#define kAMRestoreOptionsRestoreBootArgs CFSTR("RestoreBootArgs")
#define kAMRestoreOptionsSetRecoveryModeOutput CFSTR("SetRecoveryModeOutput")
#define kAMRestoreOptionsWaitForStorageDevice CFSTR("WaitForStorageDevice")
#define kAMRestoreOptionsCreateFilesystemPartitions CFSTR("CreateFilesystemPartitions")
#define kAMRestoreOptionsEraseDataPartition CFSTR("EraseDataPartition") /* Deprecated */
#define kAMRestoreOptionsSystemImage CFSTR("SystemImage")
#define kAMRestoreOptionsFlashNOR CFSTR("FlashNOR")
#define kAMRestoreOptionsUpdateBaseband CFSTR("UpdateBaseband")
#define kAMRestoreOptionsAutoPersonalizeBaseband CFSTR("AutoPersonalizeBaseband")
#define kAMRestoreOptionsPostRestoreAction CFSTR("PostRestoreAction")
#define kAMRestoreOptionsFailureAction CFSTR("FailureAction")
#define kAMRestoreOptionsClearPersistentBootArgs CFSTR("ClearPersistentBootArgs")
#define kAMRestoreOptionsPersistentBootArgModifications CFSTR("PersistantBootArgsModifications")
#define kAMRestoreOptionsRootToInstall CFSTR("RootToInstall")
#define kAMRestoreOptionsKernelCacheType CFSTR("KernelCacheType")
#define kAMRestoreOptionsSetBootPartition CFSTR("SetBootPartition")
#define kAMRestoreOptionsAutoBootDelay CFSTR("AutoBootDelay")
#define kAMRestoreOptionsRamdiskDelay CFSTR("RamdiskDelay")
#define kAMRestoreOptionsReadOnlyRootFilesystem CFSTR("ReadOnlyRootFilesystem")
#define kAMRestoreOptionsPersistentBootArgs CFSTR("PersistentBootArgs")
#define kAMRestoreOptionsSystemPartitionSize CFSTR("SystemPartitionSize")
#define kAMRestoreOptionsSetTimeOnDevice CFSTR("SetTimeOnDevice")
#define kAMRestoreOptionsCreateFactoryRestoreMarker CFSTR("CreateFactoryRestoreMarker")
#define kAMRestoreOptionsASRConnectionAttempts CFSTR("ASRConnectionAttempts")
#define kAMRestoreOptionsMinimumBatteryVoltage CFSTR("MinimumBatteryVoltage")
#define kAMRestoreOptionsLetBatteryCharge CFSTR("LetBatteryCharge")
#define kAMRestoreOptionsCloseModemTickets CFSTR("CloseModemTickets")
#define kAMRestoreOptionsForceBasebandUpdate CFSTR("ForceBasebandUpdate")
#define kAMRestoreOptionsIgnoreBadStaticEEPBackup CFSTR("IgnoreBadStaticEEPBackup")
#define kAMRestoreOptionsUpdateStaticEEPOnly CFSTR("UpdateStaticEEPOnly")
#define kAMRestoreOptionsVerifyStaticEEP CFSTR("VerifyStaticEEP")
#define kAMRestoreOptionsAllowBasebandRollbacks CFSTR("AllowBasebandRollbacks")
#define kAMRestoreOptionsAllowUntetheredRestore CFSTR("AllowUntetheredRestore")
#define kAMRestoreOptionsFakeCCCC CFSTR("FakeCCCC")
#define kAMRestoreOptionsMarketingPartNumber CFSTR("MarketingPartNumber")
#define kAMRestoreOptionsRegionCode CFSTR("RegionCode")
#define kAMRestoreOptionsSoftwareBehavior CFSTR("SoftwareBehavior")
#define kAMRestoreOptionsIsLegacy CFSTR("IsLegacy")
#define kAMRestoreOptionsAuthInstallVariant CFSTR("AuthInstallVariant")
#define kAMRestoreOptionsAuthInstallRestoreBehavior CFSTR("AuthInstallRestoreBehavior")
#define kAMRestoreOptionsAuthInstallSigningServerURL CFSTR("AuthInstallSigningServerURL")
#define kAMRestoreOptionsAuthInstallEnableAppleConnectSingleSignOn CFSTR("AuthInstallEnableSso")
#define kAMRestoreOptionsAuthInstallAppleConnectData CFSTR("AppleConnectData")
#define kAMRestoreOptionsAuthInstallAppleConnectUser CFSTR("AppleConnectUser")
#define kAMRestoreOptionsAuthInstallRequestTicketWithoutNonce CFSTR("AuthInstallRequestTicketWithoutNonce")
#define kAMRestoreOptionsUncollectedLogsAction CFSTR("UncollectedLogsAction")
#define kAMRestoreOptionsClearNVRAM CFSTR("ClearNVRAM")
#define kAMRestoreOptionsUserLocale CFSTR("UserLocale")
#define kAMRestoreOptionsEncryptDataPartition CFSTR("EncryptDataPartition")
#define kAMRestoreOptionsDataEncryptionType CFSTR("DataEncryptionType")
#define kAMRestoreOptionsUpdateGasGaugeSoftware CFSTR("UpdateGasGauge")
#define kAMRestoreOptionsBasebandFirmware CFSTR("BasebandFirmware")
#define kAMRestoreOptionsBasebandSkeyID CFSTR("BasebandSkeyID")
#define kAMRestoreOptionsBasebandNVRestoreItems CFSTR("BasebandNVRestoreItems")
#define kAMRestoreOptionsBasebandProvisioningKeyID CFSTR("BasebandProvisioningKeyID")
#define kAMRestoreOptionsBasebandActivationKeyID CFSTR("BasebandActivationKeyID")
#define kAMRestoreOptionsBasebandCalibrationKeyID CFSTR("BasebandCalibrationKeyID")
#define kAMRestoreOptionsBasebandFactoryActivationKeyID CFSTR("BasebandFactoryActivationKeyID")
#define kAMRestoreOptionsBasebandFusingProfile CFSTR("BasebandFusingProfile")
#define kAMRestoreOptionsBasebandFusingProgram CFSTR("BasebandFusingProgram")
#define kAMRestoreOptionsFusingServerURL CFSTR("FusingServerURL")
#define kAMRestoreOptionsBasebandEnableProvisioning CFSTR("BasebandEnableProvisioning")
#define kAMRestoreOptionsForceServerProvisioning CFSTR("ForceServerProvisioning")
#define kAMRestoreOptionsForceProvisioning CFSTR("ForceProvisioning")
#define kAMRestoreOptionsAuthInstallProvisioningServerURL CFSTR("AuthInstallProvisioningServerURL")
#define kAMRestoreOptionsBasebandProvisioningLineType CFSTR("BasebandProvisioningLineType")
#define kAMRestoreOptionsFormatForLwVM CFSTR("FormatForLwVM")
#define kAMRestoreOptionsBasebandAntennaType CFSTR("BasebandAntennaType")
#define kAMRestoreOptionsBasebandBehavior CFSTR("BasebandBehavior")
#define kAMRestoreOptionsBasebandUpdaterOptions CFSTR("BasebandUpdater")
#define kAMRestoreOptionsSkipPreflightPersonalization CFSTR("SkipPreflightPersonalization")
#define kAMRestoreOptionsPersonalizedDuringPreflight CFSTR("PersonalizedDuringPreflight")
#define kAMRestoreOptionsRelaxedImageVerification CFSTR("RelaxedImageVerification")
#define kAMRestoreOptionsSystemPartitionPadding CFSTR("SystemPartitionPadding")
#define kAMRestoreOptionsAddSystemPartitionPadding CFSTR("AddSystemPartitionPadding")
#define kAMRestoreOptionsFitSystemPartitionToContent CFSTR("FitSystemPartitionToContent")
#define kAMRestoreOptionsEntitlements CFSTR("RequestedEntitlements")
#define kAMRestoreOptionsPreserveBundle CFSTR("PreserveBundle")
#define kAMRestoreOptionsRestoreBootstrapOnly CFSTR("ApBootstrapOnly")
#define kAMRestoreOptionsFDRCAURL CFSTR("FDRCAURL")
#define kAMRestoreOptionsFDRDataStoreURL CFSTR("FDRDataStoreURL")
#define kAMRestoreOptionsFDRSealingURL CFSTR("FDRSealingURL")
#define kAMRestoreOptionsFDRSkipSealing CFSTR("FDRSkipSealing")
#define kAMRestoreOptionsFDRAllowUnsealedData CFSTR("FDRAllowUnsealedData")
#define kAMRestoreOptionsFDRAllowIncompleteData CFSTR("FDRAllowIncompleteData")
#define kAMRestoreOptionsFDRTrustObjectURL CFSTR("FDRTrustObjectURL")
#define kAMRestoreOptionsFDRTrustObjectFile CFSTR("FDRTrustObjectFile")
#define kAMRestoreOptionsFDRDisableSSLValidation CFSTR("FDRDisableSSLValidation")
#define kAMRestoreOptionsFDRServerTimeout CFSTR("FDRServerTimeout")
#define kAMRestoreOptionsFDRSkipDataPreservation CFSTR("FDRSkipDataPreservation")
#define kAMRestoreOptionsFDRSkipProvisioningSubmission CFSTR("FDRSkipProvisioningSubmission")
#define kAMRestoreOptionsInstallDiags CFSTR("InstallDiags")
#define kAMRestoreOptionsDiagData CFSTR("DiagData")
#define kAMRestoreOptionsDiagToInstall CFSTR("DiagToInstall")
#define kAMRestoreOptionsAuthInstallAllowMixAndMatch CFSTR("AuthInstallAllowMixAndMatch")
#define kAMRestoreOptionsAuthInstallDemotionPolicyOverride CFSTR("AuthInstallDemotionPolicyOverride")
#define kAMRestoreOptionsPersonalizedRestoreBundlePath CFSTR("PersonalizedRestoreBundlePath")
#define kAMRestoreOptionsShouldRestoreSystemImage CFSTR("ShouldRestoreSystemImage")
#define kAMRestoreOptionsBundleOverrides CFSTR("BundleOverrides")
#define kAMRestoreOptionsBuildIdentityOverride CFSTR("BuildIdentityOverride")
#define kAMRestoreOptionsSEPEEPROMTestDelay CFSTR("SEPEEPROMTestDelay")
#define kAMRestoreOptionsSEPEEPROMTestMinimumGoodBlocks CFSTR("SEPEEPROMTestMinimumGoodBlocks")
#define kAMRestoreOptionsProvisionMesa CFSTR("ProvisionMesa")
#define kAMRestoreOptionsAdditionalNVRAMVariables CFSTR("AdditionalNVRAMVariables")
#define kAMRestoreOptionsNVRAMVariableModifications CFSTR("NVRAMVariableModifications")
#define kAMRestoreOptionsLogPartitionSize CFSTR("LogPartitionSize")
#define kAMRestoreOptionsBasebandDataPartitionSize CFSTR("BasebandDataPartitionSize")
#define kAMRestoreOptionsBasebandUpdaterOutputPath CFSTR("BasebandUpdaterOutputPath")
#define kAMRestoreOptionsDFUITRequestOverrides CFSTR("RequestOverrides")
#define kAMRestoreOptionsDFUITImageTrusted CFSTR("ImageTrusted")
#define kAMRestoreOptionsRequestRestoreRulesDeviceMap CFSTR("RequestRestoreRulesDeviceMap")
#define kAMRestoreOptionsRegulatoryModelNumber CFSTR("RegulatoryModelNumber")
#define kAMRestoreOptionsUpdatePenguinController CFSTR("UpdatePenguinController")
#define kAMRestoreOptionsCheckForPendingFDRSubmissions CFSTR("CheckForPendingFDRSubmissions")
#define kAMRestoreOptionsApRequestEntries CFSTR("ApRequestEntries")
#define kAMRestoreOptionsBbRequestEntries CFSTR("BbRequestEntries")
#define kAMRestoreOptionsObjectRequestEntries CFSTR("ObjectRequestEntries")
#define kAMRestoreOptionsSkipGrapeFWUpdate CFSTR("SkipGrapeFW")
#define kAMRestoreOptionsGrapeFW CFSTR("GrapeFW")
#define kAMRestoreOptionsIgnoreSWDErrors CFSTR("IgnoreSWDFailures")
#define kAMRestoreOptionsCertifySEP CFSTR("CertifySEP")
#define kAMRestoreOptionsEnablePreboardService CFSTR("EnablePreboardService")
#define kAMRestoreOptionsSystemPartitionCushionSize CFSTR("SystemPartitionCushion")
    
#define kAMRestoreOptionsStockholmFirmwareType CFSTR("StockholmFirmwareType")
#define kAMRestoreOptionsStockholmAdvancedInstall CFSTR("AdvancedInstallStockholm")
#define kAMRestoreOptionsStockholmEnableUpdate CFSTR("InstallStockholm")
#define kAMRestoreOptionsPairStockholm CFSTR("PairStockholm")
#define kAMRestoreOptionsStockholmPairingServerURL CFSTR("StockholmPairingServerURL")
#define kAMRestoreOptionsStockholmPostflight CFSTR("StockholmPostflightScript")
#define kAMRestoreOptionsStockholmJCOPType CFSTR("StockholmJCOPType")
#define kAMRestoreOptionsStockholmSkipJCOP CFSTR("SkipJCOP")
#define kAMRestoreOptionsSkipOpalFWUpdate CFSTR("SkipOpalFW")
#define kAMRestoreOptionsOpalFW CFSTR("OpalFW")
#define kAMRestoreOptionsStockholmValidateJCOP CFSTR("ValidateJCOP")
    
/*  These options are deprecated.  Use kAMRestoreOptionsAuthInstallSigningServerURL instead.  */
#define kAMRestoreOptionsAuthInstallSigningServerHost CFSTR("AuthInstallSigningServerHost")
#define kAMRestoreOptionsAuthInstallSigningServerPort CFSTR("AuthInstallSigningServerPort")

/* These options are deprecated. Use kAMRestoreOptionsEntitlements instead */
#define kAMRestoreOptionsBasebandEnableFactoryDebug CFSTR("BasebandEnableFactoryDebug")
#define kAMRestoreOptionsBasebandEnableActivationBypass CFSTR("BasebandEnableActivationBypass")
#define kAMRestoreOptionsBasebandJtagEnable CFSTR("BasebandJtagEnable")
#define kAMRestoreOptionsBasebandEnableCalibration CFSTR("BasebandEnableCalibration")

/*
 *  Image keys
 *    Keys into the dictionary to define an image to restore.
 */
#define kAMRestoreOptionsImageFile CFSTR("ImageFile")
#define kAMRestoreOptionsImagePassword CFSTR("ImagePassword")

/*
 *  Image types
 *    The User image type refers to the image shipped to customers
 *    Internal images may contain extra content or functionality needed
 *    internally that is not exposed to customers.
 *    A third type of image, the update image, only applies to the boot image
 *    (specified with kAMRestoreOptionsBootImageType)
 *    The key kAMRestoreImageUserOrInternal may be passed with
 *    kAMRestoreOptionsBootImageType to indicate that the library should try the
 *    User image and if that key is not preset, fall back to trying the Internal
 *    key
 */
#define kAMRestoreImageUser CFSTR("User")
#define kAMRestoreImageInternal CFSTR("Internal")
#define kAMRestoreImageUpdate CFSTR("Update")
#define kAMRestoreImageUserOrInternal CFSTR("UserOrInternal")

/*  Restore Behaviors
 *    The Restore Behavior refers to the AuthInstall variant whose contents
 *    satisfy the behavior specified.  In other words, the software packaged
 *    in a variant whose behavior is "Erase" will erase the data partition
 *    as part of the restore.
 */
#define kAMAuthInstallRestoreBehaviorUpdate CFSTR("Update")
#define kAMAuthInstallRestoreBehaviorErase CFSTR("Erase")

/*
 *  Kernel cache types
 *    Possible values for the kAMRestoreOptionsKernelCacheType option.
 */
#define kAMRestoreKernelCacheRelease CFSTR("Release")
#define kAMRestoreKernelCacheDevelopment CFSTR("Development")

/*
 *  DFU file types
 *    Possible values for the kAMRestoreOptionsDFUFileType option.  Selects
 *    either the debug, development or release DFU file.
 */
#define kAMRestoreDFUDebug CFSTR("DEBUG")
#define kAMRestoreDFUDevelopment CFSTR("DEVELOPMENT")
#define kAMRestoreDFURelease CFSTR("RELEASE")

/*
 *  NOR image types
 *    Values for the kAMRestoreOptionsNORImageType key in the options
 *    dictionary.
 */
#define kAMRestoreNORImageProduction CFSTR("production")
#define kAMRestoreNORImageDevelopment CFSTR("firmwaredevelopment")
#define kAMRestoreNORImageFactorySA CFSTR("factorysa")
#define kAMRestoreNORImageFactoryFA CFSTR("factoryfa")
#define kAMRestoreNORImageDebug CFSTR("debug")

/*
 *  Post restore actions
 *    These are values for the kAMRestoreOptionsPostRestoreAction key and
 *    indicate what the device should do at the end of the restore.
 */
#define kAMRestorePostRestoreReboot CFSTR("Reboot")
#define kAMRestorePostRestoreShutdown CFSTR("Shutdown")
#define kAMRestorePostRestoreEnableAccessoryPower CFSTR("EnableAccessoryPower")
#define kAMRestorePostRestoreDisplaySuccess CFSTR("DisplaySuccess")
#define kAMRestorePostRestoreWaitForButton CFSTR("WaitForButton")

/*
 *  Post-restore failure actions
 *  These are values for the kAMRestoreOptionsFailureAction key and
 *  indicate what the device should do on a failed restore.
 *  Note these only apply if reboot_on_failure is false.
 */
#define kAMRestoreFailureActionWaitForDisconnect CFSTR("WaitForDisconnect")
#define kAMRestoreFailureActionWaitForButton     CFSTR("WaitForButton")

/*
 *  Uncollected log actions
 *    These options control what to do about uncollected logs on the device
 */
#define kAMRestoreIgnoreUncollectedLogs CFSTR("Ignore")
#define kAMRestoreFailOnUncollectedLogs CFSTR("Fail")
#define kAMRestoreRebootOnUncollectedLogs CFSTR("Reboot")

/*
 *  Boot argument modification types
 */
#define kAMRestoreBootArgsAdd CFSTR("Add")
#define kAMRestoreBootArgsRemove CFSTR("Remove")

/*
 *  Data encryption types
 *    These options control what type of data encryption to use on the device
 */
#define kAMRestoreDataEncryptionNone CFSTR("None")
#define kAMRestoreDataEncryptionDefault CFSTR("Device Default")
#define kAMRestoreDataEncryptionPerFile CFSTR("Per File Encryption")

/*
 * UUID that stays the same for the life of the restore and should be logged on the host, device and server
 */
#define kAMRestoreUUID CFSTR("UUID")

#ifdef __cplusplus
}
#endif
        
#endif
