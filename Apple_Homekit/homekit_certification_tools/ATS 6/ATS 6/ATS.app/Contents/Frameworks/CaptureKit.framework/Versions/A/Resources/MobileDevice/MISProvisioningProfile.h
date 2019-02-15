#ifndef MISPROVISIONINGPROFILE_H
#define MISPROVISIONINGPROFILE_H

#include <CoreFoundation/CoreFoundation.h>

#include <MobileDevice/MISBase.h>
#include <MobileDevice/MISProfile.h>

/* Creating a provisioning profile object */
MIS_EXPORT MISMutableProfileRef MISProfileCreateForProvisioning(CFAllocatorRef allocator);

/*
 * Create a mutable copy of a provisioning profile
 * If the original is signed, the signature is discarded (so the copy will
 * need to be resigned).  The copy is assigned a new UUID and creation time
 * Behavior is undefined if the source is not a provisioning profile
 */
MIS_EXPORT MISMutableProfileRef MISProvisioningProfileCreateMutableCopy(CFAllocatorRef allocator, MISProfileRef profile);

/* Set information within a profile; return an error if the profile cannot be modified */
MIS_EXPORT MISError MISProvisioningProfileAddEntitlement(MISMutableProfileRef profile, CFStringRef entitlement, CFTypeRef value);

/*
 * Set/test whether a device is provisioned by a profile
 * If MISProvisioningProfileSetProvisionsAllDevices is called, it creates a 
 * profile that will be considered on any device, and 
 * MISProvisioningProfileIncludesDevice will always return true for such a
 * profile.
 * On other profiles, MISProvisioningProfileIncludesDevice will only return
 * true if the profile includes the provided UDID (added at profile creation
 * with MISProvisioningProfileAddProvisionedDevice)
 * The behavior of a profile that is created with both 
 * MISProvisioningProfileSetProvisionsAllDevices and 
 * MISProvisioningProfileAddProvisionedDevice is undefined and should be avoided
 * by clients.
 */
MIS_EXPORT MISError MISProvisioningProfileSetProvisionsAllDevices(MISMutableProfileRef profile);
MIS_EXPORT MISError MISProvisioningProfileAddProvisionedDevice(MISMutableProfileRef profile, CFStringRef deviceID);
MIS_EXPORT Boolean MISProvisioningProfileIncludesDevice(MISProfileRef profile, CFStringRef deviceID);
MIS_EXPORT Boolean MISProvisioningProfileProvisionsAllDevices(MISProfileRef profile);

/* 
 * Test whether a specific entitlement is granted by the profile
 * Returns true if profile grants the entitlement, false if it is restricted
 *
 * This check depends on both the type of the value stored in the profile
 * and the type of the value being checked:
 *   If the stored entitlement is a boolean, then the passed in value must be
 *   a boolean with an equal value
 *
 *   If the stored entitlement is a string, then this function returns true if
 *   the passed in value matches the stored string.
 *
 *   If the stored entitlement is an array, then this function returns true if
 *   the passed in value is a string and it matches one of the strings
 *   contained in the array.  It also returns true if the passed in value is
 *   an array and each value is a string that matches one of the strings in
 *   the stored entitlement.
 */
MIS_EXPORT Boolean MISProvisioningProfileGrantsEntitlement(MISProfileRef profile, CFStringRef entitlement, CFTypeRef value);

/*
 * Set/get the application identifier prefix.  This is the prefix that is used
 * to generate a namespace for the application-identifier.  The caller must
 * release the array returned by 
 * MISProvisioningProfileCopyApplicationIdentifierPrefixes
 */
MIS_EXPORT MISError MISProvisioningProfileSetApplicationIdentifierPrefixes(MISMutableProfileRef profile, CFArrayRef prefixes);
MIS_EXPORT CFArrayRef MISProvisioningProfileCopyApplicationIdentifierPrefixes(MISProfileRef profile);

/* 
 * Set/get the expiration date for the blob
 * By default, a profile never expires
 */
MIS_EXPORT MISError MISProvisioningProfileSetTimeToLive(MISMutableProfileRef profile, int days);
MIS_EXPORT CFDateRef MISProvisioningProfileGetExpirationDate(MISProfileRef profile);

/* 
 * Set/get the profile name
 * The name is an arbitrary string that can be displayed to users and is not
 * used internally at all.
 */
MIS_EXPORT MISError MISProvisioningProfileSetName(MISMutableProfileRef profile, CFStringRef name);
MIS_EXPORT CFStringRef MISProvisioningProfileGetName(MISProfileRef profile);

/* 
 * Set/get the developers' public certificates
 * Since a provisioning profile can contain more than one certificate, the old
 * setter/getter methods are deprecated in favor of the new interface that
 * can handle multiple certificates.
 * XXX: Remove the deprecated methods
 */
MIS_EXPORT MISError MISProvisioningProfileAddDeveloperCertificate(MISMutableProfileRef profile, CFDataRef cert);
MIS_EXPORT MISError MISProvisioningProfileSetDeveloperCertificates(MISMutableProfileRef profile, CFArrayRef certs);
MIS_EXPORT CFArrayRef MISProvisioningProfileGetDeveloperCertificates(MISProfileRef profile);

/*
 * Functions for accessing data in the profile
 * Note that these functions do not test the profile for validity before returning
 * the value.
 */
MIS_EXPORT CFArrayRef MISProvisioningProfileGetProvisionedDevices(MISProfileRef profile);
MIS_EXPORT CFStringRef MISProvisioningProfileGetUUID(MISProfileRef profile);
MIS_EXPORT int MISProvisioningProfileGetVersion(MISProfileRef profile);
MIS_EXPORT CFDateRef MISProvisioningProfileGetCreationDate(MISProfileRef profile);
MIS_EXPORT CFDictionaryRef MISProvisioningProfileGetEntitlements(MISProfileRef profile);

/* 
 * Check that the signature on a profile is valid
 * This will use any specific anchors necessary for validating the signature
 * on a provisioning profile and should be used in preference to 
 * MISProfileValidateSignature when you know you are dealing with a provisioning
 * profile
 */
MIS_EXPORT MISError MISProvisioningProfileValidateSignature(MISProfileRef profile);

/* 
 * Check profile for validity.  This always includes signature validation but
 * the UDID and expiration checks can be skipped by passing in NULL for those
 * values.
 * Returns kMISNoError if the profile is valid, otherwise it returns an error
 * describing why it is considered invalid
 */
MIS_EXPORT MISError MISProvisioningProfileCheckValidity(MISProfileRef profile, CFStringRef deviceID, CFDateRef date);
 
/* Check to see if the profile is an Apple internal profile */
MIS_EXPORT Boolean MISProvisioningProfileIsAppleInternalProfile(MISProfileRef profile);

#endif
