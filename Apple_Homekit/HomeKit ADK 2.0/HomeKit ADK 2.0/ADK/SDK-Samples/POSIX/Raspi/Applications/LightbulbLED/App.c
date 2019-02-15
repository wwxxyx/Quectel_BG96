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

// The most basic HomeKit example: an accessory that represents a light bulb that
// only supports switching the light on and off.
//
// This implementation is intended for POSIX platforms. It depends on the onboard LED
// on the `LED_PORT` linux sysfs file (or similar) defined during compilation of the
// application. If `LED_PORT` is not defined only log messages are output when the port
// would be accessed.
//
// To enable user interaction, POSIX signals and the LED on the development board are used:
//
// - The LED is used to simulate the light bulb.
// - Signal `SIGUSR1` will toggle the light bulb.
// - Signal `SIGUSR2` will trigger a factory reset.
// - Signal `SIGTERM` will trigger a controlled shutdown.
//
//
// The code consists of multiple parts:
//
//   1. The definition of the accessory configuration and its internal state.
//
//   2. Helper functions to load and save the state of the accessory.
//
//   3. The setup of the accessory HomeKit and the bridged accessories attribute database.
//
//   4. Device specific configuration, callbacks and LED switches.
//
//   5. Signal handlers.
//
//   6. The callbacks that implement the actual behavior of the accessory. If the
//      accessory state has changed, the corresponding device functions are called.
//
//   7. The initialization of the accessory state.
//
//   8. A callback that gets called when the server state is updated.

#ifdef LED_PORT
    #include <errno.h>
    #include <fcntl.h>
    #include <string.h>
    #include <stdio.h>
    #include <unistd.h>
#endif

#include <signal.h>
#include <unistd.h>

#include "HAP.h"
#include "HAPPlatform+Init.h"

#include "App.h"
#include "DB.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Domain used in the key value store for application data.
 *
 * Purged: On factory reset.
 */
#define kAppKeyValueStoreDomain_Configuration                                   ((HAPPlatformKeyValueStoreDomain) 0x00)

/**
 * Key used in the key value store to store the configuration state.
 *
 * Purged: On factory reset.
 */
#define kAppKeyValueStoreKey_Configuration_State                                ((HAPPlatformKeyValueStoreDomain) 0x00)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Device LED file descriptor.
 */
typedef int LEDFileHandle;

/**
 * Global accessory configuration.
 */
typedef struct {
    struct {
        bool lightbulbOn;
    } state;
    struct {
        LEDFileHandle led;
    } device;
    bool nameChangeRequested;
    bool restoreFactorySettingsRequested;
    HAPAccessoryServerRef *server;
    HAPPlatformKeyValueStoreRef keyValueStore;
} AccessoryConfiguration;

static AccessoryConfiguration accessoryConfiguration;

//----------------------------------------------------------------------------------------------------------------------

/**
 * Load the accessory state from persistent memory.
 */
static void LoadAccessoryState(void)
{
    HAPPrecondition(accessoryConfiguration.keyValueStore);
    
    HAPError err;
    
    // Load persistent state if available
    bool found;
    size_t numBytes;
    
    err = HAPPlatformKeyValueStoreGet(accessoryConfiguration.keyValueStore,
        kAppKeyValueStoreDomain_Configuration, kAppKeyValueStoreKey_Configuration_State,
        &accessoryConfiguration.state, sizeof accessoryConfiguration.state, &numBytes, &found);
    
    if (err) {
        HAPAssert(err == kHAPError_Unknown);
        HAPFatalError();
    }
    if (!found || numBytes != sizeof accessoryConfiguration.state) {
        if (found) {
            HAPLogError(&kHAPLog_Default,
                "Unexpected app state found in key-value store. Resetting to default.");
        }
        HAPRawBufferZero(&accessoryConfiguration.state, sizeof accessoryConfiguration.state);
    }
}

/**
 * Save the accessory state to persistent memory.
 */
