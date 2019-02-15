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

// This PAL module is making use of GStreamer to obtain audio and video data. https://gstreamer.freedesktop.org/
// Further documentation on GStreamer usage: https://gstreamer.freedesktop.org/data/doc/gstreamer/head/manual/manual.pdf

#include <fcntl.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <pthread.h>

#include "HAP.h"
#include "HAPPlatformCamera+Init.h"
#include "HAPPlatformLog+Init.h"

static const HAPLogObject logObject = {
    .subsystem = kHAPPlatform_LogSubsystem,
    .category = "Camera"
};

/**
 * Camera logging subsystem.
 *
 * It is possible to direct logging output to different log handlers. The log handlers need to be configured in
 * @fn HAPPlatformLogCapture.
 * Usage example:
 
    @code{.c}
    
    // Log an info-level buffer in the packet logger.
    HAPLogBufferInfo(&kHAPRTPController_PacketLog, bytes, numBytes, "(%p) Packet log", (const void *) stream);
    
    // Log an info-level video payload in the video logger
    HAPLogBufferInfo(&kHAPPlatformCamera_VideoLog, bytes, numBytes, "(%p) Video log", (const void *) stream);

    // Log an info-level audio payload in the audio logger
    HAPLogBufferInfo(&kHAPPlatformCamera_AudioLog, bytes, numBytes, "(%p) Audio log", (const void *) stream);
    
    // Log an info-level audio payload in the speaker logger
    HAPLogBufferInfo(&kHAPPlatformCamera_SpeakerLog, bytes, numBytes, "(%p) Speaker log", (const void *) stream);
    @endcode
 */
/**@{ */
const HAPLogObject kHAPPlatformCamera_VideoLog = {
    .subsystem = kHAPPlatform_LogSubsystem,
    .category = "Camera"
};

const HAPLogObject kHAPPlatformCamera_AudioLog = {
    .subsystem = kHAPPlatform_LogSubsystem,
    .category = "Camera"
};

const HAPLogObject kHAPPlatformCamera_SpeakerLog = {
    .subsystem = kHAPPlatform_LogSubsystem,
    .category = "Camera"
};
/**@} */

/**
 * Maximum RTP packet size
 */
#define kMaxPacketSize          (4096)

/**
 * H264 Timestamp Frequency
 *
 * Given by RFC6184
 */
#define kH264TimestampFrequency (90000)

/**
 * Socket timeout [us] (100 ms)
 */
#define kSocketTimeoutUs        (100000)

/**
 * Stream timeout (30 s)
 */
#define kStreamTimeout          (30)

/**
 * Accessory port video
 */
#define kHAPAccessoryPort_Video ((uint16_t) 5010)

/**
 * Accessory port audio
 */
#define kHAPAccessoryPort_Audio ((uint16_t) 5012)

/**
 * Actual epoch time [ns since 1970]
 *
 * @return epoch time [ns] since 1970
 */
static HAPEpochTime ActualTime()
{
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);
    uint32_t sec = (uint32_t)spec.tv_sec;
    uint32_t ns = (uint32_t)spec.tv_nsec;
    return (HAPEpochTime)sec * HAPSecondNS + ns;
}

/**
 * Video data handler.
 *
 * @param      stream               Video stream.
 * @param      bytes                Video data.
 * @param      numBytes             Length of video data.
 * @param      sampleTime           Sample time of first data entry [ns].
 */
static void VideoStreamCallback(
    HAPPlatformVideoStream *videoStream,
    uint8_t *bytes,
    size_t numBytes,
    HAPTimeNS sampleTime)
{
    HAPPrecondition(videoStream);
    HAPPrecondition(bytes);

    HAPPlatformCameraStream *stream = (HAPPlatformCameraStream *) videoStream->context;
    HAPAssert(stream->initialized);
    HAPRTPStreamRef *rtpStream = &stream->rtpStream;

    // Log payload.
    HAPLogBufferInfo(&kHAPPlatformCamera_VideoLog, bytes, numBytes, "(%p) V", (const void *) stream);

    // Push video data into RTP buffer.
    size_t numPayloadBytes;
    HAPRTPStreamPushPayload(rtpStream, bytes, numBytes, &numPayloadBytes, sampleTime, ActualTime());
    
    // Send data if available.
    if (numPayloadBytes) {
        for (;;) {
            uint8_t packet[kMaxPacketSize];
            size_t length;
            HAPRTPStreamPollPacket(rtpStream, packet, sizeof packet, &length);
            if (length == 0) {
                break;
            }

            HAPAssert(stream->socket >= 0);
            (void) send(stream->socket, packet, length, 0);
        }
    }
}

