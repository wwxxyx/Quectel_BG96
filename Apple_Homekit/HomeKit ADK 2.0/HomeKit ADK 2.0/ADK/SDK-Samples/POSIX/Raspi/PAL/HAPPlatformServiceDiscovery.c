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

#include <arpa/inet.h>
#include <unistd.h>

#include "HAPPlatform+Init.h"
#include "HAPPlatformServiceDiscovery+Init.h"

static const HAPLogObject logObject = {
    .subsystem = kHAPPlatform_LogSubsystem,
    .category = "ServiceDiscovery"
};

// TODO Add support for re-registering service discovery in case of error while app is running.

static const char *DNSServiceErrorTypeToString(DNSServiceErrorType errorCode) {
    switch (errorCode) {
        case kDNSServiceErr_AlreadyRegistered: return "kDNSServiceErr_AlreadyRegistered";
        case kDNSServiceErr_BadFlags: return "kDNSServiceErr_BadFlags";
        case kDNSServiceErr_BadInterfaceIndex: return "kDNSServiceErr_BadInterfaceIndex";
        case kDNSServiceErr_BadKey: return "kDNSServiceErr_BadKey";
        case kDNSServiceErr_BadParam: return "kDNSServiceErr_BadParam";
        case kDNSServiceErr_BadReference: return "kDNSServiceErr_BadReference";
        case kDNSServiceErr_BadSig: return "kDNSServiceErr_BadSig";
        case kDNSServiceErr_BadState: return "kDNSServiceErr_BadState";
        case kDNSServiceErr_BadTime: return "kDNSServiceErr_BadTime";
        case kDNSServiceErr_DoubleNAT: return "kDNSServiceErr_DoubleNAT";
        case kDNSServiceErr_Firewall: return "kDNSServiceErr_Firewall";
        case kDNSServiceErr_Incompatible: return "kDNSServiceErr_Incompatible";
        case kDNSServiceErr_Invalid: return "kDNSServiceErr_Invalid";
        case kDNSServiceErr_NATPortMappingDisabled: return "kDNSServiceErr_NATPortMappingDisabled";
        case kDNSServiceErr_NATPortMappingUnsupported: return "kDNSServiceErr_NATPortMappingUnsupported";
        case kDNSServiceErr_NATTraversal: return "kDNSServiceErr_NATTraversal";
        case kDNSServiceErr_NameConflict: return "kDNSServiceErr_NameConflict";
        case kDNSServiceErr_NoAuth: return "kDNSServiceErr_NoAuth";
        case kDNSServiceErr_NoError: return "kDNSServiceErr_NoError";
        case kDNSServiceErr_NoMemory: return "kDNSServiceErr_NoMemory";
        case kDNSServiceErr_NoRouter: return "kDNSServiceErr_NoRouter";
        case kDNSServiceErr_NoSuchKey: return "kDNSServiceErr_NoSuchKey";
        case kDNSServiceErr_NoSuchName: return "kDNSServiceErr_NoSuchName";
        case kDNSServiceErr_NoSuchRecord: return "kDNSServiceErr_NoSuchRecord";
        case kDNSServiceErr_NotInitialized: return "kDNSServiceErr_NotInitialized";
        case kDNSServiceErr_PollingMode: return "kDNSServiceErr_PollingMode";
        case kDNSServiceErr_Refused: return "kDNSServiceErr_Refused";
        case kDNSServiceErr_ServiceNotRunning: return "kDNSServiceErr_ServiceNotRunning";
        case kDNSServiceErr_Timeout: return "kDNSServiceErr_Timeout";
        case kDNSServiceErr_Transient: return "kDNSServiceErr_Transient";
        case kDNSServiceErr_Unknown: return "kDNSServiceErr_Unknown";
        case kDNSServiceErr_Unsupported: return "kDNSServiceErr_Unsupported";
        default: return "Unknown kDNSServiceErr";
    }
}

static void HandleFileHandleCallback(
    HAPPlatformFileHandleRef fileHandle,
    HAPPlatformFileHandleEvent fileHandleEvents,
    void *_Nullable context)
{
    HAPAssert(fileHandle);
    HAPAssert(fileHandleEvents.isReadyForReading);
    HAPAssert(context);

    HAPPlatformServiceDiscoveryRef serviceDiscovery = context;

    HAPAssert(serviceDiscovery->fileHandle == fileHandle);

    DNSServiceErrorType errorCode = DNSServiceProcessResult(serviceDiscovery->dnsService);
    if (errorCode != kDNSServiceErr_NoError) {
        HAPLogError(&logObject, "%s: Service discovery results processing failed: %ld, %s",
            __func__, (long) errorCode, DNSServiceErrorTypeToString(errorCode));
        HAPFatalError();
    }
}

