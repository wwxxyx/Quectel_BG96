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

/*
 * Camera loggers to inspect the camera RTP stream. Output of these functions is not considered to be used in
 * production.
 *
 * Options
 *
 * LogVideoIDR: Log important video packets (key frames and configuration)
 * LogVideoRTP: Log all video RTP packets
 * LogAudioRTP: Log all audio RTP packets
 * LogStatistics: Add statistical information to selected RTP packets
 * LogRTCP: Log RTCP packets
 * Packets with wrong sequence numbers or implausible timestamps are always shown.
 *
 * Time Format
 *
 * Time is always shown in 'short NTP' format:
 * Seconds from 1900 modulo 65536 with millisecond resolution.
 * This allows to compare time fields of all kind of formats.
 *
 * SSRC Identifiers
 *
 * SSRC identifiers are logged in the form "[<hex number>]".
 * For SSRC identifiers appearing repeatedly inside the same RTCP packet the following short forms are used:
 * - "[]" for the source ID (the first ID at the start of the packet).
 * - "[>]" for the remote ID (the second ID at the start of the packet).
 * Unused IDs filled with 0 are omitted.
 *
 * Output
 *
 * Prefix:
 *   output from camera to controller:
 *     "<actual time> (stream) >"
 *   output to camera from controller:
 *     "<actual time> (stream) <"
 * RTP audio packet:
 *     "[<id>] audio: <sequence number>, <time stamp> (<length>)"
 *   with statistics:
 *     "[<id>] audio: <sequence number>, <time stamp> <bitrate>kb/s, <timestamp frequency>kHz (<length>)"
 * RTP H264 video packet:
 *   single packet:
 *     "[<id>] h264: <sequence number>, <time stamp>, <nri>, <NAL-type> (<length>)"
 *   aggregated packet:
 *     "[<id>] h264: <sequence number>, <time stamp>, <nri>, stap-a,
       <nri>, <NAL-type>:length, <nri>, <NAL-type>:length (<length>)"
 *   fragmented packet:
 *     "[<id>] h264: <sequence number>, <time stamp>, <nri>, fu-a{, <NAL-type> (<length>)"
 *       fu-a{: first fragment
 *       fu-a|: inner fragment
 *       fu-a}: last fragment
 *   key frame packet with statistics:
 *     "[<id>] h264: <sequence number>, <time stamp>, <nri>, fu-a}, idr
 *     <bitrate>kb/s, <timestamp frequency>kHz (<length>)", <framerate>f/s (<length>)"
 * RTCP packet:
 *   sender report:
 *     "SR:[<id>]; <sender-info>; <info>; ... <info> "
 *   receiver report:
 *     "RR:[<id>]; <receiver-info>; <info>; ... <info> "
 *   sender-info:
 *     "<ntp-time>, <timestamp>, <sequence number>, <packet count>, <octet count>"
 *   receiver-info:
 *     "[<sender>] <fraction lost>, <total lost>, <max index>, ~<jitter>, <last SR time>,
 *     <last SR delay>, &<round-trip-time>"
 *   source descriptor:
 *     "SDES:[<id>], <CNAME-string>", <other items>, ...
 *   feedback:
 *     "TMMBR:[<id>],[<sender>]:<bitrate>, <overhead>"
 *     "TMMBN:[<id>],[<owner>]:<bitrate>, <overhead>"
 *     "TSTR:[<id>],[<sender>]:<sequence>, <value>"
 *     "TSTN:[<id>],[<owner>]:<sequence>, <value>"
 *     "PLI":[<id>],[<sender>]"
 *     "FIR":[<id>],[<sender>]:<sequence>"
 *
 * "!" error in packet
 * "*" marked packet
 *
 */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>

#include "HAPPlatformLog+Camera.h"

/**
 * RTP packet types
 */
#define kPacketVideo       (99)           // Video data packet
#define kPacketAudio       (110)          // Audio data packet

/**
 * RTCP packet types.
 * RFC3550, RFC4585, RFC5104
 */
#define kPacketSR          (200)          // Sender report packet type.
#define kPacketRR          (201)          // Receiver report packet type.
#define kPacketSDES        (202)          // Source description packet type.
#define kPacketBYE         (203)          // Bye packet type.
#define kPacketRTPFB       (205)          // RTPFB packet type (RFC 4585, Section 6.1).
#define kPacketTMMBR       (3)            // Temporary maximum stream bit rate request.
#define kPacketTMMBN       (4)            // Temporary maximum stream bit rate notification.
#define kPacketPSFB        (206)          // PSFB packet type  (RFC 4585, Section 6.1).
#define kPacketPLI         (1)            // Picture loss indicator.
#define kPacketFIR         (4)            // Full intra request.
#define kPacketTSTR        (5)            // Temporal spatial trade-off request.
#define kPacketTSTN        (6)            // Temporal spatial trade-off notification.

/**
 * RTCP packet sizes.
 * RFC3550
 */
#define kSizeSR            (20)           // Sender report packet size.
#define kSizeRR            (24)           // Receiver report packet size.

/**
 * H264 profiles.
 * "ITU-T H.264", A.2
 */