/**
 * Audio data handler.
 *
 * @param      stream               Audio stream.
 * @param      bytes                Audio data.
 * @param      numBytes             Length of audio data.
 * @param      sampleTime           Sample time of first data entry [ns].
 * @param      rms                  RMS of raw audio data in 1.15 fixpoint format.
 */
static void AudioStreamCallback(
    HAPPlatformAudioStream *audioStream,
    uint8_t bytes[],
    size_t numBytes,
    HAPTimeNS sampleTime,
    float rms HAP_UNUSED)
{
    HAPPrecondition(audioStream);
    HAPPrecondition(bytes);

    HAPPlatformCameraStream *stream = (HAPPlatformCameraStream *) audioStream->context;
    HAPAssert(stream->initialized);
    HAPRTPStreamRef *rtpStream = &stream->rtpStream;

    // Log payload.
    HAPLogBufferInfo(&kHAPPlatformCamera_AudioLog, bytes, numBytes, "(%p) A", (const void *) stream);

    // Push audio sample into RTP buffer.
    size_t numPayloadBytes;
    HAPRTPStreamPushPayload(rtpStream, bytes, numBytes, &numPayloadBytes, sampleTime, ActualTime());
    if (numPayloadBytes) {
        // Send data if available.
        uint8_t packet[kMaxPacketSize];
        for (;;) {
            size_t length;
            HAPRTPStreamPollPacket(rtpStream, packet, sizeof packet, &length);
            if (length == 0) {
                break;
            }

            HAPAssert(stream->socket >= 0);
            (void) send(stream->socket, packet, length, 0);
        }
    }
}

/**
 * UDP Input thread to read from UDP socket.
 *
 * Uses pthread interface.
 *
 * @param      ptr                  (void*) ptr of type HAP Camera stream.
 *
 * @return PThread return value.
 */
static void *UdpInputThread(void *ptr)
{
    HAPPrecondition(ptr);

    HAPPlatformCameraStream *stream = (HAPPlatformCameraStream*)ptr;
    HAPAssert(stream->initialized);
    HAPRTPStreamRef *rtpStream = &stream->rtpStream;

    HAPAssert(stream->socket >= 0 && stream->socket < INT_MAX);

    while (true) { // loop forever
        // wait for socket input or timeout
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(stream->socket, &fds);

        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = kSocketTimeoutUs;

        int num = select(stream->socket + 1, &fds, NULL, NULL, &timeout);
        if (num == -1 && errno == EINTR) {
            continue;
        }
        if (num < 0) {
            int _errno = errno;
            HAPAssert(num == -1);
            HAPPlatformLogPOSIXError(kHAPLogType_Error, "System call 'select' failed.",
                _errno, __func__, HAP_FILE, __LINE__);
            HAPFatalError();
        }

        uint8_t packet[kMaxPacketSize];
        size_t numPacketBytes;
        HAPEpochTime time = ActualTime();
        if (num > 0) {
            // Read socket
            if (FD_ISSET(stream->socket, &fds)) {
                ssize_t numReceivedBytes;
                do {
                    numReceivedBytes = recv(stream->socket, packet, sizeof packet, 0);
                } while ((numReceivedBytes == -1) && (errno == EINTR));

                if (numReceivedBytes > 0) {
                    // A packet can be both a stream packet or a control packet. If the size is 0, then
                    // a control packet was read. Both data and control packets are sent over the same
                    // socket.
                    HAPRTPStreamPushPacket(rtpStream, packet, (size_t)numReceivedBytes, &numPacketBytes, time);
                    
                    // Check if we received new audio data.
                    if (numPacketBytes) {
                        uint8_t packet1[kMaxPacketSize];
                        size_t numPayloadBytes;
                        HAPTimeNS sampleTime;
                        HAPRTPStreamPollPayload(rtpStream, packet1, numPacketBytes, &numPayloadBytes, &sampleTime);
                        HAPPlatformAudioPushSpeakerData(&stream->audioStream, packet1, numPayloadBytes, sampleTime);
                    }
                }
            }
        }

        // Check RTP feedback.
        time = ActualTime();
        uint32_t bitRate;
        bool newKeyFrame;
        uint32_t dropoutTime;
        HAPRTPStreamCheckFeedback(rtpStream, time,
            packet, sizeof packet, &numPacketBytes,
            &bitRate, &newKeyFrame, &dropoutTime);
        if (numPacketBytes) {
            // Send feedback.
            HAPAssert(stream->socket >= 0);
            send(stream->socket, packet, numPacketBytes, 0);
        }
        
        if (bitRate) {
            HAPPlatformVideoStreamSetBitrate(&stream->videoStream, bitRate);
        }
        if (newKeyFrame) {
            HAPPlatformVideoStreamRequestKeyFrame(&stream->videoStream);
        }
        
        // Check stream timeout (Video only).
        if (dropoutTime > kStreamTimeout && stream->camera) {
            HAPLog(&logObject, "Streaming timeout.");
            // Stop stream.
            HAPPlatformCameraEndStreamingSession(
                stream->camera,
                stream->streamIndex);
            HAPError err = HAPPlatformCameraTrySetStreamStatus(
                stream->camera,
                stream->streamIndex,
                kHAPCameraStreamingStatus_Available);
            HAPAssert(err == kHAPError_None);
            return NULL; // Terminate thread.
        }
    }

    return NULL;
}

