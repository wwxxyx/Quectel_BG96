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
#include <linux/random.h>
#include <syscall.h>
#include <unistd.h>

#include "HAPPlatform.h"

/**
 * Linux Random Number generator.
 *
 * This Random Number generator makes use of the Linux getrandom(2) interface.
 * Please note that this interface is only supported from Linux 3.17 onwards.
 *
 * For more information see:
 *  - LWN - The long road to getrandom() in glibc: https://lwn.net/Articles/711013/
 *  - Getrandom Manpage: http://man7.org/linux/man-pages/man2/getrandom.2.html
 */

static const HAPLogObject logObject = {
    .subsystem = kHAPPlatform_LogSubsystem,
    .category = "RandomNumber"
};

void HAPPlatformRandomNumberFill(
    void *bytes,
    size_t numBytes)
{
    HAPPrecondition(bytes);
    
    // Read random data.
    for (int i = 0; i < 5; i++) {
        size_t o = 0;
        while (o < numBytes) {
            size_t c = numBytes - o;
            
            // Using getrandom() to read small buffers (<= 256 bytes) from the urandom source is the preferred mode of
            // usage.
            // Source: man page of getrandom(2).
            if (c > 256) {
                c = 256;
            }
            
            ssize_t n;
            do {
                // Flags to call getrandom.
                const int getrandomFlags = GRND_NONBLOCK; // Use the urandom source and do not block.
                
                // With glibc >= 2.25 it is possible to call getrandom() directly.
                // Source: man page of getrandom(2).
                n = syscall(SYS_getrandom, &((uint8_t *)bytes)[o], c, getrandomFlags);
            } while ((n == -1) && (errno == EINTR ));
    
            if (n < 0) {
                int _errno = errno;
                HAPAssert(n == -1);
                HAPLogError(&logObject, "Read from getrandom failed: %d.", _errno);
                HAPFatalError();
            }

            HAPAssert((size_t) n <= c);
            o += (size_t) n;
        }

        // Verify random data.
        if (numBytes < 128 / 8 || !HAPRawBufferIsZero(bytes, numBytes)) {
            return;
        }
    }

    // getrandom() failed/
    HAPLogError(&logObject, "getrandom only produced zeroes.");
    HAPFatalError();
}
