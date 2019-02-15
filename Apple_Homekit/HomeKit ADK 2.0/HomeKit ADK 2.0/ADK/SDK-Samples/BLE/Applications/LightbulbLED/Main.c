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

#include "app_scheduler.h"
#include "app_timer.h"
#include "ble_conn_params.h"
#include "boards.h"
#include "nrf_ble_gatt.h"
#include "nrf_delay.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"

#include "HAP.h"
#include "HAPPlatform+Init.h"
#include "HAPPlatformAccessorySetup+Init.h"
#include "HAPPlatformBLEPeripheralManager+Init.h"
#include "HAPPlatformKeyValueStore+Init.h"
#include "HAPPlatformMFiHWAuth+Init.h"
#include "HAPPlatformRunLoop+Init.h"
#include "HAPPlatformMFiTokenAuth+Init.h"

#include "App.h"
#include "DB.h"

/**
 * Number of implicit UUID bases defined by HAP library.
 *
 * 1. Characteristic Instance ID descriptor
 *    See HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *    Table 7-53 Bluetooth LE Characteristic Descriptors
 *
 * 2. Service Instance ID characteristic
 *    See HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *    Section 7.4.4.3 Service Instance ID
 *
 * 3. HAP Base UUID
 *    See HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *    Section 6.6.1 Service and Characteristic Types
 *
 * 4. ADK Version characteristic
 */
#define IMPLICIT_UUID_BASE_COUNT        4

//----------------------------------------------------------------------------------------------------------------------

/**
 * Application's BLE observer priority. You shouldn't need to modify this value.
 */
#define APP_BLE_OBSERVER_PRIO           3

/**
 * A tag identifying the SoftDevice BLE configuration.
 */
#define APP_BLE_CONN_CFG_TAG            1

//----------------------------------------------------------------------------------------------------------------------

/**
 * Minimum acceptable connection interval (0.02 seconds).
 */
#define MIN_CONN_INTERVAL               MSEC_TO_UNITS(20, UNIT_1_25_MS)

/**
 * Maximum acceptable connection interval (0.04 seconds).
 */
#define MAX_CONN_INTERVAL               MSEC_TO_UNITS(40, UNIT_1_25_MS)

/**
 * Slave latency.
 */
#define SLAVE_LATENCY                   0

/**
 * Connection supervisory timeout (4 seconds).
 */
#define CONN_SUP_TIMEOUT                MSEC_TO_UNITS(4000, UNIT_10_MS)

//----------------------------------------------------------------------------------------------------------------------

/**
 * Time from initiating event (connect or start of notification) to first time
 * sd_ble_gap_conn_param_update is called (0.5 seconds).
 */
#define FIRST_CONN_PARAMS_UPDATE_DELAY  APP_TIMER_TICKS(500)

/**
 * Time between each call to sd_ble_gap_conn_param_update after the first call (5 seconds).
 */
#define NEXT_CONN_PARAMS_UPDATE_DELAY   APP_TIMER_TICKS(5000)

/**
 * Number of attempts before giving up the connection parameter negotiation.
 */
#define MAX_CONN_PARAMS_UPDATE_COUNT    3

//----------------------------------------------------------------------------------------------------------------------

/**
 * Maximum size of scheduler events.
 */
#define SCHED_MAX_EVENT_DATA_SIZE       APP_TIMER_SCHED_EVENT_DATA_SIZE

/**
 * Maximum number of events in the scheduler queue.
 */
#define SCHED_QUEUE_SIZE                16

/**
 * GATT module instance.
 */
NRF_BLE_GATT_DEF(m_gatt);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define PREFERRED_ADVERTISING_INTERVAL (HAPBLEAdvertisingIntervalCreateFromMilliseconds(417.5f))

/**
 * Global platform objects.
 */
static struct {
    HAPPlatformKeyValueStore keyValueStore;
    HAPPlatformAccessorySetup accessorySetup;
    HAPPlatformBLEPeripheralManager blePeripheralManager;
    HAPPlatformMFiHWAuth mfiHWAuth;
    HAPPlatformMFiTokenAuth mfiTokenAuth;
} platform;

