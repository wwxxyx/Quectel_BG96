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

#ifndef SIRI_H
#define SIRI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "HAP.h"
#include "HAPPlatformMicrophone.h"

#if __has_feature(nullability)
#pragma clang assume_nonnull begin
#endif

/**
 * The Siri context associated with a data stream.
 */
typedef struct {
    HAPAccessoryServerRef *server;
    HAPDataStreamDispatcherRef *dispatcher;
    HAPDataStreamHandle dataStream;
    HAPDataSendDataStreamProtocolStreamRef dataSendStream;
    HAPPlatformTimerRef timeoutTimer;
    bool busy : 1; // A transaction is in progress.
} HAPSiriDataStreamContext;

/**
 * The Siri context associated with an audio input.
 */
typedef struct {
    HAPPlatformMicrophoneRef microphone;
    HAPPlatformMicrophoneStreamRef _Nullable microphoneStream;
    HAPAudioCodecParameters codecParameters;
    uint8_t scratchBytes[2048];
    struct {
        uint8_t bytes[128];
        size_t numBytes;
        float rms;
    } circularBuffer[1024];
    volatile size_t head, tail;
    uint32_t sequenceNumber;
    HAPSiriDataStreamContext *_Nullable activeStream;
    HAPSiriDataStreamContext *_Nullable newStream;
    bool isSampling : 1;   // Sampling audio data.
    bool endOfStream : 1;  // Last packet sent.
} HAPSiriAudioContext;

/**
 * HomeKit Siri callbacks.
 */
typedef struct {
    /**
     * The callback used to handle an accepted Siri Data Stream.
     *
     * @param      server               Accessory server.
     * @param      request              Request that originated the HomeKit Data Stream.
     */
    void (*handleSiriAccept)(
        HAPAccessoryServerRef *server,
        const HAPServiceRequest *request);
    
    /**
     * The callback used when a Siri Data Stream is invalidated.
     *
     * @param      server               Accessory server.
     * @param      request              Request that originated the HomeKit Data Stream.
     */
    void (*handleSiriInvalidate)(
        HAPAccessoryServerRef *server,
        const HAPServiceRequest *request);
    
    /**
     * The callback used to get the Siri contexts.
     *
     * @param      server               Accessory server.
     * @param      request              Request that originated the HomeKit Data Stream.
     * @param[out] audioContext         The associated Siri audio context.
     * @param[out] dataStreamContext    The associated Siri data stream context.
     */
    void (*getSiriContexts)(
        HAPAccessoryServerRef *server,
        const HAPServiceRequest *request,
        HAPSiriAudioContext *_Nonnull *_Nullable audioContext,
        HAPSiriDataStreamContext *_Nonnull *_Nullable dataStreamContext);
} HAPSiriDelegate;

/**
 * Install the callback delegate.
 *
 * @param      siriDelegate             The delaget to use.
 */
void SiriSetDelegate(
    const HAPSiriDelegate *siriDelegate);

/**
 * Initialize the Siri audio configuration.
 *
 * @param      audioContext             The Siri audio context.
 * @param      microphone               Microphone
 */
void SiriInitializeAudioConfiguration(
    HAPSiriAudioContext *audioContext,
    HAPPlatformMicrophoneRef microphone);

/**
 * Starts a Siri audio transfer.
 *
 * @param      audioContext             The Siri audio context.
 * @param      dataStreamContext        The Siri data stream context.
 */
void SiriInputStart(
    HAPSiriAudioContext *audioContext,
    HAPSiriDataStreamContext *dataStreamContext);

/**
 * Stops a Siri audio transfer.
 *
 * @param      audioContext             The Siri audio context.
 * @param      dataStreamContext        The Siri data stream context.
 */
void SiriInputStop(
    HAPSiriAudioContext *audioContext,
    HAPSiriDataStreamContext *dataStreamContext);

// HomeKit Data Stream protocol handlers.

/**
 * The callback used to handle accepted HomeKit Data Streams.
 */
void SiriHandleDataSendAccept(
    HAPAccessoryServerRef *server,
    HAPDataStreamDispatcherRef *dispatcher,
    HAPDataSendDataStreamProtocol *dataStreamProtocol,
    const HAPServiceRequest *request,
    HAPDataStreamHandle dataStream,
    void *_Nullable context);

/**
 * The callback used when a HomeKit Data Stream send request transaction is invalidated before completion.
 */
void SiriHandleDataSendInvalidate(
    HAPAccessoryServerRef *server,
    HAPDataStreamDispatcherRef *dispatcher,
    HAPDataSendDataStreamProtocol *dataStreamProtocol,
    const HAPServiceRequest *request,
    HAPDataStreamHandle dataStream,
    void *_Nullable context);

// Request handlers.

/**
 * Handle read request to the 'Siri Input Type' characteristic of the Siri service.
 */
HAP_RESULT_USE_CHECK
HAPError HandleSiriInputTypeRead(
    HAPAccessoryServerRef *server_,
    const HAPUInt8CharacteristicReadRequest *request,
    uint8_t *value,
    void *_Nullable context);

/**
 * Handle read request to the 'Supported Audio Stream Configuration' characteristic
 * of the Audio Stream Management service.
 */
HAP_RESULT_USE_CHECK
HAPError HandleAudioStreamManagementSupportedAudioStreamConfigurationRead(
    HAPAccessoryServerRef *server,
    const HAPTLV8CharacteristicReadRequest *request,
    HAPTLVWriterRef *responseWriter,
    void *_Nullable context);

/**
 * Handle read request to the 'Selected Audio Stream Configuration' characteristic
 * of the Audio Stream Management service.
 */
HAP_RESULT_USE_CHECK
HAPError HandleAudioStreamManagementSelectedAudioStreamConfigurationRead(
    HAPAccessoryServerRef *server,
    const HAPTLV8CharacteristicReadRequest *request,
    HAPTLVWriterRef *responseWriter,
    void *_Nullable context);

/**
 * Handle write request to the 'Selected Audio Stream Configuration' characteristic
 * of the Audio Stream Management service.
 */
HAP_RESULT_USE_CHECK
HAPError HandleAudioStreamManagementSelectedAudioStreamConfigurationWrite(
    HAPAccessoryServerRef *server_,
    const HAPTLV8CharacteristicWriteRequest *request,
    HAPTLVReaderRef *requestReader,
    void *_Nullable context);

#if __has_feature(nullability)
#pragma clang assume_nonnull end
#endif

#ifdef __cplusplus
}
#endif

#endif
