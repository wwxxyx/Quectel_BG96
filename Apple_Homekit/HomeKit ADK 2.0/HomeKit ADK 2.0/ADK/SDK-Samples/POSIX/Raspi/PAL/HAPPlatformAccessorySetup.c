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

#include "HAPPlatform+Init.h"
#include "HAPPlatformAccessorySetup+Init.h"
#include "HAPPlatformFileManager.h"
#include "HAPPlatformKeyValueStore+SDKDomains.h"

#if HAVE_NFC
    #include <errno.h>
    #include <unistd.h>
    #include <nfc/nfc-emulation.h>
#endif

static const HAPLogObject logObject = {
    .subsystem = kHAPPlatform_LogSubsystem,
    .category = "AccessorySetup"
};

// Programmable NFC is supported through the external library libnfc (https://libnfc.org).

#if HAVE_NFC
HAP_ENUM_BEGIN(uint8_t, NFCCommand) {
    /**
     * READ.
     */
    kNFCCommand_Read = 0x30,

    /**
     * HALT.
     */
    kNFCCommand_Halt = 0x50,
} HAP_ENUM_END(uint8_t, NFCCommand);

typedef struct {
    /**
     * NFC payload.
     */
    const void *payloadBytes;

    /**
     * Length of NFC payload.
     */
    size_t numPayloadBytes;
} NFCUserData;

static int NFCIOCallback(
    struct nfc_emulator *emulator,
    const uint8_t *data_in,
    const size_t data_in_len,
    uint8_t *data_out,
    const size_t data_out_len)
{
    HAPPrecondition(emulator->user_data);
    const NFCUserData *userData = emulator->user_data;
    const uint8_t *payloadBytes = userData->payloadBytes;
    size_t numPayloadBytes = userData->numPayloadBytes;

    HAPLogBufferDebug(&logObject, data_in, data_in_len, "NFC In.");

    if (data_in_len < 1) {
        HAPLogError(&logObject, "NFC IO callback invoked without input data.");
        return -EINVAL;
    }

    uint8_t command = data_in[0];
    switch (command) {
        case kNFCCommand_Read: {
            if (data_out_len < 16) {
                HAPLogError(&logObject, "NFC IO callback: READ invoked with too small output buffer.");
                return -ENOSPC;
            }
            uint8_t offset = data_in[1];
            uint8_t numBytes = 16;
            if (offset > (numPayloadBytes - numBytes) / 4) {
                HAPLogError(&logObject, "NFC IO callback: READ invoked with out-of-range offset %u.", offset);
                return -EINVAL;
            }
            HAPAssert(numPayloadBytes >= numBytes);
            HAPRawBufferCopyBytes(data_out, &payloadBytes[offset * 4], numBytes);
            HAPLogBufferDebug(&logObject, data_out, numBytes, "NFC Out.");
            return numBytes;
        } break;
        case kNFCCommand_Halt: {
            HAPLogDebug(&logObject, "NFC IO callback: HALT sent.");
            return -ECONNABORTED;
        } break;
        default: {
            HAPLog(&logObject, "NFC IO callback: Unsupported command (0x%02x).", command);
            return -ENOTSUP;
        } break;
    }
    HAPFatalError();
}

/**
 * Entry point for the process that manages NFC.
 */