/**
 * Initialize global platform objects.
 */
static void InitializePlatform()
{
    // Key-value store.
    HAP_ALIGNAS(4)
    static uint8_t keyValueStoreBytes[2048];
    HAPPlatformKeyValueStoreCreate(&platform.keyValueStore,
        &(const HAPPlatformKeyValueStoreOptions) {
            .baseFDSFileID = 0xBF00,
            .baseFDSRecordID = 0xBF00,
            .bytes = keyValueStoreBytes,
            .maxBytes = sizeof keyValueStoreBytes,
        });
    
    // Accessory setup manager. Depends on key-value store.
    HAPPlatformAccessorySetupCreate(&platform.accessorySetup,
        &(const HAPPlatformAccessorySetupOptions) {
            .keyValueStore = &platform.keyValueStore,
            .useDisplay = HAVE_DISPLAY,
            .useProgrammableNFC = HAVE_NFC
        });
    
    // BLE peripheral manager. Depends on key-value store.
    HAPAssert(NRF_SDH_BLE_VS_UUID_COUNT >= IMPLICIT_UUID_BASE_COUNT);
    static ble_uuid128_t uuidBases[NRF_SDH_BLE_VS_UUID_COUNT];
    HAPPlatformBLEPeripheralManagerCreate(&platform.blePeripheralManager,
        &(const HAPPlatformBLEPeripheralManagerOptions) {
            .uuidBases = uuidBases,
            .numUUIDBases = sizeof uuidBases / sizeof uuidBases[0],
            .keyValueStore = &platform.keyValueStore
        });
    
    // Apple Authentication Coprocessor provider.
    HAPPlatformMFiHWAuthCreate(&platform.mfiHWAuth,
        &(const HAPPlatformMFiHWAuthOptions) {
            .vccPin = 28,
            .sclPin = 3,
            .sdaPin = 4,
            .twiInstanceID = 0,
            .i2cAddress = 0x10
        });
    
    // Software Token provider. Depends on key-value store.
    HAPPlatformMFiTokenAuthCreate(&platform.mfiTokenAuth,
        &(const HAPPlatformMFiTokenAuthOptions) {
            .keyValueStore = &platform.keyValueStore
        });
    
    // Run loop. Depends on key-value store.
    HAPPlatformRunLoopCreate(
        &(const HAPPlatformRunLoopOptions) {
            .keyValueStore = &platform.keyValueStore
        });
}

/**
 * De-initialize global platform objects.
 */
static void DeinitializePlatform()
{
    // Apple Authentication Coprocessor provider.
    HAPPlatformMFiHWAuthRelease(&platform.mfiHWAuth);
}

/**
 * Restore platform specific factory settings.
 */
void RestorePlatformFactorySettings(void)
{
}

static HAPAccessoryServerRef accessoryServer;

/**
 * Timer initialization.
 *
 * @details Initializes the timer module. This creates and starts application timers.
 */
static void timers_init(void)
{
    // Initialize timer module.
    ret_code_t e = app_timer_init();
    HAPAssert(!e);
}

/**
 * GAP initialization.
 *
 * @details This function sets up all the necessary GAP (Generic Access Profile) parameters of the
 *          device including the device name, appearance, and the preferred connection parameters.
 */
static void gap_params_init(void)
{
    // Set BLE appearance.
    ret_code_t e = sd_ble_gap_appearance_set(BLE_APPEARANCE_UNKNOWN);
    HAPAssert(!e);
    
    // Set peripheral preferred connection parameters.
    ble_gap_conn_params_t gap_conn_params = {
        .min_conn_interval = MIN_CONN_INTERVAL,
        .max_conn_interval = MAX_CONN_INTERVAL,
        .slave_latency = SLAVE_LATENCY,
        .conn_sup_timeout = CONN_SUP_TIMEOUT
    };
    e = sd_ble_gap_ppcp_set(&gap_conn_params);
    HAPAssert(!e);
}