static void SaveAccessoryState(void)
{
    HAPPrecondition(accessoryConfiguration.keyValueStore);
    
    HAPError err;
    err = HAPPlatformKeyValueStoreSet(accessoryConfiguration.keyValueStore,
        kAppKeyValueStoreDomain_Configuration, kAppKeyValueStoreKey_Configuration_State,
        &accessoryConfiguration.state, sizeof accessoryConfiguration.state);
    if (err) {
        HAPAssert(err == kHAPError_Unknown);
        HAPFatalError();
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**
 * HomeKit accessory that provides the Lightbulb service.
 */
static HAPAccessory accessory = {
    .aid = 1,
    .category = kHAPAccessoryCategory_Lighting,
    .name = "Acme Lightbulb LED",
    .manufacturer = "Acme",
    .model = "Lightbulb1,1",
    .serialNumber = "099DB48E9E28",
    .firmwareVersion = "1",
    .hardwareVersion = "1",
    .services = (const HAPService *const[]) {
        &accessoryInformationService,
        &hapProtocolInformationService,
        &pairingService,
        &lightbulbService,
        NULL
    },
    .cameraStreamConfigurations = NULL,
    .callbacks = {
        .identify = IdentifyAccessory
    }
};

//----------------------------------------------------------------------------------------------------------------------

/**
 * Configure the LED
 */
static void DeviceConfigureLED(void)
{
    HAPLogInfo(&kHAPLog_Default, "%s", __func__);
    
    #ifdef LED_PORT
        HAPPrecondition(accessoryConfiguration.device.led == -1);

        int fd;
        do {
            fd = open(LED_PORT, O_RDWR);
        } while (fd == -1 && errno == EINTR);
        if (fd < 0) {
            int _errno = errno;
            HAPAssert(fd == -1);
            HAPLogError(&kHAPLog_Default, "%s: Opening the LED port failed: %d", __func__, _errno);
            return;
        }
        accessoryConfiguration.device.led = fd;
        HAPAssert(accessoryConfiguration.device.led >= 0);

        #ifdef LED_TRIGGER
        {
            // Create empty LED configuration.
            HAPLogInfo(&kHAPLog_Default, "%s: Disable LED trigger.", __func__);
            do {
                fd = open(LED_TRIGGER, O_RDWR);
            } while (fd == -1 && errno == EINTR);
            if (fd < 0) {
                int _errno = errno;
                HAPAssert(fd == -1);
                HAPLogError(&kHAPLog_Default, "%s: Disabling LED trigger failed: %d", __func__, _errno);
                return;
            }
            
            // Write "none" in the trigger.
            const char *bytes = "none";
            size_t numBytes = strlen(bytes);
            ssize_t n;
            do {
                n = write(fd, bytes, numBytes);
            } while (n == -1 && errno == EINTR);
            HAPAssert(n == (ssize_t)numBytes);
            (void) close(fd);
        }
        #endif
        
        // Initialize LED with off, otherwise it fails to work the first time.
        ssize_t n;
        const char* bytes = "0"; // brightness
        size_t numBytes = strlen(bytes);
        do {
            n = write(accessoryConfiguration.device.led, bytes, numBytes);
        } while (n == -1 && errno == EINTR);
        HAPAssert(n == (ssize_t)numBytes);
        
        // The configuration takes some time.
        sleep(1);
    #endif
}

/**
 * Unconfigure the LED.
 */
static void DeviceUnconfigureLED(void)
{
    HAPLogInfo(&kHAPLog_Default, "%s", __func__);
    #ifdef LED_PORT
        if (accessoryConfiguration.device.led != -1) {
            close(accessoryConfiguration.device.led);
            accessoryConfiguration.device.led = -1;
        }
    #endif
}

/**
 * Enable the LED on the device.
 */
static void DeviceEnableLED(void)
{
    HAPLogInfo(&kHAPLog_Default, "%s", __func__);
    #ifdef LED_PORT
        HAPPrecondition(accessoryConfiguration.device.led != -1);

        const char* bytes = "255"; // brightness
        const size_t numBytes = strlen(bytes);
        ssize_t len;
        do {
            len = write(accessoryConfiguration.device.led, bytes, numBytes);
        } while (len == -1 && errno == EINTR);
        HAPAssert(len == (ssize_t)numBytes);
    #endif
}

/**
 * Disable the LED on the device.
 */
static void DeviceDisableLED(void)
{
    HAPLogInfo(&kHAPLog_Default, "%s", __func__);
    #ifdef LED_PORT
        HAPPrecondition(accessoryConfiguration.device.led != -1);

        const char* bytes = "0"; // brightness
        const size_t numBytes = strlen(bytes);
        ssize_t len;
        do {
            len = write(accessoryConfiguration.device.led, bytes, numBytes);
        } while (len == -1 && errno == EINTR);
        HAPAssert(len == (ssize_t)numBytes);
    #endif
}

//----------------------------------------------------------------------------------------------------------------------

/**
 * Turn the lightbulb on.
 */
static void TurnOnLightbulb(void)
{
    DeviceEnableLED();
}

/**
 * Turn the lightbulb off.
 */
static void TurnOffLightbulb(void)
{
    DeviceDisableLED();
}

/**
 * Toggle the lightbulb.
 */
static void ToggleLightbulbState(void)
{
    accessoryConfiguration.state.lightbulbOn = !accessoryConfiguration.state.lightbulbOn;
    
    HAPLogInfo(&kHAPLog_Default, "%s: %s", __func__, accessoryConfiguration.state.lightbulbOn ? "true" : "false");
    
    if (accessoryConfiguration.state.lightbulbOn) {
        TurnOnLightbulb();
    } else {
        TurnOffLightbulb();
    }

    SaveAccessoryState();

    HAPAccessoryServerRaiseEvent(
        accessoryConfiguration.server,
        &lightbulbOnCharacteristic,
        &lightbulbService,
        &accessory);
}

//----------------------------------------------------------------------------------------------------------------------

/**
 * Global signal handler.
 */
static void HandleSignalCallback(
    void *_Nullable context,
    size_t contextSize)
{
    // Invoked from the run loop.
    
    HAPAssert(context);
    HAPAssert(contextSize == sizeof (int));
    
    int signum = *((int *) context);
    
    switch (signum) {
        case SIGUSR1: {
            ToggleLightbulbState();
        } break;
        case SIGUSR2: {
            #ifdef MANUAL_NAME_CHANGE
            // Use signal SIGUSR2 for BCT Manual Name Change.
            accessoryConfiguration.nameChangeRequested = true;
            HAPAccessoryServerStop(accessoryConfiguration.server);
            return;
            #endif
            accessoryConfiguration.restoreFactorySettingsRequested = true;
            HAPAccessoryServerStop(accessoryConfiguration.server);
        } break;
        case SIGTERM: {
            HAPAccessoryServerStop(accessoryConfiguration.server);
        } break;
        default: {
        } break;
    }
}

static void HandleSignal(
    int signum)
{
    HAPError err;
    
    err = HAPPlatformRunLoopScheduleCallback(
        HandleSignalCallback, &signum, sizeof signum);
    if (err) {
        HAPLogError(&kHAPLog_Default, "SignalHandler failed");
        HAPFatalError();
    }
}

/**
 * Initialize Signal Handlers.
 */
static void InitializeSignalHandlers()
{
    if (signal(SIGUSR1, HandleSignal) == SIG_ERR) {
        HAPLogError(&kHAPLog_Default, "SignalHandler initialization failed");
        HAPFatalError();
    }
    if (signal(SIGUSR2, HandleSignal) == SIG_ERR) {
        HAPLogError(&kHAPLog_Default, "SignalHandler initialization failed");
        HAPFatalError();
    }
    if (signal(SIGTERM, HandleSignal) == SIG_ERR) {
        HAPLogError(&kHAPLog_Default, "SignalHandler initialization failed");
        HAPFatalError();
    }
}

/**
 * Configure platform specific IO.
 */
static void ConfigureIO(void)
{
    HAPLogInfo(&kHAPLog_Default, "%s", __func__);
    
    InitializeSignalHandlers();
    DeviceConfigureLED();
}

/**
 * Unconfigure platform specific IO.
 */
static void UnconfigureIO(void)
{
    HAPLogInfo(&kHAPLog_Default, "%s", __func__);
    
    DeviceUnconfigureLED();
}

//----------------------------------------------------------------------------------------------------------------------

HAP_RESULT_USE_CHECK
HAPError IdentifyAccessory(
    HAPAccessoryServerRef *server HAP_UNUSED,
    const HAPAccessoryIdentifyRequest *request HAP_UNUSED,
    void *_Nullable context HAP_UNUSED)
{
    HAPLogInfo(&kHAPLog_Default, "%s", __func__);
    return kHAPError_None;
}

HAP_RESULT_USE_CHECK
HAPError HandleLightbulbOnRead(
    HAPAccessoryServerRef *server HAP_UNUSED,
    const HAPBoolCharacteristicReadRequest *request HAP_UNUSED,
    bool *value,
    void *_Nullable context HAP_UNUSED)
{
    *value = accessoryConfiguration.state.lightbulbOn;
    HAPLogInfo(&kHAPLog_Default, "%s: %s", __func__, *value ? "true" : "false");
    return kHAPError_None;
}

HAP_RESULT_USE_CHECK
HAPError HandleLightbulbOnWrite(
    HAPAccessoryServerRef *server,
    const HAPBoolCharacteristicWriteRequest *request,
    bool value,
    void *_Nullable context HAP_UNUSED)
{
    HAPLogInfo(&kHAPLog_Default, "%s: %s", __func__, value ? "true" : "false");
    if (accessoryConfiguration.state.lightbulbOn != value) {
        accessoryConfiguration.state.lightbulbOn = value;

        SaveAccessoryState();

        if (value) {
            TurnOnLightbulb();
        } else {
            TurnOffLightbulb();
        }

        HAPAccessoryServerRaiseEvent(
            server,
            request->characteristic,
            request->service,
            request->accessory);
    }
    return kHAPError_None;
}

//----------------------------------------------------------------------------------------------------------------------

void AppCreate(
    HAPAccessoryServerRef *server,
    HAPPlatformKeyValueStoreRef keyValueStore)
{
    HAPPrecondition(server);
    HAPPrecondition(keyValueStore);
    
    HAPLogInfo(&kHAPLog_Default, "%s", __func__);
    
    HAPRawBufferZero(&accessoryConfiguration, sizeof accessoryConfiguration);
    accessoryConfiguration.server = server;
    accessoryConfiguration.keyValueStore = keyValueStore;
    accessoryConfiguration.device.led = -1;
    
    ConfigureIO();
    
    LoadAccessoryState();
    
    if (accessoryConfiguration.state.lightbulbOn) {
        TurnOnLightbulb();
    } else {
        TurnOffLightbulb();
    }
}

void AppRelease(void)
{
    UnconfigureIO();
}

void AppAccessoryServerStart(void)
{
    HAPAccessoryServerStart(accessoryConfiguration.server, &accessory);
    
    // Enter pairing mode.
    if (HAVE_NFC) {
        HAPLogInfo(&kHAPLog_Default, "Entering pairing mode for 5 minutes...");
        HAPAccessoryServerEnterPairingMode(accessoryConfiguration.server);
    }
}

//----------------------------------------------------------------------------------------------------------------------

void AccessoryServerHandleUpdatedState(
    HAPAccessoryServerRef *server,
    void *_Nullable context)
{
    HAPPrecondition(server);
    HAPPrecondition(!context);
    
    HAPError err;
    
    switch (HAPAccessoryServerGetState(server)) {
        case kHAPAccessoryServerState_Idle: {
            HAPLogInfo(&kHAPLog_Default, "Accessory Server State did update: Idle.");
            // AccessoryServer stop requested by signal.
            if (accessoryConfiguration.restoreFactorySettingsRequested) {
                HAPLogInfo(&kHAPLog_Default, "A factory reset has been requested.");
                
                // Purge app state.
                err = HAPPlatformKeyValueStorePurgeDomain(
                    accessoryConfiguration.keyValueStore,
                    kAppKeyValueStoreDomain_Configuration);
                if (err) {
                    HAPAssert(err == kHAPError_Unknown);
                    HAPFatalError();
                }
                
                // Reset HomeKit state.
                err = HAPRestoreFactorySettings(accessoryConfiguration.keyValueStore);
                if (err) {
                    HAPAssert(err == kHAPError_Unknown);
                    HAPFatalError();
                }
                
                // Restore platform specific factory settings.
                RestorePlatformFactorySettings();
                
                // De-initialize App.
                AppRelease();
                
                accessoryConfiguration.restoreFactorySettingsRequested = false;
                
                // Re-initialize App.
                AppCreate(server, accessoryConfiguration.keyValueStore);
                
                // Restart accessory server.
                AppAccessoryServerStart();
                
                return;
            } else if (accessoryConfiguration.nameChangeRequested) {
                HAPLogInfo(&kHAPLog_Default, "A manual name change has been requested.");
    
                // Support for BCT Manual Name Change.
                accessory.name = "New - Bonjour Service Name";
                accessoryConfiguration.nameChangeRequested = false;
    
                // Restart accessory server.
                HAPAccessoryServerStart(server, &accessory);
    
                return;
            }
            
            // Signal run loop to exit.
            HAPPlatformRunLoopStop();
        } return;
        case kHAPAccessoryServerState_Running: {
            HAPLogInfo(&kHAPLog_Default, "Accessory Server State did update: Running.");
        } return;
        case kHAPAccessoryServerState_Stopping: {
            HAPLogInfo(&kHAPLog_Default, "Accessory Server State did update: Stopping.");
        } return;
    }
    HAPFatalError();
}
