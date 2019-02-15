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

// Implementation of the Remote profile.
//
// /!\ It is strongly recommended not to modify Remote.c or Siri.c, to simplify the integration of revised
// versions of these files.

#include "DB.h"
#include "Remote.h"
#include "Siri.h"

/**
 * Key value store domain for the configuration of the remote.
 */
#define kAppKeyValueStoreDomain_Configuration                                   ((HAPPlatformKeyValueStoreDomain) 0x00)

/**
 * Key value store domain for the first target of the remote.
 */
#define kAppKeyValueStoreDomain_TargetBegin                                     ((HAPPlatformKeyValueStoreDomain) 0x01)

/**
 * Key value store domain for the last target of the remote.
 */
#define kAppKeyValueStoreDomain_TargetEnd                         ((HAPPlatformKeyValueStoreDomain) kRemote_MaxTargets)

/**
 * Key for the configuration version of the key value store configuration.
 *
 * (kAppKeyValueStoreDomain_Configuration, kAppKeyValueStoreKey_ConfigurationVersion)
 */
#define kAppKeyValueStoreKey_ConfigurationVersion                               ((HAPPlatformKeyValueStoreKey)    0x00)

/**
 * Key for the active identifier of the key value store configuration.
 *
 * (kAppKeyValueStoreDomain_Configuration, kAppKeyValueStoreKey_ConfigurationActiveIdentifier)
 */
#define kAppKeyValueStoreKey_ConfigurationActiveIdentifier                      ((HAPPlatformKeyValueStoreKey)    0x01)

/**
 * Key for the target configuration.
 */
#define kAppKeyValueStoreKey_TargetConfiguration                                ((HAPPlatformKeyValueStoreKey)    0x00)

/**
 * Version number for the key value store format.
 */
#define kRemoteKeyValueStoreFormat_Version                                      ((uint16_t)                       0x00)

/**
 * Handle the number of targets
 */
HAP_STATIC_ASSERT(kRemote_MaxSessions >= kRemote_MaxTargets, NotEnoughSessionsToSupportAllTargets);
HAP_STATIC_ASSERT(kRemote_MaxTargets >= 1 && kRemote_MaxTargets <= UINT8_MAX, InvalidNumberofTargets);

/**
 * Ticks per second.
 */
#define kHAPTimeTicksPerSecond                                                  ((uint64_t)                  HAPSecond)

/**
 * Make sure the Remote Button type matches the target control characteristic values.
 */
/**@{*/
HAP_STATIC_ASSERT(kRemoteButtonType_Menu ==
    (RemoteButtonType) kHAPCharacteristicValue_TargetControl_ButtonType_Menu, kRemoteButtonType_Menu);
HAP_STATIC_ASSERT(kRemoteButtonType_PlayPause ==
    (RemoteButtonType) kHAPCharacteristicValue_TargetControl_ButtonType_PlayPause, kRemoteButtonType_PlayPause);
HAP_STATIC_ASSERT(kRemoteButtonType_TVHome ==
    (RemoteButtonType) kHAPCharacteristicValue_TargetControl_ButtonType_TVHome, kRemoteButtonType_TVHome);
HAP_STATIC_ASSERT(kRemoteButtonType_Select ==
    (RemoteButtonType) kHAPCharacteristicValue_TargetControl_ButtonType_Select, kRemoteButtonType_Select);
HAP_STATIC_ASSERT(kRemoteButtonType_ArrowUp ==
    (RemoteButtonType) kHAPCharacteristicValue_TargetControl_ButtonType_ArrowUp, kRemoteButtonType_ArrowUp);
HAP_STATIC_ASSERT(kRemoteButtonType_ArrowRight ==
    (RemoteButtonType) kHAPCharacteristicValue_TargetControl_ButtonType_ArrowRight, kRemoteButtonType_ArrowRight);
HAP_STATIC_ASSERT(kRemoteButtonType_ArrowDown ==
    (RemoteButtonType) kHAPCharacteristicValue_TargetControl_ButtonType_ArrowDown, kRemoteButtonType_ArrowDown);
HAP_STATIC_ASSERT(kRemoteButtonType_ArrowLeft ==
    (RemoteButtonType) kHAPCharacteristicValue_TargetControl_ButtonType_ArrowLeft, kRemoteButtonType_ArrowLeft);
HAP_STATIC_ASSERT(kRemoteButtonType_VolumeUp ==
    (RemoteButtonType) kHAPCharacteristicValue_TargetControl_ButtonType_VolumeUp, kRemoteButtonType_VolumeUp);
HAP_STATIC_ASSERT(kRemoteButtonType_VolumeDown ==
    (RemoteButtonType) kHAPCharacteristicValue_TargetControl_ButtonType_VolumeDown, kRemoteButtonType_VolumeDown);
HAP_STATIC_ASSERT(kRemoteButtonType_Siri ==
    (RemoteButtonType) kHAPCharacteristicValue_TargetControl_ButtonType_Siri, kRemoteButtonType_Siri);
HAP_STATIC_ASSERT(kRemoteButtonType_Power ==
    (RemoteButtonType) kHAPCharacteristicValue_TargetControl_ButtonType_Power, kRemoteButtonType_Power);
HAP_STATIC_ASSERT(kRemoteButtonType_Generic ==
    (RemoteButtonType) kHAPCharacteristicValue_TargetControl_ButtonType_Generic, kRemoteButtonType_Generic);
/**@}*/

static const HAPLogObject logObject = {
    .subsystem = "com.apple.mfi.HomeKit.Remote",
    .category = "Remote"
};

static void HandleSiriAccept(
    HAPAccessoryServerRef *server,
    const HAPServiceRequest *request);

static void HandleSiriInvalidate(
    HAPAccessoryServerRef *server,
    const HAPServiceRequest *request);

static void GetSiriContexts(
    HAPAccessoryServerRef *server,
    const HAPServiceRequest *request,
    HAPSiriAudioContext **_Nullable siriAudioContext,
    HAPSiriDataStreamContext **_Nullable siriDataStreamContext);

static const HAPSiriDelegate siriDelegate = {
    .handleSiriAccept = HandleSiriAccept,
    .handleSiriInvalidate = HandleSiriInvalidate,
    .getSiriContexts = GetSiriContexts,
};

HAP_STATIC_ASSERT(sizeof(RemoteButtonID) <= sizeof(HAPPlatformKeyValueStoreKey), RemoteButtonIDFitsIntoKVS);

/**
 * Check function to determine whether a button is valid.
 *
 * @param      value                Button Type.
 * @return                          True, iff the button type is valid. False, otherwise.
 */
HAP_RESULT_USE_CHECK
static bool IsValidButtonType(
    HAPCharacteristicValue_TargetControl_ButtonType value)
{
    switch (value) {
        case kHAPCharacteristicValue_TargetControl_ButtonType_Menu:
        case kHAPCharacteristicValue_TargetControl_ButtonType_PlayPause:
        case kHAPCharacteristicValue_TargetControl_ButtonType_TVHome:
        case kHAPCharacteristicValue_TargetControl_ButtonType_Select:
        case kHAPCharacteristicValue_TargetControl_ButtonType_ArrowUp:
        case kHAPCharacteristicValue_TargetControl_ButtonType_ArrowRight:
        case kHAPCharacteristicValue_TargetControl_ButtonType_ArrowDown:
        case kHAPCharacteristicValue_TargetControl_ButtonType_ArrowLeft:
        case kHAPCharacteristicValue_TargetControl_ButtonType_VolumeUp:
        case kHAPCharacteristicValue_TargetControl_ButtonType_VolumeDown:
        case kHAPCharacteristicValue_TargetControl_ButtonType_Siri:
        case kHAPCharacteristicValue_TargetControl_ButtonType_Power:
        case kHAPCharacteristicValue_TargetControl_ButtonType_Generic: {
            return true;
        }
    }
    return false;
}

/**
 * HAP Session Information associated to the Remote.
 */
/**@{*/
typedef struct {
    HAPTime timestamp;
    RemoteTargetIdentifier activeIdentifier;
    RemoteButtonID buttonID;
    RemoteButtonState buttonState;
} ButtonEvent;

typedef struct {
    ButtonEvent queuedEvents[kRemote_MaxQueuedButtonEvents];
    size_t numEvents; // Number of events currently in queue
    size_t tailIndex; // Index of next event to be enqueued, if not full
    size_t headIndex; // Index of next event to be dequeued, if not empty
    // headIndex == tailIndex => (numEvents == 0) || (numEvents == kRemote_MaxQueuedButtonEvents)
} QueuedButtonEvents;

typedef struct {
    bool inUse: 1;
    bool active: 1;
    bool siriChannelActive :1;
    bool buttonEventsSubscribed: 1;
    HAPSiriDataStreamContext siriDataStreamContext;
    const HAPSessionRef *session;
    QueuedButtonEvents queuedButtonEvents;
} RemoteSession;
/**@}*/

/**
 * Remote Control.
 */
/**@{*/
typedef struct {
    RemoteRef *_Nullable nextRemote;
    struct {
        const RemoteSupportedButton *buttons;
        size_t numButtons;
    } supportedButtons;
    struct {
        RemoteTargetIdentifier activeIdentifier;
    } state;
    HAPCharacteristicValue_TargetControl_Type type;
    const RemoteCallbacks *callbacks;
    HAPPlatformKeyValueStoreRef keyValueStore;
    HAPPlatformMicrophoneRef microphone;
    RemoteSession registeredSessions[kRemote_MaxSessions];
    RemoteSession *activeSessionInfo;
    RemoteSession *voiceSession;
    const HAPAccessory *accessory;
    HAPAccessoryServerRef *server;
    HAPSiriAudioContext siriAudioContext;
} Remote;
HAP_STATIC_ASSERT(sizeof(RemoteRef) >= sizeof(Remote), RemoteRef_Opaque_Type_mismatch);
/**@}*/

/**
 * Global Configuration
 */
/**@{*/
typedef struct {
    bool initialized;
    Remote *remotes;
} RemoteStorage;
static RemoteStorage remoteStorage;

static void AddRemote(
    Remote *remote)
{
    HAPPrecondition(remote);
    
    remote->nextRemote = NULL;
    if (remoteStorage.remotes == NULL) {
        remoteStorage.remotes = remote;
        return;
    }
    Remote *lastRemote = remoteStorage.remotes;
    HAPAssert(lastRemote != remote);
    while (lastRemote->nextRemote != NULL) {
        lastRemote = (Remote *) lastRemote->nextRemote;
        HAPAssert(lastRemote != remote);
    }
    lastRemote->nextRemote = (RemoteRef *) remote;
}

static void RemoveRemote(
    Remote *remote)
{
    HAPPrecondition(remote);
    HAPPrecondition(remoteStorage.remotes);
    
    Remote *prev = remoteStorage.remotes;
    Remote *next = (Remote *) prev->nextRemote;
    if (prev == remote) {
        prev->nextRemote = NULL;
        remoteStorage.remotes = next;
        return;
    }
    if (next == NULL) {
        HAPLogError(&logObject, "%s: Could not find remote %p.", __func__, (const void *) remote);
        HAPFatalError();
    }
    Remote *current = next;
    next = (Remote *) current->nextRemote;
    while (current != remote) {
        prev = current;
        current = next;
        if (current == NULL) {
            HAPLogError(&logObject, "%s: Could not find remote %p.", __func__, (const void *) remote);
            HAPFatalError();
        }
        next = (Remote *) current->nextRemote;
    }
    prev->nextRemote = (RemoteRef *) next;
    current->nextRemote = NULL;
}
/**@}*/

/**
 * Callback to enumerate targets.
 *
 * @param      remote               Remote.
 * @param      context              Enumeration context.
 * @param      targetDomain         Target domain.
 * @param[in,out] shouldContinue    True if enumeration shall continue, False otherwise. Is set to true on input.
 *
 * @return kHAPError_None           If successful.
 * @return kHAPError_Unknown        On error.
 */
HAP_RESULT_USE_CHECK
typedef HAPError (*TargetsEnumerateCallback)(
    Remote *remote,
    void *_Nullable context,
    HAPPlatformKeyValueStoreDomain targetDomain,
    bool *shouldContinue);

/**
 * Enumeration callback for configured buttons on a target.
 *
 * @param      remote               Remote.
 * @param      targetConfiguration  Target configuration.
 * @param      context              Enumeration context.
 * @param      targetDomain         Target domain.
 * @param      buttonKey            Button key.
 * @param[in,out] shouldContinue    True if enumeration shall continue, False otherwise. Is set to true on input.
 *
 * @return kHAPError_None           If successful.
 * @return kHAPError_Unknown        On error.
 */
HAP_RESULT_USE_CHECK
typedef HAPError (*TargetConfigurationButtonsEnumerateCallback)(
    Remote *remote,
    const RemoteTargetConfiguration *targetConfiguration,
    void *_Nullable context,
    HAPPlatformKeyValueStoreDomain targetDomain,
    HAPPlatformKeyValueStoreKey buttonKey,
    bool *shouldContinue);

/**
 * Enumeration target configurations attached to a remote.
 *
 * @param      remote               Remote.
 * @param      callback             Callback function to be called on each target.
 * @param      context              Enumeration context.
 *
 * @return kHAPError_None           If successful.
 * @return kHAPError_Unknown        If an error was thrown in the callback.
 * @return kHAPError_OutOfResources If not enough resources available in the callback.
 */
HAP_RESULT_USE_CHECK
static HAPError TargetConfigurationsEnumerate(
    Remote *remote,
    TargetsEnumerateCallback callback,
    void *_Nullable context)
{
    HAPPrecondition(remote);
    HAPPrecondition(remote->keyValueStore);
    
    HAPError err;
    
    bool cont = true;
    for (size_t i = (size_t) kAppKeyValueStoreDomain_TargetBegin;
         i <= (size_t) kAppKeyValueStoreDomain_TargetEnd; i++) {
        
        HAPPlatformKeyValueStoreDomain domain = (HAPPlatformKeyValueStoreDomain) i;
        bool found;
        
        err = HAPPlatformKeyValueStoreGet(remote->keyValueStore, domain, 0, NULL, 0, NULL, &found);
        if (err) {
            HAPLogError(&logObject, "Unable to access key value store.");
            HAPFatalError();
        }
        if (!found) {
            continue;
        }
        err = callback(remote, context, domain, &cont);
        if (err) {
            HAPAssert(err == kHAPError_Unknown || err == kHAPError_OutOfResources);
            return err;
        }
        if (!cont) {
            break;
        }
    }
    
    return kHAPError_None;
}

/**
 * Storage type for the Target Identifier of the Remote.
 */
typedef uint32_t StoredRemoteTargetIdentifier;
HAP_STATIC_ASSERT(sizeof(StoredRemoteTargetIdentifier) == sizeof(RemoteTargetIdentifier),
    Remote_KVS_StorageIdentifierMismatch);

/**
 * Storage type for the Target Category of the Remote.
 */
typedef uint16_t StoredRemoteCategory;

/**
 * Storage for the name of the Target.
 * Format: <uint16_t><char Buffer with 64*4+1 maximum size>
 */
/**@{*/
typedef uint16_t StoredRemoteNameLength;

#define kMaxStoredRemoteNameBytesNullTerminated (kRemote_MaxIdentifierBytes + 1)
HAP_STATIC_ASSERT(kMaxStoredRemoteNameBytesNullTerminated == (64 * 4 + 1),
    Remote_KVS_StoredRemoteNameBytesMinimalLengthNotSatisfied);
/**@}*/


static HAPError TargetConfigurationReadDomain(
    Remote *remote,
    HAPPlatformKeyValueStoreDomain domain,
    RemoteTargetConfiguration *config)
{
    HAPPrecondition(remote);
    HAPPrecondition(config);
    
    HAP_DIAGNOSTIC_PUSH
    HAP_DIAGNOSTIC_IGNORED_GCC("-Wtype-limits")
    HAPPrecondition(domain >= kAppKeyValueStoreDomain_TargetBegin);
    HAPPrecondition(domain <= kAppKeyValueStoreDomain_TargetEnd);
    HAP_DIAGNOSTIC_POP
    
    HAPPrecondition(sizeof(StoredRemoteTargetIdentifier) == sizeof(uint32_t));
    HAPPrecondition(sizeof(StoredRemoteCategory) == sizeof(uint16_t));
    HAPPrecondition(sizeof(StoredRemoteNameLength) == sizeof(uint16_t));
    
    HAPError err;
    
    HAPRawBufferZero(config, sizeof *config);
    
    char bytes[sizeof(StoredRemoteTargetIdentifier) +
               sizeof(StoredRemoteCategory) +
               sizeof(StoredRemoteNameLength) + kMaxStoredRemoteNameBytesNullTerminated];
    const size_t maxBytes = sizeof bytes;
    size_t numBytes;
    bool found;
    
    err = HAPPlatformKeyValueStoreGet(
        remote->keyValueStore,
        domain, kAppKeyValueStoreKey_TargetConfiguration,
        bytes, maxBytes,
        &numBytes,
        &found);
    if (err) {
        HAPAssert(err == kHAPError_Unknown);
        return err;
    }
    if (!found) {
        HAPLog(&logObject, "Unable to find target configuration in domain %u", domain);
        return kHAPError_Unknown;
    }
    if (numBytes > maxBytes) {
        HAPLogError(&logObject, "%s: Stored data for remote %p domain %u too big. Changed storage format?",
            __func__, (const void *) remote, domain);
        HAPFatalError();
    }
    
    size_t pos = 0;
    
    config->targetIdentifier = HAPReadLittleUInt32(&bytes[pos]);
    pos += sizeof(StoredRemoteTargetIdentifier);
    
    config->targetCategory = (HAPAccessoryCategory) HAPReadLittleUInt16(&bytes[pos]);
    pos += sizeof(StoredRemoteCategory);
    
    if (pos == numBytes) {
        config->targetName.isDefined = false;
        config->targetName.bytes[0] = '\0';
        
        return kHAPError_None;
    }
    
    // Make sure there is still space left to read.
    config->targetName.isDefined = true;
    
    HAPAssert(pos + sizeof(StoredRemoteNameLength) <= numBytes);
    size_t nameBytes = (size_t) HAPReadLittleUInt16(&bytes[pos]);
    pos += sizeof(StoredRemoteNameLength);
    
    // Check target size.
    HAPAssert(nameBytes <= sizeof(config->targetName.bytes));
    HAPAssert(pos + nameBytes <= numBytes);
    
    HAPRawBufferCopyBytes(config->targetName.bytes, &bytes[pos], nameBytes);
    pos += sizeof nameBytes;
    
    HAPAssert(config->targetName.bytes[nameBytes - 1] == '\0');
    HAPAssert(pos <= maxBytes);
    
    return kHAPError_None;
}

