#ifndef _APPLEMOBILEPROVISIONING_
#define	_APPLEMOBILEPROVISIONING_

/* Header Declarations */
#include <MobileDevice/MobileDevice.h>
#include <MobileDevice/MISProfile.h>

#if defined(__cplusplus)
extern "C" {
#endif

/*
 * Installs a provisioning profile on the device.  If an existing profile exists
 * with the same profile id, it will be replaced.  This call will return an
 * error if the profile is invalid (examples would be an invalid signature, 
 * version is too old, device id not included in profile, etc).
 */
extern AMDError AMD_API AMDeviceInstallProvisioningProfile(AMDeviceRef device, MISProfileRef profile);

/*
 * Removes a provisioning profile from the device.  The profile id is the UUID
 * of the profile.
 */
extern AMDError AMD_API AMDeviceRemoveProvisioningProfile(AMDeviceRef device, CFStringRef profileID);

/*
 * Returns a list of installed provisioning profiles on the device.  Returns
 * NULL if the list cannot be retrieved.  The caller must release the returned
 * array.
 */
extern CFArrayRef AMD_API AMDeviceCopyProvisioningProfiles(AMDeviceRef device);

#if defined(__cplusplus)
}
#endif

#endif /* _APPLEMOBILEPROVISIONING_ */