/**
 * Initialize snapshot storage.
 *
 * @param[out] camera               HAP Camera.
 */
static void InitSnapshots(
    HAPPlatformCameraRef camera)
{
    // Initialize snapshots.
    for (size_t i = 0; i < kIPCamera_MaxSnapshots; i++) {
        HAPPlatformVideoInitializeSnapshot(&camera->snapshots[i]);
    }
}

void HAPPlatformCameraCreate(
    HAPPlatformCameraRef camera,
    const HAPPlatformCameraOptions *options)
{
    HAPPrecondition(camera);
    HAPPrecondition(options);
    HAPPrecondition(options->streamingSessionStorage);

    HAPRawBufferZero(camera, sizeof *camera);

    if (options->interfaceName) {
        size_t numInterfaceNameBytes = HAPStringGetNumBytes(HAPNonnull(options->interfaceName));
        if ((numInterfaceNameBytes == 0) || (numInterfaceNameBytes >= sizeof camera->interfaceName)) {
            HAPLogError(&logObject, "Invalid local network interface name.");
            HAPFatalError();
        }
        HAPRawBufferCopyBytes(
            camera->interfaceName, HAPNonnull(options->interfaceName), numInterfaceNameBytes);
    }

    camera->streamingSessionStorage = *options->streamingSessionStorage;

    // Initialize stream storage.
    for (size_t i = 0; i < camera->streamingSessionStorage.numSessions; i++) {
        HAPRawBufferZero(&camera->streamingSessionStorage.sessions[i],
            sizeof camera->streamingSessionStorage.sessions[i]);
    }

    HAPLogInfo(&logObject, "%s", __func__);

    InitSnapshots(camera);

    HAPError err;
    err = HAPPlatformVideoStreamInitialize();
    HAPAssert(err == kHAPError_None);
    err = HAPPlatformAudioStreamInitialize();
    HAPAssert(err == kHAPError_None);
}

void HAPPlatformCameraRelease(
    HAPPlatformCameraRef camera)
{
    HAPPrecondition(camera);
    
    // Cleanup pipelines.
    for (size_t i = 0; i < camera->streamingSessionStorage.numSessions; i++) {
        HAPPlatformCameraEndStreamingSession(camera, i);
    }

    HAPLogInfo(&logObject, "%s", __func__);
    
    HAPPlatformVideoStreamCleanup();
    HAPPlatformAudioStreamCleanup();
}

void HAPPlatformCameraSetDelegate(
    HAPPlatformCameraRef camera,
    const HAPPlatformCameraDelegate *_Nullable delegate)
{
    HAPPrecondition(camera);

    if (delegate) {
        camera->delegate = *delegate;
    } else {
        HAPRawBufferZero(&camera->delegate, sizeof camera->delegate);
    }
}

HAP_RESULT_USE_CHECK
HAPCameraStreamingStatus HAPPlatformCameraGetStreamStatus(
    HAPPlatformCameraRef camera,
    size_t streamIndex)
{
    HAPPrecondition(camera);
    HAPPrecondition(streamIndex < camera->streamingSessionStorage.numSessions);

    HAPPlatformCameraStreamingSession *session = &camera->streamingSessionStorage.sessions[streamIndex];
    return session->status;
}

