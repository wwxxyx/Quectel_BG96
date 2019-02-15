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

#ifndef HAP_PLATFORM_KEY_VALUE_STORE_INIT_H
#define HAP_PLATFORM_KEY_VALUE_STORE_INIT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "HAPPlatform.h"

#if __has_feature(nullability)
#pragma clang assume_nonnull begin
#endif

/**@file
 * Key-value store for the Nordic nRF5 SDK.
 *
 * The implementation uses the FDS library of the nRF5 SDK which guarantees atomicity in case of power-off.
 * Additional CRC checks ensure that corrupted data due to flash errors is properly detected.
 *
 * - Each `HAPPlatformKeyValueStoreDomain` is mapped to an FDS file.
 * - Each `HAPPlatformKeyValueStoreKey` is mapped to an FDS record within an FDS file.
 *
 * Because there are fewer addressable key-value store domains and keys than FDS files and records
 * it is possible for the implementation to coexist with other code that makes use of FDS.
 * The implementation's block of FDS files and records can be selected during initialization.
 *
 * /!\ Data writes and deletions are first scheduled into a temporary buffer before they are persisted to flash.
 * This is necessary because flash operations may take significant time in hardly predictable patterns.
 * If power is lost before all changes have been persisted, pending writes and deletions may be lost.
 * The implementation ensures that writes and deletions occur in the same order in which they have been requested.
 *
 * /!\ If critical data is being written it may be necessary to delay BLE communication until the data is persisted.
 * The `HAPPlatformKeyValueStoreIsBusy` function may be used to query whether there is still pending data.
 *
 * /!\ Before restarting the device (e.g. `sd_nvic_SystemReset`) make sure that pending data has been persisted.
 *
 * **Example**
  
   @code{.c}
   
   // Configure nRF5 SDK in `sdk_config.h`.
   #define FDS_CRC_CHECK_ON_READ 1
   #define FDS_CRC_CHECK_ON_WRITE 1
   
   // Allocate memory for pending data while it is being written to flash. Must have 4 byte alignment.
   HAP_ALIGNAS(4)
   static uint8_t keyValueStoreBytes[2048]; // Necessary amount may differ depending on usage.
   
   // Allocate key-value store.
   static HAPPlatformKeyValueStore keyValueStore;
   
   // Initialize key-value store.
   HAPPlatformKeyValueStoreCreate(&keyValueStore,
       &(const HAPPlatformKeyValueStoreOptions) {
           .baseFDSFileID = 0xBF00,   // May need to be changed if this block is already used for different purposes.
           .baseFDSRecordID = 0xBF00, // May need to be changed if this block is already used for different purposes.
           .bytes = keyValueStoreBytes,
           .maxBytes = sizeof keyValueStoreBytes
       });
       
   // Before device restarts, ensure that pending data have been written.
   while (HAPPlatformKeyValueStoreIsBusy(keyValueStore)) {
       app_sched_execute();
       sd_app_evt_wait();
   }
 
   @endcode
 */

/**
 * Key-value store initialization options.
 */
typedef struct {
    /**
     * Platform domains will be mapped to FDS file range [FILE_BASE; FILE_BASE + 0xFF].
     *
     * Remarks about FDS files:
     * - File IDs should be in the range 0x0000 - 0xBFFF.
     * - The value 0xFFFF is used by the system.
     * - The values from 0xC000 to 0xFFFE are reserved for use by the Peer Manager module
     *   and can only be used in applications that do not include Peer Manager.
     */
    uint16_t baseFDSFileID;

    /**
     * Platform keys will be mapped to FDS record range [REC_BASE; REC_BASE + 0xFF].
     *
     * Remarks about FDS records:
     * - Record keys should be in the range 0x0001 - 0xBFFF.
     * - The value 0x0000 is reserved by the system.
     * - The values from 0xC000 to 0xFFFF are reserved for use by the Peer Manager module
     *   and can only be used in applications that do not include Peer Manager.
     */
    uint16_t baseFDSRecordID;

    /**
     * Buffer that contains data while it is being written to flash.
     *
     * - Buffer must have 4 byte alignment.
     */
    void *bytes;

    /**
     * Capacity of @p bytes.
     */
    size_t maxBytes;
} HAPPlatformKeyValueStoreOptions;

/**
 * Key-value store.
 */
struct HAPPlatformKeyValueStore {
    // Opaque type. Do not access the instance fields directly.
    /**@cond */
    HAPPlatformKeyValueStoreOptions _;
    size_t numBytes;
    size_t peakNumBytes;
    uint32_t modifiedRecordID;
    bool busy : 1;
    bool initialized : 1;
    bool isDeletingRecord : 1;
    /**@endcond */
};

/**
 * Initializes the key-value store.
 *
 * @param[out] keyValueStore        Pointer to an allocated but uninitialized HAPPlatformKeyValueStore structure.
 * @param      options              Initialization options.
 */
void HAPPlatformKeyValueStoreCreate(
    HAPPlatformKeyValueStoreRef keyValueStore,
    const HAPPlatformKeyValueStoreOptions *options);

/**
 * Checks whether there are outstanding key-value store operations that have not yet been committed to persistent store.
 *
 * @param      keyValueStore        Key-value store.
 *
 * @return true                     If there are outstanding KVS operations
 * @return false                    Otherwise.
 */
HAP_RESULT_USE_CHECK
bool HAPPlatformKeyValueStoreIsBusy(
    HAPPlatformKeyValueStoreRef keyValueStore);

/**
 * Returns whether the key-value store needs more time to persist pending data, i.e. communication should be delayed.
 *
 * This can happen when:
 * - A Software Token is being persisted.
 * - The key-value store queue is getting full (e.g. stress test).
 *
 * @param      keyValueStore        Key-value store.
 *
 * @return true                     If the key-value store needs more time to persist pending data.
 * @return false                    Otherwise.
 */
bool HAPPlatformKeyValueStoreNeedsTimeToPersist(
    HAPPlatformKeyValueStoreRef keyValueStore);

#if __has_feature(nullability)
#pragma clang assume_nonnull end
#endif

#ifdef __cplusplus
}
#endif

#endif