static void HandleServiceRegisterReply(
    DNSServiceRef service HAP_UNUSED,
    DNSServiceFlags flags HAP_UNUSED,
    DNSServiceErrorType errorCode,
    const char *name HAP_UNUSED,
    const char *regtype HAP_UNUSED,
    const char *domain HAP_UNUSED,
    void *context_ HAP_UNUSED)
{
    if (errorCode != kDNSServiceErr_NoError) {
        HAPLogError(&logObject, "%s: Service discovery registration failed: %ld, %s",
            __func__, (long) errorCode, DNSServiceErrorTypeToString(errorCode));
        HAPFatalError();
    }
}

void HAPPlatformServiceDiscoveryCreate(
    HAPPlatformServiceDiscoveryRef serviceDiscovery,
    const HAPPlatformServiceDiscoveryOptions *options)
{
    HAPPrecondition(serviceDiscovery);
    HAPPrecondition(options);

    HAPLog(&logObject, "Storage configuration: serviceDiscovery = %lu",
        (unsigned long) sizeof *serviceDiscovery);

    HAPRawBufferZero(serviceDiscovery, sizeof *serviceDiscovery);

    if (options->interfaceName) {
        size_t numInterfaceNameBytes = HAPStringGetNumBytes(HAPNonnull(options->interfaceName));
        if ((numInterfaceNameBytes == 0) || (numInterfaceNameBytes >= sizeof serviceDiscovery->interfaceName)) {
            HAPLogError(&logObject, "Invalid local network interface name.");
            HAPFatalError();
        }
        HAPRawBufferCopyBytes(
            serviceDiscovery->interfaceName, HAPNonnull(options->interfaceName), numInterfaceNameBytes);
    }

    serviceDiscovery->dnsService = NULL;
}

void HAPPlatformServiceDiscoveryRegister(
    HAPPlatformServiceDiscoveryRef serviceDiscovery,
    const char *name,
    const char *protocol,
    uint16_t port,
    HAPPlatformServiceDiscoveryTXTRecord *txtRecords,
    size_t numTXTRecords)
{
    HAPPrecondition(serviceDiscovery);
    HAPPrecondition(!serviceDiscovery->dnsService);
    HAPPrecondition(name);
    HAPPrecondition(protocol);
    HAPPrecondition(txtRecords);

    HAPError err;
    DNSServiceErrorType errorCode;

    uint32_t interfaceIndex;
    if (serviceDiscovery->interfaceName[0]) {
        unsigned int i = if_nametoindex(serviceDiscovery->interfaceName);
        if ((i == 0) || (i > UINT32_MAX)) {
            HAPLogError(&logObject, "Mapping the local network interface name to its corresponding index failed.");
            HAPFatalError();
        }
        interfaceIndex = (uint32_t) i;
    } else {
        interfaceIndex = 0;
    }

    if (HAVE_WAC && HAPStringAreEqual(protocol, "_mfi-config._tcp") && interfaceIndex) {
        // We noticed that service registration has no effect immediately after a change in the network interface
        // configuration, e.g., when switching in and out of a software access point for WAC. A delay of about 15
        // seconds seems to circumvent this problem.

        HAPLogInfo(&logObject, "Delaying service registration ...");
        unsigned int delay = 15 /* seconds */;
        do {
            delay = sleep(delay);
        } while (delay);
    }

    HAPLogDebug(&logObject, "interfaceIndex: %u", (unsigned int) interfaceIndex);
    HAPLogDebug(&logObject, "name: \"%s\"", name);
    HAPLogDebug(&logObject, "protocol: \"%s\"", protocol);
    HAPLogDebug(&logObject, "port: %u", (unsigned int) port);

    TXTRecordCreate(&serviceDiscovery->txtRecord,
        sizeof serviceDiscovery->txtRecordBuffer,
        &serviceDiscovery->txtRecordBuffer[0]);
    for (size_t i = 0; i < numTXTRecords; i++) {
        HAPPrecondition(!txtRecords[i].value.numBytes || txtRecords[i].value.bytes);
        HAPPrecondition(txtRecords[i].value.numBytes <= UINT8_MAX);
        if (txtRecords[i].value.bytes) {
            HAPLogBufferDebug(&logObject, txtRecords[i].value.bytes, txtRecords[i].value.numBytes,
                "txtRecord[%lu]: \"%s\"", (unsigned long) i, txtRecords[i].key);
        } else {
            HAPLogDebug(&logObject, "txtRecord[%lu]: \"%s\"", (unsigned long) i, txtRecords[i].key);
        }
        errorCode = TXTRecordSetValue(&serviceDiscovery->txtRecord,
            txtRecords[i].key,
            (uint8_t) txtRecords[i].value.numBytes,
            txtRecords[i].value.bytes);
        if (errorCode != kDNSServiceErr_NoError) {
            HAPLogError(&logObject, "%s: TXTRecordSetValue failed: %ld, %s",
                __func__, (long) errorCode, DNSServiceErrorTypeToString(errorCode));
            HAPFatalError();
        }
    }

    errorCode = DNSServiceRegister(&serviceDiscovery->dnsService,
        /* flags: */ 0, interfaceIndex, name, protocol, /* domain: */ NULL, /* host: */ NULL, htons(port),
        TXTRecordGetLength(&serviceDiscovery->txtRecord), TXTRecordGetBytesPtr(&serviceDiscovery->txtRecord),
        HandleServiceRegisterReply, serviceDiscovery);
    if (errorCode != kDNSServiceErr_NoError) {
        HAPLogError(&logObject, "%s: DNSServiceRegister failed: %ld, %s.",
            __func__, (long) errorCode, DNSServiceErrorTypeToString(errorCode));
        HAPFatalError();
    }

    err = HAPPlatformFileHandleRegister(&serviceDiscovery->fileHandle,
        DNSServiceRefSockFD(serviceDiscovery->dnsService),
        (HAPPlatformFileHandleEvent) {
            .isReadyForReading = true,
            .isReadyForWriting = false,
            .hasErrorConditionPending = false
        },
        HandleFileHandleCallback, serviceDiscovery);
    if (err) {
        HAPLogError(&logObject, "%s: HAPPlatformFileHandleRegister failed: %u.",
            __func__, err);
        HAPFatalError();
    }
    HAPAssert(serviceDiscovery->fileHandle);
}

