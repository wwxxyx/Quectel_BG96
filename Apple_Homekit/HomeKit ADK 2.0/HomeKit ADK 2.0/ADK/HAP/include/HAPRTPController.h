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

#ifndef HAP_RTP_CONTROLLER_H
#define HAP_RTP_CONTROLLER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "HAPPlatform.h"

#if __has_feature(nullability)
#pragma clang assume_nonnull begin
#endif

/**
 * Packet log object.
 */
extern const HAPLogObject kHAPRTPController_PacketLog;

/**
 * Nanosecond epoch time counter ([ns] since 1970).
 */
typedef uint64_t HAPEpochTime;

/**
 * Nanosecond time values.
 */
#define HAPNanosecondNS    (1ull)
#define HAPMicrosecondNS   (1000ull * HAPNanosecondNS)
#define HAPMillisecondNS   (1000ull * HAPMicrosecondNS)
#define HAPSecondNS        (1000ull * HAPMillisecondNS)

/**
 * RTP Encode type
 */
HAP_ENUM_BEGIN(uint8_t, HAPRTPEncodeType) {
    /** No interpretation of of payload data needed. */
    kHAPRTPEncodeType_Simple,
    
    /** H264 NAL units. */
    kHAPRTPEncodeType_H264
} HAP_ENUM_END(uint8_t, HAPRTPEncodeType);

/**
 * HAP RTP Stream
 */
typedef HAP_OPAQUE(864) HAPRTPStreamRef;

////////////////////////////////////////////////////////////////////////////////////////////////////
/* Controller Configuration */

/**
 * Start the HAP RTP Stream
 *
 * @param[out] stream               The rtp stream
 * @param      rtpParameters        RTP parameters
 * @param      encodeType           Encoding type
 * @param      clockFrequency       Timestamp clock frequency [Hz]
 * @param      localSSRC            Synchronization source ID of the accessory
 * @param      startTime            The start time of the stream ([ns] since 1970)
 * @param      cnameString          Canonical name of the device (for the CNAME field of the sender report)
 * @param      srtpInParameters     Input parameters for SRTP
 * @param      srtpOutParameters    Output parameters for SRTP
 *
 * @return kHAPError_None           If successful.
 */
HAP_RESULT_USE_CHECK
HAPError HAPRTPStreamStart(
    HAPRTPStreamRef *stream,
    const HAPPlatformCameraRTPParameters *rtpParameters,
    HAPRTPEncodeType encodeType,
    uint32_t clockFrequency,
    uint32_t localSSRC,
    HAPEpochTime startTime,
    const char *cnameString,
    const HAPPlatformCameraSRTPParameters *srtpInParameters,
    const HAPPlatformCameraSRTPParameters *srtpOutParameters);
    
/**
 * Reconfigure a HAP SRTP stream.
 *
 * @param      stream               The SRTP stream to be reconfigured.
 * @param      bitRate              Bit rate of the stream  [bit/s].
 * @param      rtcpInterval         The RTCP interval [ms]. Set to 0, to keep the current interval.
 */
void HAPRTPStreamReconfigure(
    HAPRTPStreamRef *stream,
    uint32_t bitRate,
    uint32_t rtcpInterval);
    
/**
 * End a HAP SRTP stream.
 *
 * @param[in,out] stream             Stream to be ended.
 */
void HAPRTPStreamEnd(
    HAPRTPStreamRef *stream);

/**
 * Set the CVO on a HAP SRTP stream.
 *
 * @param      stream               Stream where the CVO needs to be configured.
 * @param      cvoID                CVO - 1..14, 0: disable CVO.
 * @param      cvoInformation       CVO information.
 *
 * @return kHAPError_None           If successful.
 * @return kHAPError_InvalidData    If parsing failed (cvoId > 14).
 */
HAP_RESULT_USE_CHECK
HAPError HAPRTPStreamSetCVO(
    HAPRTPStreamRef *stream,
    uint8_t cvoID,
    uint8_t cvoInformation);

////////////////////////////////////////////////////////////////////////////////////////////////////
/* Sender Data */

