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

#ifndef HAP_PLATFORM_IP_INIT_H
#define HAP_PLATFORM_IP_INIT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <net/if.h>

#include "HAPPlatform.h"
#include "HAPPlatformFileHandle.h"

#if __has_feature(nullability)
#pragma clang assume_nonnull begin
#endif

/**@file
 * TCP stream manager implementation for POSIX.
 *
 * The following limitations apply if this code is not modified:
 * - Non-null values for the option interfaceName are ignored on platforms without support for the socket option
 *   SO_BINDTODEVICE which binds the socket to a particular network interface.
 *
 * **Example**

   @code{.c}
   // Allocate TCP stream manager object.
   static HAPPlatformTCPStreamManager tcpStreamManager;

   // Initialize TCP stream manager object.
   HAPPlatformTCPStreamManagerCreate(&platform.tcpStreamManager,
       &(const HAPPlatformTCPStreamManagerOptions) {
           // Listen on all available network interfaces.
           .interfaceName = NULL,

           // Listen on an unused port number from the ephemeral port range.
           .port = 0,

           // Allocate enough concurrent TCP streams to support the IP accessory.
           .maxConcurrentTCPStreams = kHAPIPSessionStorage_DefaultNumElements
   });

   @endcode
 */

/**
 * TCP stream manager initialization options.
 */
typedef struct {
    /**
     * Local network interface name on which to bind the TCP stream manager.
     * A value of NULL will use all available network interfaces.
     */
    const char *_Nullable interfaceName;

    /**
     * Local port number on which to bind the TCP stream manager.
     * A value of 0 will use an unused port number from the ephemeral port range.
     */
    uint16_t port;

    /**
     * Maximum number of concurrent TCP streams.
     */
    size_t maxConcurrentTCPStreams;
} HAPPlatformTCPStreamManagerOptions;

// Opaque type. Do not use directly.
/**@cond */
typedef struct {
    HAPPlatformTCPStreamManagerRef tcpStreamManager;

    uint32_t interfaceIndex;
    uint16_t port;

    int fileDescriptor;
    HAPPlatformFileHandleRef fileHandle;
    HAPPlatformTCPStreamListenerCallback _Nullable callback;
    void *_Nullable context;
} HAPPlatformTCPStreamListener;
/**@endcond */

// Opaque type. Do not use directly.
/**@cond */
typedef struct {
    HAPPlatformTCPStreamManagerRef tcpStreamManager;

    int fileDescriptor;
    HAPPlatformFileHandleRef fileHandle;
    HAPPlatformTCPStreamEvent interests;
    HAPPlatformTCPStreamEventCallback _Nullable callback;
    void *_Nullable context;
} HAPPlatformTCPStream;
/**@endcond */

/**
 * TCP stream manager.
 */
struct HAPPlatformTCPStreamManager {
    // Opaque type. Do not access the instance fields directly.
    /**@cond */
    size_t numTCPStreams;
    size_t maxTCPStreams;

    struct {
        char interfaceName[IFNAMSIZ];
        uint16_t port;
    } tcpStreamListenerConfiguration;

    HAPPlatformTCPStreamListener tcpStreamListener;
    HAPPlatformTCPStream *_Nullable tcpStreams;
    /**@endcond */
};

/**
 * Initializes TCP stream manager.
 *
 * @param[out] tcpStreamManager     Pointer to an allocated but uninitialized HAPPlatformTCPStreamManager structure.
 * @param      options              Initialization options.
 */
void HAPPlatformTCPStreamManagerCreate(
    HAPPlatformTCPStreamManagerRef tcpStreamManager,
    const HAPPlatformTCPStreamManagerOptions *options);

/**
 * Releases resources associated with an initialized TCP stream manager instance.
 *
 * - IMPORTANT: Do not use this method on TCP stream manager structures that are not initialized!
 *
 * @param      tcpStreamManager     TCP stream manager.
 */
void HAPPlatformTCPStreamManagerRelease(
    HAPPlatformTCPStreamManagerRef tcpStreamManager);

#if __has_feature(nullability)
#pragma clang assume_nonnull end
#endif

#ifdef __cplusplus
}
#endif

#endif
