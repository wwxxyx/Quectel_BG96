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
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h> // Required on Raspi.

#include "HAPPlatformFileManager.h"
#include "HAPPlatformWiFiManager+Init.h"

static const HAPLogObject logObject = {
    .subsystem = kHAPPlatform_LogSubsystem,
    .category = "WiFiManager"
};

#define kHAPPlatformWiFiManager_WPASupplicantConfPath "/etc/wpa_supplicant/wpa_supplicant.conf"
#define kHAPPlatformWiFiManager_DefaultInterfaceName "wlan0"

void HAPPlatformWiFiManagerCreate(
    HAPPlatformWiFiManagerRef wiFiManager,
    const HAPPlatformWiFiManagerOptions *options)
{
    HAPPrecondition(wiFiManager);
    HAPPrecondition(options);

    HAPLog(&logObject, "Storage configuration: wiFiManager = %lu",
        (unsigned long) sizeof *wiFiManager);

    HAPRawBufferZero(wiFiManager, sizeof *wiFiManager);

    const char *interfaceName = options->interfaceName ?
        options->interfaceName : kHAPPlatformWiFiManager_DefaultInterfaceName;
    size_t numInterfaceNameBytes = HAPStringGetNumBytes(interfaceName);
    if ((numInterfaceNameBytes == 0) || (numInterfaceNameBytes >= sizeof wiFiManager->interfaceName)) {
        HAPLogError(&logObject, "Invalid local network interface name.");
        HAPFatalError();
    }
    HAPRawBufferCopyBytes(wiFiManager->interfaceName, interfaceName, numInterfaceNameBytes);
}

HAP_RESULT_USE_CHECK
HAPPlatformWiFiManagerCapabilities HAPPlatformWiFiManagerGetCapabilities(
    HAPPlatformWiFiManagerRef wiFiManager)
{
    HAPPrecondition(wiFiManager);

    return (HAPPlatformWiFiManagerCapabilities) {
        .supports2_4GHz = true,
        .supports5GHz = false
    };
}

HAP_RESULT_USE_CHECK
HAPError HAPPlatformWiFiManagerGetRegulatoryDomain(
    HAPPlatformWiFiManagerRef wiFiManager,
    HAPPlatformRegulatoryDomain *regulatoryDomain)
{
    HAPPrecondition(wiFiManager);
    HAPPrecondition(regulatoryDomain);
    
    HAPRawBufferZero(regulatoryDomain, sizeof *regulatoryDomain);
    
    // Open wpa_supplicant configuration file.
    int fd;
    do {
        fd = open(kHAPPlatformWiFiManager_WPASupplicantConfPath, O_RDONLY);
    } while (fd == -1 && errno == EINTR);
    if (fd < 0) {
        int _errno = errno;
        if (_errno == ENOENT) {
            return kHAPError_InvalidState;
        }
        HAPAssert(fd == -1);
        HAPLogError(&logObject, "open %s failed: %d.", kHAPPlatformWiFiManager_WPASupplicantConfPath, _errno);
        HAPFatalError();
    }
    
    // Look for country key.
    size_t i = 0;
    bool countryCodeFound = false;
    bool ignoreLine = false;
    for (;;) {
        // Read next byte.
        char c;
        ssize_t n;
        do {
            n = read(fd, &c, sizeof c);
        } while (n == -1 && errno == EINTR);
        if (n < 0) {
            int _errno = errno;
            HAPAssert(n == -1);
            HAPLogError(&logObject, "read %s failed: %d.", kHAPPlatformWiFiManager_WPASupplicantConfPath, _errno);
            HAPFatalError();
        }
        if (n == 0) {
            break;
        }
        HAPAssert(n == sizeof c);
        
        // Skip lines until finding country code configuration.
        if (ignoreLine) {
            if (c == '\n') {
                ignoreLine = false;
            }
            continue;
        }
        
        // Match country code prefix.
        #define prefixString "country="
        size_t numPrefixBytes = sizeof prefixString - 1;
        if (i < numPrefixBytes) {
            if (c != prefixString[i]) {
                ignoreLine = c != '\n';
                i = 0;
                continue;
            }
            
            // Character matched.
            i++;
            
            // Once full prefix is read, check that there are not multiple country codes.
            if (i == numPrefixBytes) {
                if (countryCodeFound) {
                    HAPLog(&logObject, "Multiple country codes in %s", kHAPPlatformWiFiManager_WPASupplicantConfPath);
                    (void) close(fd);
                    return kHAPError_InvalidState;
                }
                countryCodeFound = true;
            }
            
            continue;
        }
        #undef prefixString
        
        // Match country code. It consists of two uppercase alphabet characters.
        size_t o = i - numPrefixBytes;
        if (o < sizeof regulatoryDomain->stringValue - 1) {
            if (c < 'A' || c > 'Z') {
                HAPLog(&logObject, "Malformed country code in %s", kHAPPlatformWiFiManager_WPASupplicantConfPath);
                (void) close(fd);
                return kHAPError_InvalidState;
            }
            regulatoryDomain->stringValue[o] = c;
            i++;
            
            continue;
        }
        
        // Match line terminator after country code.
        HAPAssert(o == sizeof regulatoryDomain->stringValue - 1);
        if (c != '\n') {
            HAPLog(&logObject,
                "Malformed country code in %s (too long).", kHAPPlatformWiFiManager_WPASupplicantConfPath);
            (void) close(fd);
            return kHAPError_InvalidState;
        }
        i = 0;
    }
    if (!countryCodeFound) {
        HAPLog(&logObject,
            "Country code has not been set in %s", kHAPPlatformWiFiManager_WPASupplicantConfPath);
        (void) close(fd);
        return kHAPError_InvalidState;
    }
    if (!regulatoryDomain->stringValue[0] || !regulatoryDomain->stringValue[1]) {
        HAPLog(&logObject,
            "Malformed country code in %s (too short).", kHAPPlatformWiFiManager_WPASupplicantConfPath);
        (void) close(fd);
        return kHAPError_InvalidState;
    }
    HAPAssert(!regulatoryDomain->stringValue[2]);
    (void) close(fd);
    
    HAPLogInfo(&logObject, "Regulatory domain: %s.", regulatoryDomain->stringValue);
    return kHAPError_None;
}

