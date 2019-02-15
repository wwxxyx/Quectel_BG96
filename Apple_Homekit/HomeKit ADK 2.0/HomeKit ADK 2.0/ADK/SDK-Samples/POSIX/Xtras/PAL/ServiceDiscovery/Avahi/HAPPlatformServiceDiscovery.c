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

#include <errno.h>
#include <sys/time.h>
#include <unistd.h>

#include <avahi-common/alternative.h>
#include <avahi-common/error.h>
#include <avahi-common/malloc.h>

#include "HAPPlatform+Init.h"
#include "HAPPlatformServiceDiscovery+Init.h"

static const HAPLogObject logObject = {
    .subsystem = kHAPPlatform_LogSubsystem,
    .category = "ServiceDiscovery"
};

// TODO Add support for re-registering service discovery in case of error while app is running.

#define AvahiFreeSafe(ptr) do { HAPAssert(ptr); avahi_free(ptr); ptr = NULL; } while(0)

#define AvahiClientFreeSafe(ptr) do { HAPAssert(ptr); avahi_client_free(ptr); ptr = NULL; } while(0)

#define AvahiStringListFreeSafe(ptr) do { HAPAssert(ptr); avahi_string_list_free(ptr); ptr = NULL; } while(0)

static void HandleFileHandleCallback(
    HAPPlatformFileHandleRef fileHandle,
    HAPPlatformFileHandleEvent fileHandleEvents,
    void *_Nullable context)
{
    HAPAssert(context);
    AvahiWatch *watch = context;

    HAPAssert(watch->fileHandle);
    HAPAssert(watch->fileHandle == fileHandle);

    HAPLogDebug(&logObject, "%s: %p.", __func__, (const void *) watch);

    watch->revents = 0;
    if (((watch->events & AVAHI_WATCH_IN) == AVAHI_WATCH_IN)
        && fileHandleEvents.isReadyForReading)
    {
        watch->revents |= AVAHI_WATCH_IN;
    }
    if (((watch->events & AVAHI_WATCH_OUT) == AVAHI_WATCH_OUT)
        && fileHandleEvents.isReadyForWriting)
    {
        watch->revents |= AVAHI_WATCH_OUT;
    }
    if (((watch->events & AVAHI_WATCH_ERR) == AVAHI_WATCH_ERR)
        && fileHandleEvents.hasErrorConditionPending)
    {
        watch->revents |= AVAHI_WATCH_ERR;
    }
    if (watch->revents != 0) {
        HAPAssert(watch->callback);
        watch->callback(watch, watch->fileDescriptor, watch->revents, watch->userdata);
    }
}

static AvahiWatch *CreateWatch(
    const AvahiPoll *api,
    int fileDescriptor,
    AvahiWatchEvent events,
    AvahiWatchCallback callback,
    void *userdata)
{
    HAPAssert(api);
    HAPAssert(api->userdata);

    HAPError err;

    HAPPlatformServiceDiscoveryRef serviceDiscovery = api->userdata;

    AvahiWatch *watch = malloc(sizeof (struct AvahiWatch));
    if (!watch) {
        HAPLogError(&logObject, "Cannot allocate more watch objects.");
        return NULL;
    }

    HAPLogDebug(&logObject, "%s: %p.", __func__, (const void *) watch);

    HAPRawBufferZero(watch, sizeof *watch);

    watch->fileDescriptor = fileDescriptor;
    watch->events = events;
    watch->revents = 0;
    watch->callback = callback;
    watch->userdata = userdata;
    watch->fileHandle = 0;

    watch->prevWatch = serviceDiscovery->watchList->prevWatch;
    watch->nextWatch = serviceDiscovery->watchList;
    serviceDiscovery->watchList->prevWatch->nextWatch = watch;
    serviceDiscovery->watchList->prevWatch = watch;

    HAPAssert((watch->events & AVAHI_WATCH_HUP) != AVAHI_WATCH_HUP);
    err = HAPPlatformFileHandleRegister(&watch->fileHandle,
        watch->fileDescriptor,
        (HAPPlatformFileHandleEvent) {
            .isReadyForReading = (watch->events & AVAHI_WATCH_IN) == AVAHI_WATCH_IN,
            .isReadyForWriting = (watch->events & AVAHI_WATCH_OUT) == AVAHI_WATCH_OUT,
            .hasErrorConditionPending = (watch->events & AVAHI_WATCH_ERR) == AVAHI_WATCH_ERR
        },
        HandleFileHandleCallback, watch);
    if (err) {
        HAPLogError(&logObject, "Service discovery file handle registration failed!");
        HAPFatalError();
    }
    HAPAssert(watch->fileHandle);

    return watch;
}

