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
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h> // Required on Raspi.

#include "HAPPlatformSoftwareAccessPoint+Init.h"
#include "HAPPlatformWiFiManager+Init.h"

static const HAPLogObject logObject = {
    .subsystem = kHAPPlatform_LogSubsystem,
    .category = "SoftwareAccessPoint"
};

#define kHAPPlatformSoftwareAccessPoint_DefaultInterfaceName "wlan0"

void HAPPlatformSoftwareAccessPointCreate(
    HAPPlatformSoftwareAccessPointRef softwareAccessPoint,
    const HAPPlatformSoftwareAccessPointOptions *options)
{
    HAPPrecondition(softwareAccessPoint);
    HAPPrecondition(options);
    HAPPrecondition(options->wiFiManager);
    
    HAPLog(&logObject, "Storage configuration: softwareAccessPoint = %lu",
        (unsigned long) sizeof *softwareAccessPoint);

    HAPRawBufferZero(softwareAccessPoint, sizeof *softwareAccessPoint);
    softwareAccessPoint->wiFiManager = options->wiFiManager;

    const char *interfaceName = options->interfaceName ?
        options->interfaceName : kHAPPlatformSoftwareAccessPoint_DefaultInterfaceName;
    size_t numInterfaceNameBytes = HAPStringGetNumBytes(interfaceName);
    if ((numInterfaceNameBytes == 0) || (numInterfaceNameBytes >= sizeof softwareAccessPoint->interfaceName)) {
        HAPLogError(&logObject, "Invalid local network interface name.");
        HAPFatalError();
    }
    HAPRawBufferCopyBytes(softwareAccessPoint->interfaceName, interfaceName, numInterfaceNameBytes);
}

/**
 * Entry point for the process that checks if SoftAP is UP.
 */
static bool IsSoftAPUp(
    const char *interfaceName)
{
    HAPPrecondition(interfaceName);

    HAPError err;

    char command[64 + IFNAMSIZ];
    err = HAPStringWithFormat(command, sizeof command, "/sbin/ip link show %s", interfaceName);
    HAPAssert(!err);

    FILE *outputStream = popen(command, "r");
    if (!outputStream) {
        int _errno = errno;
        HAPLogError(&logObject, "popen ip link show failed: %d", _errno);
        HAPFatalError();
    }

    uint8_t result[512];
    size_t n = fread(result, sizeof (uint8_t), sizeof result, outputStream);
    HAPAssert(n <= sizeof result);
    HAPLogBufferDebug(&logObject, result, n, "%s", command);

    bool ok = false;
    char patternBytes[] = "state UP mode DEFAULT";
    size_t numPatternBytes = sizeof patternBytes - 1;
    for (size_t i = numPatternBytes; !ok && i < n; i++) {
        if (HAPRawBufferAreEqual(patternBytes, &result[i - numPatternBytes], numPatternBytes)) {
            ok = true;
        }
    }

    (void) pclose(outputStream);
    return ok;
}

/**
 * Entry point for the process that sets up the IP address for SoftAP.
 */
HAP_NORETURN
static void SetIPAddressMain(
    const char *interfaceName)
{
    HAPPrecondition(interfaceName);

    HAPError err;

    char interfaceNameBuffer[IFNAMSIZ];
    err = HAPStringWithFormat(interfaceNameBuffer, sizeof interfaceNameBuffer, "%s", interfaceName);
    HAPAssert(!err);

    HAPLogInfo(&logObject, "%s: Launching ifconfig.", __func__);
    int e = execve("/sbin/ifconfig", (char *[]) {
        "/sbin/ifconfig",
        interfaceNameBuffer,
        "192.168.0.1",
        "netmask",
        "255.255.255.0",
        "broadcast",
        "255.255.255.255",
        NULL
    }, (char *[]) { NULL });
    if (e) {
        int _errno = errno;
        HAPAssert(e == -1);
        HAPLogError(&logObject, "%s: execve failed: %d.", __func__, _errno);
        HAPFatalError();
    }
    // Unreachable.
    HAPFatalError();
}

/**
 * Entry point for the process that clears the IP address configuration.
 */
