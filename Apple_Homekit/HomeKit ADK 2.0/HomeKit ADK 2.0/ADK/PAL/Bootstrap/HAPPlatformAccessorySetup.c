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

#include "HAPPlatformAccessorySetup+Init.h"

static const HAPLogObject logObject = {
    .subsystem = kHAPPlatform_LogSubsystem,
    .category = "AccessorySetup"
};

// This is a bootstrap implementation - must not be used for production accessories!

static const HAPSetupInfo kHAPPlatformAccessorySetup_SetupInfo = {
    // Setup code: 111-22-333
    .salt = {
        0x93, 0x15, 0x1A, 0x47, 0x57, 0x55, 0x3C, 0x21, 0x0B, 0x55, 0x89, 0xB8, 0xC3, 0x99, 0xA0, 0xF3
    },
    .verifier = {
        0x9E, 0x9C, 0xC3, 0x73, 0x9B, 0x04, 0x83, 0xC8, 0x13, 0x7C, 0x5B, 0x5F, 0xAC, 0xC5, 0x63, 0xDF,
        0xF4, 0xF1, 0x0F, 0x39, 0x06, 0x4A, 0x20, 0x2D, 0x53, 0x2A, 0x09, 0x20, 0x3A, 0xA6, 0xBA, 0xE3,
        0x1E, 0x42, 0x4E, 0x58, 0x4E, 0xBB, 0x44, 0x5F, 0x7F, 0xDF, 0xCC, 0x11, 0xD0, 0xF7, 0x8B, 0x35,
        0xE1, 0x16, 0xA9, 0x79, 0x30, 0xBC, 0x37, 0x19, 0x77, 0x36, 0xB1, 0xEC, 0xD4, 0x12, 0x4C, 0xE4,
        0x5D, 0xE3, 0x7E, 0x46, 0xA0, 0x2D, 0x10, 0x07, 0xAB, 0x48, 0x40, 0x36, 0xD5, 0x3F, 0x7F, 0xBE,
        0xA5, 0xAE, 0xD0, 0x25, 0x6B, 0xC4, 0x9E, 0xC8, 0x5F, 0xC9, 0x4E, 0x47, 0x0D, 0xBA, 0xD3, 0x63,
        0x44, 0x20, 0x01, 0x69, 0x97, 0xDD, 0x20, 0x54, 0x7C, 0x59, 0x78, 0x3D, 0x5C, 0x6D, 0xC7, 0x1F,
        0xE6, 0xFD, 0xA0, 0x8E, 0x9B, 0x36, 0x45, 0x1F, 0xC1, 0x4B, 0xB5, 0x26, 0xE1, 0x8E, 0xEB, 0x4C,
        0x05, 0x58, 0xD7, 0xC8, 0x80, 0xA1, 0x43, 0x7F, 0x5F, 0xDB, 0x75, 0x1B, 0x19, 0x57, 0x25, 0xAC,
        0x5D, 0xF5, 0x8D, 0xF6, 0x7B, 0xAA, 0xB7, 0x7D, 0xE0, 0x36, 0xEF, 0xEA, 0xF3, 0x57, 0xAC, 0xFE,
        0x12, 0x87, 0xF9, 0x31, 0x4C, 0xF7, 0x44, 0xBD, 0xB6, 0x26, 0x6C, 0xB4, 0x0D, 0x7C, 0x52, 0x4F,
        0x85, 0x56, 0x91, 0x5D, 0x13, 0xD8, 0xDA, 0x8C, 0x45, 0x3E, 0x73, 0xF2, 0xF9, 0x20, 0x39, 0x24,
        0x8B, 0xFB, 0xEE, 0xFD, 0x77, 0x54, 0x8D, 0x37, 0x22, 0xE8, 0x55, 0xC3, 0xD2, 0xF8, 0xB8, 0x23,
        0xB0, 0xE2, 0x9E, 0x43, 0xAE, 0xB4, 0x37, 0xFA, 0xA7, 0x03, 0xF1, 0x82, 0x68, 0x4C, 0xD4, 0x86,
        0xC6, 0x3E, 0xDE, 0x70, 0x11, 0x03, 0x77, 0x46, 0x59, 0x14, 0x97, 0xC6, 0xAE, 0x52, 0x6F, 0x03,
        0x77, 0x36, 0x40, 0xBC, 0xDE, 0xCD, 0x3D, 0xE0, 0x4F, 0x69, 0x18, 0x0D, 0xCA, 0x85, 0x7E, 0x07,
        0x30, 0xF4, 0xA1, 0xCE, 0x05, 0xB5, 0x4B, 0xE1, 0x1D, 0x43, 0xDF, 0xDB, 0x11, 0x43, 0xDE, 0x21,
        0xAC, 0x8F, 0x03, 0x9E, 0x6E, 0x9F, 0xA8, 0xE5, 0x02, 0x06, 0x1C, 0x63, 0x34, 0x22, 0x1D, 0x39,
        0xE3, 0x3D, 0x12, 0x2E, 0xA2, 0xF3, 0xFC, 0xB5, 0xB4, 0x16, 0x9E, 0x0E, 0x7C, 0x52, 0xC8, 0x7D,
        0x50, 0x3D, 0xDB, 0xF5, 0x83, 0x46, 0x18, 0x92, 0x7F, 0x4D, 0x38, 0xAD, 0x0A, 0x2A, 0xBC, 0x2A,
        0x50, 0x4B, 0xDF, 0x5D, 0xFA, 0x93, 0x41, 0x78, 0xD6, 0x45, 0x54, 0xDB, 0x44, 0x81, 0xF7, 0x5A,
        0x0A, 0xDD, 0x18, 0x4F, 0x27, 0xD7, 0xDD, 0x5E, 0xB7, 0x3E, 0x99, 0xE6, 0xE1, 0x69, 0x35, 0x74,
        0xD6, 0x98, 0x58, 0xB2, 0x13, 0x6F, 0xB7, 0x82, 0x72, 0xBC, 0xA6, 0x8B, 0xA3, 0x36, 0x2A, 0xCE,
        0x65, 0x65, 0x51, 0x08, 0x8A, 0x3D, 0x04, 0x93, 0x8F, 0x01, 0x8A, 0xAB, 0x4B, 0xFC, 0x06, 0xF9
    }
};

