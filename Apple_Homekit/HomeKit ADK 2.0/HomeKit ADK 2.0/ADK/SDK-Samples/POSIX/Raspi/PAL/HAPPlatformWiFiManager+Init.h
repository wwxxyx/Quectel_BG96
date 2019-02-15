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

#ifndef HAP_PLATFORM_WIFI_MANAGER_INIT_H
#define HAP_PLATFORM_WIFI_MANAGER_INIT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <net/if.h>

#include "HAPPlatform.h"

#if __has_feature(nullability)
#pragma clang assume_nonnull begin
#endif

/**@file
 * Wi-Fi manager based on `dnsmasq` and `hostapd`.
 *
 * The following third-party dependencies must be available to run this code without modifications:
 * - ifdown / ifup or dhcpcd: Used to reload the Wi-Fi configuration and joining the configured Wi-Fi network.
 * - wpa_supplicant:          Used to store the Wi-Fi configuration and to provide regulatory domain information.
 *
 * The following assumptions will be made if this code is not modified:
 * - The wpa_supplicant configuration file will be overwritten or deleted by the implementation.
 *   This may affect existing system-wide Wi-Fi configurations that are not managed by this code.
 * - The wpa_supplicant configuration file must be deleted if the Wi-Fi is not configured.
 *
 * **Example**
   
   @code{.c}
   
   // Allocate Wi-Fi manager.
   static HAPPlatformWiFiManager wiFiManager;
   
   // Initialize Wi-Fi manager.
   HAPPlatformWiFiManagerCreate(&wiFiManager,
       &(const HAPPlatformWiFiManagerOptions) {
           // Use default network interface for Wi-Fi configuration.
           .interfaceName = NULL
       });
   
   @endcode
 */

/**
 * Wi-Fi manager initialization options.
 */
typedef struct {
    /**
     * Local network interface name used for Wi-Fi configuration.
     * A value of NULL will use "wlan0".
     */
    const char *_Nullable interfaceName;
} HAPPlatformWiFiManagerOptions;

/**
 * Wi-Fi manager.
 */
struct HAPPlatformWiFiManager {
    // Opaque type. Do not access the instance fields directly.
    /**@cond */
    char interfaceName[IFNAMSIZ];
    /**@endcond */
};

/**
 * Initializes a Wi-Fi manager.
 *
 * @param      wiFiManager          Pointer to an allocated but uninitialized HAPPlatformWiFiManager structure.
 * @param      options              Initialization options.
 */
void HAPPlatformWiFiManagerCreate(
    HAPPlatformWiFiManagerRef wiFiManager,
    const HAPPlatformWiFiManagerOptions *options);

/**
 * Country code (ISO 3166-1 alpha-2) to indicate the country-specific
 * regulatory domain in which the device is operating.
 */
typedef struct {
    char stringValue[2 + 1];        /**< NULL-terminated. */
} HAPPlatformRegulatoryDomain;

/**
 * Gets the regulatory domain in which the device is operating.
 *
 * - Regulatory domain information can be configured system-wide in file /etc/wpa_supplicant/wpa_supplicant.conf.
 *
 * @param      wiFiManager          Wi-Fi manager.
 * @param[out] regulatoryDomain     Regulatory domain in which the device is operating.
 *
 * @return kHAPError_None           If successful.
 * @return kHAPError_InvalidState   If no regulatory domain has been configured.
 */
HAP_RESULT_USE_CHECK
HAPError HAPPlatformWiFiManagerGetRegulatoryDomain(
    HAPPlatformWiFiManagerRef wiFiManager,
    HAPPlatformRegulatoryDomain *regulatoryDomain);

#if __has_feature(nullability)
#pragma clang assume_nonnull end
#endif

#ifdef __cplusplus
}
#endif

#endif
