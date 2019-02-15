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

#include "HAP.h"
#include "HAPPlatform+Init.h"
#include "HAPPlatformAccessorySetup+Init.h"
#include "HAPPlatformKeyValueStore+Init.h"
#include "HAPPlatformMFiHWAuth+Init.h"
#include "HAPPlatformMFiTokenAuth+Init.h"
#include "HAPPlatformRunLoop+Init.h"
#include "HAPPlatformServiceDiscovery+Init.h"
#include "HAPPlatformSoftwareAccessPoint+Init.h"
#include "HAPPlatformTCPStreamManager+Init.h"
#include "HAPPlatformWiFiManager+Init.h"

#include "App.h"
#include "DB.h"
#include "Remote.h"

/**
 * Global platform objects.
 */
static struct {
    HAPPlatformKeyValueStore keyValueStore;
    HAPPlatformAccessorySetup accessorySetup;
    HAPPlatformTCPStreamManager tcpStreamManager;
    HAPPlatformServiceDiscovery serviceDiscovery;
    HAPPlatformSoftwareAccessPoint softwareAccessPoint;
    HAPPlatformWiFiManager wiFiManager;
    HAPPlatformMFiHWAuth mfiHWAuth;
    HAPPlatformMFiTokenAuth mfiTokenAuth;
} platform;

/**
 * Initialize global platform objects.
 */
static void InitializePlatform()
{
    // Key-value store.
    HAPPlatformKeyValueStoreCreate(&platform.keyValueStore, &(const HAPPlatformKeyValueStoreOptions) {
        .rootDirectory = ".HomeKitStore"
    });
    
    // Accessory setup manager. Depends on key-value store.
    const char *LIBNFC_CONNECTION_STRING = "pn532_spi:/dev/spidev0.0:280000";
    HAPPlatformAccessorySetupCreate(&platform.accessorySetup,
        &(const HAPPlatformAccessorySetupOptions) {
            .keyValueStore = &platform.keyValueStore,
            .useDisplay = HAVE_DISPLAY,
            .useProgrammableNFC = HAVE_NFC,
            .libnfcConnectionString = HAVE_NFC ? LIBNFC_CONNECTION_STRING : NULL
        });
    
    // TCP stream manager.
    HAPPlatformTCPStreamManagerCreate(&platform.tcpStreamManager, &(const HAPPlatformTCPStreamManagerOptions) {
        /* Listen on all available network interfaces. */
        .port = 0 /* Listen on unused port number from the ephemeral port range. */,
        .maxConcurrentTCPStreams = kHAPIPSessionStorage_DefaultNumElements
    });

    // Service discovery.
    HAPPlatformServiceDiscoveryCreate(&platform.serviceDiscovery, &(const HAPPlatformServiceDiscoveryOptions) {
        0 /* Register services on all available network interfaces. */
    });
    
    if (HAVE_WAC) {
        // Wi-Fi manager.
        HAPPlatformWiFiManagerCreate(&platform.wiFiManager,
            &(const HAPPlatformWiFiManagerOptions) {
                .interfaceName = NULL
            });
        
        // Software access point manager. Depends on Wi-Fi manager.
        HAPPlatformSoftwareAccessPointCreate(&platform.softwareAccessPoint,
            &(const HAPPlatformSoftwareAccessPointOptions) {
                .wiFiManager = &platform.wiFiManager,
                .interfaceName = NULL
            });
    }
    
    if (HAVE_MFI_HW_AUTH) {
        // Apple Authentication Coprocessor provider.
        HAPPlatformMFiHWAuthCreate(&platform.mfiHWAuth);
    }
    
    // Software Token provider. Depends on key-value store.
    HAPPlatformMFiTokenAuthCreate(&platform.mfiTokenAuth,
        &(const HAPPlatformMFiTokenAuthOptions) {
            .keyValueStore = &platform.keyValueStore
        });
    
    // Run loop.
    HAPPlatformRunLoopCreate();
}

/**
 * Deinitialize global platform objects.
 */
static void DeinitializePlatform()
{
    if (HAVE_MFI_HW_AUTH) {
        // Apple Authentication Coprocessor provider.
        HAPPlatformMFiHWAuthRelease(&platform.mfiHWAuth);
    }
    
    // TCP stream manager.
    HAPPlatformTCPStreamManagerRelease(&platform.tcpStreamManager);

    // Accessory setup manager.
    HAPPlatformAccessorySetupRelease(&platform.accessorySetup);
    
    // Run loop.
    HAPPlatformRunLoopRelease();
}

/**
 * Restore platform specific factory settings.
 */
void RestorePlatformFactorySettings(void) {
    if (HAVE_WAC && HAPPlatformWiFiManagerIsConfigured(&platform.wiFiManager)) {
        HAPPlatformWiFiManagerRemoveConfiguration(&platform.wiFiManager);
    }
}