HAP_RESULT_USE_CHECK
bool HAPPlatformWiFiManagerIsConfigured(
    HAPPlatformWiFiManagerRef wiFiManager)
{
    HAPPrecondition(wiFiManager);
    
    // Open wpa_supplicant configuration file.
    int fd;
    do {
        fd = open(kHAPPlatformWiFiManager_WPASupplicantConfPath, O_RDONLY);
    } while (fd == -1 && errno == EINTR);
    if (fd < 0) {
        int _errno = errno;
        if (_errno == ENOENT) {
            return false;
        }
        HAPAssert(fd == -1);
        HAPLogError(&logObject,
            "open %s failed: %d.", kHAPPlatformWiFiManager_WPASupplicantConfPath, _errno);
        HAPFatalError();
    }
    
    // Look for country key.
    size_t i = 0;
    bool ignoreLine = false;
    for (;;) {
        // Read next byte.
        char c;
        ssize_t n;
        do {
            n = read(fd, &c, sizeof c);
        } while (n == -1 && errno == EINTR);
        if (n < 0) {
            int _errno = errno;
            HAPAssert(n == -1);
            HAPLogError(&logObject,
                "read %s failed: %d.", kHAPPlatformWiFiManager_WPASupplicantConfPath, _errno);
            HAPFatalError();
        }
        if (n == 0) {
            HAPLogDebug(&logObject, "%s:%u", __func__, __LINE__);
            break;
        }
        HAPAssert(n == sizeof c);
    
        // Skip lines until finding country code configuration.
        if (ignoreLine) {
            if (c == '\n') {
                ignoreLine = false;
            }
            continue;
        }
        
        // Match network prefix.
        #define prefixString "network="
        size_t numPrefixBytes = sizeof prefixString - 1;
        HAPAssert(i < numPrefixBytes);
        if (c != prefixString[i]) {
            ignoreLine = c != '\n';
            i = 0;
            continue;
        }
    
        // Character matched.
        i++;
        
        // Network is assumed configured.
        // TODO Is this good enough?
        if (i == numPrefixBytes) {
            (void) close(fd);
            return true;
        }
        #undef prefixString
    }
    
    HAPLogDebug(&logObject, "%s:%u", __func__, __LINE__);
    
    (void) close(fd);
    return false;
}

/**
 * Entry point for the process that shuts down the Wi-Fi interface.
 */
HAP_NORETURN
static void ShutdownWiFiMain(
    const char *interfaceName)
{
    HAPPrecondition(interfaceName);

    HAPError err;

    char interfaceNameBuffer[IFNAMSIZ];
    err = HAPStringWithFormat(interfaceNameBuffer, sizeof interfaceNameBuffer, "%s", interfaceName);
    HAPAssert(!err);

    HAPLogInfo(&logObject, "%s: Launching ifdown.", __func__);
    int e = execve("/sbin/ifdown", (char *[]) {
        "/sbin/ifdown",
        interfaceNameBuffer,
        NULL
    }, (char *[]) { NULL });
    if (e) {
        int _errno = errno;
        HAPAssert(e == -1);
        HAPLogError(&logObject, "%s: execve failed: %d.", __func__,  _errno);
        HAPFatalError();
    }
    // Unreachable.
    HAPFatalError();
}