HAP_NORETURN
static void ClearIPAddressMain(
    const char *interfaceName)
{
    HAPPrecondition(interfaceName);

    HAPError err;

    char interfaceNameBuffer[IFNAMSIZ];
    err = HAPStringWithFormat(interfaceNameBuffer, sizeof interfaceNameBuffer, "%s", interfaceName);
    HAPAssert(!err);

    HAPLogInfo(&logObject, "%s: Launching ifconfig.", __func__);
    int e = execve("/sbin/ifconfig", (char *[]) {
        "/sbin/ifconfig",
        interfaceNameBuffer,
        "0.0.0.0",
        NULL
    }, (char *[]) { NULL });
    if (e) {
        int _errno = errno;
        HAPAssert(e == -1);
        HAPLogError(&logObject, "%s: execve failed: %d.", __func__, _errno);
        HAPFatalError();
    }
    // Unreachable.
    HAPFatalError();
}

/**
 * Entry point for the process that runs DHCP.
 *
 * - The process will be terminated by the parent process when DHCP functionality is no longer required.
 */
HAP_NORETURN
static void DHCPMain(
    const char *interfaceName)
{
    HAPPrecondition(interfaceName);

    HAPError err;

    char interfaceNameArgument[12 /* --interface= */ + IFNAMSIZ];
    err = HAPStringWithFormat(interfaceNameArgument, sizeof interfaceNameArgument, "--interface=%s", interfaceName);
    HAPAssert(!err);

    HAPLogInfo(&logObject, "%s: Launching dnsmasq.", __func__);
    int e = execve("/usr/sbin/dnsmasq", (char *[]) {
        "/usr/sbin/dnsmasq",
        // "-d",
        "--conf-file",
        "--keep-in-foreground",
        "--port=0",
        interfaceNameArgument,
        "--dhcp-range=192.168.0.2,192.168.0.254,255.255.255.0,1h",
        "--dhcp-authoritative",
        NULL
    }, (char *[]) { NULL });
    if (e) {
        int _errno = errno;
        HAPAssert(e == -1);
        HAPLogError(&logObject, "%s: execve failed: %d.", __func__, _errno);
        HAPFatalError();
    }
    // Unreachable.
    HAPFatalError();
}

/**
 * Appends a string to the SoftAP configuration.
 *
 * @param      fd                   File descriptor that will append to the SoftAP configuration when written.
 * @param      string               String to append. NULL-terminated.
 */
static void SoftAPConfigAppendString(
    int fd,
    const char *string)
{
    HAPPrecondition(fd > -1);
    HAPPrecondition(string);

    size_t numBytes = HAPStringGetNumBytes(string);

    size_t o = 0;
    while (o < numBytes) {
        size_t c = numBytes - o;
        if (c > SSIZE_MAX) {
            c = SSIZE_MAX;
        }

        ssize_t n;
        do {
            n = write(fd, &string[o], c);
        } while (n == -1 && errno == EINTR);
        if (n < 0) {
            int _errno = errno;
            HAPAssert(n == -1);
            HAPLogError(&logObject, "%s: write %s failed: %d.", __func__, string, _errno);
            HAPFatalError();
        }
        if (n == 0) {
            HAPLogError(&logObject, "%s: write %s returned EOF.", __func__, string);
            HAPFatalError();
        }

        HAPAssert((size_t) n <= c);
        o += (size_t) n;
    }
}

/**
 * Appends the contents of a buffer to the SoftAP configuration, converted to a hex string.
 *
 * @param      fd                   File descriptor that will append to the SoftAP configuration when written.
 * @param      bytes                Buffer containing data to append.
 * @param      numBytes             Length of buffer.
 */
static void SoftAPConfigAppendHexStringOfData(
    int fd,
    const void *bytes,
    size_t numBytes)
{
    HAPPrecondition(fd > -1);
    HAPPrecondition(bytes);

    HAPError err;

    for (size_t i = 0; i < numBytes; i++) {
        char hex[2 + 1];
        err = HAPStringWithFormat(hex, sizeof hex, "%02X", ((const uint8_t *) bytes)[i]);
        HAPAssert(!err);
        HAPAssert(HAPStringGetNumBytes(hex) == sizeof hex - 1);
        SoftAPConfigAppendString(fd, hex);
    }
}

/**
 * Entry point for the process that runs the SoftAP.
 *
 * @param      softwareAccessPoint  Software access point manager.
 * @param      regulatoryDomain     Regulatory domain in which the device is operating.
 * @param      ieBytes              Buffer containing IE data.
 * @param      numIEBytes           Length of IE data buffer.
 */
