//
// Package: HomeKit-ADK
// Version: 2.0 (16A62)
//
// Disclaimer: IMPORTANT: This Apple software is supplied to you, by Apple Inc. ("Apple"), in your
// capacity as a current, and in good standing, Licensee in the MFi Licensing Program. Use of this
// Apple software is governed by and subject to the terms and conditions of your MFi License,
// including, but not limited to, the restrictions specified in the provision entitled "Public
// Software", and is further subject to your agreement to the following additional terms, and your
// agreement that the use, installation, modification or redistribution of this Apple software
// constitutes acceptance of these additional terms. If you do not agree with these additional terms,
// you may not use, install, modify or redistribute this Apple software.
//
// Subject to all of these terms and in consideration of your agreement to abide by them, Apple grants
// you, for as long as you are a current and in good-standing MFi Licensee, a personal, non-exclusive
// license, under Apple's copyrights in this Apple software (the "Apple Software"), to use,
// reproduce, and modify the Apple Software in source form, and to use, reproduce, modify, and
// redistribute the Apple Software, with or without modifications, in binary form, in each of the
// foregoing cases to the extent necessary to develop and/or manufacture "Proposed Products" and
// "Licensed Products" in accordance with the terms of your MFi License. While you may not
// redistribute the Apple Software in source form, should you redistribute the Apple Software in binary
// form, you must retain this notice and the following text and disclaimers in all such redistributions
// of the Apple Software. Neither the name, trademarks, service marks, or logos of Apple Inc. may be
// used to endorse or promote products derived from the Apple Software without specific prior written
// permission from Apple. Except as expressly stated in this notice, no other rights or licenses,
// express or implied, are granted by Apple herein, including but not limited to any patent rights that
// may be infringed by your derivative works or by other works in which the Apple Software may be
// incorporated. Apple may terminate this license to the Apple Software by removing it from the list
// of Licensed Technology in the MFi License, or otherwise in accordance with the terms of such MFi License.
//
// Unless you explicitly state otherwise, if you provide any ideas, suggestions, recommendations, bug
// fixes or enhancements to Apple in connection with this software ("Feedback"), you hereby grant to
// Apple a non-exclusive, fully paid-up, perpetual, irrevocable, worldwide license to make, use,
// reproduce, incorporate, modify, display, perform, sell, make or have made derivative works of,
// distribute (directly or indirectly) and sublicense, such Feedback in connection with Apple products
// and services. Providing this Feedback is voluntary, but if you do provide Feedback to Apple, you
// acknowledge and agree that Apple may exercise the license granted above without the payment of
// royalties or further consideration to Participant.
//
// The Apple Software is provided by Apple on an "AS IS" basis. APPLE MAKES NO WARRANTIES, EXPRESS OR
// IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY
// AND FITNESS FOR A PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE OR
// IN COMBINATION WITH YOUR PRODUCTS.
//
// IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION, MODIFICATION
// AND/OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER UNDER THEORY OF CONTRACT, TORT
// (INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// Copyright (C) 2018 Apple Inc. All Rights Reserved.
//

#include "HAPPlatform.h"
#include "HAPPlatformKeyValueStore+SDKDomains.h"
#include "HAPPlatformMFiTokenAuth+Init.h"

static const HAPLogObject logObject = {
    .subsystem = kHAPPlatform_LogSubsystem,
    .category = "MFiTokenAuth"
};

void HAPPlatformMFiTokenAuthCreate(
    HAPPlatformMFiTokenAuthRef mfiTokenAuth,
    const HAPPlatformMFiTokenAuthOptions *options)
{
    HAPPrecondition(mfiTokenAuth);
    HAPPrecondition(options);
    HAPPrecondition(options->keyValueStore);
    
    mfiTokenAuth->keyValueStore = options->keyValueStore;
}

HAP_RESULT_USE_CHECK
HAPError HAPPlatformMFiTokenAuthLoad(
    HAPPlatformMFiTokenAuthRef mfiTokenAuth,
    bool *valid,
    HAPPlatformMFiTokenAuthUUID *_Nullable mfiTokenUUID,
    void *_Nullable mfiTokenBytes,
    size_t maxMFiTokenBytes,
    size_t *_Nullable numMFiTokenBytes)
HAP_DIAGNOSE_ERROR((!mfiTokenUUID && mfiTokenBytes) || (mfiTokenUUID && !mfiTokenBytes),
    "software token uuid and bytes may only be requested together")
