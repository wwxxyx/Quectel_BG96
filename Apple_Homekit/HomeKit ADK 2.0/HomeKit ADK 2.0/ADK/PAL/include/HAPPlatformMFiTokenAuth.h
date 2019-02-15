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

#ifndef HAP_PLATFORM_MFI_TOKEN_AUTH_H
#define HAP_PLATFORM_MFI_TOKEN_AUTH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "HAPPlatform.h"

#if __has_feature(nullability)
#pragma clang assume_nonnull begin
#endif

/**
 * Software Token provider.
 */
typedef struct HAPPlatformMFiTokenAuth HAPPlatformMFiTokenAuth;
typedef struct HAPPlatformMFiTokenAuth * HAPPlatformMFiTokenAuthRef;
HAP_NONNULL_SUPPORT(HAPPlatformMFiTokenAuth)

/**
 * Maximum number of bytes that a Software Token may have.
 */
#define kHAPPlatformMFiTokenAuth_MaxMFiTokenBytes ((size_t) 1024)

/**
 * Software Token UUID.
 *
 * - The encoding of UUIDs uses reversed byte order compared to RFC 4122, i.e. network byte order backwards.
 *
 * Sample:
 *   UUID: 00112233-4455-6677-8899-AABBCCDDEEFF
 *   bytes: 0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00
 */
typedef struct {
    uint8_t bytes[16];              /**< UUID bytes in reversed network byte order. */
} HAPPlatformMFiTokenAuthUUID;
HAP_STATIC_ASSERT(sizeof (HAPPlatformMFiTokenAuthUUID) == 16, HAPPlatformMFiTokenAuthUUID);
HAP_NONNULL_SUPPORT(HAPPlatformMFiTokenAuthUUID)

/**
 * Loads the provisioned Software Token.
 *
 * - The initial Software Token must be provisioned during manufacturing or firmware update.
 *   The Software Token may later be changed through the HAPPlatformMFiTokenAuthUpdate function.
 *
 * - If the accessory is connected to an Apple Authentication Coprocessor, this function is not used.
 *   Implement the HAPPlatformMFiHWAuth platform module instead.
 *
 * - If neither an Apple Authentication Coprocessor nor a MFi Software Token is available,
 *   a warning will be shown to the user during pairing claiming that the accessory has not been certified.
 *
 * - Software Authentication is only supported on iOS 11.3 or newer.
 *
 * @param      mfiTokenAuth         Software Token provider.
 * @param[out] valid                True if a Software Token is available. False otherwise.
 * @param[out] mfiTokenUUID         Software Token UUID.
 * @param[out] mfiTokenBytes        Software Token buffer.
 * @param      maxMFiTokenBytes     Capacity of the buffer.
 * @param[out] numMFiTokenBytes     Length of the Software Token.
 *
 * @return kHAPError_None           If successful.
 * @return kHAPError_Unknown        If an error occurred while fetching the Software Token.
 * @return kHAPError_OutOfResources If the supplied buffer is not large enough to fit the Software Token.
 */
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
    "optional buffer cannot return a length");

/**
 * Updates the provisioned Software Token.
 *
 * - /!\ WARNING: It is critical to ensure that the previous Software Token is not deleted
 *   before the update procedure is complete.
 *   Failure to do so may result in an accessory that can no longer be paired!
 *
 * - /!\ WARNING: This function must block until the new Software Token has been completely persisted.
 *   Alternatively, communication over Bluetooth LE and IP may be suppressed until the token is persisted.
 *   Allowing communication without waiting for persistence may result in an accessory that can no longer be paired!
 *
 * - If the accessory is connected to a physical Apple Authentication Coprocessor, this function is not used.
 *   Implement the HAPPlatformMFiHWAuth platform module instead.
 *
 * @param      mfiTokenAuth         Software Token provider.
 * @param      mfiTokenBytes        Software Token buffer.
 * @param      numMFiTokenBytes     Length of the Software Token.
 *
 * @return kHAPError_None           If successful.
 * @return kHAPError_Unknown        If an error occurred while updating the Software Token.
 */
HAP_RESULT_USE_CHECK
HAPError HAPPlatformMFiTokenAuthUpdate(
    HAPPlatformMFiTokenAuthRef mfiTokenAuth,
    const void *mfiTokenBytes,
    size_t numMFiTokenBytes);

#if __has_feature(nullability)
#pragma clang assume_nonnull end
#endif

#ifdef __cplusplus
}
#endif

#endif