HAP_NORETURN
static void SoftAPMain(
    HAPPlatformSoftwareAccessPointRef softwareAccessPoint,
    const HAPPlatformRegulatoryDomain *regulatoryDomain,
    const void *ieBytes,
    size_t numIEBytes)
{
    HAPPrecondition(softwareAccessPoint);
    HAPPrecondition(softwareAccessPoint->interfaceName);
    HAPPrecondition(regulatoryDomain);

    HAPPrecondition(ieBytes);
    HAPPrecondition(numIEBytes <= 257);

    // Create pipe to stdin.
    int pipeFDs[2];
    int e = pipe(pipeFDs);
    if (e) {
        int _errno = errno;
        HAPAssert(e == -1);
        HAPLogError(&logObject, "%s: pipe failed: %d.", __func__, _errno);
        HAPFatalError();
    }
    int fd = dup2(pipeFDs[0], STDIN_FILENO);
    if (fd < 0) {
        int _errno = errno;
        HAPAssert(fd == -1);
        HAPLogError(&logObject, "%s: dup2 failed: %d.", __func__, _errno);
        HAPFatalError();
    }
    HAPAssert(fd == STDIN_FILENO);

    // Generate Wi-Fi identifier.
    uint8_t wiFiIdentifier[6];
    HAPPlatformRandomNumberFill(wiFiIdentifier, sizeof wiFiIdentifier);
    
    // Write config to stdin.
    HAPLogInfo(&logObject, "%s: Writing configuration.", __func__);
    SoftAPConfigAppendString(pipeFDs[1], "interface=");
    SoftAPConfigAppendString(pipeFDs[1], softwareAccessPoint->interfaceName);
    SoftAPConfigAppendString(pipeFDs[1], "\n");
    SoftAPConfigAppendString(pipeFDs[1], "driver=nl80211\n");
    SoftAPConfigAppendString(pipeFDs[1], "\n");
    SoftAPConfigAppendString(pipeFDs[1], "ssid=Wi-Fi-");
    SoftAPConfigAppendHexStringOfData(pipeFDs[1], wiFiIdentifier, sizeof wiFiIdentifier);
    SoftAPConfigAppendString(pipeFDs[1], "\n");
    SoftAPConfigAppendString(pipeFDs[1], "country_code=");
    SoftAPConfigAppendString(pipeFDs[1], regulatoryDomain->stringValue);
    SoftAPConfigAppendString(pipeFDs[1], "\n");
    SoftAPConfigAppendString(pipeFDs[1], "hw_mode=g\n");
    SoftAPConfigAppendString(pipeFDs[1], "channel=6\n");
    SoftAPConfigAppendString(pipeFDs[1], "vendor_elements=");
    SoftAPConfigAppendHexStringOfData(pipeFDs[1], ieBytes, numIEBytes);
    SoftAPConfigAppendString(pipeFDs[1], "\n");
    (void) close(pipeFDs[1]);

    // Launch hostapd.
    HAPLogInfo(&logObject, "%s: Launching hostapd.", __func__);
    e = execve("/usr/sbin/hostapd", (char *[]) {
        "/usr/sbin/hostapd",
        // "-dd",
        "/dev/stdin",
        NULL
    }, (char *[]) { NULL });
    if (e) {
        int _errno = errno;
        HAPAssert(e == -1);
        HAPLogError(&logObject, "%s: execve failed: %d.", __func__, _errno);
        HAPFatalError();
    }
    // Unreachable.
    HAPFatalError();
}

/**
 * Entry point of the intermediate process that will launch all the other sub processes.
 *
 * - If the parent process exits for any reason, all launched sub processes will be terminated.
 *
 * @param      fromParentFD         Pipe file descriptor that is closed when parent process terminates.
 * @param      toParentFD           Pipe file descriptor where 1 byte will be written after SoftAP setup completes.
 * @param      softwareAccessPoint  Software access point manager.
 * @param      regulatoryDomain     Regulatory domain in which the device is operating.
 * @param      ieBytes              Buffer containing IE data.
 * @param      numIEBytes           Length of IE data buffer.
 */