static void UpdateWatch(
    AvahiWatch *watch,
    AvahiWatchEvent events)
{
    HAPAssert(watch);

    HAPLogDebug(&logObject, "%s: %p.", __func__, (const void *) watch);

    watch->events = events;
    watch->revents = 0;

    HAPAssert((watch->events & AVAHI_WATCH_HUP) != AVAHI_WATCH_HUP);
    HAPPlatformFileHandleUpdateInterests(watch->fileHandle,
        (HAPPlatformFileHandleEvent) {
            .isReadyForReading = (watch->events & AVAHI_WATCH_IN) == AVAHI_WATCH_IN,
            .isReadyForWriting = (watch->events & AVAHI_WATCH_OUT) == AVAHI_WATCH_OUT,
            .hasErrorConditionPending = (watch->events & AVAHI_WATCH_ERR) == AVAHI_WATCH_ERR
        },
        HandleFileHandleCallback, watch);
}

static AvahiWatchEvent GetWatchEvents(
    AvahiWatch *watch)
{
    HAPAssert(watch);

    HAPLogDebug(&logObject, "%s: %p.", __func__, (const void *) watch);

    return watch->revents;
}

static void ReleaseWatch(
    AvahiWatch *watch)
{
    HAPAssert(watch);

    HAPLogDebug(&logObject, "%s: %p.", __func__, (const void *) watch);

    if (watch->fileHandle) {
        HAPPlatformFileHandleDeregister(watch->fileHandle);
    }

    watch->prevWatch->nextWatch = watch->nextWatch;
    watch->nextWatch->prevWatch = watch->prevWatch;

    watch->fileDescriptor = -1;
    watch->events = 0;
    watch->revents = 0;
    watch->callback = NULL;
    watch->userdata = NULL;
    watch->fileHandle = 0;
    watch->prevWatch = NULL;
    watch->nextWatch = NULL;
    HAPPlatformFreeSafe(watch);
}

static void DeregisterFileHandles(
    HAPPlatformServiceDiscoveryRef serviceDiscovery)
{
    HAPAssert(serviceDiscovery);

    struct AvahiWatch *watch = serviceDiscovery->watchList->nextWatch;

    while (watch != serviceDiscovery->watchList) {
        if (watch->fileHandle) {
            HAPPlatformFileHandleDeregister(watch->fileHandle);
            watch->fileHandle = 0;
        }
        watch = watch->nextWatch;
    }
}

static void ReleaseWatchList(
    HAPPlatformServiceDiscoveryRef serviceDiscovery)
{
    HAPAssert(serviceDiscovery);

    while (serviceDiscovery->watchList->nextWatch != serviceDiscovery->watchList) {
        ReleaseWatch(serviceDiscovery->watchList->nextWatch);
    }
}

static void HandleTimeoutTimer(
    HAPPlatformTimerRef timer,
    void *_Nullable context)
{
    HAPAssert(context);
    AvahiTimeout *timeout = context;

    HAPAssert(timeout->timer);
    HAPAssert(timeout->timer == timer);

    HAPLogDebug(&logObject, "%s: %p.", __func__, (const void *) timeout);

    timeout->timer = 0;
    HAPAssert(timeout->callback);
    timeout->callback(timeout, timeout->userdata);
}