HAP_RESULT_USE_CHECK
HAPError HAPPlatformCameraTrySetStreamStatus(
    HAPPlatformCameraRef camera,
    size_t streamIndex,
    HAPCameraStreamingStatus status)
{
    HAPPrecondition(camera);
    HAPPrecondition(streamIndex < camera->streamingSessionStorage.numSessions);

    HAPPlatformCameraStreamingSession *session = &camera->streamingSessionStorage.sessions[streamIndex];

    if ((status == kHAPCameraStreamingStatus_Unavailable &&
        session->status == kHAPCameraStreamingStatus_InUse) ||
        (status == kHAPCameraStreamingStatus_InUse &&
        session->status == kHAPCameraStreamingStatus_Unavailable)) {
        // Illegal transition.
        HAPLogInfo(&logObject, "%s %d failed", __func__, (int) streamIndex);
        return kHAPError_InvalidState;
    } else if (status != session->status) {
        session->status = status;
        if (camera->delegate.handleStreamStateChanged) {
            camera->delegate.handleStreamStateChanged(camera,
                streamIndex,
                camera->delegate.context);
        }
    }
    HAPLogInfo(&logObject, "%s %d succeeded", __func__, (int) streamIndex);
    return kHAPError_None;
}

HAP_RESULT_USE_CHECK
HAPError HAPPlatformCameraGetStreamingSessionEndpoint(
    HAPPlatformCameraRef camera,
    size_t streamIndex,
    const HAPPlatformCameraIPAddress *controllerAddress,
    HAPPlatformCameraIPAddress *accessoryAddress,
    uint16_t *videoPort,
    uint16_t *audioPort)
{
    HAPPrecondition(camera);
    HAPPrecondition(streamIndex < camera->streamingSessionStorage.numSessions);
    HAPPrecondition(controllerAddress);
    HAPPrecondition(accessoryAddress);
    HAPPrecondition(videoPort);
    HAPPrecondition(audioPort);

    struct ifaddrs *ifaddr;
    int max = -1;

    HAPLogInfo(&logObject, "%s %d", __func__, (int) streamIndex);

    // Ports.
    *videoPort = (uint16_t)(kHAPAccessoryPort_Video + 4 * streamIndex);
    *audioPort = (uint16_t)(kHAPAccessoryPort_Audio + 4 * streamIndex);

    // IP address.
    accessoryAddress->version = controllerAddress->version;
    accessoryAddress->ipString[0] = 0;

    // Search for best matching address.
    if (getifaddrs(&ifaddr) == 0) {
        for (struct ifaddrs *ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
            char ipString[INET6_ADDRSTRLEN];

            if (ifa->ifa_addr == NULL || (ifa->ifa_name[0] == 'l' && ifa->ifa_name[1] == 'o')) {
               continue;
            }
            if (camera->interfaceName[0] && !HAPStringAreEqual(camera->interfaceName, ifa->ifa_name)) {
                continue;
            }
            if (controllerAddress->version == kHAPPlatformCameraIPAddressVersion_IPv4 &&
                ifa->ifa_addr->sa_family == AF_INET)
            {
                getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in),
                    ipString, INET_ADDRSTRLEN, NULL, 0, NI_NUMERICHOST);
            } else if (controllerAddress->version == kHAPPlatformCameraIPAddressVersion_IPv6 &&
                ifa->ifa_addr->sa_family == AF_INET6)
            {
                getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in6),
                    ipString, INET6_ADDRSTRLEN, NULL, 0, NI_NUMERICHOST);
            } else {
                continue;
            }
            // Count matching characters.
            int n = 0;
            while (ipString[n] == controllerAddress->ipString[n]) n++;
            // Take best match.
            if (n > max) {
                strcpy(accessoryAddress->ipString, ipString);
                max = n;
            }
        }
        freeifaddrs(ifaddr);
    }
    if (max < 0) {
        HAPLogError(&logObject, "No IP Address found");
        return kHAPError_Unknown;
    }

    return kHAPError_None;
}

/**
 * Open a socket from the local Address to the remote address.
 *
 * @param      localAddr            Local address.
 * @param      localPort            Local port.
 * @param      remoteAddr           Remote address.
 * @param      remotePort           Remote port.
 *
 * @return The opened socket.
 */