void HandleGATTEvent(
    nrf_ble_gatt_t *p_gatt,
    const nrf_ble_gatt_evt_t *p_evt)
{
    HAPPrecondition(p_gatt);
    HAPPrecondition(p_evt);
    
    HAPPlatformBLEPeripheralManagerHandleGATTEvent(&platform.blePeripheralManager, p_gatt, p_evt);
}

/**
 * Function for initializing the GATT module.
 */
static void gatt_init(void)
{
    ret_code_t e = nrf_ble_gatt_init(&m_gatt, HandleGATTEvent);
    HAPAssert(!e);
}

/**
 * Function for handling a Connection Parameters error.
 *
 * @param[in] nrf_error  Error code containing information about what went wrong.
 */
static void conn_params_error_handler(
    uint32_t nrf_error)
{
    HAPLogError(&kHAPLog_Default, "%s: %lu.", __func__, (unsigned long) nrf_error);
    HAPFatalError();
}

/**
 * Function for initializing the Connection Parameters module.
 */
static void conn_params_init(void)
{
    ble_conn_params_init_t cp_init = {
        .p_conn_params = NULL,
        .first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY,
        .next_conn_params_update_delay = NEXT_CONN_PARAMS_UPDATE_DELAY,
        .max_conn_params_update_count = MAX_CONN_PARAMS_UPDATE_COUNT,
        .start_on_notify_cccd_handle = BLE_GATT_HANDLE_INVALID,
        .disconnect_on_fail = true,
        .evt_handler = NULL,
        .error_handler = conn_params_error_handler
    };
    ret_code_t e = ble_conn_params_init(&cp_init);
    HAPAssert(!e);
}

/**
 * Function for handling BLE events.
 *
 * @param[in]   p_ble_evt   Bluetooth stack event.
 * @param[in]   p_context   Unused.
 */
static void ble_evt_handler(
    ble_evt_t const *p_ble_evt,
    void *_Nullable p_context HAP_UNUSED)
{
    HAPPrecondition(p_ble_evt);
    
    nrf_gpio_pin_clear(LED_3);
    switch (p_ble_evt->header.evt_id) {
        case BLE_GAP_EVT_CONNECTED: {
            nrf_gpio_pin_clear(LED_4);
        } break;
        case BLE_GAP_EVT_DISCONNECTED: {
            nrf_gpio_pin_set(LED_4);
        } break;
        default: {
        } break;
    }
    HAPPlatformBLEPeripheralManagerHandleBLEEvent(&platform.blePeripheralManager, p_ble_evt);
    nrf_gpio_pin_set(LED_3);
}

/**
 * Initialize the BLE stack.
 *
 * This function the SoftDevice and the BLE event interrupt.
 */
static void ble_stack_init(void)
{
    ret_code_t e = nrf_sdh_enable_request();
    HAPAssert(!e);
    
    // Configure the BLE stack using the default settings.
    // Fetch the start address of the application RAM.
    uint32_t ram_start = 0;
    e = nrf_sdh_ble_default_cfg_set(APP_BLE_CONN_CFG_TAG, &ram_start);
    HAPAssert(!e);
    
    // Enable BLE stack.
    e = nrf_sdh_ble_enable(&ram_start);
    if (e) {
        HAPAssert(e == NRF_ERROR_NO_MEM);
        HAPLogError(&kHAPLog_Default, "nrf_sdh_ble_enable failed: %lu - Update linker settings to RAM origin = 0x%08lx",
            (unsigned long) e, (unsigned long) ram_start);
        HAPFatalError();
    }
    
    // Register a handler for BLE events.
    NRF_SDH_BLE_OBSERVER(m_ble_observer, APP_BLE_OBSERVER_PRIO, ble_evt_handler, NULL);
}

/**
 * Initialize the Event Scheduler.
 */
static void scheduler_init(void)
{
    APP_SCHED_INIT(SCHED_MAX_EVENT_DATA_SIZE, SCHED_QUEUE_SIZE);
}