static void ScheduleTimeoutTimer(
    AvahiTimeout *timeout,
    const struct timeval *deadline)
{
    HAPAssert(timeout);

    if (timeout->timer) {
        HAPPlatformTimerDeregister(timeout->timer);
        timeout->timer = 0;
    }

    if (!deadline) {
        HAPLogDebug(&logObject, "%s: %p, timeout = undef.", __func__, (const void *) timeout);
        return;
    }

    struct timeval now;
    int e = gettimeofday(&now, NULL);
    if (e) {
        int _errno = errno;
        HAPLogError(&logObject, "gettimeofday failed: %d.", _errno);
        HAPFatalError();
    }

    HAPTime dt;
    if ((now.tv_sec < deadline->tv_sec) ||
        ((now.tv_sec == deadline->tv_sec)
            && (now.tv_usec < deadline->tv_usec)))
    {
        if (deadline->tv_usec >= now.tv_usec) {
            dt = (HAPTime) (deadline->tv_sec - now.tv_sec) * 1000
                + (HAPTime) (deadline->tv_usec - now.tv_usec) / 1000;
        } else {
            dt = (HAPTime) (deadline->tv_sec - now.tv_sec - 1) * 1000
                + (HAPTime) (1000000 - (now.tv_usec - deadline->tv_usec)) / 1000;
        }
    } else {
        dt = 0;
    }

    HAPTime currentClock = HAPPlatformClockGetCurrent();
    if (UINT64_MAX - currentClock < dt) {
        HAPLogError(&logObject, "Service discovery timer overflow!");
        HAPFatalError();
    }

    HAPError err = HAPPlatformTimerRegister(&timeout->timer,
        currentClock + dt,
        HandleTimeoutTimer, timeout);
    if (err) {
        HAPLogError(&logObject, "Service discovery timer registration failed!");
        HAPFatalError();
    }
    HAPAssert(timeout->timer);

    HAPLogDebug(&logObject, "%s: %p, timeout = %lu.", __func__, (const void *) timeout, (unsigned long) dt);
}

static AvahiTimeout *CreateTimeout(
    const AvahiPoll *api,
    const struct timeval *tv,
    AvahiTimeoutCallback callback,
    void *userdata)
{
    HAPAssert(api);
    HAPAssert(api->userdata);

    HAPPlatformServiceDiscoveryRef serviceDiscovery = api->userdata;

    AvahiTimeout *timeout = malloc(sizeof (struct AvahiTimeout));
    if (!timeout) {
        HAPLogError(&logObject, "Cannot allocate more timeout objects.");
        return NULL;
    }

    HAPLogDebug(&logObject, "%s: %p.", __func__, (const void *) timeout);

    HAPRawBufferZero(timeout, sizeof *timeout);

    timeout->callback = callback;
    timeout->userdata = userdata;
    timeout->timer = 0;

    timeout->prevTimeout = serviceDiscovery->timeoutList->prevTimeout;
    timeout->nextTimeout = serviceDiscovery->timeoutList;
    serviceDiscovery->timeoutList->prevTimeout->nextTimeout = timeout;
    serviceDiscovery->timeoutList->prevTimeout = timeout;

    ScheduleTimeoutTimer(timeout, tv);

    return timeout;
}

static void UpdateTimeout(
    AvahiTimeout *timeout,
    const struct timeval *tv)
{
    HAPAssert(timeout);

    HAPLogDebug(&logObject, "%s: %p.", __func__, (const void *) timeout);

    ScheduleTimeoutTimer(timeout, tv);
}

static void ReleaseTimeout(
    AvahiTimeout *timeout)
{
    HAPAssert(timeout);

    HAPLogDebug(&logObject, "%s: %p.", __func__, (const void *) timeout);

    if (timeout->timer) {
        HAPPlatformTimerDeregister(timeout->timer);
    }

    timeout->prevTimeout->nextTimeout = timeout->nextTimeout;
    timeout->nextTimeout->prevTimeout = timeout->prevTimeout;

    timeout->callback = NULL;
    timeout->userdata = NULL;
    timeout->timer = 0;
    timeout->prevTimeout = NULL;
    timeout->nextTimeout = NULL;
    HAPPlatformFreeSafe(timeout);
}

static void DeregisterTimers(
    HAPPlatformServiceDiscoveryRef serviceDiscovery)
{
    HAPAssert(serviceDiscovery);

    struct AvahiTimeout *timeout = serviceDiscovery->timeoutList->nextTimeout;

    while (timeout != serviceDiscovery->timeoutList) {
        if (timeout->timer) {
            HAPPlatformTimerDeregister(timeout->timer);
            timeout->timer = 0;
        }
        timeout = timeout->nextTimeout;
    }
}

static void ReleaseTimeoutList(
    HAPPlatformServiceDiscoveryRef serviceDiscovery)
{
    HAPAssert(serviceDiscovery);

    while (serviceDiscovery->timeoutList->nextTimeout != serviceDiscovery->timeoutList) {
        ReleaseTimeout(serviceDiscovery->timeoutList->nextTimeout);
    }
}

