#ifndef MISPROFILE_H
#define MISPROFILE_H

#include <CoreFoundation/CoreFoundation.h>

#include <MobileDevice/MISBase.h>
#include <MobileDevice/MISError.h>

typedef const struct _MISProfile *MISProfileRef;
typedef struct _MISProfile *MISMutableProfileRef;


/* Type ID for class */
MIS_EXPORT CFTypeID MISProfileGetTypeID(void);

/* Creating a profile object */
MIS_EXPORT MISMutableProfileRef MISProfileCreate(CFAllocatorRef allocator);
MIS_EXPORT MISProfileRef MISProfileCreateWithData(CFAllocatorRef allocator, CFDataRef data);
MIS_EXPORT MISProfileRef MISProfileCreateWithFile(CFAllocatorRef allocator, CFStringRef file);

/*
 * Create a mutable copy of a profile
 * If the original is signed, the signature is discarded (so the copy will
 * need to be resigned).
 * All contents of the profile are preserved
 */
MIS_EXPORT MISMutableProfileRef MISProfileCreateMutableCopy(CFAllocatorRef allocator, MISProfileRef profile);

/* Test whether the profile can be updated */
MIS_EXPORT Boolean MISProfileIsMutable(MISProfileRef profile);

/* Create the signature for a profile */
MIS_EXPORT MISError MISProfileSignWithKeyAndCertificates(MISMutableProfileRef profile, CFTypeRef key, CFArrayRef certs);
MIS_EXPORT MISError MISProfileSignWithRSACallBack(MISMutableProfileRef profile, int (*rsaCallback)(CFDataRef, CFDataRef *), CFArrayRef certs);

/* Check that the signature on a profile is valid */
MIS_EXPORT MISError MISProfileValidateSignature(MISProfileRef profile);
MIS_EXPORT MISError MISProfileValidateSignatureWithAnchors(MISProfileRef profile, CFArrayRef anchors);

/* Not for public consumption */
MIS_EXPORT MISError MISProfileValidateSignatureWithAnchorsAndPolicy(MISProfileRef profile, CFArrayRef anchors, int policyType);

/*
 * Copy a human-readable summary of the signer's certificate.  The caller must
 * release the returned string.
 */
MIS_EXPORT MISError MISProfileCopySignerSubjectSummary(MISProfileRef profile, CFStringRef *summary);

/*
 * Create a data representation of the profile.  It can be converted back to
 * a MISProfileRef with MISProfileCreateWithData
 */
MIS_EXPORT CFDataRef MISProfileCreateDataRepresentation(MISProfileRef profile);


/* Write a (signed) profile to disk */
MIS_EXPORT MISError MISProfileWriteToFile(MISProfileRef profile, CFStringRef file);

/* Generic accessor */
MIS_EXPORT CFTypeRef MISProfileGetValue(MISProfileRef profile, CFStringRef key);

/* 
 * Generic setter - replaces existing value for key if present 
 * Returns true if the value was sucessfully set
 */
MIS_EXPORT MISError MISProfileSetValue(MISMutableProfileRef profile, CFStringRef key, CFTypeRef value);

/* Remove a value from the profile by key */
MIS_EXPORT MISError MISProfileRemoveValue(MISMutableProfileRef profile, CFStringRef key);

/*
 * Set the payload of the profile to the contents of an existing dictionary
 * If data had previously been added to the profile with MISProfileSetValue
 * that data is discarded when this call is made.  After making this call,
 * the profile is still mutable and additional data can be added with
 * MISProfileSetValue
 * This call copies the contents of the provided dictionary, so modifications
 * to it after this call will not be reflected in the profile.
 */
MIS_EXPORT MISError MISProfileSetPayload(MISMutableProfileRef profile, CFDictionaryRef payload);

/*
 * Retrieve the entire payload in the form of a CFDictionary.  This works 
 * whether the profile was created with MISProfileSetValue or 
 * MISProfileSetPayload (or some combination of the two), or loaded from 
 * disk/memory.
 */
MIS_EXPORT CFDictionaryRef MISProfileCopyPayload(MISProfileRef profile);

#endif