static void *NFCMain(
    void *context)
{
    HAPPrecondition(context);
    HAPPlatformAccessorySetupRef accessorySetup = context;
    HAPAssert(accessorySetup->nfc.isActive);

    while (__sync_lock_test_and_set(&accessorySetup->nfc.nfcLock, 1));
    HAPLogDebug(&logObject, "Started NFC thread...");

    // Initialize NFC library.
    HAPAssert(!accessorySetup->nfc.nfcContext);
    nfc_init(&accessorySetup->nfc.nfcContext);
    if (!accessorySetup->nfc.nfcContext) {
        HAPLogError(&logObject, "Unable to init libnfc (malloc). Continuing without NFC.");
        goto cleanup;
    }
    const char *_Nullable connectionString = accessorySetup->libnfcConnectionString;
    HAPLog(&logObject, "libnfc version: %s.", nfc_version());
    HAPLog(&logObject, "libnfc connection string: %s.", connectionString ? connectionString : "(default)");

    // Connect to NFC hardware.
    do {
        HAPLogDebug(&logObject, "Opening NFC device.");
        HAPAssert(!accessorySetup->nfc.nfcDevice);
        accessorySetup->nfc.nfcDevice = nfc_open(accessorySetup->nfc.nfcContext, connectionString);
        if (!accessorySetup->nfc.nfcDevice) {
            HAPLogInfo(&logObject, "Unable to open NFC device. Retrying...");
            __sync_synchronize();
            __sync_lock_release(&accessorySetup->nfc.nfcLock);
            usleep(500 * 1000);
            while (__sync_lock_test_and_set(&accessorySetup->nfc.nfcLock, 1));
            if (accessorySetup->nfc.isAborted) {
                goto cleanup;
            }
        }
    } while (!accessorySetup->nfc.nfcDevice);
    HAPLogDebug(&logObject, "NFC device: %s opened.", nfc_device_get_name(accessorySetup->nfc.nfcDevice));

    // Prepare NDEF payload.
    // Based on http://www.libnfc.org/api/nfc-emulate-forum-tag2_8c_source.html
    HAPAssert(HAPStringGetNumBytes(accessorySetup->nfc.setupPayload.stringValue) == 20);
    const uint8_t *c = (const uint8_t *) accessorySetup->nfc.setupPayload.stringValue;
    uint8_t nfcPayload[] = {
        // READ requests always return 16 bytes.
        // To reflect this, 16-byte chunks are grouped together and separated by an empty line below.

        // Block 0.
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        // Block 2 (Static lock bytes: CC area and data area are read-only locked).
        0x00, 0x00, 0xFF, 0xFF,
        // Block 3 (CC - NFC-Forum Tag Type 2 version 1.0,
        // Data area (from block 4 to the end) is 5 * 8 bytes, Read-only mode).
        0xE1, // Magic number to indicate that NFC Forum defined data is stored in the data area.
        0x10, // Version number. Most significant nibble = major, least significant nibble = minor version number.
        0x04, // Memory size of the data area. Value multiplied by 8 is equal to the data area size.
        0x0F, // Read and write access capability of the data area and CC area.

        // Block 4 (NDEF).
        0x03, 5+20, 0xd1, 0x01, 0x15, 0x55, 0x00, c[0],
        c[1], c[2], c[3], c[4], c[5], c[6], c[7], c[8],

        c[ 9], c[10], c[11], c[12], c[13], c[14], c[15], c[16],
        c[17], c[18], c[19], 0x00 , 0x00 , 0x00 , 0x00 , 0x00
    };
    NFCUserData nfcUserData = {
        .payloadBytes = nfcPayload,
        .numPayloadBytes = sizeof nfcPayload
    };

    // Emulate NDEF tag.
    // Based on http://www.libnfc.org/api/nfc-emulate-forum-tag2_8c_source.html
    // See http://nfc-tools.org/index.php?title=ISO14443A
    nfc_target nfcTarget = {
        // Modulation.
        .nm = {
            // Modulation type.
            .nmt = NMT_ISO14443A,

            // Baud rate.
            .nbr = NBR_UNDEFINED,
        },

        // ISO14443A Target info.
        // The ATQA, SAK and ATS values can be used to identify the manufacturer, tag type and application.
        .nti.nai = {
            // Answer To Request acc. to ISO/IEC 14443-4.
            .abtAtqa = { 0x00, 0x04 },

            // Select Acknowledge, Type A.
            .btSak = 0x00,

            // Unique Identifier, Type A.
            .szUidLen = 4,
            .abtUid = { 0x08, 0x00, 0xb0, 0x0b },

            // Answer to Select acc. to ISO/IEC 14443-4.
            .szAtsLen = 0,
            .abtAts = { 0x00 }
        }
    };
    struct nfc_emulation_state_machine nfcStateMachine = {
        .io = NFCIOCallback,
        .data = nfcPayload
    };
    struct nfc_emulator nfcEmulator = {
        .target = &nfcTarget,
        .state_machine = &nfcStateMachine,
        .user_data = &nfcUserData,
    };
    HAPLogInfo(&logObject, "NFC enabled.");
    while (!accessorySetup->nfc.isAborted) {
        accessorySetup->nfc.isEmulating = true;
        __sync_synchronize();
        __sync_lock_release(&accessorySetup->nfc.nfcLock);
        HAPLogDebug(&logObject, "Starting NFC card emulation.");
        int e = nfc_emulate_target(accessorySetup->nfc.nfcDevice, &nfcEmulator, /* timeout: */ 0);
        while (__sync_lock_test_and_set(&accessorySetup->nfc.nfcLock, 1));
        accessorySetup->nfc.isEmulating = false;
        if (e) {
            if (e == -ECONNABORTED) {
                HAPLogDebug(&logObject, "NFC transaction complete.");
            } else {
                HAPLogInfo(&logObject, "`nfc_emulate_target` failed: %d.", e);
            }
        }
    }
    HAPLogInfo(&logObject, "NFC disabled.");
    goto cleanup;
    
    HAPFatalError(); cleanup: {
        HAPLogDebug(&logObject, "Releasing NFC resources.");
    
        if (accessorySetup->nfc.nfcDevice)  {
            nfc_close(accessorySetup->nfc.nfcDevice);
            accessorySetup->nfc.nfcDevice = NULL;
        }
    
        if (accessorySetup->nfc.nfcContext) {
            nfc_exit(accessorySetup->nfc.nfcContext);
            accessorySetup->nfc.nfcContext = NULL;
        }
    
        __sync_synchronize();
        __sync_lock_release(&accessorySetup->nfc.nfcLock);
        return NULL;
    }
}
#endif

