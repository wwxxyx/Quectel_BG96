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

#include "HAPPlatformKeyValueStore+Init.h"

static const HAPLogObject logObject = {
    .subsystem = kHAPPlatform_LogSubsystem,
    .category = "KeyValueStore"
};

// This is a bootstrap implementation - must not be used for production accessories!

void HAPPlatformKeyValueStoreCreate(
    HAPPlatformKeyValueStoreRef keyValueStore,
    const HAPPlatformKeyValueStoreOptions *options)
{
    HAPPrecondition(keyValueStore);
    HAPPrecondition(options);
    HAPPrecondition(options->items);

    HAPLog(&logObject, "Using ephemeral KVS implementation - must not be used for production accessories!");

    keyValueStore->bytes = options->items;
    keyValueStore->maxBytes = options->numItems;
    HAPRawBufferZero(keyValueStore->bytes, sizeof keyValueStore->bytes[0] * keyValueStore->maxBytes);
}

HAP_RESULT_USE_CHECK
HAPError HAPPlatformKeyValueStoreGet(
    HAPPlatformKeyValueStoreRef keyValueStore,
    HAPPlatformKeyValueStoreDomain domain,
    HAPPlatformKeyValueStoreKey key,
    void *_Nullable const bytes,
    size_t maxBytes,
    size_t *_Nullable numBytes,
    bool *found)
HAP_DIAGNOSE_ERROR(!bytes && maxBytes, "empty buffer cannot have a length")
HAP_DIAGNOSE_ERROR((bytes && !numBytes) || (!bytes && numBytes),
    "length is only available if a buffer is supplied")
{
    HAPPrecondition(keyValueStore);
    HAPPrecondition(!maxBytes || bytes);
    HAPPrecondition((bytes == NULL) == (numBytes == NULL));
    HAPPrecondition(found);

    HAPLog(&logObject, "Using ephemeral KVS implementation - must not be used for production accessories!");

    *found = false;
    for (size_t i = 0; i < keyValueStore->maxBytes; i++) {
        if (!keyValueStore->bytes[i].active) {
            continue;
        }
        if (keyValueStore->bytes[i].domain != domain || keyValueStore->bytes[i].key != key) {
            continue;
        }

        *found = true;
        if (bytes) {
            HAPAssert(keyValueStore->bytes[i].numBytes <= sizeof keyValueStore->bytes[i].bytes);
            *numBytes = keyValueStore->bytes[i].numBytes < maxBytes ? keyValueStore->bytes[i].numBytes : maxBytes;
            HAPRawBufferCopyBytes(bytes, keyValueStore->bytes[i].bytes, *numBytes);
        }
        return kHAPError_None;
    }
    return kHAPError_None;
}

HAP_RESULT_USE_CHECK
HAPError HAPPlatformKeyValueStoreSet(
    HAPPlatformKeyValueStoreRef keyValueStore,
    HAPPlatformKeyValueStoreDomain domain,
    HAPPlatformKeyValueStoreKey key,
    const void *bytes,
    size_t numBytes)
{
    HAPPrecondition(keyValueStore);
    HAPPrecondition(bytes);

    HAPLog(&logObject, "Using ephemeral KVS implementation - must not be used for production accessories!");

    size_t index = 0;
    bool found = false;
    size_t free_index = 0;
    bool free_found = false;
    for (size_t i = 0; i < keyValueStore->maxBytes; i++) {
        if (keyValueStore->bytes[i].active &&
            keyValueStore->bytes[i].domain == domain &&
            keyValueStore->bytes[i].key == key)
        {
            index = i;
            found = true;
            break;
        }
        if (!keyValueStore->bytes[i].active) {
            free_index = i;
            free_found = true;
        }
    }
    if (!found && free_found) {
        index = free_index;
        found = true;
    }

    // Update.
    if (!found) {
        HAPLog(&logObject, "No free entry to store value for domain 0x%X / key 0x%04X.", domain, key);
        return kHAPError_Unknown;
    }

    if (numBytes > sizeof keyValueStore->bytes[index].bytes) {
        HAPLog(&logObject,
            "Not enough memory to store value with length %lu for domain 0x%X / key 0x%04X.",
            (unsigned long) numBytes, domain, key);
        return kHAPError_Unknown;
    }
    keyValueStore->bytes[index].active = true;
    keyValueStore->bytes[index].domain = domain;
    keyValueStore->bytes[index].key = key;
    keyValueStore->bytes[index].numBytes = numBytes;
    if (numBytes) {
        HAPRawBufferCopyBytes(keyValueStore->bytes[index].bytes, bytes, numBytes);
    }
    return kHAPError_None;
}

HAP_RESULT_USE_CHECK
HAPError HAPPlatformKeyValueStoreRemove(
    HAPPlatformKeyValueStoreRef keyValueStore,
    HAPPlatformKeyValueStoreDomain domain,
    HAPPlatformKeyValueStoreKey key)
{
    HAPPrecondition(keyValueStore);

    HAPLog(&logObject, "Using ephemeral KVS implementation - must not be used for production accessories!");

    for (size_t i = 0; i < keyValueStore->maxBytes; i++) {
        if (!keyValueStore->bytes[i].active) {
            continue;
        }
        if (keyValueStore->bytes[i].domain == domain && keyValueStore->bytes[i].key == key) {
            keyValueStore->bytes[i].active = false;
            return kHAPError_None;
        }
    }

    return kHAPError_None;
}

HAP_RESULT_USE_CHECK
HAPError HAPPlatformKeyValueStoreEnumerate(
    HAPPlatformKeyValueStoreRef keyValueStore,
    HAPPlatformKeyValueStoreDomain domain,
    HAPPlatformKeyValueStoreEnumerateCallback callback,
    void *_Nullable context)
{
    HAPPrecondition(keyValueStore);
    HAPPrecondition(callback);

    HAPLog(&logObject, "Using ephemeral KVS implementation - must not be used for production accessories!");

    HAPError err;

    bool cont = true;
    for (size_t i = 0; cont && i < keyValueStore->maxBytes; i++) {
        if (!keyValueStore->bytes[i].active) {
            continue;
        }
        if (keyValueStore->bytes[i].domain != domain) {
            continue;
        }

        HAPAssert(keyValueStore->bytes[i].numBytes <= sizeof keyValueStore->bytes[i].bytes);
        err = callback(context, keyValueStore, keyValueStore->bytes[i].domain, keyValueStore->bytes[i].key, &cont);
        if (err) {
            HAPAssert(err == kHAPError_Unknown);
            return err;
        }
    }
    return kHAPError_None;
}

HAP_RESULT_USE_CHECK
HAPError HAPPlatformKeyValueStorePurgeDomain(
    HAPPlatformKeyValueStoreRef keyValueStore,
    HAPPlatformKeyValueStoreDomain domain)
{
    HAPPrecondition(keyValueStore);

    HAPLog(&logObject, "Using ephemeral KVS implementation - must not be used for production accessories!");

    for (size_t i = 0; i < keyValueStore->maxBytes; i++) {
        if (!keyValueStore->bytes[i].active) {
            continue;
        }
        if (keyValueStore->bytes[i].domain != domain) {
            continue;
        }
        keyValueStore->bytes[i].active = false;
    }
    return kHAPError_None;
}