HAP_RESULT_USE_CHECK
static int OpenSocket(
    const HAPPlatformCameraIPAddress *localAddr,
    uint16_t localPort,
    const HAPPlatformCameraIPAddress *remoteAddr,
    uint16_t remotePort)
{
    int sock;
    int res;

    if (localAddr->version == kHAPPlatformCameraIPAddressVersion_IPv6) {
        struct sockaddr_in6 addr;
        char buf[512];

        // Open socket.
        sock = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
        if (sock >= 0) {
            // Bind socket.
            HAPRawBufferZero(&addr, sizeof addr);
            addr.sin6_family = AF_INET6;
            addr.sin6_port = htons(localPort);

            res = inet_pton(AF_INET6, localAddr->ipString, &addr.sin6_addr);
            if (res == 0) {
                (void) close(sock);
                return -1;
            }

            res = bind(sock, (struct sockaddr *)&addr, sizeof addr);
            if (res < 0) {
                (void) close(sock);
                return -1;
            }

            HAPLogInfo(&logObject, "socket bound to %s:%d",
                inet_ntop(AF_INET6, &addr.sin6_addr, buf, sizeof buf),
                ntohs(addr.sin6_port));

            // Connect socket.
            HAPRawBufferZero(&addr, sizeof addr);
            addr.sin6_family = AF_INET6;
            addr.sin6_port = htons(remotePort);

            res = inet_pton(AF_INET6, remoteAddr->ipString, &addr.sin6_addr);
            if (res == 0) {
                (void) close(sock);
                return -1;
            }

            res = connect(sock, (struct sockaddr *)&addr, sizeof addr);
            if (res < 0) {
                (void) close(sock);
                return -1;
            }

            HAPLogInfo(&logObject, "socket connected to %s:%d",
                inet_ntop(AF_INET6, &addr.sin6_addr, buf, sizeof buf),
                ntohs(addr.sin6_port));
        }
    } else { // IPv4
        struct sockaddr_in addr;

        // Open socket.
        sock = socket(AF_INET, SOCK_DGRAM, 0);
        if (sock >= 0) {
            // Bind socket.
            HAPRawBufferZero(&addr, sizeof addr);
            addr.sin_family = AF_INET;
            addr.sin_port = htons(localPort);
            res = inet_aton(localAddr->ipString, &addr.sin_addr);
            if (res == 0) {
                (void) close(sock);
                return -1;
            }

            res = bind(sock, (struct sockaddr *)&addr, sizeof addr);
            if (res < 0) {
                (void) close(sock);
                return -1;
            }

            HAPLogInfo(&logObject, "socket bound to %s:%d",
                inet_ntoa(addr.sin_addr),
                ntohs(addr.sin_port));

            // Connect socket.
            HAPRawBufferZero(&addr, sizeof addr);
            addr.sin_family = AF_INET;
            addr.sin_port = htons(remotePort);

            res = inet_aton(remoteAddr->ipString, &addr.sin_addr);
            if (res == 0) {
                (void) close(sock);
                return -1;
            }

            res = connect(sock, (struct sockaddr *)&addr, sizeof addr);
            if (res < 0) {
                (void) close(sock);
                return -1;
            }

            HAPLogInfo(&logObject, "socket connected to %s:%d",
                inet_ntoa(addr.sin_addr),
                ntohs(addr.sin_port));
        }
    }

    return sock;
}

