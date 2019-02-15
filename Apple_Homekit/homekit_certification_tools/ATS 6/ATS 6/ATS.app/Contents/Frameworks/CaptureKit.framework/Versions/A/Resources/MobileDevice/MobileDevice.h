#ifndef _APPLEMOBILEDEVICE_
#define _APPLEMOBILEDEVICE_

/*!
 * @header MobileDevice.h
 *
 * Mediates access to all device services.
 *
 * You must establish a session with AMDeviceStartSession() to perform most commands. 
 * Sessions time out after 20 seconds of inactivity and will throw an assert, so do 
 * not hold sessions over long-lived operations. Typically, start a session to start 
 * a service or talk to the device. Long-lived operations will talk to the services 
 * started.
 *
 * This host machine must be paired with the device to start a session. Use AMDeviceIsPaired(),
 * AMDeviceValidatePair(), AMDevicePair(), and AMDeviceUnpair() to manage pairing records. 
 *
 * Use AMDeviceConnect() to connect to a device. Use AMDeviceNotificationSubscribe() to learn of
 * devices. 
 */

/* Header Declarations */
#include <CoreFoundation/CoreFoundation.h>
#include <openssl/ssl.h>

#include <MobileDevice/MobileDeviceBase.h>
#include <MobileDevice/MobileDeviceError.h>
#include <MobileDevice/AMDeviceDiscovery.h>
#include <MobileDevice/AMDevicePowerAssertion.h>
#include <MobileDevice/AMDevicePreflightOperation.h>
#include <MobileDevice/AMDeviceWakeupOperation.h>
#include <MobileDevice/MobileRestore.h>