// Setup payload (without WAC): X-HM://00527813XACME
// Setup payload    (with WAC): X-HM://005JYI1LPACME
static const HAPSetupID *kHAPPlatformAccessorySetup_SetupID = (const HAPSetupID *) "ACME";

HAP_RESULT_USE_CHECK
HAPPlatformAccessorySetupCapabilities HAPPlatformAccessorySetupGetCapabilities(
    HAPPlatformAccessorySetupRef accessorySetup)
{
    HAPPrecondition(accessorySetup);

    return (HAPPlatformAccessorySetupCapabilities) {
        .supportsDisplay = false,
        .supportsProgrammableNFC = false
    };
}

void HAPPlatformAccessorySetupLoadSetupInfo(
    HAPPlatformAccessorySetupRef accessorySetup,
    HAPSetupInfo *setupInfo)
{
    HAPPrecondition(accessorySetup);
    HAPPrecondition(setupInfo);

    HAPLog(&logObject, "Using constant setup code implementation - must not be used for production accessories!");
    *setupInfo = kHAPPlatformAccessorySetup_SetupInfo;
}

void HAPPlatformAccessorySetupLoadSetupCode(
    HAPPlatformAccessorySetupRef accessorySetup,
    HAPSetupCode *setupCode)
{
    HAPPrecondition(accessorySetup);
    HAPPrecondition(setupCode);

    HAPLogError(&logObject, "[NYI] %s.", __func__);
    HAPFatalError();
}

void HAPPlatformAccessorySetupLoadSetupID(
    HAPPlatformAccessorySetupRef accessorySetup,
    bool *valid,
    HAPSetupID *setupID)
{
    HAPPrecondition(accessorySetup);
    HAPPrecondition(valid);
    HAPPrecondition(setupID);

    HAPLog(&logObject, "Using constant setup ID implementation - must not be used for production accessories!");
    *valid = true;
    *setupID = *kHAPPlatformAccessorySetup_SetupID;
}

void HAPPlatformAccessorySetupUpdateSetupPayload(
    HAPPlatformAccessorySetupRef accessorySetup,
    const HAPSetupPayload *_Nullable setupPayload,
    const HAPSetupCode *_Nullable setupCode)
{
    HAPPrecondition(accessorySetup);
    (void) setupPayload;
    (void) setupCode;

    HAPLogError(&logObject, "[NYI] %s.", __func__);
    HAPFatalError();
}
