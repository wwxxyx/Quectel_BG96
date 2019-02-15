#ifndef _APPLEMOBILEDEVELOPMENT_
#define	_APPLEMOBILEDEVELOPMENT_

/* Header Declarations */
#include <MobileDevice/MobileDevice.h>

#if defined(__cplusplus)
extern "C" {
#endif

/* Constant Declarations */
#define	kAMDImageTypeKey			CFSTR("ImageType")
#define	kAMDImageTypeDeveloper		CFSTR("Developer")
#define	kAMDImageTypeDebug			CFSTR("Debug")
#define	kAMDImageTypeFactory        CFSTR("Factory")

#define kAMDImageSignatureKey		CFSTR("ImageSignature")

/*
* Image Mount Callback 
* Ownership of the status dictionary *status* passes to the callback function. The dict MUST BE 
* explicitly released or else it will leak.
*/
typedef 
void
(AMD_API *amd_mount_callback)(
	CFDictionaryRef status,
	void *context);
	
/*
* Mount a developer image on the device.
*/

AMDError
AMD_API
AMDeviceMountImage(
	AMDeviceRef device,
	CFStringRef image_path,
	CFDictionaryRef options,
	amd_mount_callback callback,
	void *context);

#define kAMDRelayTypeKey				CFSTR("RelayType")
#define kAMDRelayTypeFileDescriptor		CFSTR("RelayTypeFileDescriptor")
#define kAMDRelayTypeData				CFSTR("RelayTypeData")

#define kAMDRelayLocationKey			CFSTR("RelayLocation")
    
/*
 * Copy a file from the relay service.
 * Options must contain:
 *
 * kAMDRelayTypeKey: one of
 *     kAMDRelayTypeFileDescriptor
 *     kAMDRelayTypeData
 *
 * kAMDRelayLocationKey:
 *     for kAMDRelayTypeFileDescriptor, this must be a CFNumber containing a file descriptor
 *     on Windows, this must be a *file descriptor* that can be passed
 *     to the 'write' call; this is not the same as a Windows HANDLE.
 *
 *     for kAMDRelayTypeData, this must be a CFMutableData; the relayed stream will be appended
 *     to that CFMutableData
 */
AMDError
AMDeviceRelayFile(
	AMDeviceRef device,
	CFStringRef source,
	CFDictionaryRef options);

#if defined(__cplusplus)
}
#endif	
	
#endif /* _APPLEMOBILEDEVELOPMENT_ */
