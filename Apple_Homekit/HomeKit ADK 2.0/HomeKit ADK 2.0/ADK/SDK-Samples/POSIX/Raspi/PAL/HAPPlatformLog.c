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

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
    #include <Windows.h>
#else
    #include <sys/time.h>
#endif

#ifndef HAVE_CAMERA
    #define HAVE_CAMERA 0
#endif

#include "HAP.h"
#include "HAPPlatformLog+Init.h"

#if HAVE_CAMERA
    #include "HAPPlatformCamera+Init.h"
    #if HAP_LOG_LEVEL
        #include "HAPPlatformLog+Camera.h"
    #endif
#endif

#ifdef __linux__
    #if !defined(_POSIX_C_SOURCE) || _POSIX_C_SOURCE < 200112L
        #error "This file needs the XSI-compliant version of 'strerror_r'. Set _POSIX_C_SOURCE >= 200112L."
    #endif
    #if defined(_GNU_SOURCE) && _GNU_SOURCE
        #error "This file needs the XSI-compliant version of 'strerror_r'. Do not set _GNU_SOURCE."
    #endif
#endif

static const HAPLogObject logObject = {
    .subsystem = kHAPPlatform_LogSubsystem,
    .category = "Log"
};

void HAPPlatformLogPOSIXError(
    HAPLogType type,
    const char *message,
    int errorNumber,
    const char *function,
    const char *file,
    int line)
{
    HAPPrecondition(message);
    HAPPrecondition(function);
    HAPPrecondition(file);

    HAPError err;

    // Get error message.
    char errorString[256];
    int e = strerror_r(errorNumber, errorString, sizeof errorString);
    if (e == EINVAL) {
        err = HAPStringWithFormat(errorString, sizeof errorString, "Unknown error %d", errorNumber);
        HAPAssert(!err);
    } else if (e) {
        HAPAssert(e == ERANGE);
        HAPLog(&logObject, "strerror_r error: ERANGE.");
        return;
    }

    // Perform logging.
    HAPLogWithType(&logObject, type, "%s:%d:%s - %s @ %s:%d", message, errorNumber, errorString, function, file, line);
}

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
    HAPPrecondition(log);
    HAPPrecondition(message);
    HAPPrecondition(!numBufferBytes || bufferBytes);

    static volatile int captureLock = 0;
    while (__sync_lock_test_and_set(&captureLock, 1));

    // Color.
    switch (type) {
        case kHAPLogType_Debug: {
            fprintf(stderr, "\x1B[0m");
        } break;
        case kHAPLogType_Info: {
            fprintf(stderr, "\x1B[32m");
        } break;
        case kHAPLogType_Default: {
            fprintf(stderr, "\x1B[35m");
        } break;
        case kHAPLogType_Error: {
            fprintf(stderr, "\x1B[31m");
        } break;
        case kHAPLogType_Fault: {
            fprintf(stderr, "\x1B[1m\x1B[31m");
        } break;
    }

    // Format log message.
    bool logHandled = false;

    // Handle camera logs.
    #if HAVE_CAMERA && HAP_LOG_LEVEL
        static const HAPPlatformLogCameraLogOptions cameraLogOptions = {
            .logVideoIDR = true,
            .logVideoRTP = false,
            .logAudioRTP = false,
            .logRTCP = true,
            .logStatistics = true
        };
        if (log == &kHAPRTPController_PacketLog) {
            HAPPlatformLogCameraRTPPacket(HAPNonnullVoid(bufferBytes), numBufferBytes, message, cameraLogOptions);
            logHandled = true;
        } else if (log == &kHAPPlatformCamera_VideoLog) {
            // HAPPlatformLogCameraH264Payload(bufferBytes, numBufferBytes, message, cameraLogOptions);
            logHandled = true;
        } else if (log == &kHAPPlatformCamera_AudioLog) {
            // HAPPlatformLogCameraPayload(bufferBytes, numBufferBytes, message, cameraLogOptions);
            logHandled = true;
        } else if (log == &kHAPPlatformCamera_SpeakerLog) {
            // HAPPlatformLogCameraPayload(bufferBytes, numBufferBytes, message, cameraLogOptions);
            logHandled = true;
        }
    #endif

    // Perform regular logging.
    if (!logHandled) {
        // Time.
        #ifdef _WIN32
        SYSTEMTIME now;
        GetSystemTime(&now);
        (void) fprintf(stderr, "%04d-%02d-%02d'T'%02d:%02d:%02d'Z'",
            now.wYear, now.wMonth, now.wDay,
            now.wHour, now.wMinute, now.wSecond);
        #else
        struct timeval now;
        int err = gettimeofday(&now, NULL);
        if (!err) {
            struct tm g;
            struct tm *gmt = gmtime_r(&now.tv_sec, &g);
            if (gmt) {
                (void) fprintf(stderr, "%04d-%02d-%02d'T'%02d:%02d:%02d'Z'",
                    1900 + gmt->tm_year, 1 + gmt->tm_mon, gmt->tm_mday,
                    gmt->tm_hour, gmt->tm_min, gmt->tm_sec);
            }
        }
        #endif
        (void) fprintf(stderr, "\t");

        // Type.
        switch (type) {
            case kHAPLogType_Debug: {
                (void) fprintf(stderr, "Debug");
            } break;
            case kHAPLogType_Info: {
                (void) fprintf(stderr, "Info");
            } break;
            case kHAPLogType_Default: {
                (void) fprintf(stderr, "Default");
            } break;
            case kHAPLogType_Error: {
                (void) fprintf(stderr, "Error");
            } break;
            case kHAPLogType_Fault: {
                (void) fprintf(stderr, "Fault");
            } break;
        }
        (void) fprintf(stderr, "\t");

        // Subsystem / Category.
        if (log->subsystem) {
            (void) fprintf(stderr, "[%s", log->subsystem);
            if (log->category) {
                (void) fprintf(stderr, ":%s", log->category);
            }
            (void) fprintf(stderr, "] ");
        }

        // Message.
        (void) fprintf(stderr, "%s", message);
        (void) fprintf(stderr, "\n");

        // Buffer.
        if (bufferBytes) {
            size_t i, n;
            const uint8_t *b = bufferBytes;
            size_t length = numBufferBytes;
            if (length == 0) {
                (void) fprintf(stderr, "\n");
            } else {
                i = 0;
                do {
                    (void) fprintf(stderr, "    %04zx ", i);
                    for (n = 0; n != 8 * 4; n++) {
                        if (n % 4 == 0) {
                            (void) fprintf(stderr, " ");
                        }
                        if ((n <= length) && (i < length - n)) {
                            (void) fprintf(stderr, "%02x", b[i + n] & 0xff);
                        } else {
                            (void) fprintf(stderr, "  ");
                        }
                    };
                    (void) fprintf(stderr, "    ");
                    for (n = 0; n != 8 * 4; n++) {
                        if (i != length) {
                            if ((32 <= b[i]) && (b[i] < 127)) {
                                (void) fprintf(stderr, "%c", b[i]);
                            } else {
                                (void) fprintf(stderr, ".");
                            }
                            i++;
                        }
                    }
                    (void) fprintf(stderr, "\n");
                } while (i != length);
            }
        }
    }

    // Reset color.
    fprintf(stderr, "\x1B[0m");

    // Finish log.
    (void) fflush(stderr);

    __sync_lock_release(&captureLock);
}