#define kProfileBase       (66)           // Baseline profile.
#define kProfileMain       (77)           // Main profile.
#define kProfileHigh       (100)          // High profile.

/**
 * H264 NAL unit types.
 * "ITU-T H.264", Table 7-1 and "RFC6184", Table 3.
 */
#define kH264TypeIDR       (5)            // H264 IDR type
#define kH264TypeSPS       (7)            // H264 SPS type
#define kH264TypeStapA     (24)           // H264 NALU STAP-A type
#define kH264TypeFuA       (28)           // H264 NALU FU-A type
#define kH264TypeFuB       (29)           // H264 NALU FU-B type

/**
 * Other RTP constants.
 */
#define kRTPVersion        (2)            // RTP version bits.
#define kRTPHeaderExtId    (0xBEDE)       // RTP one byte header extension ID.
#define kMaxSDESStringLen  (256)          // Maximum string length in SDES packet.

/**
 * Timing constants.
 */
#define kSeconds1900To1970 (2208988800)   // Seconds from 1900 to 1970.
#define kOneSecond         (1000000000ll) // One second in ns

/**
 * Stream state.
 *
 * Used to store statistics of a stream identified by ssrc.
 */
typedef struct {
    /**
     * Stream ssrc.
     */
    uint32_t ssrc;
    
    /**
     * Timestamp.
     */
    uint32_t timestamp;
    
    /**
     * Sequence number.
     */
    uint16_t seqNum;
    
    /**
     * Previous time.
     */
    uint64_t prevTime;
    
    /**
     * Previous timestamp.
     */
    uint32_t prevTimestamp;
    
    /**
     * The number of bytes sent over the stream.
     */
    uint32_t sumBytes;
    
    /**
     * The number of frames sent over the stream.
     */
    uint32_t numFrames;
} StreamState;

/**
 * The number of entries in the stream state table.
 */
#define kMaxStreamStateEntries (16)

/**
 * The stream state table.
 */
static StreamState streamState[kMaxStreamStateEntries];

/**
 * The index of stream state.
 */
static uint32_t stateIndex = 0;

/**
 * Get stream state and increment timestamp and sequence number.
 *
 * @param      ssrc                 Stream ssrc .
 * @param      timestamp            Stream timestamp.
 * @param      seqNum               Stream sequence number.
 * @param[out] timestampInc         Difference in timestamp to previous call.
 * @param      seqNumInc            Difference in sequence numbers to previous call.
 *
 * @return StreamState of the stream. Returns NULL if the stream could not be identified in the table and there's no
 *      space left.
 */
static StreamState *GetIncrements(
    uint32_t ssrc,
    uint32_t timestamp,
    uint16_t seqNum,
    int32_t *timestampInc,
    int16_t *seqNumInc)
{
    int i;
    // search previous values by ssrc
    for (i = 0; i < kMaxStreamStateEntries; i++) {
        if (streamState[i].ssrc == ssrc) {
            // found: get increments and update
            *timestampInc = (int32_t)(timestamp - streamState[i].timestamp);
            streamState[i].timestamp = timestamp;
            *seqNumInc = (int16_t)(seqNum - streamState[i].seqNum);
            streamState[i].seqNum = seqNum;
            return &streamState[i];
        }
    }
    // not found: add ssrc to table
    if (stateIndex < kMaxStreamStateEntries) {
        streamState[stateIndex].ssrc = ssrc;
        streamState[stateIndex].timestamp = timestamp;
        streamState[stateIndex].seqNum = seqNum;
        stateIndex = (stateIndex + 1) % kMaxStreamStateEntries;
    }
    return NULL;
}

/**
 * Reads a UInt32 value from a buffer at an offset containing its corresponding big-endian representation.
 *
 * @param      bytes                Buffer to read from. Must contain at least offset + 4 bytes.
 * @param      offset               Offset to read from.
 *
 * @return Value that has been read.
 */
static uint32_t ReadBigUInt32(
    const uint8_t *bytes,
    uint32_t offset)
{
    return HAPReadBigUInt32(&bytes[offset]);
}


/**
 * Log ID.
 *
 * @param      id                   ID.
 * @param      localID              Local ID.
 * @param      remoteID             Remote ID.
 */
static void LogId(
    uint32_t id,
    uint32_t localID,
    uint32_t remoteID)
{
    if (id == localID) {
        printf("[]"); // own ssrc
    } else if (id == remoteID) {
        printf("[>]"); // remote ssrc
    } else {
        printf("[%08X]", (unsigned int)id);
    }
}

/**
 * Log ID Pair.
 *
 * @param      id1                  ID 1.
 * @param      id2                  ID 2.
 * @param      localID              Local ID.
 * @param      remoteID             Remote ID.
 */
static void LogIdPair(
    uint32_t id1,
    uint32_t id2,
    uint32_t localID,
    uint32_t remoteID)
{
    LogId(id1, localID, remoteID);
    if (id2) {
        printf(",");
        LogId(id2, localID, remoteID);
    }
}

/**
 * Log time.
 *
 * Time is always shown in 'short NTP' format:
 * Seconds from 1900 modulo 65536 with millisecond resolution.
 *
 * @param      time                 Time to be logged [ns].
 */
