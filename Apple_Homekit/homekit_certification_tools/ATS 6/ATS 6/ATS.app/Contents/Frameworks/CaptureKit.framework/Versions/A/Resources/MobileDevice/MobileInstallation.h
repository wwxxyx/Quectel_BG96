#ifndef _APPLEMOBILEINSTALLATION_
#define	_APPLEMOBILEINSTALLATION_

/*!
 * @header MobileInstallation.h
 *
 * Provides accesss to device-side service agents which deal with installing applications and
 * carrier bundles. Do not use this for installation OS updates, or data 
 * (documents, movies, music).
 *
 */

/* Header Declarations */
#include <MobileDevice/MobileDevice.h>
#include <MobileDevice/AMDServiceConnection.h>
#include <MobileDevice/AFCTypes.h>

#if defined(__cplusplus)
extern "C" {
#endif

/* Constant Declarations */
#define kAFCServiceName						CFSTR("com.apple.afc")
#define kInstallationServiceName			CFSTR("com.apple.mobile.installation_proxy")
#define kApplicationInstalledNotification	CFSTR("com.apple.mobile.application_installed")
#define kApplicationUninstalledNotification	CFSTR("com.apple.mobile.application_uninstalled")

#define	kPackageTypeKey						CFSTR("PackageType")
#define	kPackageTypeCustomer				CFSTR("Customer")
#define	kPackageTypeDeveloper				CFSTR("Developer")
#define	kPackageTypeCarrierBundle			CFSTR("CarrierBundle")

#define kLookupApplicationTypeKey			CFSTR("ApplicationType")
#define kApplicationTypeAny					CFSTR("Any")			
#define	kApplicationTypeSystem				CFSTR("System")
#define	kApplicationTypeUser				CFSTR("User")
#define	kApplicationTypeInternal			CFSTR("Internal")
	
#define kLookupAttributeKey					CFSTR("Attribute")
#define	kAttributeHasSettingsBundle			CFSTR("HasSettingsBundle")

#define kLookupBundleIDsKey					CFSTR("BundleIDs")
#define kLookupReturnAttributesKey			CFSTR("ReturnAttributes")

#define kCommandKey							CFSTR("Command")
#define kCommandVendContainer				CFSTR("VendContainer")
#define	kIdentifierKey						CFSTR("Identifier")

#define kErrorKey							CFSTR("Error")
#define kStatusKey							CFSTR("Status")
#define kStatusComplete						CFSTR("Complete")
	
#define	kPercentCompleteKey					CFSTR("PercentComplete")

#define	kArchiveTypeKey						CFSTR("ArchiveType")
#define	kArchiveTypeAll						CFSTR("All")
#define	kArchiveTypeDocumentsOnly			CFSTR("DocumentsOnly")
#define kArchiveTypeApplicationOnly			CFSTR("ApplicationOnly")

#define kDisableAFCDeltaTransfer            CFSTR("DisableDeltaTransfer")
    // we should really deprecate "kDisableDeltaTransfer" but we're keeping it around for
    // the convenience of legacy code
#define kDisableDeltaTransfer				kDisableAFCDeltaTransfer
#define kDisableUnzipOnTheFly				CFSTR("DisableUnzipOnTheFly")
    
#define kShadowParentKey                    CFSTR("ShadowParentKey")

#define	kSkipUninstallKey					CFSTR("SkipUninstall")	
#define kSuppressNotificationKey			CFSTR("SuppressNotification")
#define	kApplicationSINFKey					CFSTR("ApplicationSINF")
#define kiTunesArtworkKey					CFSTR("iTunesArtwork")
#define kiTunesMetadataKey					CFSTR("iTunesMetadata")
#define kApplicationDSIDKey					CFSTR("ApplicationDSID")
#define	kSignerIdentityKey					CFSTR("SignerIdentity")

#define kCapabilitiesMatchKey				CFSTR("CapabilitiesMatch")
#define kMismatchedCapabilitiesKey			CFSTR("MismatchedCapabilities")
	
/*!
 * @typedef amd_installation_callback
 * @abstract Function pointer type for callback function which provides information during 
 *	long operations.
 * @param status A CFDictionary providing various bits of status information. Ownership 
 *	passes to the callback fucntion. The dict MUST BE explicitly released or else it will leak.
 * @param context User-provided context for the callback function. MobileDevice.framework
 *	will pass it back to the callback function as provided.
 * @discussion Many MobileInstallation functions can potentially take a long time because they 
 *	involve network I/O, or because they communicate with the device-side installation service.
 *	These functions provide the option providing feedback to the caller through the use of a
 *	amd_installation_callback.
 *
 *	If an installation callback is provided, MobileDevice.framework will periodically provide
 *  status information in the form of a CFDictionary. Keys that may be seen include:
 *
 * <ul>
 *  <li>PercentComplete: An integer between 0 and 100, inclusive.</li>
 *	<li>Error: A string describing an error that was encountered during the operation.</li>
 *	<li>Status: A string describing what the function is currently doing.</li>
 * </ul>
 *
 *  This is not a complete list. Not all functions provide all keys. 
 */
typedef 
void
(AMD_API *amd_installation_callback)(
	CFDictionaryRef status,
	void *context);
    
/*!
 * @function AMDeviceSecureInstallApplicationBundle
 * @discussion AMDeviceSecureInstallApplicationBundle combines AMDeviceSecureTransferPath and AMDeviceSecureUpgradeApplication.
 * The options dictionary is mandatory. It must at the minimum contain
 * the key kPackageTypeKey to indicates the format that the package is in. Values can be:
 *    kPackageTypeCustomer if the package_path is pointed at a zipped .IPA file.
 *    kPackageTypeDeveloper if the package_path is pointed at an unzipped .app directory.
 * If kPackageTypeKey is kPackageTypeDeveloper, then you must also include kShadowParentKey,
 * whose value is a CFURLRef pointing to a directory where copy(s) of the bundle will be kept for the purpose of
 * performing subsequent binary delta installs. If this directory is empty, an "old style" file delta
 * install will be performed the first time, and subsequent installs will use the "new style" binary
 * delta install. If this directory doesn't exist, a file delta install will be performed, and no
 * shadow bundle will be recorded for binary delta installs in future invocations.
 *
 *
 * @param device The device ref.
 * @param appBundle Path to an application bundle on the local disk to be transferred to the device.
 * @param options Dictionary of options, see discussion above.
 * @param callback A callback to get status updates
 * @param context A context for the callback
 * @result A AMDError, kAMDSuccess if no errors.
 */
AMDError
AMD_API
AMDeviceSecureInstallApplicationBundle(
    AMDeviceRef device,
    CFURLRef appBundle,
    CFDictionaryRef options,
    amd_installation_callback callback,
    void *context);


/*!
 * @function AMDeviceTransferPath
 * @discussion AMDeviceTransferPath is an enhanced version of AMDeviceTransferApplication. The main
 * enhancement is that when transfering app directories as produced by XCode, only the files that
 * have changed from those of the installed app on the device are transfered. Otherwise it transfers
 * directories and files like AMDeviceTransferApplication and should be used instead of that API.
 * If kDisableDeltaTransfer is set to TRUE in the options dictionary, a full transfer of src_path
 * will happen instead of a delta transfer.
 *
 * When transferring .ipa files (which are really zip files), AMDeviceTransferPath will also attempt
 * to unzip the files on the host side before sending them to the device. This provides a big boost
 * to application installation time. This behavior can be turned off by setting kDisableUnzipOnTheFly
 * to TRUE in the options dictionary.
 *
 * @param device The device ref. 
 * @param src_path A file or directory on the local disk to be transferred to the device.
 * @param options Dictionary of options. See discussion for available options.
 * @param callback A callback to get status updates
 * @param context A context for the callback
 * @result A AMDError, kAMDSuccess if no errors.
 */
AMDError
AMD_API
AMDeviceTransferPath(
	AMDeviceRef device,
	CFStringRef src_path,
	CFDictionaryRef options,
	amd_installation_callback callback,
	void *context);

/*!
 * @function AMDeviceSecureTransferPath
 * @param afc_conn An AFC Connection to the specified device. May be NULL.
 * @param indevice The device ref.
 * @param package_path A file or directory on the local disk to be transferred to the device.
 * @param options Dictionary of options. See discussion for available options.
 * @param callback A callback to get status updates
 * @param context A context for the callback
 * @result A AMDError, kAMDSuccess if no errors.
 *
 * @discussion
 * Like AMDeviceTransferPath, only files that have changes from those in an installed app are
 * transferred.
 *
 * If afc_conn is not NULL, it will be used to transfer the package to the device.
 *
 * If kDisableDeltaTransfer is set to TRUE specified in the options dictionary or indevice
 * is NULL, a full transfer will happen instead of a delta transfer.
 *
 * Returns kAMDInvalidArgumentError if package_path contains a symlink with an absolute path as the target.
 */
AMDError
AMD_API
AMDeviceSecureTransferPath(
	AFCConnectionRef afc_conn,
	AMDeviceRef indevice,
	CFURLRef package_path,
	CFDictionaryRef options,
	amd_installation_callback callback,
	void *context);

/*!
 * @function AMDeviceGetLocalOrRemoteOffsetToResume
 * @abstract Determine whether either the local or the remote file is a subset of another, and if so, provide the size of the smallest file.
 * @param afc_conn An AFC Connection to the specified device.  May not be NULL.
 * @param indevice The device ref.
 * @param local_path Local path.
 * @param remote_path Remote path.
 * @result Returns the size of the smaller file, if amendments are possible to make it identical to the larger file, otherwise, returns 0.
 *
 * @discussion
 *
 * This function finds out the lowest size of the two files, and 
 * determines, via a hashing technique, whether the smaller file 
 * can be opened for append to generate a file identical to the larger file.
 *
 * If such appendment would be possible, then the size of the smaller file is returned.
 * Otherwise, a zero is returned.  Should any error occur or the functionality not be supported,
 * a zero would be returned.
 */
uint64_t
AMD_API
AMDeviceGetLocalOrRemoteOffsetToResume(
	AFCConnectionRef afc_conn,
	AMDeviceRef indevice,
	CFStringRef local_path,
	CFStringRef remote_path,
	CFDictionaryRef options);

/*!
 * @function AMDeviceTransferApplication
 * @discussion Transfer a package to the device via an AFC connection. Once the package is 
 * on the device, it can be installed with AMDeviceInstallApplication(). USE AMDeviceTransferPath()
 * INSTEAD.
 * Provides the following status keys:
 * <ul>
 *	<li>PercentComplete: Rough estimate of amount of work done vs. total work.</li>
 *	<li>TotalFiles: Total files to transfer (directories count too).</li>
 *	<li>TotalBytes: Total bytes to transfer.</li>
 *	<li>NumFiles: Number of files transferred.</li>
 *	<li>NumBytes: Number of bytes transferred so far.</li>
 *	<li>Path: If status is "CopyingFile", will also specify local path to the file currently
 *		being copied.</li>
 *	<li>Status: One of "PreflightingTransfer", "TransferringPackage", or "CopyingFile".</li>
 * </ul>
 *
 * @param package_path A zip archive on the local disk to be transferred to the device. MUST BE UNIX STYLE (FORWARD-SLASHES ONLY).
 * @result A AMDError, kAMDSuccess if no errors.
 *
 * This is deprecated, use AMDeviceTransferPath() instead.
 */
AMDError
AMD_API
AMDeviceTransferApplication(
	const int sock,
	CFStringRef package_path,
	CFDictionaryRef options,
	amd_installation_callback callback,
	void *context);

/*
 * This is deprecated, use AMDeviceSecureCheckCapabilitiesMatch() instead.
 */
AMDError
AMD_API
AMDeviceCheckCapabilitiesMatch(
	const int sock,
	CFTypeRef desired_capabilities,
	CFDictionaryRef options,
	CFDictionaryRef* results);

/*!
 * @function AMDeviceSecureCheckCapabilitiesMatch
 * @abstract Verify that the device has the desired capabilities.
 * @param serv_conn A service connection or NULL.
 * @param device An AMDeviceRef, or NULL.
 * @param options Options that are passed through to the installation service.
 * @param desired_capabilities A CFDictionary of key/value pairs that must match on the
 * device, or a CFArray of key names, all of which must be true on the device.
 * @param results Pointer to a CFDictionary that will be set to the results of the check.  The
 * dictionary contains:
 *   kCapabilitiesMatchKey - a CFBoolean indicating whether the device matches or not.
 *   kMismatchedCapabilitiesKey - if no match, this will be a CFDictionary containing
 *     the mismatched keys and their values on the device.
 * @result Returns an AMDError, kAMDAPIInternalError if the installation service cannot perform
 * the check.
 *
 * @discussion
 * The device may be specified in one of three ways, either via an sslContext connected to an instance of the
 * kInstallationServiceName service, or a socket connected to an instance of kInstallationServiceName, or via
 * an AMDeviceRef. If more than one method is specified, the function will fallback to using the socket if an
 * ssl context is not specified, or fallback to using the device ref if the socket is not provided.
 *
 * Specifying a device via the AMDeviceRef is provided as a convenience. However, using an already connected
 * sslContext or socket can avoid the performance penalty of starting a new instance of the service for each call.
 *
 * On success, the caller is responsible for releasing the dictionary returned in the parameter results.
 */
AMDError
AMD_API
AMDeviceSecureCheckCapabilitiesMatch(
	AMDServiceConnectionRef serv_conn,
	AMDeviceRef device,
	CFTypeRef desired_capabilities,
	CFDictionaryRef options,
	CFDictionaryRef* results);

/*!
 * @function AMDeviceInstallApplication
 * @discussion Install an application on the device.
 * @param package_path The path to a zip archive that was transferred to the 
 * device via AMDeviceTransferApplication.
 * @param sock A socket returned by AMDeviceStartService(kInstallationServiceName)
 * @result Returns an AMDError, kAMDAPIInternalError if the installation service cannot install
 * the application.
 *
 * @discussion
 * This is deprecated. Use AMDeviceSecureInstallApplication instead.
 */
AMDError
AMD_API
AMDeviceInstallApplication(
	const int sock,
	CFStringRef package_path,
	CFDictionaryRef options,
	amd_installation_callback callback,
	void *context);

/*!
 * @function AMDeviceSecureInstallApplication
 * @abstract Install an application on the device.
 * @param serv_conn A service connection or NULL.
 * @param device An AMDeviceRef, or NULL.
 * @param package_path The path to the path that was transferred to the device via AMDeviceTransferPath.
 * @param options An dictionary of options. See discussion for supported options.
 * @param callback A callback function to get progress updates.
 * @param context Context for the callback function.
 * @result Returns an AMDError, kAMDAPIInternalError if the installation service cannot install
 * the application.
 *
 * @discussion
 * Install iOS applications on the device.
 *
 * In the options dictionary, the key kPackageTypeKey indicates the format that the package is in.
 * This key should have value kPackageTypeCustomer if the package_path is pointed at a zipped .IPA file.
 * This key should have value kPackageTypeDeveloper if the package_path is pointed at an unzipped .app directory.
 */
AMDError
AMD_API
AMDeviceSecureInstallApplication(
    AMDServiceConnectionRef serv_conn,
    AMDeviceRef device,
    CFURLRef package_path,
    CFDictionaryRef options,
    amd_installation_callback callback,
    void *context);

/*!
 * @function AMDeviceUpgradeApplication
 * @discussion Upgrade an existing application on the device. This function is new in 
 * iPhoneOS 2.2. If the device is does not support it, MobileInstallationProxy will 
 * return kAMDUnknownCommandError.
 * @result Returns an AMDError, kAMDUnknownCommandError if the device doesn't support this command.
 *
 * This is deprecated. Use AMDeviceSecureUpgradeApplication instead.
 */
AMDError
AMD_API
AMDeviceUpgradeApplication(
	const int sock,
	CFStringRef package_path,
	CFDictionaryRef options,
	amd_installation_callback callback,
	void *context);

AMDError
AMD_API
AMDeviceSecureUpgradeApplication(
	AMDServiceConnectionRef serv_conn,
	AMDeviceRef device,
	CFURLRef package_path,
	CFDictionaryRef options,
	amd_installation_callback callback,
	void *context);

/*!
 * @function AMDeviceUninstallApplication
 * @discussion Uninstall an application from the device.
 * @param identifier An identifier for a user application
 * @result Returns an AMDError, kAMDAPIInternalError if the installation service fails.
 *
 * This is deprecated. Use AMDeviceSecureUpgradeApplication instead.
 */
AMDError
AMD_API
AMDeviceUninstallApplication(
	const int sock,
	CFStringRef identifier,
	CFDictionaryRef options,
	amd_installation_callback callback,
	void *context);
	
AMDError
AMD_API
AMDeviceSecureUninstallApplication(
	AMDServiceConnectionRef serv_conn,
	AMDeviceRef device,
	CFStringRef identifier,
	CFDictionaryRef options,
	amd_installation_callback callback,
	void *context);

/*
* Lookup the set of installed applications on the device.
* Returns kAMDSuccess if there were no errors.
*
* The info argument will point to a CFDictionary. Each key in the dictionary is an application
* identifier and the value is the Info.plist of the installed application.
*
* The client can specify options to filter the results.
* If the value of kLookupApplicationTypeKey is:
* 	kApplicationTypeAny: All application types are returned.
*	kApplicationTypeSystem: Only applications installed with the OS are returned.
* 	kApplicationTypeUser: Only third party applications are returned.
*
* If the value of kLookupAttributeKey is:
*	kAttributeHasSettingsBundle: Only applications with the "HasSettingsBundle" attribute are returned.
*	kAttributeHasSettingsBundleIcon: Only applications with the "HasSettingsBundleIcon" attribute are returned.
*
*	kLookupBundleIDsKey
*		CFString or array of strings that indicate the bundle IDs to lookup
*	kLookupReturnAttributesKey
*		CFString or array of strings indicating which of the keys from the Info.plist should be returned
*
* IMPORTANT NOTE:
* You should specify kLookupReturnAttributesKey in the options dictionary with an array value containing those
*   keys you would like returned. If you do not specify this attribute, MobileDevice will log every time you make
*   an offending call and performance will be poor. When specifying this key, you MUST include at least 
*   kCFBundleIdentifierKey in the array of keys to be returned or the call will not work properly.
*/

AMDError
AMD_API
AMDeviceLookupApplications(
	AMDeviceRef device,
	CFDictionaryRef options,
	CFDictionaryRef *info);

/*
* Archive an installed application.
* Returns kAMDSuccess if there were no errors.
*
* The options dictionary is not required, but accepts the following keys:
*	kArchiveTypeKey
*		If the value is kArchiveTypeAll, the entire application container is archived.
*		If the value is kArchiveTypeDocumentsOnly, only the documents are archived.
*		If the value is kArchiveTypeApplicationOnly, only the application bundle is archived.
*
* 	kSkipUninstallKey
*		If the value is set to kCFBooleanTrue, the application will be archived, but not uninstalled.
*
* If an error occurs, the status dictionary will contain an error string.
*
* This is deprecated. Use AMDeviceSecureArchiveApplication instead.
*/

AMDError
AMD_API
AMDeviceArchiveApplication(
	const int sock,
	CFStringRef identifier,
	CFDictionaryRef options,
	amd_installation_callback callback,
	void *context);

AMDError
AMD_API
AMDeviceSecureArchiveApplication(
	AMDServiceConnectionRef serv_conn,
	AMDeviceRef device,
	CFStringRef identifier,
	CFDictionaryRef options,
	amd_installation_callback callback,
	void *context);

/* This is deprecated. Use AMDeviceSecureRestoreApplication instead. */
AMDError
AMD_API
AMDeviceRestoreApplication(
	const int sock,
	CFStringRef identifier,
	CFDictionaryRef options,
	amd_installation_callback callback,
	void *context);
	
/*
 * This is deprecated, and remains only for compatibility for pre-Telluride (iOS 5.0)
 * devices. In Sundance and later (iOS 6.0), the device-side support necessary
 * for restoring applications no longer exists.
 *
 * In the options dictionary, the key kArchiveTypeKey must have the value kArchiveTypeDocumentsOnly.
 */
AMDError
AMD_API
AMDeviceSecureRestoreApplication(
	AMDServiceConnectionRef serv_conn,
	AMDeviceRef device,
	CFStringRef identifier,
	CFDictionaryRef options,
	amd_installation_callback callback,
	void *context);

/* This is deprecated. Use AMDeviceSecureRemoveApplicationArchive instead. */
AMDError
AMD_API
AMDeviceRemoveApplicationArchive(
	const int sock,
	CFStringRef identifier,
	CFDictionaryRef options,
	amd_installation_callback callback,
	void *context);
	
AMDError
AMD_API
AMDeviceSecureRemoveApplicationArchive(
	AMDServiceConnectionRef serv_conn,
	AMDeviceRef device,
	CFStringRef identifier,
	CFDictionaryRef options,
	amd_installation_callback callback,
	void *context);

/*!
 * @function AMDeviceLookupApplicationArchives
 * @abstract Returns information about all the application archives that are present on the device.
 * @param device Device to look up application archives on
 * @param options See discussion for supported options.
 * @param info On successful return, information.
 * @result kAMDSuccess, or an error on failure.
 *
 * @discussion
 * On successful return, info must be released by the caller or else it will leak.
 *
 * The info argument will point to a CFDictionary. Each key in the dictionary is the application's
 * CFBundleIdentifiers and the value is the Info.plist of the application.
 */
AMDError
AMD_API
AMDeviceLookupApplicationArchives(
	AMDeviceRef device,
	CFDictionaryRef options,
	CFDictionaryRef *info);

/* Deprecated. Use AMDeviceCreateHouseArrestService() instead. */
AMDError
AMD_API
AMDeviceStartHouseArrestService(
	AMDeviceRef device,
	CFStringRef identifier,
	CFDictionaryRef options,
	AMDNativeSocket *socket_out,
	SSL **ssl);

/*!
 * @function AMDeviceCreateHouseArrestService
 * @abstract Gain access to an application bundle on the device.
 * @param device The device
 * @param identifier The CFBundleID of the application to start the House Arrest service with.
 * @param options See discussion for supported options
 * @param afcConn_out The afc connection.
 * @result kAMDSuccess on success, or a descriptive error.
 *
 * @discussion
 * The House Arrest service is a way to start an AFC connection which is connected to
 * a the application bundle of an application installed on the device.
 *
 * The device must be connected, and must have an active session.
 *
 * On success, afcConn_out is an AFC connection with the same access to the filesystem
 * that a device application would have. Ownership follows the Create rule. The AFCConnection
 * created by this has closeOnInvalidate set to TRUE.
 */
AMDError
AMD_API
AMDeviceCreateHouseArrestService(
	AMDeviceRef device,
	CFStringRef identifier,
	CFDictionaryRef options,
	AFCConnectionRef *afcConn_out);

#if defined(__cplusplus)
}
#endif

#endif /* _APPLEMOBILEINSTALLATION_ */