static void SetupAvahiEntryGroup(
    HAPPlatformServiceDiscoveryRef serviceDiscovery);

static void HandleAvahiEntryGroupCallback(
    AvahiEntryGroup *entryGroup,
    AvahiEntryGroupState state,
    void *userdata)
{
    HAPAssert(entryGroup);
    HAPAssert(userdata);

    int r;

    HAPPlatformServiceDiscoveryRef serviceDiscovery = userdata;

    if (!serviceDiscovery->avahiEntryGroup) {
        serviceDiscovery->avahiEntryGroup = entryGroup;
    } else {
        HAPAssert(serviceDiscovery->avahiEntryGroup == entryGroup);
    }

    switch (state) {
        case AVAHI_ENTRY_GROUP_UNCOMMITED:
        case AVAHI_ENTRY_GROUP_REGISTERING:
        case AVAHI_ENTRY_GROUP_ESTABLISHED: {
            // skip
        } break;
        case AVAHI_ENTRY_GROUP_COLLISION: {
            char *name = avahi_alternative_service_name(serviceDiscovery->name);
            HAPAssert(name);
            AvahiFreeSafe(serviceDiscovery->name);
            serviceDiscovery->name = name;
            SetupAvahiEntryGroup(serviceDiscovery);
        } break;
        case AVAHI_ENTRY_GROUP_FAILURE: {
            r = avahi_client_errno(serviceDiscovery->avahiClient);
            HAPLogError(&logObject, "%s: AVAHI_ENTRY_GROUP_FAILURE: %d, %s", __func__, r, avahi_strerror(r));
            HAPFatalError();
        } break;
    }
}

static void SetupAvahiEntryGroup(
    HAPPlatformServiceDiscoveryRef serviceDiscovery)
{
    HAPAssert(serviceDiscovery);

    int r;

    if (HAVE_WAC && HAPStringAreEqual(serviceDiscovery->protocol, "_mfi-config._tcp")
        && serviceDiscovery->interfaceIndex)
    {
        // We noticed that service registration has no effect immediately after a change in the network interface
        // configuration, e.g., when switching in and out of a software access point for WAC. A delay of about 15
        // seconds seems to circumvent this problem.

        HAPLogInfo(&logObject, "Delaying service registration ...");
        unsigned int delay = 15 /* seconds */;
        do {
            delay = sleep(delay);
        } while (delay);
    }

    HAPLogDebug(&logObject, "interfaceIndex: %d", serviceDiscovery->interfaceIndex);
    HAPLogDebug(&logObject, "name: \"%s\"", serviceDiscovery->name);
    HAPLogDebug(&logObject, "protocol: \"%s\"", serviceDiscovery->protocol);
    HAPLogDebug(&logObject, "port: %u", (unsigned int) serviceDiscovery->port);

    if (!serviceDiscovery->avahiEntryGroup) {
        serviceDiscovery->avahiEntryGroup = avahi_entry_group_new(
            serviceDiscovery->avahiClient, HandleAvahiEntryGroupCallback, serviceDiscovery);
    }
    if (serviceDiscovery->avahiEntryGroup) {
        if (avahi_entry_group_is_empty(serviceDiscovery->avahiEntryGroup)) {
            r = avahi_entry_group_add_service_strlst(serviceDiscovery->avahiEntryGroup,
                serviceDiscovery->interfaceIndex, AVAHI_PROTO_UNSPEC, /* flags: */ 0,
                serviceDiscovery->name, serviceDiscovery->protocol, /* domain: */ NULL,
                /* host: */ NULL, serviceDiscovery->port,
                serviceDiscovery->avahiStringList);
            if (r == AVAHI_OK) {
                r = avahi_entry_group_commit(serviceDiscovery->avahiEntryGroup);
                if (r != AVAHI_OK) {
                    HAPLogError(&logObject, 
                        "%s: avahi_entry_group_commit: %d, %s", __func__, r, avahi_strerror(r));
                    HAPFatalError();
                }
            } else if (r == AVAHI_ERR_COLLISION) {
                char *name = avahi_alternative_service_name(serviceDiscovery->name);
                HAPAssert(name);
                AvahiFreeSafe(serviceDiscovery->name);
                serviceDiscovery->name = name;
                r = avahi_entry_group_reset(serviceDiscovery->avahiEntryGroup);
                if (r != AVAHI_OK) {
                    HAPLogError(&logObject, 
                        "%s: avahi_entry_group_reset: %d, %s", __func__, r, avahi_strerror(r));
                    HAPFatalError();
                }
                SetupAvahiEntryGroup(serviceDiscovery);
            } else {
                HAPLogError(&logObject, 
                    "%s: avahi_entry_group_add_service_strlst: %d, %s", __func__, r, avahi_strerror(r));
                HAPFatalError();
            }
        }
    } else {
        r = avahi_client_errno(serviceDiscovery->avahiClient);
        HAPLogError(&logObject, 
            "%s: avahi_entry_group_new: %d, %s", __func__, r, avahi_strerror(r));
        HAPFatalError();
    }
}