static void LogTime(
    uint64_t time)
{
    printf("%u.%03u ",
        (unsigned int)(time / kOneSecond & 0xFFFF),   // [sec]
        (unsigned int)(time % kOneSecond / 1000000)); // [ms]
}

/**
 * Parse and log an SR or RR packet.
 *
 * @param      bytes                Buffer containing the packet.
 * @param      numBytes             The size of the packet.
 * @param      sCount               The number of sender info blocks.
 * @param      ownId                The sender SSRC.
 * @param[out] remoteId             The first remote SSRC.
 * @param      time                 The actual time.
 */
static void LogSRPacket(
    const uint8_t *bytes,
    size_t numBytes,
    uint32_t sCount,
    uint32_t ownId,
    uint32_t *remoteId,
    uint64_t time)
{
    uint32_t rCount = (uint32_t)(bytes[0] & 0x1F);
    uint32_t length = 8 + sCount * kSizeSR + rCount * kSizeRR;;
    uint32_t pos = 8; // start of first block

    if (sCount) {
        printf(" SR:[%08X]", (unsigned int)ownId);
    } else {
        printf(" RR:[%08X]", (unsigned int)ownId);
    }
    if (length <= numBytes) {
        if (sCount) {
            // sender report block
            printf(";%u.%03u,%u,%u,%u",
               (unsigned int)(ReadBigUInt32(bytes, pos) & 0xFFFF),
               (unsigned int)(ReadBigUInt32(bytes, pos + 4) * 1000ll >> 32),
               (unsigned int)(ReadBigUInt32(bytes, pos + 8)),
               (unsigned int)(ReadBigUInt32(bytes, pos + 12)),
               (unsigned int)(ReadBigUInt32(bytes, pos + 16)));
            pos += kSizeSR;
        }
        while (rCount) {
            // receiver report block
            if (*remoteId == 0) {
                *remoteId = ReadBigUInt32(bytes, pos);
            }
            uint32_t lsr = ReadBigUInt32(bytes, pos + 16);
            uint32_t dlsr = ReadBigUInt32(bytes, pos + 20);
            uint32_t act = (uint32_t)((time % (65536 * kOneSecond) << 16) / kOneSecond); // ns -> short ntp
            uint32_t rtt = 0;
            if (lsr && dlsr) {
                rtt = act - lsr - dlsr;     // roundtrip time
                if (rtt > 0x10000) rtt = 0; // rtt calculation fails on outgoing packets
            }
            printf(";[%08X],%d,%d,%u,~%u,%u.%03u,%u.%03u",
                (unsigned int)*remoteId,
                (int)bytes[pos + 4],
                (unsigned int)(ReadBigUInt32(bytes, pos + 4) & 0x00FFFFFF),
                (unsigned int)(ReadBigUInt32(bytes, pos + 8)),  // max
                (unsigned int)(ReadBigUInt32(bytes, pos + 12)), // jitter
                (unsigned int)(lsr >> 16),  // last sr received
                (unsigned int)((lsr & 0xFFFF) * 1000 >> 16),
                (unsigned int)(dlsr >> 16), // delay since last sr
                (unsigned int)((dlsr & 0xFFFF) * 1000 >> 16));
            if (rtt) {
                printf(",&%u.%03u",
                   (unsigned int)(rtt >> 16),
                   (unsigned int)((rtt & 0xFFFF) * 1000 >> 16));
            }
            pos += kSizeRR;
            rCount--;
        }
        if (numBytes > length) {
            printf(",+"); // has extensions
        }
    } else {
        printf("!"); // wrong size
    }
}

/**
 * Parse and log an SDES packet (source description).
 *
 * @param      bytes                Buffer containing the packet.
 * @param      numBytes             The size of the packet.
 * @param      ownId                The sender SSRC.
 * @param      remoteId             The remote SSRC.
 */
static void LogSDESPacket(
    const uint8_t *bytes,
    size_t numBytes,
    uint32_t ownId,
    uint32_t remoteId)
{
    uint32_t count = (uint32_t)(bytes[0] & 0x1F);
    uint32_t pos = 4; // start of first chunk
    uint8_t type;
    uint8_t string[kMaxSDESStringLen];

    printf(";SDES:");
    while (count && pos + 5 <= numBytes) {
        // sdes chunk
        LogId(ReadBigUInt32(bytes, pos), ownId, remoteId);
        type = bytes[pos + 4];
        pos += 5;
        while (type != 0 && pos + 1 <= numBytes) {
            // sdes item
            switch (type) { // SDES type
                case 1: printf(","); break;
                case 2: printf(",NAME="); break;
                case 3: printf(",EMAIL="); break;
                case 4: printf(",PHONE="); break;
                case 5: printf(",LOC="); break;
                case 6: printf(",TOOL="); break;
                case 7: printf(",NOTE="); break;
                default: printf(",#%d=", (int)type); break;
            }
            uint32_t length = bytes[pos];
            pos++;
            if (pos + length <= numBytes) {
                memcpy(string, bytes + pos, length);
                string[length] = 0;
                printf("\"%s\"", string);
            } else {
                printf("!"); // wrong size
            }
            pos += length;
            if (pos < numBytes) {
                type = bytes[pos];
                pos++;
            }
        }
        count--;
        pos += (-pos) & 3; // enforce word alignment
    }
    if (count || pos != numBytes) {
        printf("!"); // wrong size
    }
}