/**
 * Stops programmable NFC advertisement if active.
 *
 * @param      accessorySetup       Accessory setup manager.
 */
static void NFCStop(
    HAPPlatformAccessorySetupRef accessorySetup)
{
    HAPPrecondition(accessorySetup);
    HAPPrecondition(accessorySetup->useProgrammableNFC);

    #if HAVE_NFC
        // Stop NFC.
        if (accessorySetup->nfc.isActive) {
            HAPLogDebug(&logObject, "Signalling NFC thread to stop.");
            while (__sync_lock_test_and_set(&accessorySetup->nfc.nfcLock, 1));
            accessorySetup->nfc.isAborted = true;
            while (accessorySetup->nfc.isEmulating) {
                HAPAssert(accessorySetup->nfc.nfcDevice);
                HAPLogDebug(&logObject, "Aborting NFC card emulation.");
                nfc_abort_command(accessorySetup->nfc.nfcDevice);
                __sync_synchronize();
                __sync_lock_release(&accessorySetup->nfc.nfcLock);
                usleep(100 * 1000);
                while (__sync_lock_test_and_set(&accessorySetup->nfc.nfcLock, 1));
            }
            __sync_synchronize();
            __sync_lock_release(&accessorySetup->nfc.nfcLock);

            HAPLogDebug(&logObject, "Waiting for NFC thread to stop...");
            int e = pthread_join(accessorySetup->nfc.thread, /* value_ptr: */ NULL);
            if (e) {
                HAPLogError(&logObject, "`pthread_join` failed to join NFC thread (%d).", e);
                HAPFatalError();
            }
            HAPLogDebug(&logObject, "Stopped NFC thread.");
            HAPRawBufferZero(&accessorySetup->nfc, sizeof accessorySetup->nfc);
        }
        HAPAssert(!accessorySetup->nfc.isActive);
    #endif
}

/**
 * Sets the NFC NDEF payload.
 *
 * @param      accessorySetup       Accessory setup manager.
 * @param      payloadString        NFC payload string
 */
static void NFCStart(
    HAPPlatformAccessorySetupRef accessorySetup,
    char const *payloadString)
{
    HAPPrecondition(accessorySetup);
    HAPPrecondition(accessorySetup->useProgrammableNFC);
    HAPPrecondition(payloadString);

    #if HAVE_NFC
        // Stop NFC if active.
        if (accessorySetup->nfc.isActive) {
            NFCStop(accessorySetup);
        }

        // Copy arguments.
        size_t numPayloadStringBytes = HAPStringGetNumBytes(payloadString);
        HAPAssert(numPayloadStringBytes < sizeof accessorySetup->nfc.setupPayload.stringValue);
        HAPRawBufferCopyBytes(accessorySetup->nfc.setupPayload.stringValue, payloadString, numPayloadStringBytes);

        // Start new NFC thread.
        HAPLogDebug(&logObject, "Starting NFC thread.");
        accessorySetup->nfc.isActive = true;
        int e = pthread_create(&accessorySetup->nfc.thread, /* attr: */ NULL, NFCMain, accessorySetup);
        if (e) {
            HAPLogError(&logObject, "`pthread_create` failed to create NFC thread (%d): Continuing without NFC.", e);
            HAPRawBufferZero(&accessorySetup->nfc, sizeof accessorySetup->nfc);
            return;
        }
    #endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void HAPPlatformAccessorySetupCreate(
    HAPPlatformAccessorySetupRef accessorySetup,
    const HAPPlatformAccessorySetupOptions *options)

{
    HAPPrecondition(accessorySetup);
    HAPPrecondition(options);
    HAPPrecondition(options->keyValueStore);

    if (!options->useProgrammableNFC) {
        HAPPrecondition(!options->libnfcConnectionString);
    }
    
    HAPLog(&logObject, "Storage configuration: accessorySetup = %lu",
        (unsigned long) sizeof *accessorySetup);

    HAPRawBufferZero(accessorySetup, sizeof *accessorySetup);
    
    accessorySetup->keyValueStore = options->keyValueStore;
    accessorySetup->useDisplay = options->useDisplay;
    accessorySetup->useProgrammableNFC = options->useProgrammableNFC;
    accessorySetup->libnfcConnectionString = HAVE_NFC ? options->libnfcConnectionString : NULL;
    
    HAPLog(&logObject, "Using display: %s / Programmable NFC: %s",
        accessorySetup->useDisplay ? "true" : "false",
        accessorySetup->useProgrammableNFC ? "true" : "false");
}