HAP_RESULT_USE_CHECK
HAPError HAPPlatformCameraStartStreamingSession(
    HAPPlatformCameraRef camera,
    size_t streamIndex,
    const HAPPlatformCameraEndpointParameters *controllerEndpoint,
    const HAPPlatformCameraEndpointParameters *accessoryEndpoint,
    const HAPPlatformCameraStartStreamingSessionConfiguration *streamConfiguration)
{
    HAPPrecondition(camera);
    HAPPrecondition(streamIndex < camera->streamingSessionStorage.numSessions);
    HAPPrecondition(controllerEndpoint);
    HAPPrecondition(accessoryEndpoint);
    HAPPrecondition(streamConfiguration);

    HAPLogInfo(&logObject, "%s %d", __func__, (int) streamIndex);

    HAPPlatformCameraStream *videoStream = &camera->streamingSessionStorage.sessions[streamIndex].video;
    HAPAssert(videoStream);
    HAPPlatformCameraStream *audioStream = &camera->streamingSessionStorage.sessions[streamIndex].audio;
    HAPAssert(audioStream);

    HAPRawBufferZero(videoStream, sizeof *videoStream);
    HAPRawBufferZero(audioStream, sizeof *audioStream);
    videoStream->socket = -1;
    audioStream->socket = -1;
    videoStream->initialized = true;
    audioStream->initialized = true;

    // Open video socket.
    int socket;
    socket = OpenSocket(
        &accessoryEndpoint->ipAddress, accessoryEndpoint->video.port,
        &controllerEndpoint->ipAddress, controllerEndpoint->video.port);
    if (socket < 0) {
        HAPLogError(&logObject, "Unable to open socket to communicate from %s port %d to %s port %d",
            accessoryEndpoint->ipAddress.ipString,
            accessoryEndpoint->video.port,
            controllerEndpoint->ipAddress.ipString,
            controllerEndpoint->video.port);
        return kHAPError_Unknown;
    }
    videoStream->socket = socket;

    // Open audio socket.
    socket = OpenSocket(
        &accessoryEndpoint->ipAddress, accessoryEndpoint->audio.port,
        &controllerEndpoint->ipAddress, controllerEndpoint->audio.port);
    if (socket < 0) {
        HAPLogError(&logObject, "Unable to open socket to communicate from %s port %d to %s port %d",
            accessoryEndpoint->ipAddress.ipString,
            accessoryEndpoint->audio.port,
            controllerEndpoint->ipAddress.ipString,
            controllerEndpoint->audio.port);
        return kHAPError_Unknown;
    }
    audioStream->socket = socket;
    
    // Fill in stream CName.
    gchar cname[INET6_ADDRSTRLEN + 10];
    snprintf(cname, sizeof cname, "IPCamera@%s", accessoryEndpoint->ipAddress.ipString);

    // Stream start time.
    HAPEpochTime startTime = ActualTime();

    // Start video RTP stream.
    HAPError err;
    err = HAPRTPStreamStart(
        &videoStream->rtpStream,
        &streamConfiguration->video.rtpParameters,
        kHAPRTPEncodeType_H264,
        kH264TimestampFrequency, // h264 RTP timestamp clock frequency.
        accessoryEndpoint->video.ssrc,
        startTime,
        cname,
        &controllerEndpoint->video.srtpParameters,
        &accessoryEndpoint->video.srtpParameters);
    if (err) {
        HAPLogError(&logObject, "Unable to start video RTP stream");
        return err;
    }

    // Start audio RTP stream.
    uint32_t timestampFrequency = 16000;
    switch (streamConfiguration->audio.codecParameters.sampleRate) {
        case kHAPAudioCodecSampleRate_8KHz:  timestampFrequency = 8000; break;
        case kHAPAudioCodecSampleRate_16KHz: timestampFrequency = 16000; break;
        case kHAPAudioCodecSampleRate_24KHz: timestampFrequency = 24000; break;
    }
    err = HAPRTPStreamStart(
        &audioStream->rtpStream,
        &streamConfiguration->audio.rtpParameters,
        kHAPRTPEncodeType_Simple, // Opus does not need any special payloading.
        timestampFrequency,       // RTP timestamp clock frequency = sample rate.
        accessoryEndpoint->audio.ssrc,
        startTime,
        cname,
        &controllerEndpoint->audio.srtpParameters,
        &accessoryEndpoint->audio.srtpParameters);
    if (err) {
        HAPLogError(&logObject, "Unable to start audio RTP stream");
        return err;
    }

    // Create video stream.
    err = HAPPlatformVideoStreamStart(
        &videoStream->videoStream,
        streamConfiguration->video.attributes.width,
        streamConfiguration->video.attributes.height,
        streamConfiguration->video.attributes.maxFrameRate,
        streamConfiguration->video.rtpParameters.maximumBitRate * 1000,
        streamConfiguration->video.codecParameters.profile,
        streamConfiguration->video.codecParameters.level,
        VideoStreamCallback,
        (void*)videoStream);
    if (err) {
        HAPLogError(&logObject, "Unable to start video stream");
        return err;
    }
    
    // Create input audio stream.
    err = HAPPlatformAudioStreamStartMicrophone(
        &audioStream->audioStream,
        streamConfiguration->audio.codecParameters.sampleRate,
        streamConfiguration->audio.codecParameters.bitRateMode,
        streamConfiguration->audio.rtpParameters.maximumBitRate * 1000,
        streamConfiguration->audio.rtpTime,
        AudioStreamCallback,
        (void*)audioStream);
    if (err) {
        HAPLogError(&logObject, "Unable to start microphone stream");
        return err;
    }
    HAPPlatformAudioStreamSetMicrophoneVolume(
        &audioStream->audioStream,
        camera->state.microphone.volume,
        camera->state.microphone.mute);

    // Create output audio stream.
    err = HAPPlatformAudioStreamStartSpeaker(
        &audioStream->audioStream,
        streamConfiguration->audio.codecParameters.sampleRate,
        streamConfiguration->audio.rtpTime);
    if (err) {
        HAPLogError(&logObject, "Unable to start speaker stream");
        return err;
    }
    HAPPlatformAudioStreamSetSpeakerVolume(
        &audioStream->audioStream,
        camera->state.speaker.volume,
        camera->state.speaker.mute);

    err = HAPPlatformVideoStreamEnable(&videoStream->videoStream);
    if (err) {
        HAPLogError(&logObject, "Unable to enable video stream");
        return err;
    }
    err = HAPPlatformAudioStreamEnableMicrophone(&audioStream->audioStream);
    if (err) {
        HAPLogError(&logObject, "Unable to enable microphone stream");
        return err;
    }
    err = HAPPlatformAudioStreamEnableSpeaker(&audioStream->audioStream);
    if (err) {
        HAPLogError(&logObject, "Unable to enable speaker stream");
        return err;
    }

    // Start video UDP polling thread.
    int result;
    videoStream->camera = camera;
    videoStream->streamIndex = streamIndex;
    result = pthread_create(&videoStream->thread, NULL, UdpInputThread, videoStream);
    HAPAssert(result == 0);
    videoStream->isActive = true;

    // Start audio UDP polling thread.
    audioStream->camera = NULL;
    result = pthread_create(&audioStream->thread, NULL, UdpInputThread, audioStream);
    HAPAssert(result == 0);
    audioStream->isActive = true;

    return kHAPError_None;
}