static HAPError TargetConfigurationWriteDomain(
    Remote *remote,
    HAPPlatformKeyValueStoreDomain domain,
    const RemoteTargetConfiguration *config)
{
    HAPPrecondition(remote);
    HAPPrecondition(config);
    
    HAP_DIAGNOSTIC_PUSH
    HAP_DIAGNOSTIC_IGNORED_GCC("-Wtype-limits")
    HAPPrecondition(domain >= kAppKeyValueStoreDomain_TargetBegin);
    HAPPrecondition(domain <= kAppKeyValueStoreDomain_TargetEnd);
    HAP_DIAGNOSTIC_POP
    
    HAPPrecondition(sizeof(StoredRemoteTargetIdentifier) == sizeof(uint32_t));
    HAPPrecondition(sizeof(StoredRemoteCategory) == sizeof(uint16_t));
    HAPPrecondition(sizeof(StoredRemoteNameLength) == sizeof(uint16_t));
    
    HAPError err;
    
    char bytes[sizeof(StoredRemoteTargetIdentifier) +
               sizeof(StoredRemoteCategory) +
               sizeof(StoredRemoteNameLength) + kMaxStoredRemoteNameBytesNullTerminated];
    const size_t maxBytes = sizeof bytes;
    
    size_t pos = 0;
    HAPWriteLittleUInt32(&bytes[pos], config->targetIdentifier);
    pos += sizeof(StoredRemoteTargetIdentifier);
    HAPAssert(pos < maxBytes);
    
    HAPWriteLittleUInt16(&bytes[pos], (uint16_t) config->targetCategory);
    pos += sizeof(StoredRemoteCategory);
    HAPAssert(pos < maxBytes);
    
    if (config->targetName.isDefined) {
        size_t nameLength = HAPStringGetNumBytes(config->targetName.bytes) + 1;
        HAPAssert(nameLength <= kMaxStoredRemoteNameBytesNullTerminated);
        
        HAPWriteLittleUInt16(&bytes[pos], (uint16_t) nameLength);
        pos += sizeof(StoredRemoteNameLength);
        
        HAPAssert(pos + nameLength <= maxBytes);
        
        HAPRawBufferCopyBytes(&bytes[pos], config->targetName.bytes, nameLength);
        pos += nameLength;
    }
    
    HAPAssert(pos <= maxBytes);
    
    err = HAPPlatformKeyValueStoreSet(
        remote->keyValueStore,
        domain, kAppKeyValueStoreKey_TargetConfiguration,
        bytes, pos);
    if (err) {
        HAPAssert(err == kHAPError_Unknown);
        return err;
    }
    
    return kHAPError_None;
}

typedef struct {
    RemoteTargetConfiguration *config;
    RemoteTargetIdentifier identifier;
    HAPPlatformKeyValueStoreDomain domain;
    bool found;
} RemoteTargetConfigurationContext;

static HAPError TargetConfigurationReadDomainCallback(
    Remote *remote,
    void *_Nullable context_,
    HAPPlatformKeyValueStoreDomain domain,
    bool *shouldContinue)
{
    HAPPrecondition(remote);
    HAPPrecondition(context_);
    HAPPrecondition(shouldContinue);
    
    RemoteTargetConfigurationContext *context = (RemoteTargetConfigurationContext *) context_;
    HAPPrecondition(context->identifier != 0);
    HAPPrecondition(context->config);
    
    HAPError err;
    err = TargetConfigurationReadDomain(remote, domain, context->config);
    if (err) {
        HAPLogError(&logObject, "%s: Unable to read domain %d.", __func__, domain);
        return err;
    }
    
    if (context->config->targetIdentifier == context->identifier) {
        context->found = true;
        context->domain = domain;
        *shouldContinue = false;
    }
    return kHAPError_None;
}

static HAPError TargetConfigurationRead(
    Remote *remote,
    RemoteTargetIdentifier identifier,
    RemoteTargetConfiguration *targetConfiguration,
    HAPPlatformKeyValueStoreDomain *_Nullable domain,
    bool *found)
{
    HAPPrecondition(remote);
    HAPPrecondition(remote->keyValueStore);
    HAPPrecondition(identifier);
    HAPPrecondition(targetConfiguration);
    HAPPrecondition(found);
    
    HAPError err;
    
    RemoteTargetConfigurationContext context;
    HAPRawBufferZero(&context, sizeof context);
    context.config = targetConfiguration;
    context.identifier = identifier;
    context.found = false;
    
    err = TargetConfigurationsEnumerate(remote, TargetConfigurationReadDomainCallback, &context);
    if (err) {
        HAPAssert(err == kHAPError_Unknown || err == kHAPError_OutOfResources);
        HAPLogDebug(&logObject, "%s: Unable to find configuration with identifier %lu on remote %p.",
            __func__, (unsigned long) targetConfiguration->targetIdentifier, (const void *) remote);
        return err;
    }
    *found = context.found;
    if (domain) {
        *domain = context.domain;
    }
    return kHAPError_None;
}

static HAPError TargetConfigurationWrite(
    Remote *remote,
    const RemoteTargetConfiguration *targetConfiguration)
{
    HAPPrecondition(remote);
    HAPPrecondition(targetConfiguration);
    HAPPrecondition(targetConfiguration->targetIdentifier);
    
    HAPError err;
    
    bool found;
    HAPPlatformKeyValueStoreDomain domain;
    RemoteTargetConfiguration existingConfiguration;
    err = TargetConfigurationRead(
        remote, targetConfiguration->targetIdentifier, &existingConfiguration, &domain, &found);
    if (err) {
        HAPLogError(&logObject, "%s: Unable to access configuration %lu on remote %p.",
            __func__, (unsigned long) targetConfiguration->targetIdentifier, (const void *) remote);
        HAPFatalError();
    }
    
    if (!found) {
        // Find a new place to store the configuration.
        bool full = true;
        for (size_t i = (size_t) kAppKeyValueStoreDomain_TargetBegin;
             i <= (size_t) kAppKeyValueStoreDomain_TargetEnd; i++) {
            domain = (HAPPlatformKeyValueStoreDomain) i;
            HAPAssert(i <= UINT8_MAX);
            
            bool used;
            err = HAPPlatformKeyValueStoreGet(remote->keyValueStore, domain, 0, NULL, 0, NULL, &used);
            if (err) {
                HAPLogError(&logObject,
                    "%s: Unable to access key value store on domain %d for configuration %lu and remote %p.",
                    __func__, domain, (unsigned long) targetConfiguration->targetIdentifier, (const void *) remote);
                HAPFatalError();
            }
            if (used) {
                continue;
            }
            // Found an empty domain.
            full = false;
            break;
        }
        if (full) {
            HAPLogDebug(&logObject,
                "%s: Unable to allocate a new target for identifier %lu and remote %p: Out of resources.",
                __func__, (unsigned long) targetConfiguration->targetIdentifier, (const void *) remote);
            return kHAPError_OutOfResources;
        }
        
        // Purge the domain first.
        err = HAPPlatformKeyValueStorePurgeDomain(remote->keyValueStore, domain);
        if (err) {
            HAPLogError(&logObject, "%s: Unable to purge domain %d for new identifier %lu on remote %p",
                __func__, domain, (unsigned long) targetConfiguration->targetIdentifier, (const void *) remote);
            HAPFatalError();
        }
    }
    
    err = TargetConfigurationWriteDomain(remote, domain, targetConfiguration);
    if (err) {
        HAPAssert(err == kHAPError_Unknown);
        HAPLogDebug(&logObject, "%s: Unable to write configuration for %lu and remote %p to domain %u",
            __func__, (unsigned long) targetConfiguration->targetIdentifier, (const void *) remote, domain);
        return err;
    }
    return kHAPError_None;
}

static HAPError TargetConfigurationPurge(
    Remote *remote,
    const RemoteTargetConfiguration *targetConfig)
{
    HAPPrecondition(remote);
    HAPPrecondition(targetConfig);
    HAPPrecondition(targetConfig->targetIdentifier);
    
    HAPError err;
    
    bool found;
    HAPPlatformKeyValueStoreDomain domain;
    RemoteTargetConfiguration existingConfiguration;
    err = TargetConfigurationRead(remote, targetConfig->targetIdentifier, &existingConfiguration, &domain, &found);
    if (err) {
        HAPLogError(&logObject, "%s: Unable to access configuration %lu on remote %p.",
            __func__, (unsigned long) targetConfig->targetIdentifier, (const void *) remote);
        HAPFatalError();
    }
    if (!found) {
        HAPLogDebug(&logObject, "%s: Unable to find configuration %lu on remote %p.",
            __func__, (unsigned long) targetConfig->targetIdentifier, (const void *) remote);
        return kHAPError_Unknown;
    }
    
    err = HAPPlatformKeyValueStorePurgeDomain(remote->keyValueStore, domain);
    if (err) {
        HAPLogError(&logObject, "%s: Unable to purge domain %d for new identifier %lu on remote %p.",
            __func__, domain, (unsigned long) targetConfig->targetIdentifier, (const void *) remote);
        HAPFatalError();
    }
    return kHAPError_None;
}

typedef struct {
    Remote *remote;
    const RemoteTargetConfiguration *config;
    TargetConfigurationButtonsEnumerateCallback callback;
    void *_Nullable context;
} TargetConfigurationButtonsContext;

HAP_RESULT_USE_CHECK
static HAPError TargetConfigurationButtonsEnumerateCallbackWrapper(
    void *_Nullable context_,
    HAPPlatformKeyValueStoreRef keyValueStore,
    HAPPlatformKeyValueStoreDomain targetDomain,
    HAPPlatformKeyValueStoreKey buttonKey,
    bool *shouldContinue)
{
    HAPPrecondition(context_);
    HAPPrecondition(keyValueStore);
    
    TargetConfigurationButtonsContext *context = (TargetConfigurationButtonsContext *) context_;
    HAPPrecondition(context->remote);
    HAPPrecondition(context->config);
    
    if (buttonKey == kAppKeyValueStoreKey_TargetConfiguration) {
        return kHAPError_None;
    }
    
    return context->callback(
        context->remote, context->config, context->context, targetDomain, buttonKey, shouldContinue);
}

/**
 * Enumerate buttons on a target.
 *
 * @param      remote               Remote.
 * @param      targetConfiguration  Target configuration.
 * @param      context              Enumeration context.
 * @param      callback             Button enumeration callback.
 *
 * @return kHAPError_None           If successful.
 * @return kHAPError_Unknown        If an error was thrown in the callback.
 * @return kHAPError_OutOfResources If not enough resources available in the callback.
 */
HAP_RESULT_USE_CHECK
static HAPError TargetConfigurationButtonsEnumerate(
    Remote *remote,
    const RemoteTargetConfiguration *targetConfiguration,
    void *_Nullable context,
    TargetConfigurationButtonsEnumerateCallback callback)
{
    HAPPrecondition(remote);
    HAPPrecondition(remote->keyValueStore);
    HAPPrecondition(targetConfiguration);
    
    HAPError err;
    
    bool found;
    HAPPlatformKeyValueStoreDomain domain;
    RemoteTargetConfiguration existingConfiguration;
    err = TargetConfigurationRead(
        remote, targetConfiguration->targetIdentifier, &existingConfiguration, &domain, &found);
    if (err) {
        HAPAssert(err == kHAPError_Unknown || err == kHAPError_OutOfResources);
        HAPLogError(&logObject, "%s: Unable to access configuration %lu and remote %p.",
            __func__, (unsigned long) targetConfiguration->targetIdentifier, (const void *) remote);
        HAPFatalError();
    }
    if (!found) {
        HAPLogDebug(&logObject, "%s: Unable to find configuration %lu and remote %p.",
            __func__, (unsigned long) targetConfiguration->targetIdentifier, (const void *) remote);
        return kHAPError_Unknown;
    }
    
    TargetConfigurationButtonsContext buttonContext;
    buttonContext.remote = remote;
    buttonContext.config = targetConfiguration;
    buttonContext.context = context;
    buttonContext.callback = callback;
    
    err = HAPPlatformKeyValueStoreEnumerate(
        remote->keyValueStore, domain,
        TargetConfigurationButtonsEnumerateCallbackWrapper, &buttonContext);
    if (err) {
        HAPAssert(err == kHAPError_Unknown);
    }
    return err;
}

/**
 * Storage type for the Button type of the Button Configuration.
 */
typedef uint8_t StoredRemoteButtonType;
HAP_STATIC_ASSERT(sizeof(StoredRemoteButtonType) == sizeof(HAPCharacteristicValue_TargetControl_ButtonType),
    Remote_KVS_ButtonTypeMismatch);

static HAPError TargetConfigurationButtonRead(
    Remote *remote,
    const RemoteTargetConfiguration *targetConfiguration,
    RemoteButtonID buttonID,
    RemoteButtonConfiguration *buttonConfig,
    bool *found)
{
    HAPPrecondition(remote);
    HAPPrecondition(targetConfiguration);
    HAPPrecondition(buttonID > 0);
    HAPPrecondition(buttonConfig);
    
    HAPError err;
    
    bool targetFound;
    HAPPlatformKeyValueStoreDomain domain;
    RemoteTargetConfiguration existingConfiguration;
    err = TargetConfigurationRead(
        remote, targetConfiguration->targetIdentifier, &existingConfiguration, &domain, &targetFound);
    if (err) {
        HAPLogError(&logObject, "%s: Unable to access configuration %lu and remote %p.",
            __func__, (unsigned long) targetConfiguration->targetIdentifier, (const void *) remote);
        HAPFatalError();
    }
    if (!targetFound) {
        HAPLogDebug(&logObject, "%s: Unable to find configuration %lu and remote %p.",
            __func__, (unsigned long) targetConfiguration->targetIdentifier, (const void *) remote);
        HAPFatalError();
    }
    
    char bytes[sizeof(StoredRemoteButtonType)
               + sizeof(StoredRemoteNameLength) + kMaxStoredRemoteNameBytesNullTerminated];
    
    const size_t maxBytes = sizeof bytes;
    size_t numBytes;
    
    err = HAPPlatformKeyValueStoreGet(
        remote->keyValueStore,
        domain, (HAPPlatformKeyValueStoreKey) buttonID,
        bytes, maxBytes,
        &numBytes, found);
    if (err) {
        HAPAssert(err == kHAPError_Unknown);
        return err;
    }
    if (!found) {
        HAPLogDebug(&logObject, "%s: Unable to find button %u for configuration %lu on remote %p.",
            __func__, buttonID, (unsigned long) targetConfiguration->targetIdentifier, (const void *) remote);
        return kHAPError_None;
    }
    if (numBytes < sizeof(StoredRemoteButtonType)) {
        HAPLogError(&logObject, "%s: Invalid data in Key Value store for the button configuration (%u, %u)",
            __func__, domain, buttonID);
        HAPFatalError();
    }
    
    HAPRawBufferZero(buttonConfig, sizeof buttonConfig);
    buttonConfig->buttonID = buttonID;
    
    size_t pos = 0;
    HAPAssert(sizeof(StoredRemoteButtonType) == sizeof(uint8_t));
    buttonConfig->buttonType.value = HAPReadUInt8(&bytes[pos]);
    pos += sizeof(StoredRemoteButtonType);
    
    if (buttonConfig->buttonType.value) {
        buttonConfig->buttonType.isDefined = true;
    }
    
    if (pos == numBytes) {
        return kHAPError_None;
    }
    HAPAssert(pos + sizeof(StoredRemoteNameLength) < numBytes);
    
    StoredRemoteNameLength nameLength = HAPReadLittleUInt16(&bytes[pos]);
    pos += sizeof(StoredRemoteNameLength);
    
    HAPAssert(pos + (size_t) nameLength == numBytes);
    HAPRawBufferCopyBytes(buttonConfig->buttonName.bytes, &bytes[pos], (size_t) nameLength);
    pos += (size_t) nameLength;
    
    buttonConfig->buttonName.isDefined = true;
    
    HAPAssert(numBytes == pos);
    return kHAPError_None;
}