/**
 * Initialize the nrf log module.
 */
static void log_init(void)
{
    ret_code_t e = NRF_LOG_INIT(NULL);
    HAPAssert(!e);
    
    NRF_LOG_DEFAULT_BACKENDS_INIT();
}

/**
 * Function for application main entry.
 */
int main(void)
{
    // Initialize stack.
    scheduler_init();
    timers_init();
    log_init();
    ble_stack_init();
    gap_params_init();
    gatt_init();
    conn_params_init();
    
    // Initialize global platform objects.
    InitializePlatform();
    HAPAssert(HAPGetCompatibilityVersion() == HAP_COMPATIBILITY_VERSION);
    
    // Prepare accessory server storage.
    static HAPBLEGATTTableElementRef gattTableElements[kAttributeCount];
    static HAPBLESessionCacheElementRef sessionCacheElements[kHAPBLESessionCache_MinElements];
    static HAPSessionRef session;
    static uint8_t procedureBytes[2048];
    static HAPBLEProcedureRef procedures[1];
    static HAPBLEAccessoryServerStorage bleAccessoryServerStorage = {
        .gattTableElements = gattTableElements,
        .numGATTTableElements = HAPArrayCount(gattTableElements),
        .sessionCacheElements = sessionCacheElements,
        .numSessionCacheElements = HAPArrayCount(sessionCacheElements),
        .session = &session,
        .procedures = procedures,
        .numProcedures = HAPArrayCount(procedures),
        .procedureBuffer = {
            .bytes = procedureBytes,
            .numBytes = sizeof procedureBytes
        }
    };
    
    // Initialize accessory server.
    HAPAccessoryServerCreate(&accessoryServer,
        &(const HAPAccessoryServerOptions) {
            .maxPairings = kHAPPairingStorage_MinElements,
            .ble = {
                .available = true,
                .accessoryServerStorage = &bleAccessoryServerStorage,
                .preferredAdvertisingInterval = PREFERRED_ADVERTISING_INTERVAL,
                .preferredNotificationDuration = kHAPBLENotification_MinDuration
            }
        },
        &(const HAPPlatform) {
            .accessorySetup = &platform.accessorySetup,
            .keyValueStore = &platform.keyValueStore,
            .ble = {
                .blePeripheralManager = &platform.blePeripheralManager
            },
            .authentication = {
                .mfiHWAuth = &platform.mfiHWAuth,
                .mfiTokenAuth = HAPPlatformMFiTokenAuthIsProvisioned(&platform.mfiTokenAuth) ?
                                &platform.mfiTokenAuth : NULL
            }
        },
        &(const HAPAccessoryServerCallbacks) {
            .handleUpdatedState = AccessoryServerHandleUpdatedState
        },
        /* context: */ NULL);
    
    // Start execution.
    HAPLogInfo(&kHAPLog_Default, "Application started.");
    nrf_gpio_pin_set(LED_3);
    nrf_gpio_cfg_output(LED_3);
    nrf_gpio_pin_set(LED_4);
    nrf_gpio_cfg_output(LED_4);
    
    // Create app object.
    AppCreate(&accessoryServer, &platform.keyValueStore);
    
    // Start accessory server for App.
    AppAccessoryServerStart();
    
    // Enter pairing mode.
    HAPLogInfo(&kHAPLog_Default, "Entering pairing mode for 5 minutes...");
    HAPAccessoryServerEnterPairingMode(&accessoryServer);
    
    // Run main loop until explicitly stopped.
    HAPPlatformRunLoopRun();
    // Run loop stopped explicitly by calling function HAPPlatformRunLoopStop.
    
    // Cleanup.
    AppRelease();
    
    // Release accessory server.
    HAPLogInfo(&kHAPLog_Default, "Releasing accessory server.");
    HAPAccessoryServerRelease(&accessoryServer);
    
    DeinitializePlatform();
    
    // Reboot.
    HAPLogInfo(&kHAPLog_Default, "Rebooting.");
    nrf_delay_ms(50);
    sd_nvic_SystemReset();
}