HAP_RESULT_USE_CHECK
HAPError HAPPlatformCameraReconfigureStreamingSession(
    HAPPlatformCameraRef camera,
    size_t streamIndex,
    const HAPPlatformCameraReconfigureStreamingSessionConfiguration *streamConfiguration)
{
    // Video only.
    HAPPrecondition(camera);
    HAPPrecondition(streamIndex < camera->streamingSessionStorage.numSessions);
    HAPPrecondition(streamConfiguration);

    HAPLogInfo(&logObject, "%s %d", __func__, (int) streamIndex);

    HAPPlatformCameraStream *videoStream = &camera->streamingSessionStorage.sessions[streamIndex].video;

    if (videoStream->initialized) {
        uint32_t bitrate = streamConfiguration->video.rtpParameters.maximumBitRate * 1000; // [bit/s]
        HAPError err;
        err = HAPPlatformVideoStreamReconfigure(
            &videoStream->videoStream,
            streamConfiguration->video.attributes.width,
            streamConfiguration->video.attributes.height,
            streamConfiguration->video.attributes.maxFrameRate,
            bitrate);
        if (err) {
            HAPLogError(&logObject, "Unable to reconfigure video stream");
            return err;
        }
        HAPRTPStreamReconfigure(
            &videoStream->rtpStream,
            bitrate,
            (uint32_t)(streamConfiguration->video.rtpParameters.minimumRTCPInterval * 1000.0F)); // [ms]
    }
    return kHAPError_None;
}

HAP_RESULT_USE_CHECK
HAPError HAPPlatformCameraSuspendStreamingSession(
    HAPPlatformCameraRef camera,
    size_t streamIndex)
{
    HAPPrecondition(camera);
    HAPPrecondition(streamIndex < camera->streamingSessionStorage.numSessions);

    HAPPlatformCameraStream *videoStream = &camera->streamingSessionStorage.sessions[streamIndex].video;
    HAPPlatformCameraStream *audioStream = &camera->streamingSessionStorage.sessions[streamIndex].audio;

    HAPLogInfo(&logObject, "%s %d", __func__, (int) streamIndex);

    HAPError err;
    err = HAPPlatformVideoStreamDisable(&videoStream->videoStream);
    if (err) {
        HAPLogError(&logObject, "Unable to disable video stream");
        return err;
    }
    err = HAPPlatformAudioStreamDisableMicrophone(&audioStream->audioStream);
    if (err) {
        HAPLogError(&logObject, "Unable to disable microphone stream");
        return err;
    }
    err = HAPPlatformAudioStreamDisableSpeaker(&audioStream->audioStream);
    if (err) {
        HAPLogError(&logObject, "Unable to disable speaker stream");
        return err;
    }
    
    return kHAPError_None;
}