void HAPPlatformServiceDiscoveryUpdateTXTRecords(
    HAPPlatformServiceDiscoveryRef serviceDiscovery,
    HAPPlatformServiceDiscoveryTXTRecord *txtRecords,
    size_t numTXTRecords)
{
    HAPPrecondition(serviceDiscovery);
    HAPPrecondition(serviceDiscovery->dnsService);
    HAPPrecondition(txtRecords);

    DNSServiceErrorType errorCode;

    TXTRecordDeallocate(&serviceDiscovery->txtRecord);
    TXTRecordCreate(&serviceDiscovery->txtRecord,
        sizeof serviceDiscovery->txtRecordBuffer,
        &serviceDiscovery->txtRecordBuffer[0]);
    for (size_t i = 0; i < numTXTRecords; i++) {
        HAPPrecondition(!txtRecords[i].value.numBytes || txtRecords[i].value.bytes);
        HAPPrecondition(txtRecords[i].value.numBytes <= UINT8_MAX);
        if (txtRecords[i].value.bytes) {
            HAPLogBufferDebug(&logObject, txtRecords[i].value.bytes, txtRecords[i].value.numBytes,
                "txtRecord[%lu]: \"%s\"", (unsigned long) i, txtRecords[i].key);
        } else {
            HAPLogDebug(&logObject, "txtRecord[%lu]: \"%s\"", (unsigned long) i, txtRecords[i].key);
        }
        errorCode = TXTRecordSetValue(&serviceDiscovery->txtRecord,
            txtRecords[i].key,
            (uint8_t) txtRecords[i].value.numBytes,
            txtRecords[i].value.bytes);
        if (errorCode != kDNSServiceErr_NoError) {
            HAPLogError(&logObject, "%s: TXTRecordSetValue failed: %ld, %s.",
                __func__, (long) errorCode, DNSServiceErrorTypeToString(errorCode));
            HAPFatalError();
        }
    }

    errorCode = DNSServiceUpdateRecord(serviceDiscovery->dnsService,
        /* recordRef: */ NULL, /* flags: */ 0,
        TXTRecordGetLength(&serviceDiscovery->txtRecord),
        TXTRecordGetBytesPtr(&serviceDiscovery->txtRecord),
        0);
    if (errorCode != kDNSServiceErr_NoError) {
        HAPLogError(&logObject, "%s: DNSServiceUpdateRecord failed: %ld, %s.",
            __func__, (long) errorCode, DNSServiceErrorTypeToString(errorCode));
        HAPFatalError();
    }
}

void HAPPlatformServiceDiscoveryStop(
    HAPPlatformServiceDiscoveryRef serviceDiscovery)
{
    HAPPrecondition(serviceDiscovery);
    HAPPrecondition(serviceDiscovery->dnsService);

    HAPPlatformFileHandleDeregister(serviceDiscovery->fileHandle);

    DNSServiceRefDeallocate(serviceDiscovery->dnsService);
    serviceDiscovery->dnsService = NULL;

    TXTRecordDeallocate(&serviceDiscovery->txtRecord);
}