static HAPError TargetConfigurationButtonWrite(
    Remote *remote,
    const RemoteTargetConfiguration *targetConfiguration,
    RemoteButtonConfiguration *buttonConfig)
{
    HAPPrecondition(remote);
    HAPPrecondition(targetConfiguration);
    HAPPrecondition(buttonConfig);
    HAPPrecondition(buttonConfig->buttonID > 0);
    
    HAPError err;
    
    bool targetFound;
    HAPPlatformKeyValueStoreDomain domain;
    RemoteTargetConfiguration existingConfiguration;
    err = TargetConfigurationRead(
        remote, targetConfiguration->targetIdentifier, &existingConfiguration, &domain, &targetFound);
    if (err) {
        HAPLogError(&logObject, "%s: Unable to access configuration %lu and remote %p.",
            __func__, (unsigned long) targetConfiguration->targetIdentifier, (const void *) remote);
        HAPFatalError();
    }
    if (!targetFound) {
        HAPLogDebug(&logObject, "%s: Unable to find configuration %lu and remote %p.",
            __func__, (unsigned long) targetConfiguration->targetIdentifier, (const void *) remote);
        HAPFatalError();
    }
    
    uint8_t bytes[sizeof(StoredRemoteButtonType)
               + sizeof(StoredRemoteNameLength) + kMaxStoredRemoteNameBytesNullTerminated];
    
    const size_t maxBytes = sizeof bytes;
    
    size_t pos = 0;
    
    HAPAssert(sizeof(StoredRemoteButtonType) == sizeof(uint8_t));
    if (buttonConfig->buttonType.isDefined) {
        bytes[pos] = buttonConfig->buttonType.value;
    }
    else {
        bytes[pos] = 0;
    }
    pos += sizeof(StoredRemoteButtonType);
    
    if (buttonConfig->buttonName.isDefined) {
        size_t nameBytes = HAPStringGetNumBytes(buttonConfig->buttonName.bytes) + 1;
        HAPAssert(nameBytes <= kMaxStoredRemoteNameBytesNullTerminated);
        
        HAPAssert(sizeof(StoredRemoteNameLength) == sizeof(uint16_t));
        HAPWriteLittleUInt16(&bytes[pos], (uint16_t) nameBytes);
        pos += sizeof(StoredRemoteNameLength);
        
        HAPRawBufferCopyBytes(&bytes[pos], buttonConfig->buttonName.bytes, nameBytes);
        pos += nameBytes;
        
        HAPAssert(pos <= maxBytes);
    }
    err = HAPPlatformKeyValueStoreSet(
        remote->keyValueStore, domain, (HAPPlatformKeyValueStoreKey) buttonConfig->buttonID, bytes, pos);
    if (err) {
        HAPAssert(err == kHAPError_Unknown);
        return err;
    }
    return kHAPError_None;
}

static void TargetConfigurationButtonPurge(
    Remote *remote,
    const RemoteTargetConfiguration *targetConfiguration,
    RemoteButtonID buttonID,
    bool *_Nullable found)
{
    HAPPrecondition(remote);
    HAPPrecondition(targetConfiguration);
    HAPPrecondition(buttonID > 0);
    
    HAPError err;
    
    bool targetFound;
    HAPPlatformKeyValueStoreDomain domain;
    RemoteTargetConfiguration existingConfiguration;
    err = TargetConfigurationRead(
        remote, targetConfiguration->targetIdentifier, &existingConfiguration, &domain, &targetFound);
    if (err) {
        HAPLogError(&logObject, "%s: Unable to access configuration %lu for remote %p.",
            __func__, (unsigned long) targetConfiguration->targetIdentifier, (const void *) remote);
        HAPFatalError();
    }
    
    if (found) {
        *found = targetFound;
    }
    
    if (!targetFound) {
        HAPLogDebug(&logObject, "%s: Unable to find configuration %lu for remote %p.",
            __func__, (unsigned long) targetConfiguration->targetIdentifier, (const void *) remote);
        return;
    }
    
    err = HAPPlatformKeyValueStoreRemove(remote->keyValueStore, domain, (HAPPlatformKeyValueStoreKey) buttonID);
    if (err) {
        HAPLogError(&logObject, "%s: Key value store purge failed on remote %p, identifier %lu and button %u",
            __func__, (const void *) remote, (unsigned long) targetConfiguration->targetIdentifier, buttonID);
        HAPFatalError();
    }
}

/**
 * The version type of the remote configuration.
 */
typedef uint16_t StoredRemoteStorageVersion;

static void ConfigurationRead(
    Remote *remote)
{
    HAPPrecondition(remote);
    HAPPrecondition(remote->keyValueStore);
    HAPPrecondition(sizeof(StoredRemoteStorageVersion) == sizeof(uint16_t));
    HAPPrecondition(sizeof(StoredRemoteTargetIdentifier) == sizeof(uint32_t));
    
    HAPError err;
    
    // Empty state.
    HAPRawBufferZero(&remote->state, sizeof remote->state);
    // Set default state
    remote->state.activeIdentifier = 0;
    
    // Read version information.
    {
        char versionBytes[sizeof(StoredRemoteStorageVersion)];
        
        bool found;
        const size_t maxBytes = sizeof versionBytes;
        size_t numBytes;
        err = HAPPlatformKeyValueStoreGet(
            remote->keyValueStore,
            kAppKeyValueStoreDomain_Configuration, kAppKeyValueStoreKey_ConfigurationVersion,
            versionBytes, maxBytes,
            &numBytes, &found);
        if (err) {
            HAPLogError(&logObject, "%s: Error reading remote configuration for %p in key value store.",
                __func__, (const void *) remote);
            HAPFatalError();
        }
        if (!found) {
            return;
        }
        
        uint16_t version = HAPReadLittleUInt16(&versionBytes);
        if (version != kRemoteKeyValueStoreFormat_Version) {
            HAPLogError(&logObject, "%s: Invalid data format detected. Got: %u, expected %u",
                __func__, version, kRemoteKeyValueStoreFormat_Version);
            // TODO: Purge data?
            HAPFatalError();
        }
    }
    
    // Read configuration blob.
    {
        char bytes[sizeof(StoredRemoteTargetIdentifier)];
        
        bool found;
        const size_t maxBytes = sizeof bytes;
        size_t numBytes;
        
        err = HAPPlatformKeyValueStoreGet(
            remote->keyValueStore,
            kAppKeyValueStoreDomain_Configuration, kAppKeyValueStoreKey_ConfigurationActiveIdentifier,
            bytes, maxBytes,
            &numBytes, &found);
        if (err) {
            HAPLogError(&logObject, "%s: Error reading remote active identifier configuration for %p.",
                __func__, (const void *) remote);
            HAPFatalError();
        }
        if (!found) {
            return;
        }
        if (numBytes != maxBytes) {
            HAPLogError(&logObject, "%s: Invalid data in key value store for %p.",
                __func__, (const void *) remote);
            // TODO: Purge data?
            return;
        }
        
        size_t pos = 0;
        remote->state.activeIdentifier = HAPReadLittleUInt32(&bytes[pos]);
        pos += sizeof(StoredRemoteTargetIdentifier);
        
        HAPAssert(pos == maxBytes);
        
        HAPLog(&logObject, "%s: Remote %p Read active identifier %lu",
            __func__, (const void *) remote, (unsigned long) remote->state.activeIdentifier);
    }
}

static void ConfigurationWrite(
    Remote *remote)
{
    HAPPrecondition(remote);
    HAPPrecondition(remote->keyValueStore);
    HAPPrecondition(sizeof(StoredRemoteStorageVersion) == sizeof(uint16_t));
    HAPPrecondition(sizeof(StoredRemoteTargetIdentifier) == sizeof(uint32_t));
    
    HAPError err;
    
    // Write version information.
    {
        char versionBytes[sizeof(StoredRemoteStorageVersion)];
        HAPWriteLittleUInt16(versionBytes, kRemoteKeyValueStoreFormat_Version);
        
        err = HAPPlatformKeyValueStoreSet(
            remote->keyValueStore,
            kAppKeyValueStoreDomain_Configuration, kAppKeyValueStoreKey_ConfigurationVersion,
            versionBytes, sizeof(versionBytes));
        if (err) {
            HAPLogError(&logObject, "%s: Unable to write version information into key value store for remote %p.",
                __func__, (const void *) remote);
            HAPFatalError();
        }
    }
    
    // Write configuration blob.
    {
        // Write Active Identifier
        size_t pos = 0;
        char bytes[sizeof(StoredRemoteTargetIdentifier)];
        HAPWriteLittleUInt32(&bytes[pos], remote->state.activeIdentifier);
        
        err = HAPPlatformKeyValueStoreSet(
            remote->keyValueStore,
            kAppKeyValueStoreDomain_Configuration, kAppKeyValueStoreKey_ConfigurationActiveIdentifier,
            bytes, sizeof bytes);
        if (err) {
            HAPLogError(&logObject, "%s: Unable to write active identifier into key value store for remote %p.",
                __func__, (const void *) remote);
            HAPFatalError();
        }
    }
}