/**
 * HomeKit accessory server that hosts the Remote Control accessory.
 */
static HAPAccessoryServerRef accessoryServer;

int main()
{
    // Initialize global platform objects.
    InitializePlatform();
    
    // Prepare accessory server storage.
    
    // Note the: Buffers required can be higher/lower depending on the number of buttons.
    // The maximum buffer size depends on several factors:
    //  - The number of configured buttons
    //  - The number of supported targets
    static HAPIPSession ipSessions[HAPMax(kHAPIPSessionStorage_DefaultNumElements, kRemote_MaxSessions)];
    static uint8_t ipInboundBuffers[HAPArrayCount(ipSessions)][kHAPIPSession_DefaultInboundBufferSize
                                                                                            * kRemote_MaxTargets];
    static uint8_t ipOutboundBuffers[HAPArrayCount(ipSessions)][kHAPIPSession_DefaultOutboundBufferSize
                                                                                            * kRemote_MaxTargets];
    static HAPIPEventNotificationRef ipEventNotifications[HAPArrayCount(ipSessions)][kAttributeCount];
    for (size_t i = 0; i < HAPArrayCount(ipSessions); i++) {
        ipSessions[i].inboundBuffer.bytes = ipInboundBuffers[i];
        ipSessions[i].inboundBuffer.numBytes = sizeof ipInboundBuffers[i];
        ipSessions[i].outboundBuffer.bytes = ipOutboundBuffers[i];
        ipSessions[i].outboundBuffer.numBytes = sizeof ipOutboundBuffers[i];
        ipSessions[i].eventNotifications = ipEventNotifications[i];
        ipSessions[i].numEventNotifications = HAPArrayCount(ipEventNotifications[i]);
    }
    static HAPIPReadContextRef ipReadContexts[kAttributeCount];
    static HAPIPWriteContextRef ipWriteContexts[kAttributeCount];
    static uint8_t ipScratchBuffer[kHAPIPSession_DefaultScratchBufferSize * kRemote_MaxTargets];
    static HAPIPAccessoryServerStorage ipAccessoryServerStorage = {
        .sessions = ipSessions,
        .numSessions = HAPArrayCount(ipSessions),
        .readContexts = ipReadContexts,
        .numReadContexts = HAPArrayCount(ipReadContexts),
        .writeContexts = ipWriteContexts,
        .numWriteContexts = HAPArrayCount(ipWriteContexts),
        .scratchBuffer = {
            .bytes = ipScratchBuffer,
            .numBytes = sizeof ipScratchBuffer
        }
    };
    HAPAssert(HAPGetCompatibilityVersion() == HAP_COMPATIBILITY_VERSION);

    // Initialize accessory server.
    HAPAccessoryServerCreate(&accessoryServer,
        &(const HAPAccessoryServerOptions) {
            .maxPairings = kHAPPairingStorage_MinElements,
            .ip = {
                .available = true,
                .accessoryServerStorage = &ipAccessoryServerStorage,
                .wac = {
                    .available = HAVE_WAC ? true : false
                }
            }
        },
        &(const HAPPlatform) {
            .accessorySetup = &platform.accessorySetup,
            .keyValueStore = &platform.keyValueStore,
            .ip = {
                .tcpStreamManager = &platform.tcpStreamManager,
                .serviceDiscovery = &platform.serviceDiscovery,
                .wac = {
                    .softwareAccessPoint = HAVE_WAC ? &platform.softwareAccessPoint : NULL,
                    .wiFiManager = HAVE_WAC ? &platform.wiFiManager : NULL
                }
            },
            .authentication = {
                .mfiHWAuth = HAVE_MFI_HW_AUTH ? &platform.mfiHWAuth : NULL,
                .mfiTokenAuth = HAPPlatformMFiTokenAuthIsProvisioned(&platform.mfiTokenAuth) ?
                                &platform.mfiTokenAuth : NULL
            }
        },
        &(const HAPAccessoryServerCallbacks) {
            .handleUpdatedState = AccessoryServerHandleUpdatedState,
            .handleSessionAccept = RemoteHandleSessionAccept,
            .handleSessionInvalidate = RemoteHandleSessionInvalidate
        },
        /* context: */ NULL);
    
    // Create app object.
    AppCreate(&accessoryServer, &platform.keyValueStore);
    
    // Start accessory server for App.
    AppAccessoryServerStart();
    
    // Run main loop until explicitly stopped.
    HAPPlatformRunLoopRun();
    // Run loop stopped explicitly by calling function HAPPlatformRunLoopStop.
    
    // Cleanup.
    AppRelease();
    
    HAPAccessoryServerRelease(&accessoryServer);
    
    DeinitializePlatform();
    
    return 0;
}