/**
 * Entry point for the process that starts the Wi-Fi interface.
 */
HAP_NORETURN
static void StartWiFiMain(
    const char *interfaceName)
{
    HAPPrecondition(interfaceName);

    HAPError err;

    char interfaceNameBuffer[IFNAMSIZ];
    err = HAPStringWithFormat(interfaceNameBuffer, sizeof interfaceNameBuffer, "%s", interfaceName);
    HAPAssert(!err);

    HAPLogInfo(&logObject, "%s: Launching ifup.", __func__);
    int e = execve("/sbin/ifup", (char *[]) {
        "/sbin/ifup",
        interfaceNameBuffer,
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
 * Entry point for the process that reloads the network configuration.
 */
HAP_NORETURN
static void ReloadNetworkConfigurationMain(void)
{
    HAPLogInfo(&logObject, "%s: Restarting dhcpcd service.", __func__);
    int e = execve("/usr/sbin/service", (char *[]) {
        "/usr/sbin/service",
        "dhcpcd",
        "restart",
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
 * Entry point for the process that restarts the Wi-Fi interface.
 */
HAP_NORETURN
static void RestartWiFiMain(
    const char *interfaceName)
{
    HAPPrecondition(interfaceName);

    // Detach from parent process to avoid getting killed when parent process tree gets killed.
    int e = setpgid(0, 0);
    if (e) {
        int _errno = errno;
        HAPAssert(e == -1);
        HAPLogError(&logObject, "%s: setpgid failed: %d.", __func__, _errno);
        HAPFatalError();
    }

    // Shut down interface.
    HAPLogInfo(&logObject, "%s: Shutting down interface.", __func__);
    pid_t shutdownWiFiPID = fork();
    if (shutdownWiFiPID < 0) {
        int _errno = errno;
        HAPAssert(shutdownWiFiPID == -1);
        HAPLogError(&logObject, "%s: fork failed: %d.", __func__, _errno);
        HAPFatalError();
    }
    if (shutdownWiFiPID == 0) {
        ShutdownWiFiMain(interfaceName);
        // Unreachable.
    }

    // Wait for interface shutdown to complete.
    HAPLogInfo(&logObject, "%s: Waiting for interface shutdown to complete.", __func__);
    pid_t pid;
    int shutdownWiFiStatusValue = -1;
    do {
        pid = wait(&shutdownWiFiStatusValue);
    } while (pid == -1 && errno == EINTR);
    if (pid < 0 && errno != ECHILD) {
        int _errno = errno;
        HAPAssert(pid == -1);
        HAPLogError(&logObject, "%s: wait failed: %d.", __func__, _errno);
        HAPFatalError();
    }
    HAPAssert(pid == shutdownWiFiPID || (pid == -1 && errno == ECHILD));
    HAPLogInfo(&logObject, "%s: Interface shutdown complete.", __func__);

    // Start interface.
    HAPLogInfo(&logObject, "%s: Starting interface.", __func__);
    pid_t startWiFiPID = fork();
    if (startWiFiPID < 0) {
        int _errno = errno;
        HAPAssert(startWiFiPID == -1);
        HAPLogError(&logObject, "%s: fork failed: %d.", __func__, _errno);
        HAPFatalError();
    }
    if (startWiFiPID == 0) {
        StartWiFiMain(interfaceName);
        // Unreachable.
    }

    // Wait for interface start to complete.
    HAPLogInfo(&logObject, "%s: Waiting for interface start to complete.", __func__);
    int startWiFiStatusValue = -1;
    do {
        pid = wait(&startWiFiStatusValue);
    } while (pid == -1 && errno == EINTR);
    if (pid < 0 && errno != ECHILD) {
        int _errno = errno;
        HAPAssert(pid == -1);
        HAPLogError(&logObject, "%s: wait failed: %d.", __func__, _errno);
        HAPFatalError();
    }
    HAPAssert(pid == startWiFiPID || (pid == -1 && errno == ECHILD));
    HAPLogInfo(&logObject, "%s: Interface start complete.", __func__);

    // If ifdown / ifup don't work, try reloading the network configuration.
    if (WIFEXITED(shutdownWiFiStatusValue) && WEXITSTATUS(shutdownWiFiStatusValue) &&
        WIFEXITED(startWiFiStatusValue) && WEXITSTATUS(startWiFiStatusValue))
    {
        HAPLogInfo(&logObject, "%s: ifdown failed (%d).", __func__, WEXITSTATUS(shutdownWiFiStatusValue));
        HAPLogInfo(&logObject, "%s: ifup failed (%d).", __func__, WEXITSTATUS(startWiFiStatusValue));

        // Reload network configuration if ifdown / ifup did not work.
        HAPLogInfo(&logObject, "%s: Reloading network configuration.", __func__);
        pid_t reloadNetworkConfigurationPID = fork();
        if (reloadNetworkConfigurationPID < 0) {
            int _errno = errno;
            HAPAssert(reloadNetworkConfigurationPID == -1);
            HAPLogError(&logObject, "%s: fork failed: %d.", __func__, _errno);
            HAPFatalError();
        }
        if (reloadNetworkConfigurationPID == 0) {
            ReloadNetworkConfigurationMain();
            // Unreachable.
        }

        // Wait for network configuration reloading to complete.
        HAPLogInfo(&logObject, "%s: Waiting for network configuration reload to complete.", __func__);
        do {
            pid = wait(NULL);
        } while (pid == -1 && errno == EINTR);
        if (pid < 0 && errno != ECHILD) {
            int _errno = errno;
            HAPAssert(pid == -1);
            HAPLogError(&logObject, "%s: wait failed: %d.", __func__, _errno);
            HAPFatalError();
        }
        HAPAssert(pid == reloadNetworkConfigurationPID || (pid == -1 && errno == ECHILD));
        HAPLogInfo(&logObject, "%s: Network configuration reload complete.", __func__);
    }

    exit(0);
}

/**
 * Updates the wpa_supplicant configuration.
 *
 * @param      wiFiManager          Wi-Fi manager.
 * @param      wiFiConfiguration    Configuration to write. NULL-terminated.
 */
static void UpdateWiFiConfiguration(
    HAPPlatformWiFiManagerRef wiFiManager,
    const char *wiFiConfiguration)
{
    HAPPrecondition(wiFiManager);
    HAPPrecondition(wiFiManager->interfaceName);
    HAPPrecondition(wiFiConfiguration);
    
    HAPError err;
    
    // Write configuration.
    HAPLogSensitiveInfo(&logObject,
        "Writing %s.\n%s", kHAPPlatformWiFiManager_WPASupplicantConfPath, wiFiConfiguration);
    err = HAPPlatformFileManagerWriteFile(kHAPPlatformWiFiManager_WPASupplicantConfPath,
        wiFiConfiguration, HAPStringGetNumBytes(wiFiConfiguration));
    if (err) {
        HAPLogError(&logObject, "Write to %s failed.", kHAPPlatformWiFiManager_WPASupplicantConfPath);
        HAPFatalError();
    }
    
    // Restart interface.
    int restartWiFiPID = fork();
    if (restartWiFiPID < 0) {
        int _errno = errno;
        HAPAssert(restartWiFiPID == -1);
        HAPLogError(&logObject, "%s: fork failed: %d.", __func__, _errno);
        HAPFatalError();
    }
    if (restartWiFiPID == 0) {
        RestartWiFiMain(wiFiManager->interfaceName);
        // Unreachable.
    }
    
    // Wait for interface restart to complete.
    HAPLogInfo(&logObject, "%s: Waiting for interface restart to complete.", __func__);
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
    HAPAssert(pid == restartWiFiPID || (pid == -1 && errno == ECHILD));
    HAPLogInfo(&logObject, "%s: Interface restart complete.", __func__);
}

void HAPPlatformWiFiManagerApplyConfiguration(
    HAPPlatformWiFiManagerRef wiFiManager,
    const char *ssid,
    const char *_Nullable const passphrase)
{
    HAPPrecondition(wiFiManager);
    HAPPrecondition(ssid);
    
    HAPError err;
    
    HAPLogDebug(&logObject, "%s", __func__);
    
    // Get regulatory domain.
    HAPPlatformRegulatoryDomain regulatoryDomain;
    err = HAPPlatformWiFiManagerGetRegulatoryDomain(wiFiManager, &regulatoryDomain);
    if (err) {
        HAPLogError(&logObject,
            "%s: Regulatory domain has not been configured in wpa_supplicant configuration.", __func__);
        HAPFatalError();
    }
    
    // Initial configuration file.
    char wiFiConfiguration[512];
    err = HAPStringWithFormat(wiFiConfiguration, sizeof wiFiConfiguration,
        "country=%s\n"
        "ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev\n"
        "update_config=1\n", regulatoryDomain.stringValue);
    HAPAssert(!err);
    
    // Append network configuration.
    size_t n = HAPStringGetNumBytes(wiFiConfiguration);
    err = HAPStringWithFormat(&wiFiConfiguration[n], sizeof wiFiConfiguration - n,
        "\n"
        "network={\n"
        "    ssid=");
    HAPAssert(!err);
    n += HAPStringGetNumBytes(&wiFiConfiguration[n]);
    
    // SSID.
    size_t numSSIDBytes = HAPStringGetNumBytes(ssid);
    for (size_t i = 0; i < numSSIDBytes; i++) {
        err = HAPStringWithFormat(&wiFiConfiguration[n], sizeof wiFiConfiguration - n, "%02X", ssid[i]);
        HAPAssert(!err);
        n += HAPStringGetNumBytes(&wiFiConfiguration[n]);
    }
    err = HAPStringWithFormat(&wiFiConfiguration[n], sizeof wiFiConfiguration - n, "\n");
    HAPAssert(!err);
    n += HAPStringGetNumBytes(&wiFiConfiguration[n]);
    
    // Passphrase.
    if (!passphrase) {
        err = HAPStringWithFormat(&wiFiConfiguration[n], sizeof wiFiConfiguration - n, "    key_mgmt=NONE\n");
        HAPAssert(!err);
        n += HAPStringGetNumBytes(&wiFiConfiguration[n]);
    } else {
        err = HAPStringWithFormat(&wiFiConfiguration[n], sizeof wiFiConfiguration - n, "    psk=");
        HAPAssert(!err);
        n += HAPStringGetNumBytes(&wiFiConfiguration[n]);
    
        size_t numPassphraseBytes = HAPStringGetNumBytes(passphrase);
        if (numPassphraseBytes == 64) {
            // PSK, 64 hex characters.
            err = HAPStringWithFormat(&wiFiConfiguration[n], sizeof wiFiConfiguration - n, "%s", passphrase);
            HAPAssert(!err);
            n += HAPStringGetNumBytes(&wiFiConfiguration[n]);
        } else {
            HAPAssert(numPassphraseBytes >= 8 && numPassphraseBytes <= 63);
        
            // Derive PSK.
            // - Supports special characters like " in the passphrase.
            // - Added security because the raw passphrase (which may be shared with other services) is not stored.
            uint8_t psk[kHAPWiFiWPAPSK_NumBytes];
            HAPWiFiGetWPAPSKForPassphrase(psk, ssid, passphrase);
            HAPLogSensitiveBufferDebug(&logObject, psk, sizeof psk, "PSK derived from passphrase.");
    
            for (size_t i = 0; i < sizeof psk; i++) {
                err = HAPStringWithFormat(&wiFiConfiguration[n], sizeof wiFiConfiguration - n, "%02X", psk[i]);
                HAPAssert(!err);
                n += HAPStringGetNumBytes(&wiFiConfiguration[n]);
            }
        }
        
        err = HAPStringWithFormat(&wiFiConfiguration[n], sizeof wiFiConfiguration - n, "\n");
        HAPAssert(!err);
        n += HAPStringGetNumBytes(&wiFiConfiguration[n]);
    }
    
    // Network configuration terminator.
    err = HAPStringWithFormat(&wiFiConfiguration[n], sizeof wiFiConfiguration - n, "}\n");
    HAPAssert(!err);
    n += HAPStringGetNumBytes(&wiFiConfiguration[n]);
    
    // Update Wi-Fi configuration.
    UpdateWiFiConfiguration(wiFiManager, wiFiConfiguration);
}

void HAPPlatformWiFiManagerRemoveConfiguration(
    HAPPlatformWiFiManagerRef wiFiManager)
{
    HAPPrecondition(wiFiManager);
    
    HAPError err;
    
    HAPLogDebug(&logObject, "%s", __func__);
    
    // Get regulatory domain.
    HAPPlatformRegulatoryDomain regulatoryDomain;
    err = HAPPlatformWiFiManagerGetRegulatoryDomain(wiFiManager, &regulatoryDomain);
    if (err) {
        HAPLogError(&logObject,
            "%s: Regulatory domain has not been configured in wpa_supplicant configuration.", __func__);
        HAPFatalError();
    }
    
    // Initial configuration file.
    char wiFiConfiguration[512];
    err = HAPStringWithFormat(wiFiConfiguration, sizeof wiFiConfiguration,
        "country=%s\n"
        "ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev\n"
        "update_config=1\n", regulatoryDomain.stringValue);
    HAPAssert(!err);
    
    // Update Wi-Fi configuration.
    UpdateWiFiConfiguration(wiFiManager, wiFiConfiguration);
}