HAP_RESULT_USE_CHECK
HAPError HAPPlatformCameraResumeStreamingSession(
    HAPPlatformCameraRef camera,
    size_t streamIndex)
{
    HAPPrecondition(camera);
    HAPPrecondition(streamIndex < camera->streamingSessionStorage.numSessions);

    HAPLogInfo(&logObject, "%s %d", __func__, (int) streamIndex);

    HAPPlatformCameraStream *videoStream = &camera->streamingSessionStorage.sessions[streamIndex].video;
    HAPPlatformCameraStream *audioStream = &camera->streamingSessionStorage.sessions[streamIndex].audio;

    HAPError err;
    err = HAPPlatformVideoStreamEnable(&videoStream->videoStream);
    if (err) {
        HAPLogError(&logObject, "Unable to enable video stream");
        return err;
    }
    err = HAPPlatformAudioStreamEnableMicrophone(&audioStream->audioStream);
    if (err) {
        HAPLogError(&logObject, "Unable to enable microphone stream");
        return err;
    }
    err = HAPPlatformAudioStreamEnableSpeaker(&audioStream->audioStream);
    if (err) {
        HAPLogError(&logObject, "Unable to enable speaker stream");
        return err;
    }

    return kHAPError_None;
}

void HAPPlatformCameraEndStreamingSession(
    HAPPlatformCameraRef camera,
    size_t streamIndex)
{
    HAPPrecondition(camera);
    HAPPrecondition(streamIndex < camera->streamingSessionStorage.numSessions);

    HAPPlatformCameraStream *videoStream = &camera->streamingSessionStorage.sessions[streamIndex].video;
    HAPPlatformCameraStream *audioStream = &camera->streamingSessionStorage.sessions[streamIndex].audio;

    HAPLogInfo(&logObject, "%s %d", __func__, (int) streamIndex);

    if (videoStream->initialized) {
        // Kill polling thread.
        if (videoStream->isActive && !pthread_equal(videoStream->thread, pthread_self())) {
            pthread_cancel(videoStream->thread);
        }

        HAPPlatformVideoStreamStop(&videoStream->videoStream);

        HAPRTPStreamEnd(&videoStream->rtpStream);

        // Close socket.
        if (videoStream->socket >= 0) {
            (void) close(videoStream->socket);
            videoStream->socket = -1;
        }

        videoStream->isActive = false;
    }

    if (audioStream->initialized) {
        // Kill polling thread.
        if (audioStream->isActive && !pthread_equal(audioStream->thread, pthread_self())) {
            pthread_cancel(audioStream->thread);
        }
    
        HAPPlatformAudioStreamStopMicrophone(&audioStream->audioStream);
        HAPPlatformAudioStreamStopSpeaker(&audioStream->audioStream);

        HAPRTPStreamEnd(&audioStream->rtpStream);

        // Close socket.
        if (audioStream->socket >= 0) {
            (void) close(audioStream->socket);
            audioStream->socket = -1;
        }

        audioStream->isActive = false;
    }
}

HAP_RESULT_USE_CHECK
HAPError HAPPlatformCameraTakeSnapshot(
    HAPPlatformCameraRef camera,
    uint16_t imageWidth,
    uint16_t imageHeight,
    HAPPlatformCameraSnapshotReader **snapshotReader)
{
    HAPPrecondition(camera);
    HAPPrecondition(snapshotReader);

    HAPLogInfo(&logObject, "%s: w = %d, h = %d", __func__, imageWidth, imageHeight);

    size_t i = 0;
    while ((i != kIPCamera_MaxSnapshots) && (camera->snapshots[i].size != 0)) {
        i++;
    }
    if (i == kIPCamera_MaxSnapshots) {
        HAPLogError(&logObject, "not enough snapshot blocks");
        return kHAPError_OutOfResources;
    }
    
    HAPError err;
    err = HAPPlatformVideoTakeSnapshot(&camera->snapshots[i], imageWidth, imageHeight, snapshotReader);
    return err;
}

void HAPPlatformCameraSetMicrophone(
    HAPPlatformCamera *camera)
{
    HAPPrecondition(camera);
    
    if (camera->streamingSessionStorage.sessions) {
        for (size_t i = 0; i < camera->streamingSessionStorage.numSessions; i++) {
            HAPPlatformCameraStream *audioStream =
                &camera->streamingSessionStorage.sessions[i].audio;
            HAPPlatformAudioStreamSetMicrophoneVolume(
                &audioStream->audioStream,
                camera->state.microphone.volume,
                camera->state.microphone.mute);
        }
    }
}

void HAPPlatformCameraSetSpeaker(
    HAPPlatformCamera *camera)
{
    HAPPrecondition(camera);
    
    if (camera->streamingSessionStorage.sessions) {
        for (size_t i = 0; i < camera->streamingSessionStorage.numSessions; i++) {
            HAPPlatformCameraStream *audioStream =
                &camera->streamingSessionStorage.sessions[i].audio;
            HAPPlatformAudioStreamSetSpeakerVolume(
                &audioStream->audioStream,
                camera->state.speaker.volume,
                camera->state.speaker.mute);
        }
    }
}