/**
 * Push camera/microphone input to a HAP RTP stream.
 *
 * Call \fn HAPRTPStreamPollPacket() after \fn HAPRTPStreamPushPayload() until numPayloadBytes is 0. \p bytes must
 * remain valid while HAPRTPStreamPopPacket is called.
 *
 * @param      stream               Stream where payload should be pushed to.
 * @param      bytes                Payload. The pointer is stored in the stream
 *                                  until the entire output of the stream has been polled.
 * @param      numBytes             Payload length.
 * @param[out] numPayloadBytes      The size of the unformatted output.
 * @param      sampleTime           The sample time from stream start [ns].
 * @param      actualTime           The actual time ([ns] since 1970).
 */
void HAPRTPStreamPushPayload(
    HAPRTPStreamRef *stream,
    const void *bytes,
    size_t numBytes,
    size_t *numPayloadBytes,
    HAPTimeNS sampleTime,
    HAPEpochTime actualTime);

/**
 * Poll RTP packet from HAP RTP stream to be sent to network.
 *
 * Must be called repeatedly until it returns 0.
 *
 * @param      stream               Stream to read from.
 * @param[out] bytes                Buffer that will be filled with data.
 * @param      maxBytes             Maximum number of bytes that may be filled into the buffer.
 * @param[out] numBytes             Effective number of bytes written to the buffer.
 */
void HAPRTPStreamPollPacket(
    HAPRTPStreamRef *stream,
    void *bytes,
    size_t maxBytes,
    size_t *numBytes);

////////////////////////////////////////////////////////////////////////////////////////////////////
/* Receiver Data */

/**
 * Push RTP Packet from network to HAP RTP stream.
 *
 * Call \fn HAPRTPStreamPollPayload() after \fn HAPRTPStreamPushPacket(). \p bytes must remain valid
 * while \fn HAPRTPStreamPollPayload() is called.
 *
 * @param      stream               Stream to write network data into.
 * @param      bytes                Received RTP packet. The pointer is stored in the stream
 *                                  until the entire output of the stream has been polled.
 *                                  The data may be overwritten.
 * @param      numBytes             Length of the packet.
 * @param[out] numPayloadBytes      Number of payload bytes in the packet.
 * @param      actualTime           The actual time ([ns] since 1970).
 */
void HAPRTPStreamPushPacket(
    HAPRTPStreamRef *stream,
    void *bytes,
    size_t numBytes,
    size_t *numPayloadBytes,
    HAPEpochTime actualTime);

/**
 * Poll a payload block from the HAP RTP stream (speaker output).
 *
 * @param      stream               HAP RTP stream to read from.
 * @param[out] bytes                Buffer that will be filled with data.
 * @param      maxBytes             Maximum number of bytes that may be filled into the buffer.
 * @param[out] numBytes             Effective number of bytes written to the buffer.
 * @param[out] sampleTime           The sample time of the payload, [ns] from stream start.
 */
void HAPRTPStreamPollPayload(
    HAPRTPStreamRef *stream,
    void *bytes,
    size_t maxBytes,
    size_t *numBytes,
    HAPTimeNS *sampleTime);

////////////////////////////////////////////////////////////////////////////////////////////////////
/* Control Data */

/**
 * Check the feedback of a HAP RTP stream.
 *
 * - Generate a RTCP feedback packet when needed.
 * - Trigger a new keyframe.
 * - Adapt bitrate when needed.
 * - Check for timeout.
 *
 * @param      stream               HAP RTP stream.
 * @param      actualTime           Actual time of the stream ([ns] since 1970).
 * @param[out] bytes                Packet buffer for feedback output packet.
 * @param      maxBytes             Maximum number of bytes that may be filled into the buffer.
 * @param[out] numBytes             Effective number of bytes written to the buffer.
 * @param[out] bitRate              Check and modify bitrate, if \p bitrate == 0 the bitrate is
 *                                  not modified.
 * @param[out] newKeyFrame          Check and trigger keyframe, if \p newKeyFrame == false no keyframe
 *                                  is triggered.
 *                                  if *(\p newKeyFrame) == true a keyframe is triggered.
 * @param[out] dropoutTime          Time since last feedback received [s].
 *                                  Can be used to trigger a timeout.
 */
void HAPRTPStreamCheckFeedback(
    HAPRTPStreamRef *stream,
    HAPEpochTime actualTime,
    void *bytes,
    size_t maxBytes,
    size_t *numBytes,
    uint32_t *_Nullable bitRate,
    bool *_Nullable newKeyFrame,
    uint32_t *_Nullable dropoutTime);

#if __has_feature(nullability)
#pragma clang assume_nonnull end
#endif

#ifdef __cplusplus
}
#endif

#endif