void HAPPlatformAccessorySetupRelease(
    HAPPlatformAccessorySetupRef accessorySetup)
{
    HAPPrecondition(accessorySetup);

    if (accessorySetup->useProgrammableNFC) {
        NFCStop(accessorySetup);
    }
}

HAP_RESULT_USE_CHECK
HAPPlatformAccessorySetupCapabilities HAPPlatformAccessorySetupGetCapabilities(
    HAPPlatformAccessorySetupRef accessorySetup)
{
    HAPPrecondition(accessorySetup);

    return (HAPPlatformAccessorySetupCapabilities) {
        .supportsDisplay = accessorySetup->useDisplay,
        .supportsProgrammableNFC = accessorySetup->useProgrammableNFC
    };
}

void HAPPlatformAccessorySetupLoadSetupInfo(
    HAPPlatformAccessorySetupRef accessorySetup,
    HAPSetupInfo *setupInfo)
{
    HAPPrecondition(accessorySetup);
    HAPPrecondition(setupInfo);
    
    HAPError err;
    
    bool found;
    size_t numBytes;
    err = HAPPlatformKeyValueStoreGet(accessorySetup->keyValueStore,
        kSDKKeyValueStoreDomain_Provisioning, kSDKKeyValueStoreKey_Provisioning_SetupInfo,
        setupInfo, sizeof *setupInfo, &numBytes, &found);
    if (err) {
        HAPAssert(err == kHAPError_Unknown);
        HAPFatalError();
    }
    if (!found) {
        HAPLogError(&logObject, "No setup code found in key-value store.");
        HAPFatalError();
    }
    if (numBytes != sizeof *setupInfo) {
        HAPLogError(&logObject, "Invalid setup code size %lu.", (unsigned long) numBytes);
        HAPFatalError();
    }
}

void HAPPlatformAccessorySetupLoadSetupCode(
    HAPPlatformAccessorySetupRef accessorySetup,
    HAPSetupCode *setupCode)
{
    HAPPrecondition(accessorySetup);
    HAPPrecondition(setupCode);
    
    HAPError err;
    
    bool found;
    size_t numBytes;
    err = HAPPlatformKeyValueStoreGet(accessorySetup->keyValueStore,
        kSDKKeyValueStoreDomain_Provisioning, kSDKKeyValueStoreKey_Provisioning_SetupCode,
        setupCode, sizeof *setupCode, &numBytes, &found);
    if (err) {
        HAPAssert(err == kHAPError_Unknown);
        HAPFatalError();
    }
    if (!found) {
        HAPLogError(&logObject, "No NFC setup code found in key-value store.");
        HAPFatalError();
    }
    if (numBytes != sizeof *setupCode) {
        HAPLogError(&logObject, "Invalid NFC setup code size %lu.", (unsigned long) numBytes);
        HAPFatalError();
    }
}

void HAPPlatformAccessorySetupLoadSetupID(
    HAPPlatformAccessorySetupRef accessorySetup,
    bool *valid,
    HAPSetupID *setupID)
{
    HAPPrecondition(accessorySetup);
    HAPPrecondition(valid);
    HAPPrecondition(setupID);
    
    HAPError err;
    
    size_t numBytes;
    err = HAPPlatformKeyValueStoreGet(accessorySetup->keyValueStore,
        kSDKKeyValueStoreDomain_Provisioning, kSDKKeyValueStoreKey_Provisioning_SetupID,
        setupID, sizeof *setupID, &numBytes, valid);
    if (err) {
        HAPAssert(err == kHAPError_Unknown);
        HAPFatalError();
    }
    if (*valid && numBytes != sizeof *setupID) {
        HAPLogError(&logObject, "Invalid setup ID size %lu.", (unsigned long) numBytes);
        HAPFatalError();
    }
}

void HAPPlatformAccessorySetupUpdateSetupPayload(
    HAPPlatformAccessorySetupRef accessorySetup,
    const HAPSetupPayload *_Nullable setupPayload,
    const HAPSetupCode *_Nullable setupCode)
{
    HAPPrecondition(accessorySetup);

    if (setupCode) {
        HAPLogInfo(&logObject, "##### Setup code: %s", setupCode->stringValue);
    } else {
        HAPLogInfo(&logObject, "##### Setup code invalidated.");
    }
    if (setupPayload) {
        HAPLogInfo(&logObject, "##### Setup payload: %s", setupPayload->stringValue);
        if (accessorySetup->useProgrammableNFC) {
            NFCStart(accessorySetup, setupPayload->stringValue);
        }
    }
}
