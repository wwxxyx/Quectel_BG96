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

#include "nfc_t2t_lib.h"
#include "nfc_uri_msg.h"

#include "HAPPlatformAccessorySetup+Init.h"
#include "HAPPlatformKeyValueStore+SDKDomains.h"

static const HAPLogObject logObject = {
    .subsystem = kHAPPlatform_LogSubsystem,
    .category = "AccessorySetup"
};

/**
 * NFC Event handler.
 */
static void NFCEventHandler(
    void *p_context HAP_UNUSED,
    nfc_t2t_event_t event,
    const uint8_t *p_data HAP_UNUSED,
    size_t data_length HAP_UNUSED)
{
    switch (event)
    {
        case NFC_T2T_EVENT_FIELD_ON:
        case NFC_T2T_EVENT_FIELD_OFF:
        default:
            // No implementation required.
            break;
    }
}

void HAPPlatformAccessorySetupCreate(
    HAPPlatformAccessorySetupRef accessorySetup,
    const HAPPlatformAccessorySetupOptions *options)

{
    HAPPrecondition(accessorySetup);
    HAPPrecondition(options);
    HAPPrecondition(options->keyValueStore);
    
    HAPRawBufferZero(accessorySetup, sizeof *accessorySetup);
    
    accessorySetup->keyValueStore = options->keyValueStore;
    accessorySetup->useDisplay = options->useDisplay;
    accessorySetup->useProgrammableNFC = options->useProgrammableNFC;
    
    // Set up programmable NFC.
    if (accessorySetup->useProgrammableNFC) {
        // Set up NFC Type 2 Tag library.
        ret_code_t e = nfc_t2t_setup(NFCEventHandler, NULL);
        if (e) {
            HAPLogError(&logObject, "nfc_t2t_setup failed: 0x%04x.", (unsigned int) e);
            HAPFatalError();
        }
    }
    
    HAPLog(&logObject, "Using display: %s / Programmable NFC: %s",
        accessorySetup->useDisplay ? "true" : "false",
        accessorySetup->useProgrammableNFC ? "true" : "false");
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

/**
 * Sets the NFC NDEF payload.
 *
 * @param      payloadString        NFC payload string.
 */
static void SetNFCPayload(
    char const *payloadString)
{
    HAPPrecondition(payloadString);

    static uint8_t ndefMessageBytes[100];
    static bool emulationRunning = false;

    uint32_t numNDEFMessageBytes = sizeof ndefMessageBytes;

    // Stop Type 2 Tag emulation to encode new payload.
    if (emulationRunning) {
        ret_code_t e = nfc_t2t_emulation_stop();
        if (e) {
            HAPLogError(&logObject, "nfc_t2t_emulation_stop failed: %lu.", (unsigned long) e);
            HAPFatalError();
        }
        emulationRunning = false;
    }

    // Encode URI message into buffer.
    size_t numPayloadBytes = HAPStringGetNumBytes(payloadString);
    HAPAssert(numPayloadBytes <= UINT8_MAX);
    ret_code_t e = nfc_uri_msg_encode(
        NFC_URI_NONE,
        (const uint8_t *) payloadString,
        (uint8_t) numPayloadBytes,
        ndefMessageBytes,
        &numNDEFMessageBytes);
    if (e) {
        HAPLogError(&logObject, "nfc_uri_msg_encode failed: %lu.", (unsigned long) e);
        HAPFatalError();
    }

    // Set created message as the NFC payload.
    e = nfc_t2t_payload_set(ndefMessageBytes, numNDEFMessageBytes);
    if (e) {
        HAPLogError(&logObject, "nfc_t2t_payload_set failed: %lu.", (unsigned long) e);
        HAPFatalError();
    }

    // Start Type 2 Tag emulation.
    e = nfc_t2t_emulation_start();
    if (e) {
        HAPLogError(&logObject, "nfc_t2t_emulation_start failed: %lu.", (unsigned long) e);
        HAPFatalError();
    }

    emulationRunning = true;
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
            SetNFCPayload(setupPayload->stringValue);
        }
    }
}
