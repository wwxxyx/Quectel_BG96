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

#ifndef HAP_PLATFORM_BLE_PERIPHERAL_MANAGER_INIT_H
#define HAP_PLATFORM_BLE_PERIPHERAL_MANAGER_INIT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ble.h"
#include "nrf_ble_gatt.h"

#include "HAPPlatform.h"

#if __has_feature(nullability)
#pragma clang assume_nonnull begin
#endif

/**@file
 * BLE peripheral manager for the Nordic nRF5 SDK.
 *
 * The implementation registers the vendor-specific UUID bases that are used with the SoftDevice.
 * For HomeKit, at least 3 vendor-specific UUID bases are necessary to accommodate:
 * - Apple-defined HomeKit characteristics
 * - Service Instance ID characteristics
 * - Characteristic Instance ID descriptors
 * If additional vendor-specific HomeKit characteristics are present with different UUID bases,
 * those are registered as well by the implementation. Additional memory needs to be allocated by the client.
 *
 * Events from the Nordic BLE stack need to be forwarded to the implementation's function
 * `HAPPlatformBLEPeripheralManagerHandleBLEEvent` and `HAPPlatformBLEPeripheralManagerHandleGATTEvent`.
 *
 * /!\ The implementation does not support concurrent connections or non-HomeKit characteristics at this time.
 * If that functionality is required by the application, the implementation needs to be adjusted.
 * Alternatively, non-HomeKit BLE events may be processed by the client without forwarding them to the implementation.
 *
 * /!\ If the HAPAccessoryServer is stopped the Nordic SoftDevice needs to be restarted.
 * This is necessary because there is no clean way to remove the HomeKit specific GATT attributes at this time.
 *
 * **Example**
  
   @code{.c}
   
   // Configure nRF5 SDK in `sdk_config.h`.
   #define NRF_SDH_BLE_VS_UUID_COUNT 3 // At least 3 vendor-specific UUID bases are necessary for HomeKit.
   
   // Initialize nRF5 BLE stack.
   // This is typically accomplished by the function `ble_stack_init` in the nRF5 SDK examples.
  
   // Get dependencies.
   HAPPlatformKeyValueStoreRef keyValueStore;
   
   // Allocate memory for vendor-specific UUID bases.
   static ble_uuid128_t uuidBases[NRF_SDH_BLE_VS_UUID_COUNT];
   
   // Allocate BLE peripheral manager.
   static HAPPlatformBLEPeripheralManager blePeripheralManager;
   
   // Initialize BLE peripheral manager.
   HAPPlatformBLEPeripheralManagerCreate(&blePeripheralManager,
        &(const HAPPlatformBLEPeripheralManagerOptions) {
            .keyValueStore = keyValueStore,
            .uuidBases = uuidBases,
            .numUUIDBases = HAPArrayCount(uuidBases)
        });
   
   // Adjust BLE event handler to forward events to the BLE peripheral manager.
   // The BLE event handler is typically called `ble_evt_handler` in the nRF5 SDK examples.
   ble_evt_t const *p_ble_evt; // Received from the nRF5 SDK.
   HAPPlatformBLEPeripheralManagerHandleBLEEvent(&blePeripheralManager, p_ble_evt);
 
   // Adjust GATT event handler to forward events to the BLE peripheral manager.
   // If necessary, a GATT event handler may be registered in advance with the nRF5 SDK function `nrf_ble_gatt_init`.
   nrf_ble_gatt_t *p_gatt; const nrf_ble_gatt_evt_t *p_evt; // Received from the nRF5 SDK.
   HAPPlatformBLEPeripheralManagerHandleGATTEvent(&blePeripheralManager, p_gatt, p_evt);
 
   @endcode
 */

/**
 * BLE peripheral manager initialization options.
 */