HAP_DIAGNOSE_ERROR(!mfiTokenBytes && maxMFiTokenBytes, "empty buffer cannot have a length")
HAP_DIAGNOSE_ERROR((!mfiTokenBytes && numMFiTokenBytes) || (mfiTokenBytes && !numMFiTokenBytes),
    "optional buffer cannot return a length")
{
    HAPPrecondition(mfiTokenAuth);
    HAPPrecondition(valid);
    HAPPrecondition((mfiTokenUUID == NULL) == (mfiTokenBytes == NULL));
    HAPPrecondition(!maxMFiTokenBytes || mfiTokenBytes);
    HAPPrecondition((mfiTokenBytes == NULL) == (numMFiTokenBytes == NULL));
    
    HAPError err;
    
    bool foundMFiTokenUUID;
    size_t numMFiTokenUUIDBytes = 0;
    err = HAPPlatformKeyValueStoreGet(mfiTokenAuth->keyValueStore,
        kSDKKeyValueStoreDomain_Provisioning, kSDKKeyValueStoreKey_Provisioning_MFiTokenUUID,
        mfiTokenUUID, mfiTokenUUID ? sizeof *mfiTokenUUID : 0, mfiTokenUUID ? &numMFiTokenUUIDBytes : NULL,
        &foundMFiTokenUUID);
    if (err) {
        HAPAssert(err == kHAPError_Unknown);
        return err;
    }
    bool foundMFiToken;
    size_t numMFiTokenBytes_ = 0;
    err = HAPPlatformKeyValueStoreGet(mfiTokenAuth->keyValueStore,
        kSDKKeyValueStoreDomain_Provisioning, kSDKKeyValueStoreKey_Provisioning_MFiToken,
        mfiTokenBytes, maxMFiTokenBytes, mfiTokenBytes ? &numMFiTokenBytes_ : NULL, &foundMFiToken);
    if (err) {
        HAPAssert(err == kHAPError_Unknown);
        return err;
    }
    if (numMFiTokenBytes) {
        *numMFiTokenBytes = numMFiTokenBytes_;
    }
    
    *valid = foundMFiTokenUUID && foundMFiToken;
    if (!*valid) {
        return kHAPError_None;
    }
    if (numMFiTokenBytes && *numMFiTokenBytes == maxMFiTokenBytes) {
        HAPLog(&logObject, "Software Token does not fit into buffer: available = %lu bytes.",
            (unsigned long) maxMFiTokenBytes);
        return kHAPError_OutOfResources;
    }
    
    return kHAPError_None;
}

bool HAPPlatformMFiTokenAuthIsProvisioned(
    HAPPlatformMFiTokenAuthRef mfiTokenAuth)
{
    HAPPrecondition(mfiTokenAuth);
    
    HAPError err;
    
    bool valid;
    err = HAPPlatformMFiTokenAuthLoad(mfiTokenAuth, &valid, NULL, NULL, 0, NULL);
    if (err) {
        HAPAssert(err == kHAPError_Unknown);
        return false;
    }
    
    return valid;
}

HAP_RESULT_USE_CHECK
HAPError HAPPlatformMFiTokenAuthUpdate(
    HAPPlatformMFiTokenAuthRef mfiTokenAuth,
    const void *mfiTokenBytes,
    size_t numMFiTokenBytes)
{
    HAPPrecondition(mfiTokenAuth);
    HAPPrecondition(mfiTokenBytes);
    HAPPrecondition(numMFiTokenBytes <= kHAPPlatformMFiTokenAuth_MaxMFiTokenBytes);
    
    HAPError err;
    
    // Try to find old Software Token.
    bool found;
    err = HAPPlatformKeyValueStoreGet(mfiTokenAuth->keyValueStore,
        kSDKKeyValueStoreDomain_Provisioning, kSDKKeyValueStoreKey_Provisioning_MFiToken,
        NULL, 0, NULL, &found);
    if (err) {
        HAPAssert(err == kHAPError_Unknown);
        return err;
    }
    if (!found) {
        HAPLogInfo(&logObject, "Trying to update Software Token but no Software Token is present in key-value store.");
        return kHAPError_Unknown;
    }
    
    // Update Software Token.
    err = HAPPlatformKeyValueStoreSet(mfiTokenAuth->keyValueStore,
        kSDKKeyValueStoreDomain_Provisioning, kSDKKeyValueStoreKey_Provisioning_MFiToken,
        mfiTokenBytes, numMFiTokenBytes);
    if (err) {
        HAPAssert(err == kHAPError_Unknown);
        return err;
    }
    
    return kHAPError_None;
}