HAP_NORETURN
static void IntermediateMain(
    int fromParentFD,
    int toParentFD,
    HAPPlatformSoftwareAccessPointRef softwareAccessPoint,
    const HAPPlatformRegulatoryDomain *regulatoryDomain,
    const void *ieBytes,
    size_t numIEBytes)
{
    HAPPrecondition(softwareAccessPoint);
    HAPPrecondition(softwareAccessPoint->interfaceName);
    HAPPrecondition(regulatoryDomain);

    // Remove signal handlers as they are inherited when we fork.
    if (signal(SIGTERM, SIG_DFL) == SIG_ERR) {
        int _errno = errno;
        HAPLogError(&logObject, "%s: signal TERM failed: %d.", __func__, _errno);
        HAPFatalError();
    }
    if (signal(SIGUSR1, SIG_DFL) == SIG_ERR) {
        int _errno = errno;
        HAPLogError(&logObject, "%s: signal USR1 failed: %d.", __func__, _errno);
        HAPFatalError();
    }
    if (signal(SIGUSR2, SIG_DFL) == SIG_ERR) {
        int _errno = errno;
        HAPLogError(&logObject, "%s: signal USR2 failed: %d.", __func__, _errno);
        HAPFatalError();
    }

    // Detach from parent process to avoid getting killed when parent process tree gets killed.
    // We want to do a graceful cleanup in that case (fromParentFD will close when parent process is terminated).
    int e = setpgid(0, 0);
    if (e) {
        int _errno = errno;
        HAPAssert(e == -1);
        HAPLogError(&logObject, "%s: setpgid failed: %d.", __func__, _errno);
        HAPFatalError();
    }

    // Report "ready for HAPPlatformSoftwareAccessPointStop" by writing to pipe.
    ssize_t n;
    do {
        n = write(toParentFD, (const uint8_t[]) { 0 }, 1);
    } while (n == -1 && errno == EINTR);
    if (n < 0) {
        int _errno = errno;
        HAPAssert(n == -1);
        HAPLogError(&logObject, "%s: write failed: %d.", __func__, _errno);
        HAPFatalError();
    }
    if (n == 0) {
        HAPLogError(&logObject, "%s: write returned EOF.", __func__);
        HAPFatalError();
    }
    HAPAssert(n == 1);

    // Try to keep up SoftAP until stop.
    bool isRunning = true;
    while (isRunning) {
        // Set own IP.
        HAPLogInfo(&logObject, "%s: Setting own IP.", __func__);
        pid_t setIPAddressPID = fork();
        if (setIPAddressPID < 0) {
            int _errno = errno;
            HAPAssert(setIPAddressPID == -1);
            HAPLogError(&logObject, "%s: fork failed: %d.", __func__, _errno);
            HAPFatalError();
        }
        if (setIPAddressPID == 0) {
            (void) close(fromParentFD);
            SetIPAddressMain(softwareAccessPoint->interfaceName);
            // Unreachable.
        }

        // Wait for IP setting to complete.
        HAPLogInfo(&logObject, "%s: Waiting for IP setting to complete.", __func__);
        pid_t pid;
        do {
            pid = wait(NULL);
        } while (pid == -1 && errno == EINTR);
        if (pid < 0 && errno != ECHILD) {
            int _errno = errno;
            HAPAssert(pid == -1);
            HAPLogError(&logObject, "%s: wait failed: %d.", __func__, _errno);
            HAPFatalError();
        }
        HAPAssert(pid == setIPAddressPID || (pid == -1 && errno == ECHILD));
        HAPLogInfo(&logObject, "%s: IP setting complete.", __func__);

        // Start DHCP process.
        HAPLogInfo(&logObject, "%s: Starting DHCP.", __func__);
        pid_t dhcpPID = fork();
        if (dhcpPID < 0) {
            int _errno = errno;
            HAPAssert(dhcpPID == -1);
            HAPLogError(&logObject, "%s: fork failed: %d.", __func__, _errno);
            HAPFatalError();
        }
        if (dhcpPID == 0) {
            (void) close(fromParentFD);
            DHCPMain(softwareAccessPoint->interfaceName);
            // Unreachable.
        }

        // Start SoftAP process.
        HAPLogInfo(&logObject, "%s: Starting SoftAP.", __func__);
        pid_t softAPPID = fork();
        if (softAPPID < 0) {
            int _errno = errno;
            HAPAssert(softAPPID == -1);
            HAPLogError(&logObject, "%s: fork failed: %d.", __func__, _errno);
            HAPFatalError();
        }
        if (softAPPID == 0) {
            (void) close(fromParentFD);
            SoftAPMain(softwareAccessPoint, regulatoryDomain, ieBytes, numIEBytes);
            // Unreachable.
        }

        // Wait for SoftAP startup or HAPPlatformSoftwareAccessPointStop.
        HAPLogDebug(&logObject, "%s: Waiting for SoftAP to go up.", __func__);
        for (int i = 0; i < 3; i++) {
            fd_set fds;
            FD_ZERO(&fds);
            FD_SET(fromParentFD, &fds);
            struct timeval timeout = {
                .tv_sec = 2,
                .tv_usec = 0
            };
            bool isUp = IsSoftAPUp(softwareAccessPoint->interfaceName);
            do {
                e = select(fromParentFD + 1, &fds, NULL, NULL, isUp ? NULL : &timeout);
            } while (e == -1 && errno == EINTR);
            if (e < 0) {
                int _errno = errno;
                HAPAssert(e == -1);
                HAPLogError(&logObject, "%s - select failed: %d", __func__, _errno);
                HAPFatalError();
            }

            if (FD_ISSET(fromParentFD, &fds)) {
                // Check if parent process did exit.
                // When this happens, the pipe is closed and read returns EOF (0).
                do {
                    uint8_t bytes[1] = {0};
                    n = read(fromParentFD, bytes, 1);
                } while (n == -1 && errno == EINTR);
                if (n < 0) {
                    int _errno = errno;
                    HAPAssert(n == -1);
                    HAPLogError(&logObject, "%s: read failed: %d.", __func__, _errno);
                    HAPFatalError();
                }
                (void) close(fromParentFD);
                HAPLogInfo(&logObject, "%s: Parent process did exit.", __func__);
                isRunning = false;
                break;
            } else {
                // Timeout.
                HAPLogDebug(&logObject, "%s: Waiting for SoftAP to go up (%u).", __func__, i + 1);
            }
        }
        if (isRunning) {
            HAPLog(&logObject, "%s: SoftAP failed to go up. Stopping and trying again.", __func__);
        }

        // Terminate SoftAP process.
        HAPLogInfo(&logObject, "%s: Stopping SoftAP (pid = %d).", __func__, softAPPID);
        e = kill(softAPPID, SIGTERM);
        if (e) {
            int _errno = errno;
            HAPAssert(e == -1);
            HAPLog(&logObject, "kill failed: %d.", _errno);
        }

        // Terminate DHCP process.
        HAPLogInfo(&logObject, "%s: Stopping DHCP (pid = %d).", __func__, dhcpPID);
        e = kill(dhcpPID, SIGTERM);
        if (e) {
            int _errno = errno;
            HAPAssert(e == -1);
            HAPLog(&logObject, "%s: kill failed: %d.", __func__, _errno);
        }

        // Reap children.
        HAPLogInfo(&logObject, "%s: Reaping children.", __func__);
        do {
            do {
                pid = wait(NULL);
            } while (pid == -1 && errno == EINTR);
            if (pid < 0 && errno != ECHILD) {
                int _errno = errno;
                HAPAssert(pid == -1);
                HAPLogError(&logObject, "%s: wait failed: %d.", __func__, _errno);
                HAPFatalError();
            } else if (pid > -1) {
                HAPLogInfo(&logObject, "%s: Child reaped: %d.", __func__, pid);
            }
        } while (pid > -1);

        // Clear own IP.
        HAPLogInfo(&logObject, "%s: Clearing own IP.", __func__);
        pid_t clearIPAddressPID = fork();
        if (clearIPAddressPID < 0) {
            int _errno = errno;
            HAPAssert(clearIPAddressPID == -1);
            HAPLogError(&logObject, "%s: fork failed: %d.", __func__, _errno);
            HAPFatalError();
        }
        if (clearIPAddressPID == 0) {
            (void) close(fromParentFD);
            ClearIPAddressMain(softwareAccessPoint->interfaceName);
            // Unreachable.
        }

        // Wait for IP clearing to complete.
        HAPLogInfo(&logObject, "%s: Waiting for IP clearing to complete.", __func__);
        do {
            pid = wait(NULL);
        } while (pid == -1 && errno == EINTR);
        if (pid < 0 && errno != ECHILD) {
            int _errno = errno;
            HAPAssert(pid == -1);
            HAPLogError(&logObject, "%s: wait failed: %d.", __func__, _errno);
            HAPFatalError();
        }
        HAPAssert(pid == clearIPAddressPID || (pid == -1 && errno == ECHILD));
        HAPLogInfo(&logObject, "%s: IP clearing complete.", __func__);
    }

    // Exit intermediate process.
    HAPLogInfo(&logObject, "%s: Exiting.", __func__);
    exit(0);
}