/**
 * Parse and log a BYE packet.
 *
 * @param      bytes                Buffer containing the packet.
 * @param      numBytes             The size of the packet.
 * @param      ownId                The sender SSRC.
 * @param      remoteId             The remote SSRC.
 */
static void LogBYEPacket(
    const uint8_t *bytes,
    size_t numBytes,
    uint32_t ownId,
    uint32_t remoteId)
{
    uint32_t count = (uint32_t)(bytes[0] & 0x1F);
    uint32_t pos = 4; // start of first chunk

    printf(";BYE:");
    while (count && pos + 4 <= numBytes) {
        LogId(ReadBigUInt32(bytes, pos), ownId, remoteId);
        pos += 4;
        count--;
    }
    if (count) {
        printf("!"); // wrong size
    } else if (pos < numBytes) {
        printf(",+"); // has extensions
    }
}

/**
 * Parse and log a RTPFB packet (RTP feedback).
 *
 * @param      bytes                Buffer containing the packet.
 * @param      numBytes             The size of the packet.
 * @param      ownId                The sender SSRC.
 * @param      remoteId             The remote SSRC.
 */
static void LogRTPFBPacket(
    const uint8_t *bytes,
    size_t numBytes,
    uint32_t ownId,
    uint32_t remoteId)
{
    uint32_t type = (uint32_t)(bytes[0] & 0x1F);
    
    if (numBytes >= 12) {
        uint32_t s1 = ReadBigUInt32(bytes, 4);
        uint32_t s2 = ReadBigUInt32(bytes, 8);
        uint32_t pos = 12;
        if (type == kPacketTMMBR || type == kPacketTMMBN) { // TMMBR/TMMBN
            if (type == kPacketTMMBR) { // TMMBR (temporary maximum stream bit rate request)
                printf(";TMMBR:");
            } else { // TMMBN (temporary maximum stream bit rate notification)
                printf(";TMMBN:");
            }
            LogIdPair(s1, s2, ownId, remoteId);
            while (pos + 8 <= numBytes) {
                uint32_t w = ReadBigUInt32(bytes, pos + 4);
                uint32_t exp = w >> 26;
                uint32_t mant = (w >> 9) & 0x1FFFF;
                uint32_t rate = mant << exp;
                uint32_t b = w & 0x1FF;
                printf(",");
                LogId(ReadBigUInt32(bytes, pos), ownId, remoteId);
                if (rate >> exp == mant) {
                    printf(":%u,%u",
                       (unsigned int)rate,
                       (unsigned int)b);
                } else {
                    printf(":%ue%u,%u",
                       (unsigned int)mant,
                       (unsigned int)exp,
                       (unsigned int)b);
                }
                pos += 8;
            }
        } else {
            printf(";RTPFB/%d:", (int)type);
            LogIdPair(s1, s2, ownId, remoteId);
            pos += 8;
        }
        if (pos != numBytes) {
            printf("!"); // wrong size
        }
    } else {
        printf(";?:RTPFB,%d", (int)type);
    }
}

/**
 * Parse and log a PSFB packet (payload specific feedback).
 *
 * @param      bytes                Buffer containing the packet.
 * @param      numBytes             The size of the packet.
 * @param      ownId                The sender SSRC.
 * @param      remoteId             The remote SSRC.
 */
static void LogPSFBPacket(
    const uint8_t *bytes,
    size_t numBytes,
    uint32_t ownId,
    uint32_t remoteId)
{
    uint32_t type = (uint32_t)(bytes[0] & 0x1F);
    
    if (numBytes >= 12) {
        uint32_t s1 = ReadBigUInt32(bytes, 4);
        uint32_t s2 = ReadBigUInt32(bytes, 8);
        uint32_t pos = 12;
        if (type == kPacketPLI) { // PLI (picture loss indicator)
            printf(";PLI:");
            LogIdPair(s1, s2, ownId, remoteId);
        } else if (type == kPacketFIR) { // FIR (full intra request)
            printf(";FIR:");
            LogIdPair(s1, s2, ownId, remoteId);
            while (pos + 8 <= numBytes) {
                printf(",");
                LogId(ReadBigUInt32(bytes, pos), ownId, remoteId);
                printf(":%d", (int)bytes[pos + 4]);
                pos += 8;
            }
        } else if (type == kPacketTSTR || type == kPacketTSTN) { // TSTR/TSTN
            if (type == kPacketTSTR) { // TSTR (temporal spatial trade-off request)
                printf(";TSTR:");
            } else { // TSTN (temporal spatial trade-off notification)
                printf(";TSTN:");
            }
            LogIdPair(s1, s2, ownId, remoteId);
            while (pos + 8 <= numBytes) {
                printf(",");
                LogId(ReadBigUInt32(bytes, pos), ownId, remoteId);
                printf(":%u,%u",
                   (unsigned int)bytes[pos + 4],
                   (unsigned int)bytes[pos + 7]);
                pos += 8;
            }
        } else {
            printf(";PSFB/%d:", (int)type);
            LogIdPair(s1, s2, ownId, remoteId);
            pos += 8;
        }
        if (pos != numBytes) {
            printf("!"); // wrong size
        }
    } else {
        printf(";?:PSFB,%d", (int)type);
    }
}