void RemoteRestoreFactorySettings(
    HAPPlatformKeyValueStoreRef keyValueStore)
{
    HAPPrecondition(keyValueStore);
    
    HAPError err;
    
    HAPLogDebug(&logObject, "%s", __func__);
    
    err = HAPPlatformKeyValueStorePurgeDomain(keyValueStore, kAppKeyValueStoreDomain_Configuration);
    if (err) {
        HAPLogError(&logObject, "%s: Unable to purge remote configuration in domain %u.",
            __func__, kAppKeyValueStoreDomain_Configuration);
        HAPFatalError();
    }
    
    for (size_t i = (size_t) kAppKeyValueStoreDomain_TargetBegin;
         i <= (size_t) kAppKeyValueStoreDomain_TargetEnd; i++)
    {
        err = HAPPlatformKeyValueStorePurgeDomain(keyValueStore, (HAPPlatformKeyValueStoreDomain) i);
        if (err) {
            HAPLogError(&logObject, "%s: Unable to purge target configuration %zu in directory.",
                __func__, i);
            HAPFatalError();
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

typedef struct {
    void *_Nullable context;
    RemoteEnumerateTargetsCallback callback;
} RemoteEnumerateTargetsCallbackWrapper_Context;

static HAPError RemoteEnumerateTargetsCallbackWrapper(
    Remote *remote,
    void *_Nullable context_,
    HAPPlatformKeyValueStoreDomain targetDomain,
    bool *shouldContinue)
{
    HAPPrecondition(remote);
    HAPPrecondition(context_);
    
    HAPError err;
    
    RemoteTargetConfiguration targetConfiguration;
    err = TargetConfigurationReadDomain(remote, targetDomain, &targetConfiguration);
    if (err) {
        HAPLogError(&logObject, "%s: Unable to read target domain %u", __func__, targetDomain);
        HAPFatalError();
    }
    
    RemoteEnumerateTargetsCallbackWrapper_Context *context = (RemoteEnumerateTargetsCallbackWrapper_Context *) context_;
    
    context->callback(context->context, (RemoteRef *) remote, &targetConfiguration, shouldContinue);
    return kHAPError_None;
}

void RemoteEnumerateTargets(
    RemoteRef *remote_,
    RemoteEnumerateTargetsCallback wrappedCallback,
    void *_Nullable wrappedContext)
{
    HAPPrecondition(remote_);
    Remote *remote = (Remote *) remote_;
    HAPPrecondition(remote->keyValueStore);
    
    HAPError err;
    
    RemoteEnumerateTargetsCallbackWrapper_Context context = {
        .context = wrappedContext,
        .callback = wrappedCallback
    };
    
    err = TargetConfigurationsEnumerate(remote, RemoteEnumerateTargetsCallbackWrapper, &context);
    if (err) {
        HAPLogError(&logObject, "%s: Unable to enumerate targets.", __func__);
        HAPFatalError();
    }
}

//----------------------------------------------------------------------------------------------------------------------

typedef struct {
    void *_Nullable context;
    RemoteEnumerateTargetButtonsCallback callback;
} RemoteEnumerateTargetButtonsCallbackWrapper_Context;

static HAPError RemoteEnumerateTargetButtonsCallbackWrapper(
    Remote *remote,
    const RemoteTargetConfiguration *targetConfiguration,
    void *_Nullable context_,
    HAPPlatformKeyValueStoreDomain targetDomain,
    HAPPlatformKeyValueStoreKey buttonKey,
    bool *shouldContinue)
{
    HAPPrecondition(remote);
    HAPPrecondition(remote->keyValueStore);
    HAPPrecondition(targetConfiguration);
    
    HAPPrecondition(context_);
    
    HAPError err;
    
    bool found;
    RemoteButtonConfiguration buttonConfig;
    err = TargetConfigurationButtonRead(remote, targetConfiguration, buttonKey, &buttonConfig, &found);
    if (err) {
        HAPLogError(&logObject, "%s: Unable to read button %u for target in domain %u",
            __func__, buttonKey, targetDomain);
        HAPFatalError();
    }
    HAPAssert(found);
    
    RemoteEnumerateTargetButtonsCallbackWrapper_Context *context =
        (RemoteEnumerateTargetButtonsCallbackWrapper_Context *) context_;
    
    context->callback(context->context, (RemoteRef *) remote, targetConfiguration, &buttonConfig, shouldContinue);
    return kHAPError_None;
}

void RemoteEnumerateButtons(
    RemoteRef *remote_,
    RemoteTargetIdentifier targetIdentifier,
    RemoteEnumerateTargetButtonsCallback wrappedCallback,
    void *_Nullable wrappedContext)
{
    HAPPrecondition(remote_);
    Remote *remote = (Remote *) remote_;
    HAPPrecondition(targetIdentifier);
    
    HAPError err;
    
    RemoteEnumerateTargetButtonsCallbackWrapper_Context context = {
        .context = wrappedContext,
        .callback = wrappedCallback
    };
    
    RemoteTargetConfiguration targetConfiguration;
    bool found;
    err = TargetConfigurationRead(remote, targetIdentifier, &targetConfiguration, NULL, &found);
    if (err) {
        HAPLogError(&logObject, "%s: Unable to read target configuration %lu for remote %p.",
            __func__, (unsigned long) targetIdentifier, (const void *) remote);
        HAPFatalError();
    }
    if (!found) {
        HAPLogError(&logObject, "%s: Target %lu does not exist in remote %p.",
            __func__, (unsigned long) targetIdentifier, (const void *) remote);
        HAPFatalError();
    }
    
    err = TargetConfigurationButtonsEnumerate(
        remote, &targetConfiguration, &context, RemoteEnumerateTargetButtonsCallbackWrapper);
    if (err) {
        HAPLogError(&logObject, "%s: Unable to enumerate buttons for target %lu and remote %p.",
            __func__, (unsigned long) targetIdentifier, (const void *) remote);
        HAPFatalError();
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**
 * Return the remote control associated to an accessory.
 *
 * @param      accessory            Accessory of the remote.
 *
 * @return Remote*                  If a remote is registered with the accessory.
 * @return NULL                     Otherwise.
 */
HAP_RESULT_USE_CHECK
static Remote *_Nullable GetRemote(
    const HAPAccessory *accessory)
{
    HAPPrecondition(accessory);
    Remote *_Nullable remote = remoteStorage.remotes;
    while (remote != NULL) {
        if (remote->accessory == accessory) {
            break;
        }
        remote = (Remote *_Nullable) remote->nextRemote;
    }
    return remote;
}

/**
 * Get the index of a button supported by the remote.
 *
 * @param      remote               Remote.
 * @param      buttonID             Button ID to search.
 * @param[out] index                Index of buttonID.
 * @param[out] found                If the button has been found.
 */
static void GetSupportedButtonIndex(
    Remote *remote,
    RemoteButtonID buttonID,
    size_t *index,
    bool *found)
{
    HAPPrecondition(index);
    
    *found = false;
    for (size_t i = 0; i < remote->supportedButtons.numButtons; i++) {
        if (remote->supportedButtons.buttons[i].buttonID == buttonID) {
            *index = i;
            *found = true;
            return;
        }
    }
}

typedef struct {
    bool previousFound;
    RemoteTargetIdentifier previous;
    RemoteTargetIdentifier new;
} GetNextIdentifierContext;

static HAPError GetNextIdentifierCallback(
    Remote *remote,
    void *_Nullable context_,
    HAPPlatformKeyValueStoreDomain targetDomain,
    bool *shouldContinue)
{
    HAPPrecondition(remote);
    HAPPrecondition(context_);
    HAPPrecondition(shouldContinue);
    
    HAPError err;
    
    GetNextIdentifierContext *context = (GetNextIdentifierContext *) context_;
    
    RemoteTargetConfiguration targetConfiguration;
    err = TargetConfigurationReadDomain(remote, targetDomain, &targetConfiguration);
    if (err) {
        HAPLogError(&logObject, "%s: Unable to read target domain %u for remote %p.",
            __func__, targetDomain, (const void *) remote);
        HAPFatalError();
    }
    
    *shouldContinue = true;
    if (!context->previousFound && context->new == 0) {
        context->new = targetConfiguration.targetIdentifier;
    }
    if (context->previous == targetConfiguration.targetIdentifier) {
        context->previousFound = true;
        return kHAPError_None;
    }
    if (context->previousFound) {
        context->new = targetConfiguration.targetIdentifier;
        *shouldContinue = false;
    }
    return kHAPError_None;
}

typedef struct {
    bool exists;
    RemoteTargetIdentifier identifier;
} CheckIdentifierContext;

static HAPError CheckIdentifierCallBack(
    Remote *remote,
    void *_Nullable context_,
    HAPPlatformKeyValueStoreDomain targetDomain,
    bool *shouldContinue)
{
    HAPPrecondition(remote);
    HAPPrecondition(context_);
    HAPPrecondition(shouldContinue);
    
    HAPError err;
    
    CheckIdentifierContext *context = (CheckIdentifierContext *) context_;
    
    RemoteTargetConfiguration targetConfiguration;
    err = TargetConfigurationReadDomain(remote, targetDomain, &targetConfiguration);
    if (err) {
        HAPLogError(&logObject, "%s: Unable to read target domain %u for remote %p.",
            __func__, targetDomain, (const void *) remote);
        HAPFatalError();
    }
    
    *shouldContinue = true;
    
    if (targetConfiguration.targetIdentifier == context->identifier) {
        context->exists = true;
        *shouldContinue = false;
    }
    return kHAPError_None;
}

/**
 * Find the next identifier of a remote.
 *
 * @param      remote               Remote where the next identifier should be found.
 * @param      previous             Previous identifier.
 *
 * @return 0                        If no target configuration is registered.
 * @return value > 0                For the next identifier.
 */
HAP_RESULT_USE_CHECK
static uint32_t GetNextIdentifier(
    Remote *remote,
    uint32_t previous)
{
    HAPPrecondition(remote);
    
    HAPError err;
    
    GetNextIdentifierContext context;
    HAPRawBufferZero(&context, sizeof context);
    context.previousFound = false;
    context.previous = previous;
    context.new = kRemoteTargetIdentifier_NonHomeKit;
    
    err = TargetConfigurationsEnumerate(remote, GetNextIdentifierCallback, &context);
    if (err) {
        HAPLogError(&logObject, "%s: Unable to enumerate targets for remote %p.",
            __func__, (const void *) remote);
        HAPFatalError();
    }
    return context.new;
}

//----------------------------------------------------------------------------------------------------------------------

static void StartVoiceInput(
    Remote *remote)
{
    HAPPrecondition(remote);
    
    if (!remote->activeSessionInfo) {
        HAPLog(&logObject, "%s: Ignoring voice input because no session is not active.", __func__);
        return;
    }
    
    if (remote->voiceSession) {
        HAPLog(&logObject, "%s: Voice input is already active for session %p.",
            __func__, (const void *) remote->voiceSession);
        return;
    }
    
    RemoteSession *voiceSession = remote->activeSessionInfo;
    
    if (!voiceSession->siriChannelActive) {
        // TODO: Move to siri handler.
        HAPLogError(&logObject, "%s: No siri channel is open on remote %p and session %p",
            __func__, (const void *) remote, (const void *) voiceSession);
        return;
    }
    
    HAPLogDebug(&logObject, "%s: Starting voice input for %p.", __func__,
        (const void *) remote->activeSessionInfo->session);
    
    remote->voiceSession = voiceSession;
    SiriInputStart(&remote->siriAudioContext, &remote->voiceSession->siriDataStreamContext);
}

static void StopVoiceInput(
    Remote *remote)
{
    HAPPrecondition(remote);
    
    if (remote->activeSessionInfo == NULL) {
        HAPLog(&logObject, "%s: No session is setup on remote %p.", __func__, (const void *) remote);
        return;
    }
    
    if (!remote->voiceSession) {
        HAPLog(&logObject, "%s: No voice input active on remote %p. Ignoring.", __func__, (const void *) remote);
        return;
    }
    
    if (!remote->voiceSession->siriChannelActive) {
        HAPLogInfo(&logObject, "%s: No siri channel is active on remote %p. Removing voice flag.",
            __func__, (const void *) remote);
        remote->voiceSession = NULL;
        return;
    }
    
    HAPLog(&logObject, "%s: Disabling voice input for %p.", __func__,
        (const void *) remote->voiceSession);
    
    SiriInputStop(&remote->siriAudioContext, &remote->voiceSession->siriDataStreamContext);
    remote->voiceSession = NULL;
}

//----------------------------------------------------------------------------------------------------------------------

/**
 * Session Management.
 */
/**@{*/

/**
 * Search the session database for registered sessions on a remote.
 *
 * @param      remote               Remote Control where the session needs to be searched.
 * @param      session              Associated session.
 * @return RemoteSession*           If a registered session has been found.
 * @return NULL                     If no session has been found.
 */
HAP_RESULT_USE_CHECK
static RemoteSession *_Nullable GetRegisteredSession(
    Remote *remote,
    const HAPSessionRef *session)
{
    HAPPrecondition(remote);
    HAPPrecondition(session);
    
    for (size_t i = 0; i < HAPArrayCount(remote->registeredSessions); i++) {
        RemoteSession *remoteSession = &remote->registeredSessions[i];
        if (remoteSession->inUse && remoteSession->session == session) {
            return remoteSession;
        }
    }
    return NULL;
}

HAP_RESULT_USE_CHECK
static HAPError SessionActivate(
    Remote *remote,
    const HAPSessionRef *session,
    void *_Nullable context);

static void SessionDeactivate(
    Remote *remote,
    void *_Nullable context);

/**
 * Register a session with a remote.
 *
 * @param      remote               Remote where the session should be registered.
 * @param      session              Session to register.
 *
 * @return kHAPError_None           If successful.
 * @return kHAPError_Unknown        If the session already exists.
 * @return kHAPError_OutOfResources If no space is available to store the session.
 */
HAP_RESULT_USE_CHECK
static HAPError SessionRegister(
    Remote *remote,
    const HAPSessionRef *session)
{
    HAPPrecondition(session);
    
    HAPLogDebug(&logObject, "%s: Registering new session %p", __func__, (const void *) session);
    
    if (GetRegisteredSession(remote, session) != NULL) {
        HAPLog(&logObject, "%s: Session %p already exists.", __func__, (const void *) session);
        return kHAPError_Unknown;
    }
    
    for (size_t i = 0; i < HAPArrayCount(remote->registeredSessions); i++) {
        RemoteSession *active = &remote->registeredSessions[i];
        if (!active->inUse) {
            HAPRawBufferZero(active, sizeof *active);
            
            active->inUse = true;
            active->session = session;
            
            return kHAPError_None;
        }
    }
    
    HAPLog(&logObject, "%s: Not enough sessions to register %p", __func__, (const void *) session);
    return kHAPError_OutOfResources;
}

/**
 * Deregister a session with a remote.
 *
 * @param      remote               Remote.
 * @param      session              Session to invalidate on the remote.
 * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
 *
 * @return kHAPError_None           If successful.
 * @return kHAPError_Unknown        If the session could not be invalidated.
 */
HAP_RESULT_USE_CHECK
static HAPError SessionDeregister(
    Remote *remote,
    const HAPSessionRef *session,
    void *_Nullable context)
{
    HAPPrecondition(remote);
    HAPPrecondition(session);
    
    HAPLogDebug(&logObject, "%s: Invalidate session %p", __func__, (const void *) session);
    
    RemoteSession *remoteSession = GetRegisteredSession(remote, session);
    if (remoteSession == NULL) {
        HAPLog(&logObject, "%s: Unable to invalidate session %p: The session does not exist.",
            __func__, (const void *) session);
        return kHAPError_Unknown;
    }
    
    if (remoteSession->active) {
        SessionDeactivate(remote, context);
    }
    if (remote->voiceSession == remoteSession) {
        StopVoiceInput(remote);
    }
    
    HAPRawBufferZero(remoteSession, sizeof *remoteSession);
    return kHAPError_None;
}

/**
 * Activate a session the remote. If a session is already active, the session is invalidated and siri input stopped.
 *
 * @param      remote               Remote.
 * @param      session              Session to activate on the remote.
 * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
 *
 * @return kHAPError_None           If successful.
 * @return kHAPError_OutOfResources If the supplied buffer was not large enough.
 */
HAP_RESULT_USE_CHECK
static HAPError SessionActivate(
    Remote *remote,
    const HAPSessionRef *session,
    void *_Nullable context)
{
    HAPPrecondition(remote);
    HAPPrecondition(session);
    
    HAPLog(&logObject, "%s: Activating session %p.", __func__, (const void *) session);
    
    RemoteSession *remoteSession = GetRegisteredSession(remote, session);
    
    if (remoteSession == NULL) {
        HAPLog(&logObject, "%s: Session %p does not exist.", __func__, (const void *) session);
        return kHAPError_OutOfResources;
    }
    
    if (remote->activeSessionInfo != NULL) {
        HAPLog(&logObject, "%s: Previous session %p was not invalidated.", __func__,
            (const void *) remote->activeSessionInfo->session);
    }
    if ((remoteSession != remote->activeSessionInfo)
        && remote->voiceSession) {
        HAPLog(&logObject, "%s: Stopping voice input from old session.", __func__);
        StopVoiceInput(remote);
    }
    
    remote->activeSessionInfo = remoteSession;
    
    ConfigurationWrite(remote);
    
    if (remote->callbacks && remote->callbacks->handleActiveChange) {
        remote->callbacks->handleActiveChange((RemoteRef *) remote, context);
    }
    
    return kHAPError_None;
}

/**
 * Deactivate the active session on the remote.
 *
 * @param      remote               Remote where the session should be deactivated.
 * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
 */
static void SessionDeactivate(
    Remote *remote,
    void *_Nullable context)
{
    HAPPrecondition(remote);
    
    if (remote->activeSessionInfo == NULL) {
        HAPLogDebug(&logObject, "%s: No session is active.", __func__);
        return;
    }
    else {
        HAPLog(&logObject, "%s: Deactivating session: %p", __func__,
            (const void *) remote->activeSessionInfo->session);
        if (remote->voiceSession) {
            StopVoiceInput(remote);
        }
    }
    
    remote->activeSessionInfo->active = false;
    remote->activeSessionInfo = NULL;
    
    if (remote->callbacks && remote->callbacks->handleActiveChange) {
        remote->callbacks->handleActiveChange((RemoteRef *) remote, context);
    }
    
    HAPAccessoryServerRaiseEvent(
        remote->server,
        &activeCharacteristic,
        &targetControlService,
        remote->accessory);
}

//----------------------------------------------------------------------------------------------------------------------

void RemoteRaiseButtonEvent(
    RemoteRef *remote_,
    RemoteButtonID buttonID,
    RemoteButtonState buttonState,
    HAPTime timestamp)
{
    HAPPrecondition(remote_);
    Remote *remote = (Remote *) remote_;
    
    HAPError err;
    
    if (buttonID == 0) {
        HAPLogError(&logObject, "%s: Invalid button id 0.", __func__);
        return;
    }
    
    if (remote->state.activeIdentifier == 0) {
        HAPLogDebug(&logObject, "%s: No active identifier is set for the remote registered with remote %p.",
            __func__, (const void *) remote);
        return;
    }
    
    if (remote->activeSessionInfo == NULL) {
        HAPLogDebug(&logObject, "%s: Remote %p is not active. Ignoring button press %u.",
            __func__, (const void *) remote, buttonID);
        return;
    }
    
    // Get the configuration of the active target
    RemoteTargetConfiguration targetConfiguration;
    HAPPlatformKeyValueStoreDomain targetDomain;
    bool found;
    err = TargetConfigurationRead(
        remote, remote->state.activeIdentifier,
        &targetConfiguration, &targetDomain, &found);
    if (err) {
        HAPLogError(&logObject, "%s: Unable to read target configuration for identifier %lu and remote %p.",
            __func__, (unsigned long) remote->state.activeIdentifier, (const void *) remote);
        HAPFatalError();
    }
    if (!found) {
        HAPLogError(&logObject, "%s: No target configuration exists for identifier %lu and remote %p.",
            __func__, (unsigned long) remote->state.activeIdentifier, (const void *) remote);
        return;
    }
    
    // Check if the button exists
    RemoteButtonConfiguration buttonConfig;
    err = TargetConfigurationButtonRead(remote, &targetConfiguration, buttonID, &buttonConfig, &found);
    if (err) {
        HAPLogError(&logObject, "%s: Unable to read button %u for identifier %lu and remote %p.",
            __func__, buttonID, (unsigned long) remote->state.activeIdentifier, (const void *) remote);
        HAPFatalError();
    }
    if (!found) {
        HAPLogDebug(&logObject, "%s: No button %u is found for for identifier %lu and remote %p",
            __func__, buttonID, (unsigned long) remote->state.activeIdentifier, (const void *) remote);
        return;
    }

    if (buttonConfig.buttonType.isDefined &&
        buttonConfig.buttonType.value == kRemoteButtonType_Siri) {
        if (buttonState == kRemoteButtonState_Down) {
            HAPLogDebug(&logObject, "%s: Voice input button down: Starting audio stream on remote %p.",
                __func__, (const void *) remote);
            StartVoiceInput(remote);
        } else {
            HAPLogDebug(&logObject, "%s: Voice input button up: Stopping audio stream on remote %p.",
                __func__, (const void *) remote);
            StopVoiceInput(remote);
        }
        return;
    }
    
    // Enqueue event.
    size_t numEventsEnqueued = 0;
    for (size_t i = 0; i < HAPArrayCount(remote->registeredSessions); i++) {
        RemoteSession *sessionInfo = &remote->registeredSessions[i];
        if (sessionInfo->inUse && sessionInfo->buttonEventsSubscribed) {
            QueuedButtonEvents *buttonEvents = &sessionInfo->queuedButtonEvents;
            const size_t maxEvents = HAPArrayCount(buttonEvents->queuedEvents);
            
            if (buttonEvents->numEvents == maxEvents) {
                HAPAssert(buttonEvents->headIndex == buttonEvents->tailIndex);
                HAPLog(&logObject, "%s: Unable to insert button event. The queue for %p is already full.",
                    __func__, (const void *) sessionInfo->session);
                continue;
            }
            HAPAssert(buttonEvents->numEvents < maxEvents);
            
            size_t eventIndex = buttonEvents->tailIndex;
            
            // Set button event
            ButtonEvent *buttonEvent = &buttonEvents->queuedEvents[eventIndex];
            HAPRawBufferZero(buttonEvent, sizeof *buttonEvent);
            
            buttonEvent->buttonID = buttonID;
            buttonEvent->buttonState = buttonState;
            buttonEvent->timestamp = timestamp;
            buttonEvent->activeIdentifier = remote->state.activeIdentifier;
            
            eventIndex++;
            if (eventIndex >= maxEvents) {
                eventIndex = 0;
            }
            
            // Update queue
            buttonEvents->tailIndex = eventIndex;
            
            buttonEvents->numEvents++;
            HAPAssert(buttonEvents->numEvents <= maxEvents);
            
            numEventsEnqueued++;
        }
    }
    
    if (numEventsEnqueued) {
        HAPLog(&logObject, "%s: Remote %p: Sending button event with id %u for %lu",
            __func__, (const void *) remote, buttonID, (unsigned long) remote->state.activeIdentifier);
        
        // Raise event
        HAPAccessoryServerRaiseEvent(
            remote->server,
            &buttonEventCharacteristic,
            &targetControlService,
            remote->accessory);
    }
}

/**
 * Read the button event of a remote and its associated session.
 *
 * @param      remote               Remote.
 * @param      session              Session of the button event that should be read.
 *
 * @return ButtonEvent*             If a button event is found.
 * @return NULL                     If no button event is registered with the session or the session is not registered.
 */
HAP_RESULT_USE_CHECK
static const ButtonEvent *_Nullable SessionReadButtonEvent(
    Remote *remote,
    const HAPSessionRef *session)
{
    HAPPrecondition(remote);
    HAPPrecondition(session);
    
    RemoteSession *remoteSession = GetRegisteredSession(remote, session);
    if (remoteSession == NULL) {
        HAPLog(&logObject, "%s: The session %p is not registered.", __func__, (const void *) session);
        return NULL;
    }
    
    QueuedButtonEvents *buttonEvents = &remoteSession->queuedButtonEvents;
    if (buttonEvents->numEvents == 0) {
        HAPAssert(buttonEvents->headIndex == buttonEvents->tailIndex);
        HAPLog(&logObject, "%s: The queue for %p is empty.", __func__, (const void *) session);
        return NULL;
    }
    
    // Get event
    size_t index = buttonEvents->headIndex;
    const ButtonEvent *event = &buttonEvents->queuedEvents[index];
    
    const size_t maxEvents = HAPArrayCount(buttonEvents->queuedEvents);
    HAPAssert(index < maxEvents);
    
    // Update queue
    index++;
    if (index >= maxEvents) {
        index = 0;
    }
    buttonEvents->headIndex = index;
    
    buttonEvents->numEvents--;
    
    return event;
}

/**
 * Clear the button event queue associated to a session.
 *
 * @param      remote               Remote.
 * @param      remoteSession        Remote Session.
 */
static void SessionClearButtonEvents(
    Remote *remote,
    RemoteSession *remoteSession)
{
    HAPPrecondition(remote);
    HAPPrecondition(remoteSession);
    
    QueuedButtonEvents *buttonEvents = &remoteSession->queuedButtonEvents;
    HAPRawBufferZero(buttonEvents, sizeof(*buttonEvents));
}

/**
 * Clear the button events attached to a remote.
 *
 * @param      remote               Remote.
 */
static void ClearButtonEvents(
    Remote *remote)
{
    HAPPrecondition(remote);
    
    for (size_t i = 0; i < HAPArrayCount(remote->registeredSessions); i++) {
        RemoteSession *remoteSession = &remote->registeredSessions[i];
        if (remoteSession->inUse) {
            SessionClearButtonEvents(remote, remoteSession);
        }
    };
}

/**
 * Check whether a session queue is empty or not.
 *
 * @param      remote               Remote.
 * @param      session              Session to be checked.
 *
 * @return true                     If there are no events, or no session is registered.
 * @return false                    If the session queue is non-empty.
 */
HAP_RESULT_USE_CHECK
static bool SessionQueuedButtonEventsIsEmpty(
    Remote *remote,
    const HAPSessionRef *session)
{
    HAPPrecondition(remote);
    HAPPrecondition(session);
    
    RemoteSession *remoteSession = GetRegisteredSession(remote, session);
    if (remoteSession == NULL) {
        HAPLog(&logObject, "%s: Session %p is not registered.", __func__, (const void *) session);
        return true;
    }
    
    QueuedButtonEvents *buttonEvents = &remoteSession->queuedButtonEvents;
    return buttonEvents->numEvents == 0;
}

//----------------------------------------------------------------------------------------------------------------------

HAP_RESULT_USE_CHECK
static HAPCharacteristicValue_TargetControl_Type GetTargetControlType(
    RemoteType type)
{
    switch (type) {
        case kRemoteType_Software: {
            return kHAPCharacteristicValue_TargetControl_Type_Software;
        }
        case kRemoteType_Hardware: {
            return kHAPCharacteristicValue_TargetControl_Type_Hardware;
        }
    }
    HAPLogError(&logObject, "%s: Invalid target control type.", __func__);
    HAPFatalError();
}

void RemoteCreate(
    RemoteRef *remote_,
    const RemoteOptions* options,
    void *_Nullable context HAP_UNUSED)
{
    HAPPrecondition(remote_);
    Remote *remote = (Remote *) remote_;
    HAPPrecondition(options);
    HAPPrecondition(options->type);
    HAPPrecondition(options->remoteKeyValueStore);
    HAPPrecondition(options->remoteMicrophone);
    HAPPrecondition(options->server);
    HAPPrecondition(options->accessory);
    
    HAPLogDebug(&logObject, "%s", __func__);
    
    if (!remoteStorage.initialized) {
        remoteStorage.initialized = true;
        remoteStorage.remotes = NULL;
        SiriSetDelegate(&siriDelegate);
    }
    
    HAPRawBufferZero(remote, sizeof(remote));
    
    HAPLog(&logObject, "%s: Creating remote %p", __func__, (const void *) remote);
    
    if (options->numSupportedButtons > kRemote_MaxButtons) {
        HAPLog(&logObject, "%s: %zu buttons are not supported.", __func__, options->numSupportedButtons);
        HAPFatalError();
    }
    
    AddRemote(remote);
    
    HAPRawBufferZero(remote, sizeof *remote);
    remote->type = GetTargetControlType(options->type);
    remote->supportedButtons.buttons = options->supportedButtons;
    remote->supportedButtons.numButtons = options->numSupportedButtons;
    remote->keyValueStore = options->remoteKeyValueStore;
    remote->microphone = options->remoteMicrophone;
    remote->accessory = options->accessory;
    remote->server = options->server;
    remote->callbacks = options->remoteCallbacks;
    
    ConfigurationRead(remote);
    
    // Initialize Key Value Store.
    ConfigurationWrite(remote);
    
    // Initialize Siri audio configuration.
    SiriInitializeAudioConfiguration(&remote->siriAudioContext, remote->microphone);
}

void RemoteRelease(
    RemoteRef *remote_,
    void *_Nullable context)
{
    HAPPrecondition(remote_);
    Remote *remote = (Remote *) remote_;
    
    HAPLogDebug(&logObject, "%s", __func__);
    
    if (remote->activeSessionInfo) {
        SessionDeactivate(remote, context);
    }
    
    RemoveRemote(remote);
    
    ConfigurationWrite(remote);
}

static void ResetRemote(
    Remote *remote,
    void *_Nullable context)
{
    HAPPrecondition(remote);
    
    HAPLogDebug(&logObject, "%s", __func__);
    
    if (remote->activeSessionInfo) {
        SessionDeactivate(remote, context);
    }
    HAPRawBufferZero(&remote->state, sizeof(remote->state));
    RemoteRestoreFactorySettings(remote->keyValueStore);
    
    ClearButtonEvents(remote);
    ConfigurationWrite(remote);
}

void RemoteHandleUnpair(
    RemoteRef *remote_,
    void *_Nullable context)
{
    HAPPrecondition(remote_);
    Remote *remote = (Remote *) remote_;
    
    HAPLogDebug(&logObject, "%s", __func__);
    ResetRemote(remote, context);
}


//----------------------------------------------------------------------------------------------------------------------

/**
 * Update the identifier of a remote.
 *
 * @param      remote               Remote.
 * @param      newIdentifier        New identifier to be set.
 */
static void UpdateIdentifier(
    Remote *remote,
    RemoteTargetIdentifier newIdentifier,
    void *_Nullable context)
{
    HAPPrecondition(remote);
    
    RemoteTargetIdentifier current = remote->state.activeIdentifier;
    if (newIdentifier != current) {
        HAPLog(&logObject, "%s: Updating identifier: %lu -> %lu",
            __func__, (unsigned long) current, (unsigned long) newIdentifier);
        SessionDeactivate(remote, context);
        
        remote->state.activeIdentifier = newIdentifier;
        
        ConfigurationWrite(remote);
        
        HAPAccessoryServerRaiseEvent(
            remote->server,
            &activeIdentifierCharacteristic,
            &targetControlService,
            remote->accessory);
    }
}

HAP_RESULT_USE_CHECK
RemoteTargetIdentifier RemoteGetActiveTargetIdentifier(
    RemoteRef *remote_)
{
    HAPPrecondition(remote_);
    Remote *remote = (Remote *) remote_;
    
    return remote->state.activeIdentifier;
}

void RemoteGetTargetConfiguration(
    RemoteRef *remote_,
    RemoteTargetIdentifier targetIdentifier,
    RemoteTargetConfiguration *targetConfiguration,
    bool *found)
{
    HAPPrecondition(remote_);
    Remote *remote = (Remote *) remote_;
    HAPPrecondition(targetConfiguration);
    HAPPrecondition(found);
    
    HAPError err;
    
    *found = false;
    if (targetIdentifier == 0) {
        return;
    }
    
    err = TargetConfigurationRead(remote, targetIdentifier, targetConfiguration, NULL, found);
    if (err) {
        HAPLogError(&logObject, "%s: Unable to read target configuration for remote %p and identifier %lu",
            __func__, (const void*) remote, (unsigned long) targetIdentifier);
        HAPFatalError();
    }
}

void RemoteSetActiveTargetIdentifier(
    RemoteRef *remote_,
    RemoteTargetIdentifier identifier,
    void *_Nullable context)
{
    HAPPrecondition(remote_);
    Remote *remote = (Remote *) remote_;
    
    HAPError err;
    
    if (identifier != 0) {
        CheckIdentifierContext enumerationContext;
        HAPRawBufferZero(&enumerationContext, sizeof enumerationContext);
        enumerationContext.identifier = identifier;
        
        err = TargetConfigurationsEnumerate(remote, CheckIdentifierCallBack, &enumerationContext);
        if (err) {
            HAPLogError(&logObject, "%s: Unable to enumerate targets for remote %p.",
                __func__, (const void *) remote);
            HAPFatalError();
        }
        
        if (!enumerationContext.exists) {
            HAPLogError(&logObject, "%s: Unable to set identifier %lu on remote %p: The target does not exist.",
                __func__, (unsigned long) identifier, (const void *) remote);
            HAPFatalError();
        }
    }
    
    HAPLogDebug(&logObject, "%s: Setting the identifier of remote %p to %lu.",
        __func__, (const void *) remote, (unsigned long) identifier);
    UpdateIdentifier(remote, identifier, context);
}

HAP_RESULT_USE_CHECK
bool RemoteIsActive(
    RemoteRef *remote_)
{
    HAPPrecondition(remote_);
    Remote *remote = (Remote *) remote_;
    
    return remote->activeSessionInfo != NULL;
}

void RemoteSwitchToNextTarget(
    RemoteRef *remote_,
    void *_Nullable context)
{
    HAPPrecondition(remote_);
    Remote *remote = (Remote *) remote_;
    HAPPrecondition(remote->keyValueStore);
    
    RemoteTargetIdentifier previous = remote->state.activeIdentifier;
    RemoteTargetIdentifier new = GetNextIdentifier(remote, previous);
    
    HAPLog(&logObject, "%s: Previous identifier %lu, new identifier: %lu",
        __func__, (unsigned long) previous, (unsigned long) new);
    
    UpdateIdentifier(remote, new, context);
}

//----------------------------------------------------------------------------------------------------------------------

void HandleSiriAccept(
    HAPAccessoryServerRef *server,
    const HAPServiceRequest *request)
{
    HAPPrecondition(server);
    HAPPrecondition(request);
    
    const HAPSessionRef *session = request->session;
    const HAPAccessory *accessory = request->accessory;
    Remote *remote = GetRemote(accessory);
    
    RemoteSession *remoteSession = GetRegisteredSession(remote, session);
    if (remoteSession == NULL) {
        HAPLogError(&logObject, "%s: The session %p is not registered.", __func__, (const void *) session);
        HAPFatalError();
    }
    remoteSession->siriChannelActive = true;
}

void HandleSiriInvalidate(
    HAPAccessoryServerRef *server,
    const HAPServiceRequest *request)
{
    HAPPrecondition(server);
    HAPPrecondition(request);
    
    const HAPSessionRef *session = request->session;
    const HAPAccessory *accessory = request->accessory;
    Remote *remote = GetRemote(accessory);
    
    RemoteSession *remoteSession = GetRegisteredSession(remote, session);
    if (remoteSession == NULL) {
        HAPLogDebug(&logObject, "%s: No session %p is registered for remote %p.",
            __func__, (const void *) session, (const void *) remote);
        return;
    }
    if (remote->voiceSession == remoteSession) {
        HAPLogInfo(&logObject, "%s: Siri session invalidated on remote %p before voice input stop.",
            __func__, (const void *) remote);
        remote->voiceSession = NULL;
    }
    remoteSession->siriChannelActive = false;
}

void GetSiriContexts(
    HAPAccessoryServerRef *server,
    const HAPServiceRequest *request,
    HAPSiriAudioContext **_Nullable siriAudioContext,
    HAPSiriDataStreamContext **_Nullable siriDataStreamContext)
{
    HAPPrecondition(server);
    HAPPrecondition(request);
    
    const HAPSessionRef *session = request->session;
    const HAPAccessory *accessory = request->accessory;
    Remote *remote = GetRemote(accessory);
    
    if (siriAudioContext) {
        *siriAudioContext = &remote->siriAudioContext;
    }
    
    if (siriDataStreamContext) {
        RemoteSession *remoteSession = GetRegisteredSession(remote, session);
        if (remoteSession == NULL) {
            HAPLogError(&logObject, "%s: The session %p is not registered.", __func__, (const void *) session);
            HAPFatalError();
        }
        *siriDataStreamContext = &remoteSession->siriDataStreamContext;
    }
}

//----------------------------------------------------------------------------------------------------------------------

HAP_RESULT_USE_CHECK
HAPError RemoteHandleTargetControlSupportedConfigurationRead(
    HAPAccessoryServerRef *server_,
    const HAPTLV8CharacteristicReadRequest *request,
    HAPTLVWriterRef *responseWriter,
    void *_Nullable context HAP_UNUSED)
{
    HAPPrecondition(server_);
    HAPPrecondition(request);
    HAPPrecondition(request->accessory);
    HAPPrecondition(responseWriter);
    
    HAPError err;
    
    HAPLogInfo(&logObject, "%s", __func__);
    
    Remote *remote = GetRemote(request->accessory);
    if (remote == NULL) {
        HAPLog(&logObject, "%s: No remote registered for accessory %p.",
            __func__, (const void *) request->accessory);
        return kHAPError_InvalidState;
    }
    
    // Maximum Targets
    if (kRemote_MaxTargets > 1) {
        uint8_t maximumTargetBytes[] = { (uint8_t) kRemote_MaxTargets };
        err = HAPTLVWriterAppend(responseWriter, &(const HAPTLV) {
            .type = kHAPCharacteristicValue_TargetControlSupportedConfiguration_MaximumTargets,
            .value = {
                .bytes = maximumTargetBytes,
                .numBytes = sizeof maximumTargetBytes
            }
        });
        if (err) {
            HAPAssert(err = kHAPError_OutOfResources);
            return err;
        }
    }
    // Ticks per second
    {
        uint8_t ticksPerSecondBytes[] = { HAPExpandLittleUInt64(kHAPTimeTicksPerSecond) };
        err = HAPTLVWriterAppend(responseWriter, &(const HAPTLV) {
            .type = kHAPCharacteristicValue_TargetControlSupportedConfiguration_TicksPerSecond,
            .value = {
                .bytes = ticksPerSecondBytes,
                .numBytes = sizeof ticksPerSecondBytes
            }
        });
        if (err) {
            HAPAssert(err = kHAPError_OutOfResources);
            return err;
        }
    }
    // Supported Button configuration
    {
        uint8_t tlvBytes[8 * kRemote_MaxButtons];
        HAPTLVWriterRef subWriter;
        HAPTLVWriterCreate(&subWriter, tlvBytes, sizeof tlvBytes);
        for (size_t i = 0; i < remote->supportedButtons.numButtons; i++) {
            // TODO: setup getter.
            const RemoteSupportedButton *button = &remote->supportedButtons.buttons[i];
            // Button ID
            {
                uint8_t buttonIDBytes[] = { (uint8_t) button->buttonID };
                err = HAPTLVWriterAppend(&subWriter, &(const HAPTLV) {
                    .type = kHAPCharacteristicValue_TargetControlSupportedConfiguration_ButtonConfiguration_ButtonID,
                    .value = {
                        .bytes = buttonIDBytes,
                        .numBytes = sizeof buttonIDBytes
                    }
                });
                if (err) {
                    HAPAssert(err = kHAPError_OutOfResources);
                    return err;
                }
            }
            // Button type
            {
                uint8_t buttonTypeBytes[] = { HAPExpandLittleUInt16(button->buttonType) };
                err = HAPTLVWriterAppend(&subWriter, &(const HAPTLV) {
                    .type =kHAPCharacteristicValue_TargetControlSupportedConfiguration_ButtonConfiguration_ButtonType,
                    .value = {
                        .bytes = buttonTypeBytes,
                        .numBytes = sizeof buttonTypeBytes
                    }
                });
                if (err) {
                    HAPAssert(err = kHAPError_OutOfResources);
                    return err;
                }
            }
        }
        
        void *bytes;
        size_t numBytes;
        HAPTLVWriterGetBuffer(&subWriter, &bytes, &numBytes);
        err = HAPTLVWriterAppend(responseWriter, &(const HAPTLV) {
            .type = kHAPCharacteristicValue_TargetControlSupportedConfiguration_SupportedButtonConfiguration,
            .value = {
                .bytes = bytes,
                .numBytes = numBytes
            }
        });
        if (err) {
            HAPAssert(err = kHAPError_OutOfResources);
            return err;
        }
    }
    // Type
    {
        uint8_t typeBytes[] = { (uint8_t) remote->type };
        err = HAPTLVWriterAppend(responseWriter, &(const HAPTLV) {
            .type = kHAPCharacteristicValue_TargetControlSupportedConfiguration_Type,
            .value = {
                .bytes = typeBytes,
                .numBytes = sizeof typeBytes
            }
        });
        if (err) {
            HAPAssert(err = kHAPError_OutOfResources);
            return err;
        }
    }
    
    return kHAPError_None;
}

/**
 * Helper Struct to parse Target Control List.
 */
/**@{*/
typedef struct {
    bool inUse : 1;
    uint8_t buttonID;
    struct {
        bool inUse : 1;
        uint16_t value;
    } buttonType;
    struct {
        bool inUse : 1;
        size_t numBytes;
        const void *_Nullable bytes;
    } buttonName;
} ParsedTargetConfigurationTLV_ButtonConfiguration;

typedef struct {
    struct {
        bool inUse : 1;
        uint32_t value;
    } targetIdentifier;
    struct {
        bool inUse : 1;
        const void *_Nullable bytes;
        size_t numNameBytes;
    } targetName;
    struct {
        bool inUse : 1;
        uint16_t value;
    } targetCategory;
    ParsedTargetConfigurationTLV_ButtonConfiguration buttonConfiguration[kRemote_MaxButtons];
} ParsedTargetConfigurationTLV;
/**@}*/

HAP_RESULT_USE_CHECK
static HAPError TryParseTargetControlList_TargetConfiguration_ButtonConfiguration(
    Remote *remote,
    HAPTLVReaderRef *requestReader,
    ParsedTargetConfigurationTLV *parsedTLV)
{
    HAPPrecondition(remote);
    HAPPrecondition(requestReader);
    HAPPrecondition(parsedTLV);
    
    HAPError err;
    
    ParsedTargetConfigurationTLV_ButtonConfiguration *config = NULL;
    for (;;) {
        bool found;
        HAPTLV tlv;
        err = HAPTLVReaderGetNext(requestReader, &found, &tlv);
        if (err) {
            HAPAssert(err == kHAPError_InvalidData);
            return err;
        }
        if (!found) {
            break;
        }
        if (tlv.type == kHAPCharacteristicValue_TargetControlList_ButtonConfiguration_ButtonID) {
            // Handle Button ID
            
            if (tlv.value.numBytes != sizeof(uint8_t)) {
                HAPLogError(&logObject, "%s: Button ID has invalid length %zu.", __func__, tlv.value.numBytes);
                return kHAPError_InvalidData;
            }
            
            bool buttonFound;
            size_t buttonIndex;
            uint8_t buttonID = HAPReadUInt8(tlv.value.bytes);
            GetSupportedButtonIndex(remote, buttonID, &buttonIndex, &buttonFound);
            if (buttonID == 0 || !buttonFound) {
                HAPLogError(&logObject, "%s: Button ID %u is not available on device", __func__, buttonID);
                return kHAPError_InvalidData;
            }
            
            if (parsedTLV->buttonConfiguration[buttonIndex].inUse) {
                HAPLogError(&logObject, "%s: Button ID %u already present.", __func__, buttonID);
                return kHAPError_InvalidData;
            }
            config = &parsedTLV->buttonConfiguration[buttonIndex];
            config->inUse = true;
            config->buttonID = buttonID;
            HAPAssert(config->buttonID == remote->supportedButtons.buttons[buttonIndex].buttonID);
            
        } else if (tlv.type == kHAPCharacteristicValue_TargetControlList_ButtonConfiguration_ButtonType) {
            // Handle Button Type
            
            if (!config) {
                HAPLogError(&logObject, "%s: Button ID not set.", __func__);
                return kHAPError_InvalidData;
            }
            if (tlv.value.numBytes != sizeof(uint16_t) || !config) {
                HAPLogError(&logObject, "%s: Button type has invalid length %zu.", __func__, tlv.value.numBytes);
                return kHAPError_InvalidData;
            }
            
            uint16_t type = HAPReadLittleUInt16(tlv.value.bytes);
            if (!IsValidButtonType((HAPCharacteristicValue_TargetControl_ButtonType) type)) {
                HAPLog(&logObject, "%s: Invalid button type: %u", __func__, type);
                return kHAPError_InvalidData;
            }
            if (config->buttonType.inUse) {
                HAPLogError(&logObject, "%s: Multiple button types defined.", __func__);
                return kHAPError_InvalidData;
            }
            config->buttonType.inUse = true;
            config->buttonType.value = type;
            
        } else if (tlv.type == kkHAPCharacteristicValue_TargetControlList_ButtonConfiguration_ButtonName) {
            // Handle Button Name
            
            if (!config) {
                HAPLogError(&logObject, "%s: Button ID not set.", __func__);
                return kHAPError_InvalidData;
            }
            if (tlv.value.numBytes > kRemote_MaxIdentifierBytes || !config) {
                HAPLogError(&logObject, "%s: Button Name is too long: %zu bytes.", __func__, tlv.value.numBytes);
                return kHAPError_InvalidData;
            }
            if (config->buttonName.inUse) {
                HAPLogError(&logObject, "%s: Multiple Button Names defined.", __func__);
                return kHAPError_InvalidData;
            }
            config->buttonName.inUse = true;
            config->buttonName.bytes = tlv.value.bytes;
            config->buttonName.numBytes = tlv.value.numBytes;
        }
    }
    return kHAPError_None;
}

HAP_RESULT_USE_CHECK
static HAPError TryParseTargetControlList_TargetConfiguration(
    Remote *remote,
    HAPTLVReaderRef *requestReader,
    ParsedTargetConfigurationTLV *parsedTLV,
    const void **_Nullable nextIdentifier)
{
    HAPPrecondition(remote);
    HAPPrecondition(requestReader);
    HAPPrecondition(parsedTLV);
    HAPPrecondition(nextIdentifier);
    
    HAPError err;
    
    HAPRawBufferZero(parsedTLV, sizeof *parsedTLV);
    
    if (*nextIdentifier) {
        parsedTLV->targetIdentifier.inUse = true;
        parsedTLV->targetIdentifier.value = HAPReadLittleUInt32(*nextIdentifier);
        *nextIdentifier = NULL;
    }
    
    for (;;) {
        bool found;
        HAPTLV tlv;
        err = HAPTLVReaderGetNext(requestReader, &found, &tlv);
        if (err) {
            HAPAssert(err == kHAPError_InvalidData);
            HAPLogError(&logObject, "%s: Parsing error.", __func__);
            return err;
        }
        if (!found) {
            break;
        }
        
        if (tlv.type == kHAPCharacteristicValue_TargetControlList_Configuration_TargetIdentifier) {
            // Target Identifier
            if (tlv.value.numBytes != sizeof(parsedTLV->targetIdentifier.value)) {
                HAPLogError(&logObject, "%s: Target Identifier has invalid length: %zu.",
                    __func__, tlv.value.numBytes);
                return kHAPError_InvalidData;
            }
            if (parsedTLV->targetIdentifier.inUse) {
                *nextIdentifier = tlv.value.bytes;
                return kHAPError_None;
            }
            parsedTLV->targetIdentifier.inUse = true;
            parsedTLV->targetIdentifier.value = HAPReadLittleUInt32(tlv.value.bytes);
    
            if (!parsedTLV->targetIdentifier.value) {
                HAPLogError(&logObject, "%s: Target identifier invalid: %lu.",
                    __func__, (unsigned long) parsedTLV->targetIdentifier.value);
                return kHAPError_InvalidData;
            }
    
            HAPLogDebug(&logObject, "%s read: Target Identifier: %lu",
                __func__, (unsigned long) parsedTLV->targetIdentifier.value);
        } else if (tlv.type == kHAPCharacteristicValue_TargetControlList_Configuration_TargetName) {
            // Target Name
            if (tlv.value.numBytes > kRemote_MaxIdentifierBytes) {
                HAPLogError(&logObject, "%s: Target name is too long: %zu.", __func__, tlv.value.numBytes);
                return kHAPError_InvalidData;
            }
            if (parsedTLV->targetName.bytes) {
                HAPLogError(&logObject, "%s: Target name is already present.", __func__);
                return kHAPError_InvalidData;
            }
            parsedTLV->targetName.inUse = true;
            parsedTLV->targetName.bytes = tlv.value.bytes;
            parsedTLV->targetName.numNameBytes = tlv.value.numBytes;
            
            HAPLogDebug(
                &logObject,
                "%s read: Target Name: %s",
                __func__,
                (const char *) parsedTLV->targetName.bytes);
        } else if (tlv.type == kHAPCharacteristicValue_TargetControlList_Configuration_TargetCategory) {
            // Target Category
            if (tlv.value.numBytes != sizeof(parsedTLV->targetCategory.value)) {
                HAPLogError(&logObject, "%s: Target category has invalid length: %zu.",
                    __func__, tlv.value.numBytes);
                return kHAPError_InvalidData;
            }
            if (parsedTLV->targetCategory.inUse) {
                HAPLogError(&logObject, "%s: Target Category already present.", __func__);
                return kHAPError_InvalidData;
            }
            parsedTLV->targetCategory.inUse = true;
            parsedTLV->targetCategory.value = (HAPAccessoryCategory) HAPReadLittleUInt16(tlv.value.bytes);
            
            HAPLogDebug(&logObject, "%s read: Target Category: %u", __func__, parsedTLV->targetCategory.value);
        } else if (tlv.type == kHAPCharacteristicValue_TargetControlList_Configuration_ButtonConfiguration) {
            // Button Configuration
            
            HAPTLVReaderRef subReader;
            HAPTLVReaderCreate(&subReader,
                (void *) (uintptr_t) tlv.value.bytes,
                tlv.value.numBytes);
            
            err = TryParseTargetControlList_TargetConfiguration_ButtonConfiguration(remote, &subReader, parsedTLV);
            if (err) {
                HAPAssert(err == kHAPError_InvalidData);
                HAPLogError(&logObject, "%s: Unable to parse Button Configuration from TLV.", __func__);
                return err;
            }
        } else if (tlv.value.numBytes == 0) {
            HAPLogDebug(&logObject, "%s: Zero-Length separator detected. Skipping.", __func__);
        }else {
            HAPLogError(&logObject, "%s read invalid type: %u", __func__, tlv.type);
            return kHAPError_InvalidData;
        }
    }
    
    return kHAPError_None;
}

static void SetButtonConfiguration(
    Remote *remote,
    const RemoteTargetConfiguration *targetConfiguration,
    const ParsedTargetConfigurationTLV_ButtonConfiguration *parsedButtonConfiguration)
{
    HAPPrecondition(remote);
    HAPPrecondition(targetConfiguration);
    HAPPrecondition(parsedButtonConfiguration);
    
    HAPError err;
    
    RemoteButtonConfiguration buttonConfig;
    HAPRawBufferZero(&buttonConfig, sizeof buttonConfig);
    
    HAPAssert(parsedButtonConfiguration->buttonID);
    buttonConfig.buttonID = parsedButtonConfiguration->buttonID;
    if (parsedButtonConfiguration->buttonType.inUse) {
        buttonConfig.buttonType.isDefined = true;
        buttonConfig.buttonType.value = (RemoteButtonType) parsedButtonConfiguration->buttonType.value;
    }
    if (parsedButtonConfiguration->buttonName.inUse) {
        HAPAssert(parsedButtonConfiguration->buttonName.numBytes <= sizeof(buttonConfig.buttonName.bytes));
        buttonConfig.buttonName.isDefined = true;
        HAPRawBufferCopyBytes(
            buttonConfig.buttonName.bytes,
            HAPNonnullVoid(parsedButtonConfiguration->buttonName.bytes),
            parsedButtonConfiguration->buttonName.numBytes + 1);
    }
    
    err = TargetConfigurationButtonWrite(remote, targetConfiguration, &buttonConfig);
    if (err) {
        HAPAssert(err == kHAPError_Unknown);
        HAPLogDebug(&logObject, "%s: Unable to write button configuration for remote %p, target %lu and button %u. ",
            __func__,
            (const void *) remote,
            (unsigned long) targetConfiguration->targetIdentifier,
            buttonConfig.buttonID);
        HAPFatalError();
    }
}

/**
 * Add or update a configuration target.
 *
 * @param      parsedTLV            The parsed TLV containing the configuration of the new target.
 * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
 *
 * @return kHAPErrorNone            The target was added successfully.
 * @return kHAPError_InvalidData    The desired target configuration was invalid.
 * @return kHAPError_InvalidState   The maximum number of targets is already reached, or the target is already present.
 */
HAP_RESULT_USE_CHECK
static HAPError AddOrUpdateTarget(
    Remote *remote,
    const ParsedTargetConfigurationTLV *parsedTLV,
    void *_Nullable context)
{
    HAPPrecondition(remote);
    HAPPrecondition(parsedTLV);
    HAPPrecondition(parsedTLV->targetIdentifier.inUse);
    
    HAPError err;
    
    const RemoteTargetIdentifier identifier = parsedTLV->targetIdentifier.value;
    if (!parsedTLV->targetCategory.inUse
        || !parsedTLV->targetName.bytes) {
        HAPLog(&logObject, "%s no category or name present", __func__);
        return kHAPError_InvalidData;
    }
    
    bool targetConfigFound;
    RemoteTargetConfiguration targetConfiguration;
    err = TargetConfigurationRead(remote, identifier, &targetConfiguration, NULL, &targetConfigFound);
    if (err) {
        HAPLogError(&logObject, "%s: Unable to read target configuration for remote %p and identifier %lu",
            __func__, (const void*) remote, (unsigned long) identifier);
        HAPFatalError();
    }
    if (targetConfigFound) {
        HAPLogInfo(&logObject, "%s Target identifier %lu already present. Zeroing existing button configuration.",
            __func__, (unsigned long) identifier);
        err = TargetConfigurationPurge(remote, &targetConfiguration);
        if (err) {
            HAPLogError(&logObject,
                "%s: TargetConfigurationPurge reported an unknown error when purging %lu from %p",
                __func__, (unsigned long) identifier, (const void *) remote);
            HAPFatalError();
        }
    }
    
    HAPRawBufferZero(&targetConfiguration, sizeof targetConfiguration);
    targetConfiguration.targetIdentifier = identifier;
    targetConfiguration.targetCategory = (HAPAccessoryCategory) parsedTLV->targetCategory.value;
    if (parsedTLV->targetName.inUse) {
        targetConfiguration.targetName.isDefined = true;
        HAPRawBufferCopyBytes(
            targetConfiguration.targetName.bytes,
            HAPNonnullVoid(parsedTLV->targetName.bytes),
            parsedTLV->targetName.numNameBytes+1);
    }
    
    
    err = TargetConfigurationWrite(remote, &targetConfiguration);
    if (err == kHAPError_OutOfResources) {
        HAPLog(&logObject,
            "%s: Unable to write target configuration for remote %p and identifier %lu maximum number of "
            "targets present",
            __func__, (const void *) remote, (unsigned long) targetConfiguration.targetIdentifier);
        return kHAPError_InvalidState;
    } else if (err) {
        HAPLogError(&logObject,
            "%s: Error writing target configuration for remote %p and identifier %lu.",
            __func__, (const void *) remote, (unsigned long) targetConfiguration.targetIdentifier);
        HAPFatalError();
    }
    
    size_t numStoredButtonConfigurations = 0;
    for (size_t i = 0; i < remote->supportedButtons.numButtons; i++) {
        const ParsedTargetConfigurationTLV_ButtonConfiguration *parsed = &parsedTLV->buttonConfiguration[i];
        if (parsed->inUse) {
            SetButtonConfiguration(remote, &targetConfiguration, parsed);
            numStoredButtonConfigurations++;
        }
    }
    if (!numStoredButtonConfigurations) {
        HAPLog(&logObject,
            "%s No button configuration for remote %p and identifier %lu.",
            __func__, (const void *) remote, (unsigned long) targetConfiguration.targetIdentifier);
        
        err = TargetConfigurationPurge(remote, &targetConfiguration);
        if (err) {
            HAPLogError(&logObject,
                "%s: RemoteTargetConfigurationPurge reported an unknown error when purging %lu from %p",
                __func__, (unsigned long) identifier, (const void *) remote);
            HAPFatalError();
        }
        
        return kHAPError_InvalidData;
    }
    
    if (remote->state.activeIdentifier == 0) {
        RemoteSwitchToNextTarget((RemoteRef *) remote, context);
    }
    
    return kHAPError_None;
}

/**
 * Remove a configuration target.
 *
 * If a button configuration is present, then only the available buttons are removed. Otherwise the whole target is
 * removed.
 *
 * @param      parsedTLV            The parsed TLV containing the configuration of the buttons/configuration to
 *                                  remove.
 * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
 *
 * @return kHAPErrorNone            The target was removed successfully.
 * @return kHAPError_InvalidData    The desired target configuration was invalid or the identifier does not exist.
 */
HAP_RESULT_USE_CHECK
static HAPError RemoveTarget(
    Remote *remote,
    const ParsedTargetConfigurationTLV *parsedTLV,
    void *_Nullable context)
{
    HAPPrecondition(remote);
    HAPPrecondition(parsedTLV);
    HAPPrecondition(parsedTLV->targetIdentifier.inUse);
    
    HAPError err;
    
    const RemoteTargetIdentifier identifier = parsedTLV->targetIdentifier.value;
    
    bool targetConfigFound;
    RemoteTargetConfiguration targetConfiguration;
    err = TargetConfigurationRead(remote, identifier, &targetConfiguration, NULL, &targetConfigFound);
    if (err) {
        HAPLogError(&logObject, "%s: Unable to read target configuration for remote %p and identifier %lu",
            __func__, (const void*) remote, (unsigned long) identifier);
        HAPFatalError();
    }
    if (!targetConfigFound) {
        HAPLogError(&logObject, "%s: Target %lu on remote %p was not found.",
            __func__, (unsigned long) identifier, (const void *) remote);
        return kHAPError_InvalidData;
    }
    
    if (parsedTLV->buttonConfiguration->inUse) {
        HAPLogDebug(&logObject, "%s: Removing buttons target %lu", __func__, (unsigned long) identifier);
        
        // Button Configuration is present. Only remove buttons that are present.
        for (size_t i = 0; i < remote->supportedButtons.numButtons; i++) {
            const ParsedTargetConfigurationTLV_ButtonConfiguration *removeButton = &parsedTLV->buttonConfiguration[i];
            if (removeButton->inUse) {
                TargetConfigurationButtonPurge(remote, &targetConfiguration, removeButton->buttonID, NULL);
            }
        }
    } else {
        // No Button Configuration is present: Remove the entire entry.
        HAPLogDebug(&logObject, "%s: Removing target %lu", __func__, (unsigned long) identifier);
        
        // Remove configuration from target control list
        err = TargetConfigurationPurge(remote, &targetConfiguration);
        if (err) {
            HAPLogError(&logObject,
                "%s: RemoteTargetConfigurationPurge reported an unknown error when purging %lu from %p",
                __func__, (unsigned long) identifier, (const void *) remote);
            HAPFatalError();
        }
        
        // Update identifier, to avoid starting from index 0, we look for the new identifier with the
        // old configuration still present.
        const uint32_t oldActiveIdentifier = remote->state.activeIdentifier;
        if (oldActiveIdentifier == identifier) {
            uint32_t newActiveIdentifier = GetNextIdentifier(remote, identifier);
            UpdateIdentifier(remote, newActiveIdentifier, context);
        }
        
        ConfigurationWrite(remote);
    }
    
    return kHAPError_None;
}

/**
 * Update a target configuration.
 *
 * @param      parsedTLV            The parsed TLV containing the configuration that should be updated.
 *
 * @return kHAPErrorNone            The target was updated successfully.
 * @return kHAPError_InvalidData    The target configuration was invalid or the identifier does not exist.
 */
HAP_RESULT_USE_CHECK
static HAPError UpdateTarget(
    Remote *remote,
    const ParsedTargetConfigurationTLV *parsedTLV)
{
    HAPPrecondition(remote);
    HAPPrecondition(parsedTLV);
    HAPPrecondition(parsedTLV->targetIdentifier.inUse);
    
    HAPError err;
    
    HAPLogDebug(&logObject, "%s", __func__);
    
    if (!parsedTLV->targetIdentifier.inUse) {
        HAPLogError(&logObject, "%s: No target identifier present.", __func__);
        return kHAPError_InvalidData;
    }
    
    const RemoteTargetIdentifier identifier = parsedTLV->targetIdentifier.value;
    
    bool targetConfigFound;
    RemoteTargetConfiguration targetConfiguration;
    err = TargetConfigurationRead(remote, identifier, &targetConfiguration, NULL, &targetConfigFound);
    if (err) {
        HAPLogError(&logObject, "%s: Unable to read target configuration for remote %p and identifier %lu",
            __func__, (const void*) remote, (unsigned long) identifier);
        HAPFatalError();
    }
    if (!targetConfigFound) {
        HAPLog(&logObject, "%s: Identifier %lu not present in remote %p.",
            __func__, (unsigned long) identifier, (const void *) remote);
        return kHAPError_InvalidData;
    }
    HAPLogDebug(&logObject, "%s: Updating configuration %lu in remote %p.",
        __func__, (unsigned long) identifier, (const void *) remote);
    
    if (parsedTLV->targetName.inUse) {
        HAPAssert(parsedTLV->targetName.numNameBytes < sizeof(targetConfiguration.targetName.bytes));
        HAPLogDebug(&logObject, "%s: Updating name: %s", __func__, (const char*) parsedTLV->targetName.bytes);
        targetConfiguration.targetName.isDefined = true;
        HAPRawBufferCopyBytes(
            targetConfiguration.targetName.bytes,
            HAPNonnullVoid(parsedTLV->targetName.bytes),
            parsedTLV->targetName.numNameBytes+1);
    }
    
    if (parsedTLV->targetCategory.inUse) {
        if (parsedTLV->targetCategory.value != kHAPAccessoryCategory_AppleTV) {
            HAPLogError(&logObject, "%s: Invalid category %u.", __func__, parsedTLV->targetCategory.value);
            return kHAPError_InvalidData;
        }
        HAPLogDebug(&logObject, "%s: Updating category: %u", __func__, parsedTLV->targetCategory.value);
        targetConfiguration.targetCategory = (HAPAccessoryCategory) parsedTLV->targetCategory.value;
    }
    
    err = TargetConfigurationWrite(remote, &targetConfiguration);
    if (err) {
        HAPAssert(err == kHAPError_OutOfResources);
        HAPLogError(&logObject, "%s: Unable to write target configuration for identifier %lu on remote %p.",
            __func__, (unsigned long) identifier, (const void *) remote);
        return err;
    }
    
    for (size_t i = 0; i < remote->supportedButtons.numButtons; i++) {
        // Update if present.
        const ParsedTargetConfigurationTLV_ButtonConfiguration *newButtonConfig = &parsedTLV->buttonConfiguration[i];
        if (newButtonConfig->inUse) {
            HAPLogDebug(&logObject, "%s: Updating button %u", __func__, newButtonConfig->buttonID);
            SetButtonConfiguration(remote, &targetConfiguration, newButtonConfig);
        }
    }
    
    return kHAPError_None;
}

/**
 * Checks whether a value represents a valid Target Control List operation.
 *
 * @param      value                Value to check.
 *
 * @return true                     If the value is valid.
 * @return false                    Otherwise.
 */
HAP_RESULT_USE_CHECK
static bool IsValidOperation(
    uint8_t value)
{
    HAPAssert(sizeof value == sizeof (HAPCharacteristicValue_TargetControlList_Operation));
    switch ((HAPCharacteristicValue_TargetControlList_Operation) value) {
        case kHAPCharacteristicValue_TargetControlList_Operation_Add:
        case kHAPCharacteristicValue_TargetControlList_Operation_Remove:
        case kHAPCharacteristicValue_TargetControlList_Operation_Update:
        case kHAPCharacteristicValue_TargetControlList_Operation_Reset:
        case kHAPCharacteristicValue_TargetControlList_Operation_List: {
            return true;
        }
    }
    return false;
}

HAP_RESULT_USE_CHECK
HAPError RemoteHandleTargetControlListWrite(
    HAPAccessoryServerRef *server_,
    const HAPTLV8CharacteristicWriteRequest *request,
    HAPTLVReaderRef *requestReader,
    void *_Nullable context)
{
    HAPPrecondition(server_);
    HAPPrecondition(request);
    HAPPrecondition(request->accessory);
    HAPPrecondition(requestReader);
    
    HAPError err;
    
    HAPLogInfo(&logObject, "%s", __func__);
    
    Remote *remote = GetRemote(request->accessory);
    if (remote == NULL) {
        HAPLog(&logObject, "%s: No remote registered for accessory %p.",
            __func__, (const void *) request->accessory);
        return kHAPError_InvalidState;
    }
    
    HAPCharacteristicValue_TargetControlList_Operation operation;
    // Operation
    {
        bool found;
        HAPTLV operationTLV;
        err = HAPTLVReaderGetNext(requestReader, &found, &operationTLV);
        if (err) {
            HAPAssert(err == kHAPError_InvalidData);
            return err;
        }
        if (!found || operationTLV.type != kHAPCharacteristicValue_TargetControlList_Operation) {
            HAPLogError(&logObject, "%s: TLV8 Operation - Not found", __func__);
            return kHAPError_InvalidData;
        }
        HAPAssert(operationTLV.value.numBytes == sizeof(uint8_t));
        uint8_t value = HAPReadUInt8(operationTLV.value.bytes);
        if (!IsValidOperation(value)) {
            HAPLogError(&logObject, "%s: Invalid operation: %d.", __func__, (int) value);
            return kHAPError_InvalidData;
        }
        operation = (HAPCharacteristicValue_TargetControlList_Operation) value;
    }
    
    switch (operation) {
        case kHAPCharacteristicValue_TargetControlList_Operation_Add: {
            HAPLogDebug(&logObject, "%s: Action - Add", __func__);
        } break;
        case kHAPCharacteristicValue_TargetControlList_Operation_Remove: {
            HAPLogDebug(&logObject, "%s: Action - Remove", __func__);
        } break;
        case kHAPCharacteristicValue_TargetControlList_Operation_Update: {
            HAPLogDebug(&logObject, "%s: Action - Update", __func__);
        } break;
        case kHAPCharacteristicValue_TargetControlList_Operation_Reset: {
            HAPLogDebug(&logObject, "%s: Action - Reset", __func__);
            ResetRemote(remote, context);
            if (remote->callbacks) {
                remote->callbacks->handleConfigurationChange((RemoteRef *) remote, context);
            }
            return kHAPError_None;
        }
        case kHAPCharacteristicValue_TargetControlList_Operation_List: {
            HAPLogDebug(&logObject, "%s: Action - List", __func__);
            return kHAPError_None;
        }
    }
    
    // Target Configuration
    for (size_t counter = 1; /* Looping over TLVs */; counter++)
    {
        HAPLogDebug(&logObject, "%s: Reading configuration %zu", __func__, counter);
        HAPTLV targetConfigurationTLV;
        
        bool found;
        err = HAPTLVReaderGetNext(requestReader, &found, &targetConfigurationTLV);
        if (err) {
            HAPLogError(&logObject, "%s: TLV8 Target configuration - Invalid payload", __func__);
            HAPAssert(err = kHAPError_InvalidData);
            return err;
        }
        if (!found) {
            break;
        }
        if (!found || targetConfigurationTLV.type != kHAPCharacteristicValue_TargetControlList_Configuration) {
            HAPLogInfo(&logObject, "%s: TLV8 Target configuration - Not found, ignoring", __func__);
            continue;
        }
        
        HAPTLVReaderRef subReader;
        HAPTLVReaderCreate(&subReader,
            (void *) (uintptr_t) targetConfigurationTLV.value.bytes,
            targetConfigurationTLV.value.numBytes);
        const void *_Nullable nextIdentifier = NULL;
        
        // Parse target configuration
        // The configuration might be split into:
        // - Target Identifier #1
        // - Target Name
        // - Target Name
        // - Button Configuration
        // - Target Identifier #2
        // ...
        // or the configuration might be stored in a configuration TLV.
        for (size_t targetCounter = 1; /* Loop over targets */; targetCounter++){
            HAPLogDebug(&logObject, "%s: Parsing configuration %zu, %zu ", __func__, counter, targetCounter);
            ParsedTargetConfigurationTLV parsedTargetConfiguration;
            err = TryParseTargetControlList_TargetConfiguration(
                remote, &subReader, &parsedTargetConfiguration, &nextIdentifier);
            if (err) {
                HAPLogError(&logObject, "%s: Unable to parse target configuration.", __func__);
                HAPAssert(err == kHAPError_InvalidData);
                return err;
            }
            
            if (!parsedTargetConfiguration.targetIdentifier.inUse) {
                HAPLogError(&logObject, "%s: Add/Remove/Update Target identifier not present", __func__);
                return kHAPError_InvalidData;
            }
            
            if (operation == kHAPCharacteristicValue_TargetControlList_Operation_Add) {
                err = AddOrUpdateTarget(remote, &parsedTargetConfiguration, context);
                if (err) {
                    HAPLogError(&logObject, "%s: Unable to process action Add.", __func__);
                    HAPAssert(err == kHAPError_Unknown
                              || err == kHAPError_InvalidData
                              || err == kHAPError_InvalidState);
                    return err;
                }
            } else if (operation == kHAPCharacteristicValue_TargetControlList_Operation_Remove) {
                err = RemoveTarget(remote, &parsedTargetConfiguration, context);
                if (err) {
                    HAPLogError(&logObject, "%s: Unable to process action Remove.", __func__);
                    HAPAssert(err == kHAPError_Unknown
                              || err == kHAPError_InvalidData
                              || err == kHAPError_InvalidState);
                    return err;
                }
            } else if (operation == kHAPCharacteristicValue_TargetControlList_Operation_Update) {
                err = UpdateTarget(remote, &parsedTargetConfiguration);
                if (err) {
                    HAPLogError(&logObject, "%s: Unable to process action Update.", __func__);
                    HAPAssert(err == kHAPError_Unknown
                              || err == kHAPError_InvalidData
                              || err == kHAPError_InvalidState);
                    return err;
                }
            } else {
                HAPLogError(&logObject, "%s Invalid case", __func__);
                return kHAPError_Unknown;
            }
            if (nextIdentifier == NULL) {
                break;
            }
        }
    }
    
    if (remote->callbacks) {
        remote->callbacks->handleConfigurationChange((RemoteRef *) remote, context);
    }
    
    return kHAPError_None;
}

typedef struct {
    HAPTLVWriterRef *buttonWriter;
} TargetControlListReadCallbackContext_Target_Button;

HAP_RESULT_USE_CHECK
static HAPError TargetControlListReadCallback_Target_Button(
    Remote *remote,
    const RemoteTargetConfiguration *targetConfiguration,
    void *_Nullable context_,
    HAPPlatformKeyValueStoreDomain targetDomain,
    HAPPlatformKeyValueStoreKey buttonKey,
    bool *shouldContinue HAP_UNUSED)
{
    HAPPrecondition(remote);
    HAPPrecondition(targetConfiguration);
    HAPPrecondition(context_);
    TargetControlListReadCallbackContext_Target_Button *context =
        (TargetControlListReadCallbackContext_Target_Button *) context_;
    HAPPrecondition(context->buttonWriter);
    
    HAPError err;
    
    bool buttonConfigFound;
    RemoteButtonConfiguration buttonConfig;
    err = TargetConfigurationButtonRead(remote, targetConfiguration, buttonKey, &buttonConfig, &buttonConfigFound);
    if (err) {
        HAPLogError(&logObject, "%s: Unable to read button %u in domain %u for identifier %lu in remote %p.",
            __func__,
            buttonKey,
            targetDomain,
            (unsigned long) targetConfiguration->targetIdentifier,
            (const void *) remote);
        HAPFatalError();
    }
    
    HAPTLVWriterRef *buttonWriter = context->buttonWriter;
    
    // Button ID
    {
        err = HAPTLVWriterAppend(buttonWriter, &(const HAPTLV) {
            .type = kHAPCharacteristicValue_TargetControlList_ButtonConfiguration_ButtonID,
            .value = {
                .bytes = (const uint8_t[]) {
                    buttonConfig.buttonID
                },
                .numBytes = sizeof buttonConfig.buttonID
            }
        });
        if (err) {
            HAPAssert(err = kHAPError_OutOfResources);
            return err;
        }
    }
    // Button Type
    if (buttonConfig.buttonType.isDefined) {
        uint8_t buttonTypeBytes[] = {HAPExpandLittleUInt16(buttonConfig.buttonType.value)};
        err = HAPTLVWriterAppend(buttonWriter, &(const HAPTLV) {
            .type = kHAPCharacteristicValue_TargetControlList_ButtonConfiguration_ButtonType,
            .value = {
                .bytes = buttonTypeBytes,
                .numBytes = sizeof buttonTypeBytes
            }
        });
        if (err) {
            HAPAssert(err = kHAPError_OutOfResources);
            return err;
        }
    }
    // Button Name
    if (buttonConfig.buttonName.isDefined) {
        err = HAPTLVWriterAppend(buttonWriter, &(const HAPTLV) {
            .type = kkHAPCharacteristicValue_TargetControlList_ButtonConfiguration_ButtonName,
            .value = {
                .bytes = buttonConfig.buttonName.bytes,
                .numBytes = HAPStringGetNumBytes(buttonConfig.buttonName.bytes)
            }
        });
        if (err) {
            HAPAssert(err = kHAPError_OutOfResources);
            return err;
        }
    }
    
    return kHAPError_None;
}

typedef struct {
    HAPTLVWriterRef *configurationWriter;
} TargetControlListReadCallbackContext_Target;

static HAPError TargetControlListReadCallback_Target(
    Remote *remote,
    void *_Nullable context_,
    HAPPlatformKeyValueStoreDomain targetDomain,
    bool *shouldContinue HAP_UNUSED)
{
    HAPPrecondition(remote);
    HAPPrecondition(context_);
    TargetControlListReadCallbackContext_Target *context = (TargetControlListReadCallbackContext_Target *) context_;
    HAPPrecondition(context->configurationWriter);
    
    HAPError err;
    
    HAPTLVWriterRef *configurationWriter = context->configurationWriter;
    
    RemoteTargetConfiguration targetConfiguration;
    err = TargetConfigurationReadDomain(remote, targetDomain, &targetConfiguration);
    if (err) {
        HAPLogError(&logObject, "%s: Unable to read target domain %u for remote %p.",
            __func__, targetDomain, (const void *) remote);
        HAPFatalError();
    }
    
    // Target Identifier
    {
        uint8_t identifierBytes[] = {HAPExpandLittleUInt32(targetConfiguration.targetIdentifier)};
        err = HAPTLVWriterAppend(configurationWriter, &(const HAPTLV) {
            .type = kHAPCharacteristicValue_TargetControlList_Configuration_TargetIdentifier,
            .value = {
                .bytes = identifierBytes,
                .numBytes = sizeof(identifierBytes)
            }
        });
        if (err) {
            HAPAssert(err = kHAPError_OutOfResources);
            return err;
        }
    }
    // Target Name
    if (targetConfiguration.targetName.isDefined) {
        err = HAPTLVWriterAppend(configurationWriter, &(const HAPTLV) {
            .type = kHAPCharacteristicValue_TargetControlList_Configuration_TargetName,
            .value = {
                .bytes = targetConfiguration.targetName.bytes,
                .numBytes = HAPStringGetNumBytes(targetConfiguration.targetName.bytes)
            }
        });
        if (err) {
            HAPAssert(err = kHAPError_OutOfResources);
            return err;
        }
    }
    // Target Category
    if (targetConfiguration.targetCategory) {
        uint8_t targetCategoryBytes[] = {HAPExpandLittleUInt16(targetConfiguration.targetCategory)};
        err = HAPTLVWriterAppend(configurationWriter, &(const HAPTLV) {
            .type = kHAPCharacteristicValue_TargetControlList_Configuration_TargetCategory,
            .value = {
                .bytes = targetCategoryBytes,
                .numBytes = sizeof(targetCategoryBytes)
            }
        });
        if (err) {
            HAPAssert(err = kHAPError_OutOfResources);
            return err;
        }
    }
    // Button Configuration
    {
        // Button ID: 3 Bytes
        // Button Type: 4 Bytes
        // Button Name: 2 + kAppIdentifier_MaxBytes + 2 (TLV Fragmentation)
        uint8_t buttonTLVBuffer[(3 + 4 + 2 + kRemote_MaxIdentifierBytes + 2) * kRemote_MaxButtons];
        HAPTLVWriterRef buttonWriter;
        HAPTLVWriterCreate(&buttonWriter, buttonTLVBuffer, sizeof buttonTLVBuffer);
        
        TargetControlListReadCallbackContext_Target_Button buttonContext;
        buttonContext.buttonWriter = &buttonWriter;
        
        err = TargetConfigurationButtonsEnumerate(
            remote,
            &targetConfiguration,
            &buttonContext,
            TargetControlListReadCallback_Target_Button);
        if (err) {
            HAPLogError(&logObject, "%s: Unable to enumerate buttons for target %lu in remote %p.",
                __func__, (unsigned long) targetConfiguration.targetIdentifier, (const void *) remote);
            return err;
        }
        
        void *bytes;
        size_t numBytes;
        HAPTLVWriterGetBuffer(&buttonWriter, &bytes, &numBytes);
        if (numBytes) {
            err = HAPTLVWriterAppend(configurationWriter, &(const HAPTLV) {
                .type = kHAPCharacteristicValue_TargetControlList_Configuration_ButtonConfiguration,
                .value = {
                    .bytes = bytes,
                    .numBytes = numBytes
                }
            });
            if (err) {
                HAPAssert(err = kHAPError_OutOfResources);
                return err;
            }
        }
    }
    return kHAPError_None;
}

HAP_RESULT_USE_CHECK
HAPError RemoteHandleTargetControlListRead(
    HAPAccessoryServerRef *server_,
    const HAPTLV8CharacteristicReadRequest *request,
    HAPTLVWriterRef *responseWriter,
    void *_Nullable context HAP_UNUSED)
{
    HAPPrecondition(server_);
    HAPPrecondition(request);
    HAPPrecondition(request->accessory);
    HAPPrecondition(responseWriter);
    
    HAPError err;
    
    HAPLogInfo(&logObject, "%s", __func__);
    
    Remote *remote = GetRemote(request->accessory);
    if (remote == NULL) {
        HAPLog(&logObject, "%s: No remote registered for accessory %p.",
            __func__, (const void *) request->accessory);
        return kHAPError_InvalidState;
    }

    // Target configuration
    void *configurationTLVBytes;
    size_t configurationTLVMaxBytes;
    HAPTLVWriterGetScratchBytes(responseWriter, &configurationTLVBytes, &configurationTLVMaxBytes);
    
    HAPTLVWriterRef configurationWriter;
    HAPTLVWriterCreate(&configurationWriter, configurationTLVBytes, configurationTLVMaxBytes);
    
    TargetControlListReadCallbackContext_Target targetContext;
    targetContext.configurationWriter = &configurationWriter;
    
    err = TargetConfigurationsEnumerate(remote, TargetControlListReadCallback_Target, &targetContext);
    if (err) {
        HAPLogError(&logObject, "%s: Unable to enumerate targets for remotes %p.", __func__, (const void *) remote);
        HAPFatalError();
    }
    
    // Target Configuration
    void *bytes;
    size_t numBytes;
    HAPTLVWriterGetBuffer(&configurationWriter, &bytes, &numBytes);
    if (numBytes) {
        err = HAPTLVWriterAppend(responseWriter, &(const HAPTLV) {
            .type = kHAPCharacteristicValue_TargetControlList_Configuration,
            .value = {
                .bytes = bytes,
                .numBytes = numBytes
            }
        });
        if (err) {
            HAPAssert(err = kHAPError_OutOfResources);
            return err;
        }
    }
    return kHAPError_None;
}

//----------------------------------------------------------------------------------------------------------------------

HAP_RESULT_USE_CHECK
HAPError RemoteHandleActiveIdentifierRead(
    HAPAccessoryServerRef *server_,
    const HAPUInt32CharacteristicReadRequest *request,
    uint32_t *value,
    void *_Nullable context HAP_UNUSED)
{
    HAPPrecondition(server_);
    HAPPrecondition(request);
    HAPPrecondition(request->accessory);
    HAPPrecondition(value);
    
    Remote *remote = GetRemote(request->accessory);
    if (remote == NULL) {
        HAPLog(&logObject, "%s: No remote registered for accessory %p.",
            __func__, (const void *) request->accessory);
        return kHAPError_InvalidState;
    }
    
    *value = remote->state.activeIdentifier;
    HAPLogInfo(&logObject, "%s: Read %lu", __func__, (unsigned long) *value);
    return kHAPError_None;
}

HAP_RESULT_USE_CHECK
HAPError RemoteHandleActiveRead(
    HAPAccessoryServerRef *server_,
    const HAPUInt8CharacteristicReadRequest *request,
    uint8_t *value,
    void *_Nullable context HAP_UNUSED)
{
    HAPPrecondition(server_);
    HAPPrecondition(request);
    HAPPrecondition(value);
    
    Remote *remote = GetRemote(request->accessory);
    if (remote == NULL) {
        HAPLog(&logObject, "%s: No remote registered for accessory %p.",
            __func__, (const void *) request->accessory);
        return kHAPError_InvalidState;
    }
    
    *value = RemoteIsActive((RemoteRef *) remote) ?
             kHAPCharacteristicValue_Active_Active: kHAPCharacteristicValue_Active_Inactive;
    
    HAPLogInfo(&logObject, "%s: Read %u", __func__, *value);
    return kHAPError_None;
}

HAP_RESULT_USE_CHECK
HAPError RemoteHandleActiveWrite(
    HAPAccessoryServerRef *server_,
    const HAPUInt8CharacteristicWriteRequest *request,
    uint8_t value,
    void *_Nullable context)
{
    HAPPrecondition(server_);
    HAPPrecondition(request);
    
    Remote *remote = GetRemote(request->accessory);
    if (remote == NULL) {
        HAPLog(&logObject, "%s: No remote registered for accessory %p.",
            __func__, (const void *) request->accessory);
        return kHAPError_InvalidState;
    }
    if (remote->state.activeIdentifier == 0) {
        HAPLog(&logObject, "%s: Active Identifier of remote %p is 0. Cannot modify active state.",
            __func__, (const void *) remote);
        return kHAPError_InvalidState;
    }
    
    HAPLogInfo(&logObject, "%s: Write %u", __func__, value);
    
    if (value == kHAPCharacteristicValue_Active_Inactive) {
        SessionDeactivate(remote, context);
        return kHAPError_None;
    }
    else {
        return SessionActivate(remote, request->session, context);
    }
}

HAP_RESULT_USE_CHECK
HAPError RemoteHandleButtonEventRead(
    HAPAccessoryServerRef *server_,
    const HAPTLV8CharacteristicReadRequest *request,
    HAPTLVWriterRef *responseWriter,
    void *_Nullable context HAP_UNUSED)
{
    HAPPrecondition(server_);
    HAPPrecondition(request);
    HAPPrecondition(request->accessory);
    HAPPrecondition(responseWriter);
    
    HAPError err;
    
    Remote *remote = GetRemote(request->accessory);
    if (remote == NULL) {
        HAPLog(&logObject, "%s: No remote registered for accessory %p.",
            __func__, (const void *) request->accessory);
        return kHAPError_InvalidState;
    }
    
    const ButtonEvent *buttonEvent = SessionReadButtonEvent(remote, request->session);
    if (buttonEvent == NULL) {
        HAPLog(&logObject,
            "%s: No button events registered for accessory %p and session %p: Active identifier %lu",
            __func__, (const void *) request->accessory, (const void *) request->session,
            (unsigned long) remote->state.activeIdentifier);
        return kHAPError_InvalidState;
    }
    
    HAPLogInfo(&logObject, "%s: Sending button event: ID %u, state: %u, timestamp: %lu, identifier: %lu", __func__,
        buttonEvent->buttonID, buttonEvent->buttonState,
        (unsigned long) buttonEvent->timestamp, (unsigned long) buttonEvent->activeIdentifier);
    
    // Button ID
    {
        uint8_t buttonIDBytes[] = { (uint8_t) buttonEvent->buttonID };
        err = HAPTLVWriterAppend(responseWriter, &(const HAPTLV) {
            .type = kHAPCharacteristicValue_ButtonEvent_ButtonID,
            .value = {
                .bytes = buttonIDBytes,
                .numBytes = sizeof buttonIDBytes
            }
        });
        if (err) {
            HAPAssert(err = kHAPError_OutOfResources);
            HAPLog(&logObject, "%s: out of resources when writing the Button ID.", __func__);
            return err;
        }
    }
    
    // Button State
    {
        HAPCharacteristicValue_ButtonEvent_ButtonState buttonState;
        if (buttonEvent->buttonState == kRemoteButtonState_Up) {
            buttonState = kHAPCharacteristicValue_ButtonEvent_ButtonState_Up;
        } else {
            buttonState = kHAPCharacteristicValue_ButtonEvent_ButtonState_Down;
        }
        uint8_t buttonStateBytes[] = { buttonState };
        err = HAPTLVWriterAppend(responseWriter, &(const HAPTLV) {
            .type = kHAPCharacteristicValue_ButtonEvent_ButtonState,
            .value = {
                .bytes = buttonStateBytes,
                .numBytes = sizeof buttonStateBytes
            }
        });
        if (err) {
            HAPAssert(err = kHAPError_OutOfResources);
            HAPLog(&logObject, "%s: out of resources when writing the Button State.", __func__);
            return err;
        }
    }
    
    // Timestamp
    {
        uint8_t timestampBytes[] = { HAPExpandLittleUInt64(buttonEvent->timestamp) };
        err = HAPTLVWriterAppend(responseWriter, &(const HAPTLV) {
            .type = kHAPCharacteristicValue_ButtonEvent_Timestamp,
            .value = {
                .bytes = timestampBytes,
                .numBytes = sizeof timestampBytes
            }
        });
        if (err) {
            HAPAssert(err = kHAPError_OutOfResources);
            HAPLog(&logObject, "%s: out of resources when writing the Button Timestamp.", __func__);
            return err;
        }
    }
    
    // Active Identifier
    {
        uint8_t identifierBytes[] = { HAPExpandLittleUInt32(buttonEvent->activeIdentifier) };
        err = HAPTLVWriterAppend(responseWriter, &(const HAPTLV) {
            .type = kHAPCharacteristicValue_ButtonEvent_ActiveIdentifier,
            .value = {
                .bytes = identifierBytes,
                .numBytes = sizeof identifierBytes
            }
        });
        if (err) {
            HAPAssert(err = kHAPError_OutOfResources);
            HAPLog(&logObject, "%s: out of resources when writing the Active Identifier.", __func__);
            return err;
        }
    }
    
    // Raise button event
    if (!SessionQueuedButtonEventsIsEmpty(remote, request->session)) {
        HAPAccessoryServerRaiseEvent(server_,
            &buttonEventCharacteristic,
            &targetControlService,
            request->accessory);
    }
    return kHAPError_None;
}

void RemoteHandleButtonEventSubscribe(
    HAPAccessoryServerRef *server,
    const HAPTLV8CharacteristicSubscriptionRequest *request,
    void *_Nullable context HAP_UNUSED)
{
    HAPPrecondition(server);
    HAPPrecondition(request);
    
    HAPLogDebug(&logObject, "%s: On accessory %p with session %p.",
        __func__, (const void *) request->accessory, (const void *) request->session);
    
    const HAPAccessory *accessory = request->accessory;
    
    Remote *remote = GetRemote(accessory);
    if (remote == NULL) {
        HAPLog(&logObject, "%s: No remote registered for accessory %p.",  __func__, (const void *) accessory);
        return;
    }
    
    RemoteSession *remoteSession = GetRegisteredSession(remote, request->session);
    if (remoteSession == NULL) {
        HAPLog(&logObject, "%s: No session %p registered for remote %p.",
            __func__, (const void *) request->session, (const void *) remote);
        return;
    }
    
    SessionClearButtonEvents(remote, remoteSession);
    
    HAPLogDebug(&logObject, "%s: Remote %p subscribing session %p.",
        __func__, (const void *) remote, (const void *) request->session);
    
    remoteSession->buttonEventsSubscribed = true;
}

void RemoteHandleButtonEventUnsubscribe(
    HAPAccessoryServerRef *server,
    const HAPTLV8CharacteristicSubscriptionRequest *request,
    void *_Nullable context HAP_UNUSED)
{
    HAPPrecondition(server);
    HAPPrecondition(request);
    
    HAPLogDebug(&logObject, "%s: On accessory %p with session %p.",
        __func__, (const void *) request->accessory, (const void *) request->session);
    
    const HAPAccessory *accessory = request->accessory;
    
    Remote *remote = GetRemote(accessory);
    if (remote == NULL) {
        HAPLog(&logObject, "%s: No remote registered for accessory %p.",  __func__, (const void *) accessory);
        return;
    }
    
    RemoteSession *remoteSession = GetRegisteredSession(remote, request->session);
    if (remoteSession == NULL) {
        HAPLog(&logObject, "%s: Session %p registered for remote %p.",
            __func__, (const void *) request->session, (const void *) remote);
        return;
    }
    
    SessionClearButtonEvents(remote, remoteSession);
    
    HAPLogDebug(&logObject, "%s: Remote %p unsubscribing session %p.",
        __func__, (const void *) remote, (const void *) request->session);
    
    remoteSession->buttonEventsSubscribed = false;
}

void RemoteHandleSessionAccept(
    HAPAccessoryServerRef *server,
    HAPSessionRef *session,
    void *_Nullable context HAP_UNUSED)
{
    HAPPrecondition(server);
    HAPPrecondition(session);
    
    HAPError err;
    
    Remote *remote = (Remote *) remoteStorage.remotes;
    while (remote != NULL) {
        err = SessionRegister(remote, session);
        if (err != kHAPError_None) {
            HAPLogError(&logObject, "%s: Unable to register session %p on remote %p,",
                __func__, (const void *) session, (const void *) remote);
        }
        remote = (Remote *) remote->nextRemote;
    }
}

void RemoteHandleSessionInvalidate(
    HAPAccessoryServerRef *server,
    HAPSessionRef *session,
    void *_Nullable context)
{
    HAPPrecondition(server);
    HAPPrecondition(session);
    
    HAPError err;
    Remote *remote = (Remote *) remoteStorage.remotes;
    while (remote != NULL) {
        err = SessionDeregister(remote, session, context);
        if (err != kHAPError_None) {
            HAPLogError(&logObject, "%s: Unable to register session %p on remote %p,",
                __func__, (const void *) session, (const void *) remote);
        }
        remote = (Remote *) remote->nextRemote;
    }
}

//----------------------------------------------------------------------------------------------------------------------

void RemoteHandleTargetControlIdentifierUpdate(
    HAPAccessoryServerRef *server,
    HAPDataStreamDispatcherRef *dispatcher,
    HAPTargetControlDataStreamProtocol *dataStreamProtocol,
    const HAPServiceRequest *request,
    HAPDataStreamHandle dataStream,
    HAPTargetControlDataStreamProtocolTargetIdentifier targetIdentifier,
    void *_Nullable context HAP_UNUSED)
{
    HAPPrecondition(server);
    HAPPrecondition(dispatcher);
    HAPPrecondition(dataStreamProtocol);
    HAPPrecondition(request);
    
    HAPLogInfo(&kHAPLog_Default, "[%u] Target Control identifier updated: %lu.",
        dataStream, (unsigned long) targetIdentifier);
}