/**
 * PID of the intermediate process.
 */
static pid_t intermediatePID = -1;

/**
 * Pipe file descriptor that is closed when software access point should be stopped.
 */
static int toIntermediateFD = -1;

void HAPPlatformSoftwareAccessPointStart(
    HAPPlatformSoftwareAccessPointRef softwareAccessPoint,
    const void *ieBytes,
    size_t numIEBytes)
{
    HAPPrecondition(softwareAccessPoint);
    HAPPrecondition(ieBytes);
    
    HAPError err;

    HAPLogInfo(&logObject, "%s", __func__);
    if (intermediatePID != -1) {
        // Already running.
        HAPLogInfo(&logObject, "%s: SoftAP already running.", __func__);
        return;
    }
    
    // Get regulatory domain.
    HAPPlatformRegulatoryDomain regulatoryDomain;
    err = HAPPlatformWiFiManagerGetRegulatoryDomain(softwareAccessPoint->wiFiManager, &regulatoryDomain);
    if (err) {
        HAPLogError(&logObject,
            "%s: Regulatory domain has not been configured in wpa_supplicant configuration.", __func__);
        HAPFatalError();
    }

    // Create pipe for intermediate process to detect when the main process dies.
    int toIntermediateFDs[2];
    int e = pipe(toIntermediateFDs);
    if (e) {
        int _errno = errno;
        HAPAssert(e == -1);
        HAPLogError(&logObject, "%s: pipe failed: %d.", __func__, _errno);
        HAPFatalError();
    }

    // Create pipe to detect when AP startup completes.
    int fromIntermediateFDs[2];
    e = pipe(fromIntermediateFDs);
    if (e) {
        int _errno = errno;
        HAPAssert(e == -1);
        HAPLogError(&logObject, "%s: pipe failed: %d.", __func__, _errno);
        HAPFatalError();
    }

    // Fork into intermediate process.
    // Goal is to detect when parent process dies and terminate sub processes when this happens.
    intermediatePID = fork();
    if (intermediatePID < 0) {
        int _errno = errno;
        HAPAssert(intermediatePID == -1);
        HAPLogError(&logObject, "%s: fork failed: %d.", __func__, _errno);
        HAPFatalError();
    }
    if (intermediatePID == 0) {
        (void) close(fromIntermediateFDs[0]);
        (void) close(toIntermediateFDs[1]);
        IntermediateMain(toIntermediateFDs[0], fromIntermediateFDs[1],
            softwareAccessPoint, &regulatoryDomain, ieBytes, numIEBytes);
        // Unreachable.
    }
    (void) close(fromIntermediateFDs[1]);
    (void) close(toIntermediateFDs[0]);
    toIntermediateFD = toIntermediateFDs[1];

    // Wait for SoftAP startup to complete.
    HAPLogInfo(&logObject, "%s: Waiting for AP startup.", __func__);
    ssize_t n;
    do {
        uint8_t bytes[1] = {0};
        n = read(fromIntermediateFDs[0], bytes, 1);
    } while (n == -1 && errno == EINTR);
    if (n < 0) {
        int _errno = errno;
        HAPAssert(n == -1);
        HAPLogError(&logObject, "%s: read failed: %d.", __func__, _errno);
        HAPFatalError();
    }
    if (n == 0) {
        HAPLogError(&logObject, "%s: read returned EOF.", __func__);
        HAPFatalError();
    }
    HAPAssert(n == 1);
    (void) close(fromIntermediateFDs[0]);

    HAPLogInfo(&logObject, "%s: AP startup complete.", __func__);
}

void HAPPlatformSoftwareAccessPointStop(
    HAPPlatformSoftwareAccessPointRef softwareAccessPoint)
{
    HAPPrecondition(softwareAccessPoint);

    HAPLogInfo(&logObject, "%s", __func__);
    if (intermediatePID == -1) {
        return;
    }

    // Signal sub processes to exit.
    HAPLogInfo(&logObject, "%s: Signalling sub processes to exit.", __func__);
    (void) close(toIntermediateFD);
    toIntermediateFD = -1;

    // Wait for sub processes to exit.
    pid_t pid;
    do {
        pid = wait(NULL);
    } while (pid == -1 && errno == EINTR);
    if (pid < 0 && errno != ECHILD) {
        int _errno = errno;
        HAPAssert(pid == -1);
        HAPLogError(&logObject, "%s: wait failed: %d.", __func__, _errno);
        HAPFatalError();
    }
    HAPAssert(pid == intermediatePID || (pid == -1 && errno == ECHILD));
    HAPLogInfo(&logObject, "%s: Sub processes exited.", __func__);
    intermediatePID = -1;
}