/**
 * Parse and log an RTCP package.
 *
 * @param      bytes                Buffer to be logged.
 * @param      numBytes             The size of the buffer to be logged.
 * @param      logMessage           The associated log message.
 * @param      time                 The actual time.
 */
static void LogRtcpPackage(
    const uint8_t *bytes,
    size_t numBytes,
    const char *logMessage,
    uint64_t time)
{
    uint32_t length, type;
    uint32_t ownId = 0, remoteId = 0;
    const uint8_t *packet = bytes;
    const uint8_t *end = bytes + numBytes;
    
    LogTime(time);
    printf("%s", logMessage);
    
    while (packet + 4 <= end) {
        // We have at least one word.
        type = (uint32_t)packet[1];
        length = (uint32_t)((packet[2] << 10) + (packet[3] << 2)) + 4;
        if (packet + length <= end) {
            // We have all the bytes needed.
            if (ownId == 0) {
                ownId = ReadBigUInt32(bytes, 4);
            }
            switch (type) {
                case kPacketSR: { // SR packet (one sender block).
                    LogSRPacket(packet, length, 1, ownId, &remoteId, time);
                } break;
                case kPacketRR: { // RR packet (no sender block).
                    LogSRPacket(packet, length, 0, ownId, &remoteId, time);
                } break;
                case kPacketSDES: { // SDES packet (source description).
                    LogSDESPacket(packet, length, ownId, remoteId);
                } break;
                case kPacketBYE: { // BYE packet.
                    LogBYEPacket(packet, length, ownId, remoteId);
                } break;
                case kPacketRTPFB: { // RTPFB packet (RTP feedback).
                    LogRTPFBPacket(packet, length, ownId, remoteId);
                } break;
                case kPacketPSFB: { // PSFB packet (payload specific feedback).
                    LogPSFBPacket(packet, length, ownId, remoteId);
                } break;
                default: {
                    printf(";?:#%d", (int)type);
                } break;
            }
        }
        packet += length;
    }
    if (packet != end) { // wrong length
        printf(" !%u/%u",
            (unsigned int)(packet - bytes),
            (unsigned int)numBytes);
    }
    printf("\n");
}

/**
 * Parse and log Audio Package.
 *
 * @param      bytes                Buffer to be logged.
 * @param      numBytes             The size of the buffer to be logged.
 * @param      logMessage           The associated log message.
 * @param      time                 The actual time.
 * @param      options              Logging options.
 */
static void LogAudioPackage(
    const uint8_t *bytes,
    size_t numBytes,
    const char *logMessage,
    uint64_t time,
    HAPPlatformLogCameraLogOptions options)
{
    uint16_t seqNum;
    int16_t seqInc;
    uint32_t ts, id;
    int32_t timeInc;
    bool badInc = false;
    uint64_t elapsed;
    
    if (numBytes >= 12) {
        seqNum = (uint16_t)((bytes[2] << 8) + bytes[3]);
        ts = ReadBigUInt32(bytes, 4);
        id = ReadBigUInt32(bytes, 8);
        elapsed = 0;
        StreamState *state = GetIncrements(id, ts, seqNum, &timeInc, &seqInc);
        if (state) {
            badInc = seqInc != 1 || timeInc > 24000 || timeInc < 0;
            elapsed = time - state->prevTime;
            state->sumBytes += numBytes;
        }
        if (options.logAudioRTP || (options.logStatistics && elapsed > 5 * kOneSecond) || badInc) {
            LogTime(time);
            printf("%s [%08X] audio: ", logMessage, (unsigned int)id);
            if (badInc) {
                printf("%u(%+d),%u(%+d) ! ",
                   (unsigned int)seqNum,
                   (int)seqInc,
                   (unsigned int)ts,
                   (int)timeInc);
            } else {
                printf("%u,%u",
                   (unsigned int)seqNum,
                   (unsigned int)ts);
            }
            if (options.logStatistics && elapsed > 5 * kOneSecond) { // every 5s
                // add bitrate and timestamp frequency statistics
                uint32_t bitrate = (uint32_t)(state->sumBytes * 8000000LL / elapsed);
                uint32_t frequency = (uint32_t)((ts - state->prevTimestamp) * kOneSecond / elapsed);
                printf(" %ukb/s,%ukHz",
                   (unsigned int)bitrate,
                   (unsigned int)((frequency + 500) / 1000));
                state->prevTime = time;
                state->prevTimestamp = ts;
                state->sumBytes = 0;
            }
            if (bytes[1] >= 0x80) {
                printf("*"); // marker flag
            }
            printf(" (%u)\n", (unsigned int)numBytes);
        }
    }
}

/**
 * Log H264 Unit Type.
 * See: "ITU-T H.264", Table 7-1 and "RFC6184", Table 3.
 *
 * @param      type                 H264 Unit Type.
 */