static void HandleAvahiClientCallback(
    AvahiClient *client,
    AvahiClientState state,
    void *userdata)
{
    HAPAssert(client);
    HAPAssert(userdata);

    int r;

    HAPPlatformServiceDiscoveryRef serviceDiscovery = userdata;

    if (!serviceDiscovery->avahiClient) {
        serviceDiscovery->avahiClient = client;
    }   else {
        HAPAssert(serviceDiscovery->avahiClient == client);
    }

    switch (state) {
        case AVAHI_CLIENT_S_RUNNING: {
            SetupAvahiEntryGroup(serviceDiscovery);
        } break;
        case AVAHI_CLIENT_S_COLLISION:
        case AVAHI_CLIENT_S_REGISTERING: {
            if (serviceDiscovery->avahiEntryGroup) {
                r = avahi_entry_group_reset(serviceDiscovery->avahiEntryGroup);
                if (r != AVAHI_OK) {
                    HAPLogError(&logObject, "%s: avahi_entry_group_reset: %d, %s", __func__, r, avahi_strerror(r));
                    HAPFatalError();
                }
            }
        } break;
        case AVAHI_CLIENT_FAILURE: {
            r = avahi_client_errno(serviceDiscovery->avahiClient);
            HAPLogError(&logObject, "%s: AVAHI_CLIENT_FAILURE: %d, %s", __func__, r, avahi_strerror(r));
            HAPFatalError();
        } break;
        case AVAHI_CLIENT_CONNECTING: {
            // skip
        } break;
    }
}

void HAPPlatformServiceDiscoveryCreate(
    HAPPlatformServiceDiscoveryRef serviceDiscovery,
    const HAPPlatformServiceDiscoveryOptions *options)
{
    HAPPrecondition(serviceDiscovery);
    HAPPrecondition(options);

    HAPLog(&logObject, "Storage configuration: serviceDiscovery = %lu",
        (unsigned long) sizeof *serviceDiscovery);
    HAPLog(&logObject, "Storage configuration: watch = %lu",
        (unsigned long) sizeof (struct AvahiWatch));    
    HAPLog(&logObject, "Storage configuration: timeout = %lu",
        (unsigned long) sizeof (struct AvahiTimeout));    

    HAPRawBufferZero(serviceDiscovery, sizeof *serviceDiscovery);

    if (options->interfaceName) {
        size_t numInterfaceNameBytes = HAPStringGetNumBytes(HAPNonnull(options->interfaceName));
        if ((numInterfaceNameBytes == 0) || (numInterfaceNameBytes >= sizeof serviceDiscovery->interfaceName)) {
            HAPLogError(&logObject, "Invalid local network interface name.");
            HAPFatalError();
        }
        HAPRawBufferCopyBytes(
            serviceDiscovery->interfaceName, HAPNonnull(options->interfaceName), numInterfaceNameBytes);
    }
    serviceDiscovery->interfaceIndex = AVAHI_IF_UNSPEC;

    serviceDiscovery->name = NULL;
    serviceDiscovery->protocol = NULL;
    serviceDiscovery->port = 0;

    serviceDiscovery->avahiPoll.userdata = serviceDiscovery;
    serviceDiscovery->avahiPoll.watch_new = CreateWatch;
    serviceDiscovery->avahiPoll.watch_update = UpdateWatch;
    serviceDiscovery->avahiPoll.watch_get_events = GetWatchEvents;
    serviceDiscovery->avahiPoll.watch_free = ReleaseWatch;
    serviceDiscovery->avahiPoll.timeout_new = CreateTimeout;
    serviceDiscovery->avahiPoll.timeout_update = UpdateTimeout;
    serviceDiscovery->avahiPoll.timeout_free = ReleaseTimeout;

    serviceDiscovery->avahiStringList = NULL;
    serviceDiscovery->avahiEntryGroup = NULL;
    serviceDiscovery->avahiClient = NULL;

    serviceDiscovery->timeoutSentinel.callback = NULL;
    serviceDiscovery->timeoutSentinel.userdata = NULL;
    serviceDiscovery->timeoutSentinel.timer = 0;
    serviceDiscovery->timeoutSentinel.prevTimeout = &serviceDiscovery->timeoutSentinel;
    serviceDiscovery->timeoutSentinel.nextTimeout = &serviceDiscovery->timeoutSentinel;
    serviceDiscovery->timeoutList = &serviceDiscovery->timeoutSentinel;

    serviceDiscovery->watchSentinel.fileDescriptor = -1;
    serviceDiscovery->watchSentinel.events = 0;
    serviceDiscovery->watchSentinel.revents = 0;
    serviceDiscovery->watchSentinel.callback = NULL;
    serviceDiscovery->watchSentinel.userdata = NULL;
    serviceDiscovery->watchSentinel.fileHandle = 0;
    serviceDiscovery->watchSentinel.prevWatch = &serviceDiscovery->watchSentinel;
    serviceDiscovery->watchSentinel.nextWatch = &serviceDiscovery->watchSentinel;
    serviceDiscovery->watchList = &serviceDiscovery->watchSentinel;
}

