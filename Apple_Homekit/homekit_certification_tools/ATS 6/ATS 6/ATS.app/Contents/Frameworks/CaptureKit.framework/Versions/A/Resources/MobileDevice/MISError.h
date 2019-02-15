#ifndef MISERROR_H
#define MISERROR_H

#include <CoreFoundation/CoreFoundation.h>

#include <MobileDevice/MISBase.h>

/*
 * The format of MISError is as described in <mach/error.h>
 * The value consists of a system, subsystem and error code
 * MIS-specific errors are defined and described below, but an MISError may
 * be an error from another systems/subsystems (like errno's or IOReturn's)
 *
 * MIS errors are in the MobileDevice system (0x3A), subsystem 2
 */

/* Wrap an errno value in the format of an MISError */
#define MISMakeErrnoError(err) ((3 << 14) | (err))  /* see unix_err in <mach/error.h> */

/* Constants a la IOReturn.h */
#define sys_mobiledevice     (0x3A << 26)   /* err_system(0x3A) */
#define sub_mobiledevice_mis (0x02 << 14)   /* err_sub(2) */

/* Macro for simple generation of error codes */
#define MISMakeError(err) (sys_mobiledevice | sub_mobiledevice_mis | err)

/* MIS errors */
enum {
    kMISNoError = 0,
    
    /* Generic "something went wrong but we don't know what" error */
    kMISUnknownError = MISMakeError(0x01),
    
    /* A mutable operation was performed an immutable object */
    kMISNotMutableError = MISMakeError(0x02),
    
    /* 
     * Some operation encountered a problem that should not happen with
     * a properly formed profile (for instance, a required key was missing)
     */
    kMISMalformedProfileError = MISMakeError(0x03),
    
    /* An operation that expected a signature did not find one */
    kMISProfileNotSignedError = MISMakeError(0x04),
    
    /* An error occurred trying to decode the public key from a file or data */
    kMISPublicKeyError = MISMakeError(0x05),

    /* An error occurred trying to decode the private key from a file or data */
    kMISPrivateKeyError = MISMakeError(0x06),
    
    /* An error occurred handling a certificate */
    kMISCertificateError = MISMakeError(0x07),
    
    /* An error occurred during signing */
    kMISSigningError = MISMakeError(0x08),
    
    /* 
     * The signature was not valid for the provided data.  Where possible, the 
     * MIS API will attempt to return kMISApplicationSignatureNotValidError or 
     * kMISProfileSignatureNotValidError to indicate what type of data's
     * signature was not valid.
     */
    kMISSignatureNotValidError = MISMakeError(0x09),

    /* An attempt to allocate memory or create an object failed */
    kMISAllocationError = MISMakeError(0x0a),
    
    /* A generic file error occurred where we don't have more specific info */
    kMISFileError = MISMakeError(0x0b),
    
    /* A connection was unexpectedly closed */
    kMISConnectionClosedError = MISMakeError(0x0c),
    
    /* Received data did not match protocol */
    kMISProtocolError = MISMakeError(0x0d),

    /* 
     * No signature
     * Deprecated in favor of kMISProfileNotSignedError and 
     * kMISCodeNotSignedError which convey specifically what is unsigned
     */
    kMISUnsignedError = MISMakeError(0x0e),

    /* The signature on a binary is valid, but it has an unexpected hash */
    kMISUnexpectedHashError = MISMakeError(0x0f),

    /* A version is unsupported */
    kMISUnsupportedVersionError = MISMakeError(0x10),

    /* Something has expired */
    kMISExpiredError = MISMakeError(0x11),

    /* The device is not supported */
    kMISUnsupportedDeviceError = MISMakeError(0x12),
    
    /* Cert chain isn't trusted */
    kMISUntrustedCertChainError = MISMakeError(0x13),
    
    /* Code directory is present, but not signed */
    kMISAdHocSignatureError = MISMakeError(0x14),
    
    /* The signer on a binary did not match a provisioning profile */
    kMISInvalidSignerError = MISMakeError(0x15),
    
    /* A binary contained entitlements not permitted by its profile */
    kMISInvalidEntitlementError = MISMakeError(0x16),
    
    /* Application not valid because resources have been modified */
    kMISModifiedResourcesError = MISMakeError(0x17),

    /* 
     * Signature not valid because the certificate used for signing was not
     * valid (it may have expired or been revoked, among other things)
     */
    kMISInvalidSignerCertError = MISMakeError(0x18),

    /* The application signature was not valid for the provided data */
    kMISApplicationSignatureNotValidError = MISMakeError(0x19),

    /* The profile signature was not valid for the provided data */
    kMISProfileSignatureNotValidError = MISMakeError(0x1A),
    
    /* Communication with the device failed */
    kMISDeviceCommunicationError = MISMakeError(0x1B),

    /* The binary or bundle was not signed */
    kMISCodeNotSignedError = MISMakeError(0x1C),

    /* 
     * This is just used in the tests to make it easy to test that all errors
     * have an associated string (so we don't add new errors without strings)
     */
    kMISMaximumErrorValue,
};
typedef int MISError;

MIS_EXPORT CFStringRef MISCopyErrorStringForErrorCode(MISError error);

#endif