static void LogH264UnitType(
    int type)
{
    type &= 0x1F;
    switch (type) {
        case  1: printf(",slice"); break;
        case  2: printf(",sliceA"); break;
        case  3: printf(",sliceB"); break;
        case  4: printf(",sliceC"); break;
        case  5: printf(",idr"); break;
        case  6: printf(",sei"); break;
        case  7: printf(",sps"); break;
        case  8: printf(",pps"); break;
        case  9: printf(",delimiter"); break;
        case 10: printf(",endSeq"); break;
        case 11: printf(",endStream"); break;
        case 12: printf(",filler"); break;
        case 13: printf(",seqParamExt"); break;
        case 14: printf(",nal"); break;
        case 15: printf(",subSeqParam"); break;
        case 16: printf(",depthParam"); break;
        case 19: printf(",sliceAux"); break;
        case 20: printf(",sliceExt"); break;
        case 21: printf(",sliceExtD"); break;
        case 24: printf(",stap-a");break;
        case 25: printf(",stap-b"); break;
        case 26: printf(",mtab-16"); break;
        case 27: printf(",mtab-24"); break;
        case 28: printf(",fu-a"); break;
        case 29: printf(",fu-b"); break;
        default: printf(",%d",type); break;
    }
}

/**
 * Read a fixed size or Exp-Golomb coded variable size field from a bit sequence.
 * See: "ITU-T H.264", 9.1
 *
 * @param      bytes                Byte sequence.
 * @param      numBytes             The size of the byte sequence.
 * @param[in,out] pos               The bit position of the code.
 * @param      length               The length of the field to extract or 0 for a variable size field.
 *
 * @return Value of the extracted field.
 */
static uint32_t ReadBitField(
    const uint8_t *bytes,
    size_t numBytes,
    uint32_t *pos,
    uint32_t length)
{
    uint32_t index = *pos >> 3;     // byte index
    uint32_t bits = 8 - (*pos & 7); // number of valid bits in actual byte
    uint32_t actual;                // actual byte
    
    // get valid bits from first byte
    if (index >= numBytes) return 0;
    actual = bytes[index++] & ~(0xFFFFFFFF << bits);
    
    // get bit count
    uint32_t count = length;
    if (length == 0) { // variable size field
        // count zero bits
        while (actual == 0) {
            count += bits;
            if (index >= numBytes) return 0;
            actual = bytes[index++];
            bits = 8;
        }
        uint32_t mask = 1 << (bits - 1); // first bit
        while ((actual & mask) == 0) {
            count++;
            mask >>= 1;
            bits--;
        }
        count++; // field length = zero bit count + 1
    }
    
    // extract bits
    uint32_t value = 0;
    while (count > bits) {
        value = (value << bits) + actual;
        count -= bits;
        if (index >= numBytes) return 0;
        actual = bytes[index++];
        bits = 8;
    }
    value = (value << count) + (actual >> (bits - count));
    bits -= count;
    if (length == 0) {
        value--; // variable size code needs to be decremented
    }
    
    *pos = (index << 3) - bits; // final bit position
    return value;
}

/**
 * Log H264 SPS packet.
 * See: "ITU-T H.264", 7.3.2.1
 *
 * @param      bytes                SPS data to be logged.
 * @param      numBytes             The size of the SPS data.
 */