typedef struct {
    /**
     * Key-value store.
     * If critical data is pending, BLE reads will be delayed until the data is persisted.
     */
    HAPPlatformKeyValueStoreRef keyValueStore;
    
    /**
     * Pointer to a static memory address where base UUIDs can be stored.
     * This memory must remain valid while the BLE peripheral manager is initialized.
     */
    ble_uuid128_t *uuidBases;
    
    /**
     * Number of UUID bases that the memory holds.
     *
     * - HAP applications require at least @c uuid_manager_NUM_UUID_BASES_MIN custom UUID bases.
     *   Additional UUID bases are necessary to support vendor-defined services and characteristics.
     */
    size_t numUUIDBases;
} HAPPlatformBLEPeripheralManagerOptions;

// Opaque type. Do not use directly.
/**@cond */
HAP_ENUM_BEGIN(uint8_t, HAPPlatformBLEPeripheralManagerGATTOperation) {
    kHAPPlatformBLEPeripheralManagerGATTOperation_None,
    kHAPPlatformBLEPeripheralManagerGATTOperation_Disconnecting,
    kHAPPlatformBLEPeripheralManagerGATTOperation_Read,
    kHAPPlatformBLEPeripheralManagerGATTOperation_Write
} HAP_ENUM_END(uint8_t, HAPPlatformBLEPeripheralManagerGATTOperation);
/**@endcond */

/**
 * BLE peripheral manager.
 */
struct HAPPlatformBLEPeripheralManager {
    // Opaque type. Do not access the instance fields directly.
    /**@cond */
    HAPPlatformKeyValueStoreRef keyValueStore;
    
    HAPPlatformBLEPeripheralManagerDelegate delegate;
    
    ble_uuid128_t *uuidBases;
    size_t numUUIDBases;
    size_t numUsedUUIDBases;
    
    uint8_t advertisingHandle;
    uint8_t advData[BLE_GAP_ADV_SET_DATA_SIZE_MAX];
    uint8_t scanRspData[BLE_GAP_ADV_SET_DATA_SIZE_MAX];
    
    uint16_t connectionHandle;
    uint16_t mtu;
    uint16_t buggedMTU;
    uint16_t clientMTU;
    uint8_t bytes[512];
    struct {
        HAPPlatformBLEPeripheralManagerGATTOperation operation;
        uint16_t handle;
        uint16_t offset;
        uint16_t numBytes;
        HAPPlatformTimerRef retryResponseTimer;
    } gatt;
    bool indicationInProgress : 1;
    /**@endcond */
};

/**
 * Initializes the BLE peripheral manager.
 *
 * @param[out] blePeripheralManager Pointer to an allocated but uninitialized HAPPlatformBLEPeripheralManager structure.
 * @param      options              Initialization options.
 */
void HAPPlatformBLEPeripheralManagerCreate(
    HAPPlatformBLEPeripheralManagerRef blePeripheralManager,
    const HAPPlatformBLEPeripheralManagerOptions *options);

/**
 * Handles a BLE event from the Bluetooth stack.
 *
 * - If custom non-HomeKit characteristics are added, the related events must be filtered before calling this function.
 *
 * @param      blePeripheralManager BLE peripheral manager.
 * @param      event                BLE event to handle.
 */
void HAPPlatformBLEPeripheralManagerHandleBLEEvent(
    HAPPlatformBLEPeripheralManagerRef blePeripheralManager,
    const ble_evt_t *event);

/**
 * Handles a GATT event from the Bluetooth stack.
 *
 * - If custom non-HomeKit characteristics are added, the related events must be filtered before calling this function.
 *
 * @param      blePeripheralManager BLE peripheral manager.
 * @param      gatt                 GATT structure.
 * @param      event                GATT event to handle.
 */
void HAPPlatformBLEPeripheralManagerHandleGATTEvent(
    HAPPlatformBLEPeripheralManagerRef blePeripheralManager,
    const nrf_ble_gatt_t *gatt,
    const nrf_ble_gatt_evt_t *event);

#if __has_feature(nullability)
#pragma clang assume_nonnull end
#endif

#ifdef __cplusplus
}
#endif

#endif
