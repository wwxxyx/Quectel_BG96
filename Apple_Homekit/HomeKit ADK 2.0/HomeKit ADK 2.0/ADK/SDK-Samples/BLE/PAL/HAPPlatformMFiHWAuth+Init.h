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

#ifndef HAP_PLATFORM_MFI_HW_AUTH_INIT_H
#define HAP_PLATFORM_MFI_HW_AUTH_INIT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "nrf_drv_twi.h"

#include "HAPPlatform.h"

#if __has_feature(nullability)
#pragma clang assume_nonnull begin
#endif

/**@file
 * Apple Authentication Coprocessor provider for the Nordic nRF5 SDK.
 *
 * The implementation uses the TWI library of the nRF5 SDK.
 * It allows configuration of the pins connected to the Apple Authentication Coprocessor.
 *
 * **Example**

   @code{.c}

   // Configure nRF5 SDK in `sdk_config.h`.
   #define TWI0_ENABLED 1 // If a different TWI instance is used, that instance needs to be enabled instead.

   // Allocate Apple Authentication Coprocessor provider.
   static HAPPlatformMFiHWAuth mfiHWAuth;

   // Initialize Apple Authentication Coprocessor provider.
   // This example uses a pin configuration for a nRF52832 development kit.
   // Pins may need to be adjusted if the Apple Authentication Coprocessor is connected differently.
   HAPPlatformMFiHWAuthCreate(&mfiHWAuth,
        &(const HAPPlatformMFiHWAuthOptions) {
            .vccPin = 28,
            .sclPin = 3,
            .sdaPin = 4,
            .twiInstanceID = 0, // The TWI instance needs to be enabled in the nRF5 SDK configuration (`sdk_config.h`).
            .i2cAddress = 0x10
        });

   // Before device restarts, ensure that resources are properly released.
   HAPPlatformMFiHWAuthRelease(&mfiHWAuth);

   @endcode
 */

/**
 * Apple Authentication Coprocessor provider initialization options.
 */
typedef struct {
    uint32_t vccPin;                /**< Power pin. */
    uint32_t sclPin;                /**< SCL pin. */
    uint32_t sdaPin;                /**< SDA pin. */
    uint8_t twiInstanceID;          /**< TWI instance ID. */
    uint8_t i2cAddress;             /**< I2C address. */
} HAPPlatformMFiHWAuthOptions;

/**
 * Apple Authentication Coprocessor provider.
 */
struct HAPPlatformMFiHWAuth {
    // Opaque type. Do not access the instance fields directly.
    /**@cond */
    bool initialized : 1;           /**< True if initialized. False otherwise. */
    bool enabled : 1;               /**< True if enabled. False otherwise. */

    uint32_t vccPin;                /**< Power pin. Used for power saving purposes. */
    uint32_t sclPin;                /**< SCL pin. */
    uint32_t sdaPin;                /**< SDA pin. */
    nrf_drv_twi_t twiInstanceID;    /**< TWI instance. */
    uint8_t i2cAddress;             /**< I2C address. */

    uint32_t sclConfiguration;      /**< SCL configuration while powered on. */
    uint32_t sdaConfiguration;      /**< SDA configuration while powered on. */
    /**@endcond */
};

/**
 * Initializes an Apple Authentication Coprocessor provider.
 *
 * @param[out] mfiHWAuth            Pointer to an allocated but uninitialized HAPPlatformMFiHWAuth structure.
 * @param      options              Initialization options.
 */
void HAPPlatformMFiHWAuthCreate(
    HAPPlatformMFiHWAuthRef mfiHWAuth,
    const HAPPlatformMFiHWAuthOptions *options);

/**
 * Deinitializes an Apple Authentication Coprocessor.
 *
 * @param      mfiHWAuth            Initialized Apple Authentication Coprocessor provider.
 */
void HAPPlatformMFiHWAuthRelease(
    HAPPlatformMFiHWAuthRef mfiHWAuth);

#if __has_feature(nullability)
#pragma clang assume_nonnull end
#endif

#ifdef __cplusplus
}
#endif

#endif