static void LogH264SPS(
    const uint8_t *bytes,
    size_t numBytes)
{
    uint32_t pos = 0; // actual bit position
    uint32_t profile = ReadBitField(bytes, numBytes, &pos, 8); // profile_idc
    ReadBitField(bytes, numBytes, &pos, 8); // constraint_set_flag
    uint32_t level = ReadBitField(bytes, numBytes, &pos, 8); // level_idc
    switch (profile) {
        case kProfileBase: printf("{baseline"); break;
        case kProfileMain: printf("{main"); break;
        case kProfileHigh: printf("{high"); break;
        default: printf("{profile:%d", profile); break;
    }
    printf(",%d.%d", level / 10, level % 10);
    ReadBitField(bytes, numBytes, &pos, 0); // seq_parameter_set_id
    if (profile >= kProfileHigh) {
        uint32_t chromaFormat = ReadBitField(bytes, numBytes, &pos, 0); // chroma_format_idc
        if (chromaFormat == 3) {
            ReadBitField(bytes, numBytes, &pos, 1); // separate_colour_plane_flag
        }
        ReadBitField(bytes, numBytes, &pos, 0); // bit_depth_luma_minus8
        ReadBitField(bytes, numBytes, &pos, 0); // bit_depth_chroma_minus8
        ReadBitField(bytes, numBytes, &pos, 1); // qpprime_y_zero_transform_bypass_flag
        uint32_t matrixPresent = ReadBitField(bytes, numBytes, &pos, 1); // seq_scaling_matrix_present_flag
        if (matrixPresent) {
            return; // not supported
        }
    }
    ReadBitField(bytes, numBytes, &pos, 0); // log2_max_frame_num_minus4
    uint32_t orderCntType = ReadBitField(bytes, numBytes, &pos, 0); // pic_order_cnt_type
    if (orderCntType == 0) {
        ReadBitField(bytes, numBytes, &pos, 0); // log2_max_pic_order_cnt_lsb_minus4
    } else if (orderCntType == 1) {
        ReadBitField(bytes, numBytes, &pos, 1); // delta_pic_order_always_zero_flag
        ReadBitField(bytes, numBytes, &pos, 0); // offset_for_non_ref_pic
        ReadBitField(bytes, numBytes, &pos, 0); // offset_for_top_to_bottom_field
        uint32_t numRefFrames = ReadBitField(bytes, numBytes, &pos, 0); // num_ref_frames_in_pic_order_cnt_cycle
        for (unsigned int i = 0; i < numRefFrames; i++) {
            ReadBitField(bytes, numBytes, &pos, 0); // offset_for_ref_frame[i]
        }
    }
    ReadBitField(bytes, numBytes, &pos, 0); // max_num_ref_frames
    ReadBitField(bytes, numBytes, &pos, 1); // gaps_in_frame_num_value_allowed_flag
    uint32_t widthInMbsMinus1 = ReadBitField(bytes, numBytes, &pos, 0); // pic_width_in_mbs_minus1
    uint32_t heightInMapsUnitsMinus1 = ReadBitField(bytes, numBytes, &pos, 0); // pic_height_in_map_units_minus1
    uint32_t frameMbsOnlyFlag = ReadBitField(bytes, numBytes, &pos, 1); // frame_mbs_only_flag
    if (frameMbsOnlyFlag == 0) {
        ReadBitField(bytes, numBytes, &pos, 1); // mb_adaptive_frame_field_flag
    }
    ReadBitField(bytes, numBytes, &pos, 1); // direct_8x8_inference_flag
    uint32_t croppingFlag = ReadBitField(bytes, numBytes, &pos, 1); // frame_cropping_flag
    uint32_t cropLeft = 0;
    uint32_t cropRight = 0;
    uint32_t cropTop = 0;
    uint32_t cropBottom = 0;
    if (croppingFlag) {
        cropLeft = ReadBitField(bytes, numBytes, &pos, 0);   // frame_crop_left_offset
        cropRight = ReadBitField(bytes, numBytes, &pos, 0);  // frame_crop_right_offset
        cropTop = ReadBitField(bytes, numBytes, &pos, 0);    // frame_crop_top_offset
        cropBottom = ReadBitField(bytes, numBytes, &pos, 0); // frame_crop_bottom_offset
    }
    uint32_t w = (widthInMbsMinus1 + 1) * 16 - (cropLeft + cropRight) * 2;
    uint32_t h = (2 - frameMbsOnlyFlag) * (heightInMapsUnitsMinus1 + 1) * 16 - (cropTop + cropBottom) * 2;
    printf(",w=%u,h=%u}", (unsigned int)w, (unsigned int)h);
}

/**
 * Parse and log H264 Package.
 *
 * @param      bytes                Buffer to be logged.
 * @param      numBytes             The size of the buffer to be logged.
 * @param      logMessage           The associated log message.
 * @param      time                 The actual time.
 * @param      options              Logging options.
 */
static void LogH264Package(
    const uint8_t *bytes,
    size_t numBytes,
    const char *logMessage,
    uint64_t time,
    HAPPlatformLogCameraLogOptions options)
{
    int type, htype, nri;
    uint32_t ts, id;
    int32_t timeInc;
    uint16_t seqNum;
    int16_t seqInc;
    bool badInc = false;
    StreamState *state;
    uint64_t elapsed;
    uint32_t offset = 12;
    uint8_t extId = 0, extByte = 0;
   
    if (numBytes >= 14) {
        htype = bytes[1];
        seqNum = (uint16_t)((bytes[2] << 8) + bytes[3]);
        ts = ReadBigUInt32(bytes, 4);
        id = ReadBigUInt32(bytes, 8);
        if (bytes[0] & 0x10 && numBytes >= 22) { // header extension
            if ((bytes[12] << 8) + bytes[13] == kRTPHeaderExtId) { // one byte extension
                extId = bytes[16];
                extByte = bytes[17];
            }
            offset += (bytes[15] + 1) * 4; // extension size
        }
        nri = bytes[offset] >> 5;
        type = bytes[offset] & 0x1F;
        offset++;
        elapsed = 0;
        state = GetIncrements(id, ts, seqNum, &timeInc, &seqInc);
        if (state) {
            badInc = seqInc != 1 || timeInc > 90000 || timeInc < 0;
            elapsed = time - state->prevTime;
            state->sumBytes += numBytes;
            if (htype >= 0x80) { // marked packet
                state->numFrames++;
            }
        }
        if (options.logVideoRTP ||
            ((options.logVideoIDR || options.logStatistics)
                && ((type == kH264TypeFuA && bytes[offset] == 0x40 + kH264TypeIDR) ||
                    (type >= kH264TypeIDR && type < kH264TypeFuA))) ||
            badInc) {
            LogTime(time);
            printf("%s [%08X] h264: ", logMessage, (unsigned int)id);
            if (badInc) {
                printf("%u(%+d),%u(%+d) ! %d",
                   (unsigned int)seqNum,
                   (int)seqInc,
                   (unsigned int)ts,
                   (int)timeInc,
                   nri);
            } else {
                printf("%u,%u,%d",
                   (unsigned int)seqNum,
                   (unsigned int)ts,
                   nri);
            }
            LogH264UnitType(type); // packet type
            if (type == kH264TypeFuA || type == kH264TypeFuB) { // fragmented
                type = bytes[offset];
                switch (type >> 6) {
                    case 0: printf("|"); break;  // inner fragment
                    case 1: printf("}"); break;  // last fragment
                    case 2: printf("{"); break;  // first fragement
                    default: printf("!"); break; // illegal
                }
                LogH264UnitType(type); // type of fragmented unit
                if (options.logStatistics && type == (0x40 + kH264TypeIDR) && elapsed) { // last idr fragment
                    // add bitrate and timestamp frequency statistics
                    uint32_t bitrate = (uint32_t)(state->sumBytes * 8000000LL / elapsed);
                    uint32_t frequency = (uint32_t)((ts - state->prevTimestamp) * kOneSecond / elapsed);
                    uint32_t framerate = (uint32_t)(state->numFrames * kOneSecond / elapsed);
                    printf(" %ukb/s,%ukHz,%uf/s",
                       (unsigned int)bitrate,
                       (unsigned int)((frequency + 500) / 1000),
                       (unsigned int)framerate);
                    state->prevTime = time;
                    state->prevTimestamp = ts;
                    state->sumBytes = 0;
                    state->numFrames = 0;
                }
            } else if (type == kH264TypeStapA) { // stap-a aggregated
                while(offset + 3 <= numBytes) {
                    uint32_t length = (uint32_t)((bytes[offset] << 8) + bytes[offset + 1]);
                    int utype = bytes[offset + 2]; // unit type
                    printf(",%d", utype >> 5);
                    LogH264UnitType(utype);
                    printf(":%d", length);
                    if ((utype & 0x1F) == kH264TypeSPS) { // sps
                        LogH264SPS(&bytes[offset + 3], length - 1);
                    }
                    offset += 2;
                    offset += length;
                }
            }
            if (extId) {
                printf(" {%d,%02X}", (int)(extId >> 4), (unsigned int)extByte);
            }
            if (htype >= 0x80) {
                printf("*"); // marker flag
            }
            printf(" (%u)\n", (unsigned int)numBytes);
        }
    }
}

