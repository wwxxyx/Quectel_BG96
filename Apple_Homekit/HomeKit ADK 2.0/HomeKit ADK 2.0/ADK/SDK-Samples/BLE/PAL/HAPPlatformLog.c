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

#include "SEGGER_RTT.h"

#include "HAPPlatform.h"

#define kRTT_LogChannel ((unsigned int) 0)

HAP_RESULT_USE_CHECK
HAPPlatformLogEnabledTypes HAPPlatformLogGetEnabledTypes(
    const HAPLogObject *log HAP_UNUSED)
{
    switch (HAP_LOG_LEVEL) {
        case 0: {
            return kHAPPlatformLogEnabledTypes_None;
        }
        case 1: {
            return kHAPPlatformLogEnabledTypes_Default;
        }
        case 2: {
            return kHAPPlatformLogEnabledTypes_Info;
        }
        case 3: {
            return kHAPPlatformLogEnabledTypes_Debug;
        }
        default: {
            HAPFatalError();
        }
    }
}

void HAPPlatformLogCapture(
    const HAPLogObject *log,
    HAPLogType type,
    const char *message,
    const void *_Nullable bufferBytes,
    size_t numBufferBytes)
HAP_DIAGNOSE_ERROR(!bufferBytes && numBufferBytes, "empty buffer cannot have a length")
{
    HAPError err;
    
    // Color.
    switch (type) {
        case kHAPLogType_Debug: {
            (void) SEGGER_RTT_printf(kRTT_LogChannel, "\x1B[0m");
        } break;
        case kHAPLogType_Info: {
            (void) SEGGER_RTT_printf(kRTT_LogChannel, "\x1B[32m");
        } break;
        case kHAPLogType_Default: {
            (void) SEGGER_RTT_printf(kRTT_LogChannel, "\x1B[35m");
        } break;
        case kHAPLogType_Error: {
            (void) SEGGER_RTT_printf(kRTT_LogChannel, "\x1B[31m");
        } break;
        case kHAPLogType_Fault: {
            (void) SEGGER_RTT_printf(kRTT_LogChannel, "\x1B[1m\x1B[31m");
        } break;
    }

    // Time.
    HAPTime now = HAPPlatformClockGetCurrent();
    char timeString[64];
    err = HAPStringWithFormat(timeString, sizeof timeString, "%8llu.%03llu",
        (unsigned long long) (now / HAPSecond), (unsigned long long) (now % HAPSecond));
    HAPAssert(!err);
    (void) SEGGER_RTT_printf(kRTT_LogChannel, "%s", timeString);
    (void) SEGGER_RTT_printf(kRTT_LogChannel, "\t");
    
    // Type.
    switch (type) {
        case kHAPLogType_Debug: {
            (void) SEGGER_RTT_printf(kRTT_LogChannel, "Debug");
        } break;
        case kHAPLogType_Info: {
            (void) SEGGER_RTT_printf(kRTT_LogChannel, "Info");
        } break;
        case kHAPLogType_Default: {
            (void) SEGGER_RTT_printf(kRTT_LogChannel, "Default");
        } break;
        case kHAPLogType_Error: {
            (void) SEGGER_RTT_printf(kRTT_LogChannel, "Error");
        } break;
        case kHAPLogType_Fault: {
            (void) SEGGER_RTT_printf(kRTT_LogChannel, "Fault");
        } break;
    }
    (void) SEGGER_RTT_printf(kRTT_LogChannel, "\t");

    // Subsystem / Category.
    if (log->subsystem) {
        (void) SEGGER_RTT_printf(kRTT_LogChannel, "[%s", log->subsystem);
        if (log->category) {
            (void) SEGGER_RTT_printf(kRTT_LogChannel, ":%s", log->category);
        }
        (void) SEGGER_RTT_printf(kRTT_LogChannel, "] ");
    }

    // Message.
    if ((__get_IPSR() & 0xFF) != 0) {
        (void) SEGGER_RTT_printf(kRTT_LogChannel, "<!!! CALLED FROM INTERRUPT !!!> ");
    }
    (void) SEGGER_RTT_printf(kRTT_LogChannel, "%s", message);
    (void) SEGGER_RTT_printf(kRTT_LogChannel, "\n");

    // Buffer.
    if (bufferBytes) {
        size_t i, o;
        const uint8_t *b = bufferBytes;
        size_t length = numBufferBytes;
        if (length == 0) {
            (void) SEGGER_RTT_printf(kRTT_LogChannel, "\n");
        } else {
            i = 0;
            do {
                (void) SEGGER_RTT_printf(kRTT_LogChannel, "    %04x ", i);
                for (o = 0; o != 8 * 4; o++) {
                    if (o % 4 == 0) {
                        (void) SEGGER_RTT_printf(kRTT_LogChannel, " ");
                    }
                    if ((o <= length) && (i < length - o)) {
                        (void) SEGGER_RTT_printf(kRTT_LogChannel, "%02x", b[i + o] & 0xff);
                    } else {
                        (void) SEGGER_RTT_printf(kRTT_LogChannel, "  ");
                    }
                };
                (void) SEGGER_RTT_printf(kRTT_LogChannel, "    ");
                for (o = 0; o != 8 * 4; o++) {
                    if (i != length) {
                        if ((32 <= b[i]) && (b[i] < 127)) {
                            (void) SEGGER_RTT_printf(kRTT_LogChannel, "%c", b[i]);
                        } else {
                            (void) SEGGER_RTT_printf(kRTT_LogChannel, ".");
                        }
                        i++;
                    }
                }
                (void) SEGGER_RTT_printf(kRTT_LogChannel, "\n");
            } while (i != length);
        }
    }

    // Reset color.
    (void) SEGGER_RTT_printf(kRTT_LogChannel, "\x1B[0m");
}