void HAPPlatformServiceDiscoveryRegister(
    HAPPlatformServiceDiscoveryRef serviceDiscovery,
    const char *name,
    const char *protocol,
    uint16_t port,
    HAPPlatformServiceDiscoveryTXTRecord *txtRecords,
    size_t numTXTRecords)
{
    HAPPrecondition(serviceDiscovery);
    HAPPrecondition(!serviceDiscovery->avahiClient);
    HAPPrecondition(name);
    HAPPrecondition(protocol);
    HAPPrecondition(txtRecords);

    int r;

    if (serviceDiscovery->interfaceName[0]) {
        unsigned int i = if_nametoindex(serviceDiscovery->interfaceName);
        if ((i == 0) || (i > UINT32_MAX)) {
            HAPLogError(&logObject, "Mapping the local network interface name to its corresponding index failed.");
            HAPFatalError();
        }
        serviceDiscovery->interfaceIndex = (int) i;
    } else {
        serviceDiscovery->interfaceIndex = AVAHI_IF_UNSPEC;
    }

    HAPAssert(!serviceDiscovery->name);
    serviceDiscovery->name = avahi_strdup(name);
    if (!serviceDiscovery->name) {
        HAPLogError(&logObject, "avahi_strdup(name) failed.");
        HAPFatalError();
    }

    HAPAssert(!serviceDiscovery->protocol);
    serviceDiscovery->protocol = avahi_strdup(protocol);
    if (!serviceDiscovery->protocol) {
        HAPLogError(&logObject, "avahi_strdup(protocol) failed.");
        HAPFatalError();
    }

    serviceDiscovery->port = port;

    HAPAssert(!serviceDiscovery->avahiStringList);
    for (size_t i = 0; i < numTXTRecords; i++) {
        HAPPrecondition(!txtRecords[i].value.numBytes || txtRecords[i].value.bytes);
        HAPPrecondition(txtRecords[i].value.numBytes <= UINT8_MAX);
        if (txtRecords[i].value.bytes) {
            HAPLogBufferDebug(&logObject, txtRecords[i].value.bytes, txtRecords[i].value.numBytes,
                "txtRecord[%lu]: \"%s\"", (unsigned long) i, txtRecords[i].key);
        } else {
            HAPLogDebug(&logObject, "txtRecord[%lu]: \"%s\"", (unsigned long) i, txtRecords[i].key);
        }
        AvahiStringList *l = avahi_string_list_add_pair_arbitrary(serviceDiscovery->avahiStringList,
            txtRecords[i].key, txtRecords[i].value.bytes, txtRecords[i].value.numBytes);
        if (!l) {
            HAPLogError(&logObject, "avahi_string_list_add_pair_arbitrary failed.");
            HAPFatalError();
        }
        serviceDiscovery->avahiStringList = l;
    }

    HAPAssert(!serviceDiscovery->avahiEntryGroup);

    AvahiClient *c = avahi_client_new(&serviceDiscovery->avahiPoll, /* flags: */ 0,
        HandleAvahiClientCallback, serviceDiscovery, &r);
    if (!c) {
        HAPLogError(&logObject, "%s: avahi_client_new: %d, %s", __func__, r, avahi_strerror(r));
        HAPFatalError();
    }

    if (!serviceDiscovery->avahiClient) {
        serviceDiscovery->avahiClient = c;
    }   else {
        HAPAssert(serviceDiscovery->avahiClient == c);
    }
}