void HAPPlatformLogCameraH264Payload(
    const void *bytes,
    size_t numBytes,
    const char *logMessage,
    HAPPlatformLogCameraLogOptions options)
{
    HAPPrecondition(bytes);
    HAPPrecondition(logMessage);
    
    const uint8_t *b = bytes;
    
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);
    
    uint64_t time = ((uint32_t)spec.tv_sec + kSeconds1900To1970) * kOneSecond + (uint32_t)spec.tv_nsec;
    
    int nri = b[0] >> 5;
    int type = b[0] & 0x1F;
    if (options.logVideoRTP ||
        (options.logVideoIDR && type >= 5)) {
        LogTime(time);
        printf("%s H264: %d", logMessage, nri);
        LogH264UnitType(type); // packet type
        printf(" (%u)\n", (unsigned int)numBytes);
    }
}

void HAPPlatformLogCameraPayload(
    const void *bytes,
    size_t numBytes,
    const char *logMessage,
    HAPPlatformLogCameraLogOptions options)
{
    HAPPrecondition(bytes);
    HAPPrecondition(logMessage);
    
    (void)bytes;
    
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);
    uint64_t time = ((uint32_t)spec.tv_sec + kSeconds1900To1970) * kOneSecond + (uint32_t)spec.tv_nsec;
    
    if (options.logAudioRTP) {
        LogTime(time);
        printf("%s (%u)\n", logMessage, (unsigned int)numBytes);
    }
}

void HAPPlatformLogCameraRTPPacket(
    const void *bytes,
    size_t numBytes,
    const char *logMessage,
    HAPPlatformLogCameraLogOptions options)
{
    HAPPrecondition(bytes);
    HAPPrecondition(logMessage);
    
    const uint8_t *b = bytes;
    
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);
    
    uint64_t time = ((uint32_t)spec.tv_sec + kSeconds1900To1970) * kOneSecond + (uint32_t)spec.tv_nsec;
    int type = b[1];
    int seq = (b[2] << 8) + b[3];
    int id1 = (int)ReadBigUInt32(bytes, 4);
    
    if ((b[0] >> 6) == kRTPVersion) {
        switch (type) {
            case kPacketSR:
            case kPacketRR: {
                if (options.logRTCP) {
                    LogRtcpPackage(bytes, numBytes, logMessage, time);
                }
            } break;
            case kPacketVideo:
            case kPacketVideo + 0x80: {
                LogH264Package(bytes, numBytes, logMessage, time, options);
            } break;
            case kPacketAudio:
            case kPacketAudio + 0x80: {
                LogAudioPackage(bytes, numBytes, logMessage, time, options);
            } break;
            default: {
                LogTime(time);
                printf("%s %02X:[%08X] %u (%u)\n", logMessage,
                   (unsigned int)type,
                   (unsigned int)id1,
                   (unsigned int)seq,
                   (unsigned int)numBytes);
            } break;
        }
    } else {
        printf("%s %08X (%u)\n", logMessage, ReadBigUInt32(bytes, 0), (unsigned int)numBytes);
    }
}