#if defined(__cplusplus)
extern "C" {
#endif

/* Supported Protocol Version */
extern const CFStringRef kAMDSupportedProtocolVersion;

/* Attribute Domains */
#define	kAMDCheckpointDomain			CFSTR("com.apple.mobile.iTunes")
#define kAMDDiskUsageDomain				CFSTR("com.apple.disk_usage")
#define	kAMDBatteryDomain				CFSTR("com.apple.mobile.battery")
#define kAMDInternationalDomain			CFSTR("com.apple.international")
#define kAMDFairPlayDomainKey			CFSTR("com.apple.fairplay")
#define kAMDSyncDataClassDomainKey		CFSTR("com.apple.mobile.sync_data_class")
#define kAMDWirelessLockdownDomainKey	CFSTR("com.apple.mobile.wireless_lockdown")
#define kAMDUserPreferencesDomainKey	CFSTR("com.apple.mobile.user_preferences")


/* Device Attributes */
#define kAMDProtocolVersionKey 			CFSTR("ProtocolVersion")
#define kAMDDeviceClassKey 				CFSTR("DeviceClass")
#define kAMDUniqueDeviceIDKey 			CFSTR("UniqueDeviceID")
#define kAMDDeviceNameKey 				CFSTR("DeviceName")
#define kAMDActivationStateKey 			CFSTR("ActivationState")
#define kAMDReservedBytesKey 			CFSTR("ReservedBytes")
#define kAMDDeviceColorKey				CFSTR("DeviceColor")
#define kAMDDeviceEnclosureColorKey     CFSTR("DeviceEnclosureColor")
#define kAMDDTProductTypesKey			CFSTR("DTProductTypes")
	
#define kAMDBuildVersionKey 			CFSTR("BuildVersion")
#define kAMDProductVersionKey 			CFSTR("ProductVersion")
#define kAMDProductTypeKey 				CFSTR("ProductType")
#define kAMDSerialNumberKey 			CFSTR("SerialNumber")
#define kAMDIMEIKey 					CFSTR("InternationalMobileEquipmentIdentity")
#define kAMDIMSIKey 					CFSTR("InternationalMobileSubscriberIdentity")
#define kAMDICCIDKey					CFSTR("IntegratedCircuitCardIdentity")
#define kAMDSIMStatusKey 				CFSTR("SIMStatus")
#define kAMDPhoneNumberKey				CFSTR("PhoneNumber")
#define kAMDBasebandVersionKey			CFSTR("BasebandVersion")
#define kAMDFirmwareVersionKey 			CFSTR("FirmwareVersion")

#define kAMDWifiAddressKey 				CFSTR("WiFiAddress")
#define kAMDBluetoothAddressKey 		CFSTR("BluetoothAddress")

#define kAMDDeviceCertificateKey 		CFSTR("DeviceCertificate")

#define kAMDiTunesHasConnectedKey		CFSTR("iTunesHasConnected")
#define kAMDiTunesSetupCompleteKey		CFSTR("iTunesSetupComplete")

#define kAMDActivationInfoKey 			CFSTR("ActivationInfo")
#define kAMDFairPlayKeyDataKey 			CFSTR("FairPlayKeyData")

#define kAMDFairPlayIDKey 				CFSTR("FairPlayID")
#define kAMDFairPlayGUIDKey 			CFSTR("FairPlayGUID")
#define kAMDFairPlayCertificateKey 		CFSTR("FairPlayCertificate")
#define kAMDColorSyncProfileKey 		CFSTR("ColorSyncProfile")

#define	kAMDActivationStateAcknowledgedKey	CFSTR("ActivationStateAcknowledged")

#define	kAMDBatteryIsChargingKey		CFSTR("BatteryIsCharging")
#define	kAMDBatteryCurrentCapacityKey	CFSTR("BatteryCurrentCapacity")

#define kAMDTotalDiskCapacityKey 		CFSTR("TotalDiskCapacity")
#define kAMDTotalSystemCapacityKey 		CFSTR("TotalSystemCapacity")

#define kAMDTotalSystemAvailableKey 	CFSTR("TotalSystemAvailable")
#define kAMDTotalDataCapacityKey 		CFSTR("TotalDataCapacity")
#define kAMDTotalDataAvailableKey 		CFSTR("TotalDataAvailable")

#define kAMDAmountDataReservedKey 		CFSTR("AmountDataReserved")
#define kAMDAmountDataAvailableKey 		CFSTR("AmountDataAvailable")

#define kAMDPhotoUsageKey 				CFSTR("PhotoUsage")
#define kAMDCameraUsageKey 				CFSTR("CameraUsage")
#define kAMDCalendarUsageKey 			CFSTR("CalendarUsage")
#define kAMDVoicemailUsageKey 			CFSTR("VoicemailUsage")
#define kAMDNotesUsageKey 				CFSTR("NotesUsage")
#define kAMDMediaCacheUsageKey 			CFSTR("MediaCacheUsage")	
#define kAMDCalculateDiskUsageKey 		CFSTR("CalculateDiskUsage")

#define kAMDAmountCameraAvailableKey 	CFSTR("AmountCameraAvailable")


#define kAMDTimeZoneOffsetFromUTCKey 	CFSTR("TimeZoneOffsetFromUTC")
#define kAMDPasswordProtectedKey 		CFSTR("PasswordProtected")

#define kAMDLanguageKey					CFSTR("Language")
#define kAMDKeyboardKey					CFSTR("Keyboard")
#define kAMDLocaleKey					CFSTR("Locale")

#define kAMDRentalBagRequestKey			CFSTR("RentalBagRequest")
#define kAMDRentalBagResponseKey		CFSTR("RentalBagResponse")
#define	kAMDRentalClockBiasKey		CFSTR("RentalClockBias")
#define	kAMDRentalBagRequestVersionKey		CFSTR("RBRequestVersion")
#define	kAMDRentalCheckinAckRequestKey		CFSTR("RentalCheckinAckRequest")
#define	kAMDRentalCheckinAckResponseKey		CFSTR("RentalCheckinAckResponse")
	
#define kAMDTimeIntervalSince1970Key	CFSTR("TimeIntervalSince1970")
#define kAMDTimeZoneKey					CFSTR("TimeZone")
#define	kAMDUses24HourClockKey			CFSTR("Uses24HourClock")

#define kAMDNikitaDomainKey				CFSTR("com.apple.mobile.nikita")
#define kAMDNikitaEnabledKey			CFSTR("EnableEnhancedEffectiveness")
#define	kAMDNikitaSupportedKey		CFSTR("EnhancedEffectivenessSupported")

#define	kAMDCarrierBundleInfoKey		CFSTR("CarrierBundleInfo")
#define kAMDInternalDomainKey				CFSTR("com.apple.mobile.internal")
#define kAMDIsInternalKey					CFSTR("IsInternal")
#define kAMDDevToolsAvailableKey			CFSTR("DevToolsAvailable")
#define kAMDNoDevToolsAvailableKey			CFSTR("None")
#define kAMDInternalToolsAvailableKey		CFSTR("Internal")
#define kAMDStandardToolsAvailableKey		CFSTR("Standard")
	
#define	kAMDRegionInfoKey					CFSTR("RegionInfo")
#define kAMDBasebandBootloaderVersionKey	CFSTR("BasebandBootloaderVersion")
#define kAMDModelNumberKey 					CFSTR("ModelNumber")

/* For use with kAMDUserPreferencesDomainKey domain. */
#define kAMDFileSharingEnabled					CFSTR("FileSharingEnabled")

/* Elements of the kAMDWirelessLockdownDomainKey. */
#define kAMDWirelessSupportsWifi                CFSTR("SupportsWifi")
#define kAMDWirelessSupportsWifiSyncing         CFSTR("SupportsWifiSyncing")
#define kAMDWirelessEnableWifiConnections		CFSTR("EnableWifiConnections")
#define kAMDWirelessBuddyID						CFSTR("WirelessBuddyID")

/* Notifications */
#define	kAMDNotificationDeviceNameChanged		CFSTR("com.apple.mobile.lockdown.device_name_changed")
#define	kAMDNotificationPairedDeviceNameChanged	CFSTR("com.apple.mobile.lockdown.paired_device_name_changed")
#define	kAMDNotificationActivationStateChanged	CFSTR("com.apple.mobile.lockdown.activation_state")
#define kAMDNotificationApplicationInstalled	CFSTR("com.apple.mobile.application_installed")
#define kAMDNotificationApplicationUninstalled	CFSTR("com.apple.mobile.application_uninstalled")
#define kAMDNotificationDiskUsageChanged		CFSTR("com.apple.mobile.lockdown.disk_usage_changed")
#define kAMDNotificationFileSharingChanged		CFSTR("com.apple.mobile.file_sharing_changed")

/* Service Options */
#define	kAMDServiceOptionTimeoutConnection		CFSTR("TimeoutConnection")
#define kAMDServiceOptionUnlockEscrowBag		CFSTR("UnlockEscrowBag")
	
/* Old Device Attributes */
#define kAMDProtocolVersion CFSTR("ProtocolVersion")
#define kAMDDeviceClass CFSTR("DeviceClass")
#define kAMDDeviceName CFSTR("DeviceName")
#define kAMDUniqueDeviceID CFSTR("UniqueDeviceID")
#define kAMDActivationState CFSTR("ActivationState")
#define kAMDBuildVersionKey CFSTR("BuildVersion")
#define kAMDBasebandVersion CFSTR("BasebandVersion")
#define kAMDActivationInfo CFSTR("ActivationInfo")

/*
* Activation States
*
* 1. Activated
* The device is fully activated by the iTunes Music Store.
*
* 2. Unactivated
* The device has not been activated.
*
* 3. Soft Activation
* The device only contains an activation record for the IMEI.
* This will happen when the device is serviced by AppleCare and the user
* must go to the iTunes Music Store to fully activate the device.

* 4. Mismatch
*	a) Mismatched IMEI
*	The device's IMEI does not match the activation record.
*	This will happen if the device was serviced by AppleCare and not properly reactivated.
*
*	b) Mismatched ICCID
*	The device's ICCID does not match the activation record.
*	This will happen if the user has put a different SIM card in his phone.
*
*	c) Missing ICCID
*	There is no SIM card in the device.
*/

#define kAMDUnactivated CFSTR("Unactivated")
#define kAMDActivated CFSTR("Activated")
#define	kAMDUnlocked CFSTR("Unlocked")
#define kAMDSoftActivation CFSTR("SoftActivation")
#define kAMDMismatchedIMEI CFSTR("MismatchedIMEI")
#define kAMDMismatchedICCID CFSTR("MismatchedICCID")
#define kAMDMissingSIM CFSTR("MissingSIM")
#define kAMDFactoryActivated CFSTR("FactoryActivated")
#define	kAMDWildcardActivated		CFSTR("WildcardActivated")
#define kAMDExpired CFSTR("Expired")

/* Apple Mobile Device */


/*!
 * @function AMDeviceGetTypeID
 * @abstract Get the CFTypeID for AMDevice objects.
 * @result The type ID for AMDevice objects.
 * @discussion Don't cache, this can change. Use to determine whether
 * or not an object is am AMDevice.
 */
CFTypeID
AMD_API
AMDeviceGetTypeID(
	void);

/*!
 * @function AMDeviceSetUserInfo
 * @abstract Allow clients to associate any arbitrary piece of data with an AMDeviceRef
 * @param device The device
 * @param pUserInfo The user information
 * @param pUserInfoIsCFType Treat the info as a CFType.
 * @discussion If pamd_userInfoIsCFType is true, the user info will be treated as a CFType. It will
 * be retained when it is associated with a device ref, and released when a different info is
 * associated with the device ref. It will also be released when the device ref itself is destroyed.
 */
void
AMD_API
AMDeviceSetUserInfo(
	AMDeviceRef device,
	void *pUserInfo,
	bool pUserInfoIsCFType);

/*!
 * @function AMDeviceGetUserInfo
 * @abstract Allow clients to retrieve a user context associated with this AMDeviceRef
 * @param device The device to get the user info from
 * @discussion Will return the value that was previously set with AMDeviceSetUserInfo, or NULL
 * if the user info has not yet been set.
 */
void *
AMD_API
AMDeviceGetUserInfo(
	AMDeviceRef device);

/*!
 * @enum WirelessBuddyOptions
 * @constant kAMDWirelessBuddy Controls whether or not the device considers this host machine its wireless buddy.
 * @constant kAMDSyncOverWifi Controls whether or not the device shall advertise the wifi sync service.
 */
enum WirelessBuddyOptions {

	kAMDWirelessBuddy = 1 << 0,
	kAMDSyncOverWifi = 1 << 1,
};

/*!
 * @function AMDeviceSetWirelessBuddyFlags
 * @abstract Set the device's wireless buddy options
 * @param device The device to set options on.
 * @param flags This is a bitfield.
 * @result kAMDSuccess on success, otherwise an error.
 *
 * @discussion
 * A lockdown session must be active to perform this operation.
 *
 * Returns kAMDNoWifiSyncSupportError if the device does not support WiFi sync.
 * See enum WirelessBuddyOptions for a description of the options.
 */
AMDError
AMD_API
AMDeviceSetWirelessBuddyFlags(
    AMDeviceRef device,
	CFOptionFlags flags);

/*!
 * @function AMDeviceGetWirelessBuddyFlags
 * @abstract Get the wireless buddy options
 * @param device The device to get options for
 * @param flags_out On success, the wireless buddy flags.
 * @result kAMDSuccess on success, otherwise an error.
 *
 * @discussion
 * Returns kAMDNoWifiSyncSupportError if the device does not support WiFi sync.
 * See enum WirelessBuddyOptions for a description of the options.
 */
AMDError
AMD_API
AMDeviceGetWirelessBuddyFlags(
	AMDeviceRef device,
	CFOptionFlags *flags_out);

/*!
 * @function AMDeviceCopyDeviceIdentifier
 * @abstract Get the unique device identifier.
 * @param device The device whose identifier should be copied.
 * @result A CFStringRef of the device's id, or NULL.
 * @discussion This is the device's UDID.
 */
CFStringRef
AMD_API
AMDeviceCopyDeviceIdentifier( 
	AMDeviceRef device);
    
/*!
 * @function AMDeviceCopyPairedCompanion
 * @abstract Get the AMDeviceRef for the paired companion (if one exists)
 * @param device The paired gizmo device
 * @result The AMDeviceRef for the paired companion (if one exists)
 */
AMDeviceRef
AMD_API
AMDeviceCopyPairedCompanion(
    AMDeviceRef device);

/*!
 * @function AMDeviceCopyDeviceLocation
 * @abstract Get the USB location number (as defined by the USB multiplexor).
 * @param device The device to get the location ID of.
 * @result 0 if no location ID is available, otherwise non-zero.
 *
 * @discussion Location is not available on Windows or for WiFi devices.
 * In those cases, this function returns 0.
 */
uint32_t
AMD_API
AMDeviceCopyDeviceLocation(
	AMDeviceRef device);

/*!
 * @function AMDeviceIsValid
 * @abstract Determine whether the device is still connected.
 * @param device The device
 * @result 1 if the device is connected, 0 if the valid is not connected.
 *
 * @discussion
 * When the physical device represented by an AMDeviceRef is no longer attached,
 * the AMDeviceRef is no longer valid. Most operations on an unconnected device
 * ref will return AMDDeviceDisconnectedError.
 */
int
AMD_API
AMDeviceIsValid(
	AMDeviceRef device);

/*!
 * @function AMDeviceGetConnectionID
 * @abstract Return the connection ID as handed to us by the USB multiplexor library.
 * @param device The device whose ID should be returned
 * @result Returns 0 on failure, or a postive integer connection ID on success.
 *
 * @discussion
 * This is the number that usbmuxd uses to identify the device.
 */
unsigned int
AMD_API
AMDeviceGetConnectionID(
	AMDeviceRef device);

/*!
 * @function AMDeviceIsPaired
 * @abstract Checks to see whether or not we are paired with the device.
 * @param device The device to check pair-ed-ness with.
 * @result Returns 1 if paired, 0 is not paired.
 * @discussion Only checks locally. May return true even if not paired. This may happen if the 
 * device has recently been restored. Use AMDeviceValidatePair() to confer with device 
 * about pair-ed-ness. Can be done without having a session.
 *
 * FOR ALMOST ALL USAGES, AMDeviceValidatePairing() IS BETTER!
 */
unsigned int
AMD_API
AMDeviceIsPaired(
	AMDeviceRef device);

/*!
 * @function AMDeviceGetInterfaceType
 * @abstract Get the interface type.
 * @param device The device to find out about.
 * @result -1 on failure. On success, kAMDeviceInterfaceConnectionTypeDirect if
 * the device is attached via USB, kAMDeviceInterfaceConnectionTypeIndirect if
 * the device is attached over the network (currently just WiFi).
 * kAMDeviceInterfaceConnectionTypeProxied if the device is paired to a companion
 * device connected over USB/WiFi.
 */
AMDeviceInterfaceConnectionType
AMD_API
AMDeviceGetInterfaceType( 
	AMDeviceRef device);

/*
* Get the interface speed for communicating with this device.
*/
UInt32
AMD_API
AMDeviceGetInterfaceSpeed( 
	AMDeviceRef device);

/*!
 * @function AMDeviceConnect
 * @abstract Connect to the mobile device.
 * @param device The device to connect to
 * @result Returns an AMDError describing the outcome.
 * @discussion If you are already connected, this function will attempt to
 * verify that the connection is still open.
 *
 * Returns kAMDWrongDroidError if the device is in the restore OS.
 */
AMDError
AMD_API
AMDeviceConnect(
	AMDeviceRef device);
	
/*!
 * @function AMDeviceDisconnect
 * @abstract Disconnect from the device.
 * @param device The device to disconnect from
 */
AMDError
AMD_API
AMDeviceDisconnect(
	AMDeviceRef device);
	
/*!
 * @function AMDeviceCopyValueWithError
 * @abstract Copy a value from the device
 * @param device The device to copy from
 * @param domain The domain to query. May be NULL.
 * @param key The key to query. May be NULL.
 * @param error_out On return, an error code describing the result of the operation. May be NULL.
 * @result A new plist value, or NULL.
 *
 * @discussion
 * Copies a value from the lockdown property store. A key argument of NULL asks for the
 * contents of the whole domain. A domain argument of NULL asks for the global domain.
 * Some properties are unavailable outside of a session.
 *
 * Returns kAMDDeviceDisconnectedError is the device is no longer attached.
 * Returns kAMDInvalidArgumentError if device is not a valid device ref, or if domain or key arguments are
 * non-NULL and not string.
 */
CFPropertyListRef
AMD_API
AMDeviceCopyValueWithError(
	AMDeviceRef device,
	CFStringRef domain,
	CFStringRef key,
	AMDError *error_out);

/*
 * AMDeviceCopyValue() has a return type of CFTypeRef, but will only ever return property list types.
 * AMDeviceCopyValueWithError() is preferred.
 */
CFTypeRef
AMD_API
AMDeviceCopyValue(
	AMDeviceRef device,
	CFStringRef domain,
	CFStringRef key);

/*
* Set a value on the device.
*/

AMDError
AMD_API
AMDeviceSetValue(
	AMDeviceRef device,
	CFStringRef domain,
	CFStringRef key,
	CFTypeRef value);
	
/*
* Remove a value on the device.
*/

AMDError
AMD_API
AMDeviceRemoveValue(
	AMDeviceRef device,
	CFStringRef domain,
	CFStringRef key);
	
/*!
 * @function AMDevicePair
 * @abstract Create a pairing relationship with an iOS device
 * @param device The device to pair with
 * @result kAMDSuccess or an error
 *
 * @discussion
 * This is equivalent to calling AMDevicePairWithOptions with options set to NULL.
 */
AMDError
AMD_API
AMDevicePair(
	AMDeviceRef device);

/*
 * @function AMDevicePairWithOptions
 * @abstract Create a pairing relationship with an iOS device
 * @param device The device to pair with
 * @param options See discussion of AMDeviceExtendedPairWithOptions
 * @result kAMDSuccess or an error
 *
 * @discussion
 * This is equivalent to calling AMDeviceExtendedPairWithOptions with extendedResponse set to NULL.
 */
AMDError
AMD_API
AMDevicePairWithOptions(
    AMDeviceRef device,
    CFDictionaryRef options);
	
#define kAMDPairingOptionChaperoneCert  CFSTR("ChaperoneCertificate")
#define kAMDPairingOptionExtendedErrors CFSTR("ExtendedPairingErrors")
    
#define kAMDPairingChallenge            CFSTR("PairingChallenge")

/*
 * @function AMDeviceExtendedPairWithOptions
 * @abstract Create a pairing relationship with an iOS device
 * @param device The device to pair with
 * @param options See discussion for more options
 * @param extendedResponse See discussion
 * @result kAMDSuccess or an error
 *
 * @discussion
 * Establishes a pairing relationship with the device.
 *
 * Supported Options:
 * - kAMDPairingOptionChaperoneCert: The value of this key should be the chaperone identity.
 *   Returns kAMDInvalidArgumentError if the value is not a valid property list type.
 *
 * On return, if extendedResponse is non-NULL, it will be set to a dictionary containing
 * extended pairing information.
 * 
 * Supported extendedResponse keys:
 * - kAMDPairingChallenge: A ManagedConfiguration policy on the device requires secure,
 *   challenge/response based pairing. The value of this key is the challenge.
 *
 * Pairing is only allowed with devices that are connected via USB. A return
 * value of kAMDPairingProhibitedError will be returned if pairing is attempted with
 * a non-USB device.
 */
AMDError
AMD_API
AMDeviceExtendedPairWithOptions(
    AMDeviceRef device,
    CFDictionaryRef options,
    CFDictionaryRef *extendedResponse);
	
/*
* Unpair with the device.
*/
AMDError
AMD_API
AMDeviceUnpair(
	AMDeviceRef device);
	
/*!
 * @function AMDeviceValidatePairing
 * @abstract Validate the pairing with the device.
 * @param device The device to validate the pairing with.
 * @discussion
 *
 * Checks to see if the host and device are paired. Prefer this to AMDeviceIsPaired().
 * On success, the device will also be notified that it is attached to a Trusted Host.
 */	
AMDError
AMD_API
AMDeviceValidatePairing(
	AMDeviceRef device);
	
/*!
 * @function AMDeviceStartSession
 * @abstract Start a session with the device.
 * @param device The device to start a session with.
 * @result Returns an AMDError describing the outcome.
 * @discussion You must have paired with the device before you can start a session.
 *
 * A return value of kAMDInvalidPairRecordError is possible if the pair records have
 * been damaged. In this case the pairing records will be discarded and the 
 * device connection will be shut down. 
 *
 * To recover: AMDeviceConnect(), AMDeviceUnpair(), AMDevicePair() and try to start 
 * a session again.
 */
AMDError
AMD_API
AMDeviceStartSession(
	AMDeviceRef device);

/*!
 * @function AMDeviceStopSession
 * @abstract Close an open session with the device.
 * @param device The device to stop a session with.
 * @discussion The connection is still active.
 */
AMDError
AMD_API
AMDeviceStopSession(
	AMDeviceRef device);

/*!
 * @function AMDeviceActivate
 * @abstract Activate the device
 * @param device The device to be activated
 * @param activationRecord An activation record from the activation servers.
 * @result Returns an AMDError describing the outcome.
 * @discussion Activate the device with information provided by the iTMS.
 * You must have started a session before you can activate.
 */
AMDError
AMD_API
AMDeviceActivate(
	AMDeviceRef device,
	CFDictionaryRef activationRecord);

/*!
 * @function AMDeviceDeactivate
 * @abstract Deactivate a device.
 * @param device The device to deactivate
 * @discussion Tells a device to deactivate itself.
 */
AMDError
AMD_API
AMDeviceDeactivate(
	AMDeviceRef device);

/*!
 * @function AMDeviceEnterRecovery
 * @abstract Puts the device into recovery mode. Does NOT require a session.
 * @param device The device to put into recovery mode.
 * @result Returns an AMDError describing the outcome.
 * @discussion Note that this requires the device to reboot. If this function
 * returns success, the reboot may already be in progress. At that point, the only
 * thing that may reliably be done with the device reference is pass it to
 * AMDeviceDisconnect (other API will likely generate errors attempting to
 * communicate with the device).
 */
AMDError
AMD_API
AMDeviceEnterRecovery(
	AMDeviceRef device);

/* This is deprecated. Use AMDeviceSecureStartService() which is both more general and more better. */
AMDError
AMD_API
AMDeviceStartService(
	AMDeviceRef device,
	CFStringRef service,
	CFSocketNativeHandle *socket_out,
	SSL **ssl);

/* This is deprecated. Use AMDeviceSecureStartService() which is both more general and more better. */
AMDError
AMD_API
AMDeviceStartServiceWithOptions(
	AMDeviceRef device,
	CFStringRef service,
	CFDictionaryRef options,
	CFSocketNativeHandle *socket_out);


/*
 * kAMDOptionInvalidateOnDetach is obsolete and doesn't do anything anymore.
 */
#define kAMDOptionInvalidateOnDetach        CFSTR("InvalidateOnDetach")
#define kAMDOptionCloseOnInvalidate         CFSTR("CloseOnInvalidate")

/*!
 * @function AMDeviceSecureStartService
 * @abstract Securely start a service on the device specifying options.
 * @param device The device on which to start a service. May not be NULL.
 * @param service The name of the service to start. May not be NULL.
 * @param options A dictionary of options. See discussion for supported options.
 * @param service_out On successful return, an AMDServiceConnection object. May not be NULL.
 * @result Returns an AMDError describing the outcome.
 * @discussion
 *
 * Starts a service on the device. Requires that a session with the device be active.
 * Attempting to start a service without an active session will result in kAMDInvalidArgumentError.
 * Fails with kAMDNoWifiSyncSupportError if the device side service does not support SSL.
 * Fails with kAMDServiceProhibitedError if the service is not allowed to run. Some services are
 * only allowed to run when a device has been activated.
 *
 * The device may request that a connection be encrypted. If so, this call will also perform
 * the initial SSL handshake. See <MobileDevice/AMDServiceConnection.h> for more information
 * on using the service connection.
 *
 * Options:
 *
 * kAMDServiceOptionTimeoutConnection - The service connection will have SO_SNDTIMEO and
 *  SO_RCVTIMEO set. It will be closed if either input or output operations fail to
 *  complete within 25 seconds (use carefully if you expect your service to perform
 *  long-lived operations on your behalf). Valid values are boolean true or false.
 *  This is off by default.
 *
 * kAMDServiceOptionUnlockEscrowBag - Attempt the passcode-unlock the device when starting
 *	a service.
 *
 * kAMDOptionCloseOnInvalidate - The returned service connection will take ownership of
 *  the underlying file descriptor and SSL context. Manually closing the fd or SSL_free()'ing
 *  the ssl context will result in an double free()s and close()s when the service connection
 *  object is invalidate or ultimately released. Valid values are boolean true or false.
 *  This is on by default.
 *
 *
 * On success, the AMDServiceConnection returned in service_out must be CFRelease()'d by the caller.
 */
AMDError
AMD_API
AMDeviceSecureStartService(
	AMDeviceRef device,
	CFStringRef service,
	CFDictionaryRef options,
	AMDServiceConnectionRef *service_out);

/*!
 * @function AMDeviceCopyAuthInstallPreflightOptions
 * @abstract Attempts to personalize a restore bundle for the given device
 * @param device The device for which to personalize
 * @param baseOptions The initial restore options
 * @param outPreflightOptions The restore options modified to use the newly personalized restore bundle
 * @result An AMRestore error indicating whether personalization was successful
 * @discussion
 *
 * This function creates a copy of the AMDeviceRef and does not manipulate the original.
 *
 * Given an AMRecoveryModeDeviceRef and an options dictionary, attempt to contact
 * the signing server and personalize the bundle.  On success, the outPreflightOptions 
 * dictionary will be non-NULL and its contents will contain the original restore 
 * options modified to use the newly-personalized restore bundle.  On error, the 
 * outPreflightOptions dictionary will be NULL.
 */
int
AMD_API
AMDeviceCopyAuthInstallPreflightOptions( 
    AMDeviceRef device, 
    CFDictionaryRef baseOptions, 
    CFDictionaryRef *outPreflightOptions );

/*!
 * @function AMDeviceRequestAbbreviatedSendSync
 * @abstract Attempts to send an abbreviated request to the personalization server
 * @param device The device we are requesting the build for
 * @param uniqueBuildID The build ID for the build we are requesting to install on the device
 * @param outResponse The response dictionary returned from the personalization server
 * @result An AMRestore error indicating whether the request was successful
 * @discussion
 *
 * Given an AMDeviceRef and a build ID, we attempt to send the personalization server
 * an abbreviated request. The server will return an AMRestore error specifying whether
 * this device is eligible for the build at this time.
 */
int
AMD_API
AMDeviceRequestAbbreviatedSendSync( 
    AMDeviceRef device, 
    CFDataRef uniqueBuildID, 
    CFDictionaryRef *outResponse );

/*!
 * @function AMDCopySystemBonjourUniqueID
 * @abstract Copy the System Bonjour Unique ID
 * @param options See discussion for supported options.
 * @result A CFString that represents a UUID.
 * @discussion
 *
 * The System Bonjour Unique ID is a string that identifies this particular OS installation (it is not
 * tied to any particular piece of hardware). A bonjour service advertised by this host machine may wish
 * to include this ID in the bonjour service advertisement. Devices which are paired with this host machine
 * will be able to use the ID included in the advertisment to quickly identify machines which are both
 * advertising services they want to use AND and with which they are already paired.
 *
 * No options are currently supported. Unsupported options are ignored.
 */
CFStringRef
AMD_API
AMDCopySystemBonjourUniqueID(
    CFDictionaryRef options);

/*!
 * @function AMDCopyAuthenticationIdentityForDevice
 * @param someIdentifier An identifier for the identity to retrieve.
 * @param options See discussion for supported options.
 * @param identity_out On success, a CFDictionary to be released by the caller.
 * @result See discussion for a description of the returned data
 * @discussion
 *
 * someIdentifier should be the Unique Device ID for the target device.
 *
 * The returned dictionary will have the following elements:
 * kAMDHostCertificate - A CFData object that represents the raw data of an SSL certificate as a PEM.
 * kAMDHostPrivateKey - A CFData object that repesebts the raw data of an SSL key as a PEM.
 *
 * No options are currently supported. Unsupported options are ignored.
 */
AMDError
AMD_API
AMDCopyAuthenticationIdentityForDevice(
    CFStringRef someIdentifier,
    CFDictionaryRef options,
    CFDictionaryRef *identity_out);

/*!
 * @function AMDCopyUDIDForPairingIdentity
 * @param pairingIdentity A CFData of the PEM encoded device certificate
 * @param udid_out On success, a new CFString of the UDID.
 * @result kAMDSuccess, or an error.
 *
 * @discussion
 * On success, a CFString will be returned in udid_out. Ownership folows the Copy rule.
 *
 * Returns kAMDNotFoundError if the pairingIdentity is unknown.
 */
AMDError
AMD_API
AMDCopyUDIDForPairingIdentity(
    CFDataRef pairingIdentity,
    CFStringRef *udid_out);

#if defined(__cplusplus)
}
#endif

#endif /* _APPLEMOBILEDEVICE_ */