void HAPPlatformServiceDiscoveryUpdateTXTRecords(
    HAPPlatformServiceDiscoveryRef serviceDiscovery,
    HAPPlatformServiceDiscoveryTXTRecord *txtRecords,
    size_t numTXTRecords)
{
    HAPPrecondition(serviceDiscovery);
    HAPPrecondition(serviceDiscovery->avahiClient);
    HAPPrecondition(txtRecords);

    HAPLogDebug(&logObject, "interfaceIndex: %d", serviceDiscovery->interfaceIndex);
    HAPLogDebug(&logObject, "name: \"%s\"", serviceDiscovery->name);
    HAPLogDebug(&logObject, "protocol: \"%s\"", serviceDiscovery->protocol);
    HAPLogDebug(&logObject, "port: %u", (unsigned int) serviceDiscovery->port);

    if (serviceDiscovery->avahiStringList) {
        AvahiStringListFreeSafe(serviceDiscovery->avahiStringList);
    }

    HAPAssert(!serviceDiscovery->avahiStringList);
    for (size_t i = 0; i < numTXTRecords; i++) {
        HAPPrecondition(!txtRecords[i].value.numBytes || txtRecords[i].value.bytes);
        HAPPrecondition(txtRecords[i].value.numBytes <= UINT8_MAX);
        if (txtRecords[i].value.bytes) {
            HAPLogBufferDebug(&logObject, txtRecords[i].value.bytes, txtRecords[i].value.numBytes,
                "txtRecord[%lu]: \"%s\"", (unsigned long) i, txtRecords[i].key);
        } else {
            HAPLogDebug(&logObject, "txtRecord[%lu]: \"%s\"", (unsigned long) i, txtRecords[i].key);
        }
        AvahiStringList *l = avahi_string_list_add_pair_arbitrary(serviceDiscovery->avahiStringList,
            txtRecords[i].key, txtRecords[i].value.bytes, txtRecords[i].value.numBytes);
        if (!l) {
            HAPLogError(&logObject, "avahi_string_list_add_pair_arbitrary failed.");
            HAPFatalError();
        }
        serviceDiscovery->avahiStringList = l;
    }

    int r = avahi_entry_group_update_service_txt_strlst(serviceDiscovery->avahiEntryGroup,
        serviceDiscovery->interfaceIndex, AVAHI_PROTO_UNSPEC, /* flags: */ 0,
        serviceDiscovery->name, serviceDiscovery->protocol, /* domain: */ NULL,
        serviceDiscovery->avahiStringList);
    if (r != AVAHI_OK) {
        HAPLogError(&logObject, "avahi_entry_group_update_service_txt_strlst failed: %d.", r);
        HAPFatalError();
    }
}

void HAPPlatformServiceDiscoveryStop(
    HAPPlatformServiceDiscoveryRef serviceDiscovery)
{
    HAPPrecondition(serviceDiscovery);
    HAPPrecondition(serviceDiscovery->avahiClient);

    DeregisterTimers(serviceDiscovery);
    DeregisterFileHandles(serviceDiscovery);

    AvahiClientFreeSafe(serviceDiscovery->avahiClient);

    if (serviceDiscovery->avahiEntryGroup) {
        serviceDiscovery->avahiEntryGroup = NULL;
    }

    if (serviceDiscovery->avahiStringList) {
        AvahiStringListFreeSafe(serviceDiscovery->avahiStringList);
    }

    AvahiFreeSafe(serviceDiscovery->protocol);

    AvahiFreeSafe(serviceDiscovery->name);

    ReleaseTimeoutList(serviceDiscovery);
    ReleaseWatchList(serviceDiscovery);
}
