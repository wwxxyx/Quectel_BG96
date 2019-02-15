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

#ifndef HAP_DATA_STREAM_PROTOCOLS_DATA_SEND_H
#define HAP_DATA_STREAM_PROTOCOLS_DATA_SEND_H

#ifdef __cplusplus
extern "C" {
#endif

#include "HAP.h"

#if __has_feature(nullability)
#pragma clang assume_nonnull begin
#endif

/**
 * "dataSend" HomeKit Data Stream protocol name.
 */
#define kHAPDataSendDataStreamProtocol_Name "dataSend"

/**
 * "dataSend" HomeKit Data Stream protocol handler context.
 */
typedef HAP_OPAQUE(8) HAPDataSendDataStreamProtocolContextRef;

/**
 * "dataSend" HomeKit Data Stream protocol handler base.
 */
extern const HAPDataStreamProtocolBase kHAPDataSendDataStreamProtocol_Base;

/**
 * "dataSend" HomeKit Data Stream protocol handler.
 */
typedef struct HAPDataSendDataStreamProtocol HAPDataSendDataStreamProtocol;
struct HAPDataSendDataStreamProtocol {
    /**
     * HomeKit Data Stream protocol handler base. Must refer to kHAPDataSendDataStreamProtocol_Base.
     */
    const HAPDataStreamProtocolBase *base;
    
    /**
     * "dataSend" HomeKit Data Stream protocol handler storage. Must be zero initialized.
     */
    struct {
        /**
         * Number of concurrent HomeKit Data Streams that the HomeKit Data Stream dispatcher supports
         * where this HomeKit Data Stream protocol handler is registered.
         */
        size_t numDataStreams;
    
        /**
         * Memory for HomeKit Data Stream protocol handler contexts. Must be zero-initialized.
         *
         * - One instance must be provided for each concurrently supported HomeKit Data Stream.
         */
        HAPDataSendDataStreamProtocolContextRef *protocolContexts;
    } storage;
    
    /**
     * Callbacks.
     */
    struct {
        /**
         * The callback used to handle accepted HomeKit Data Streams.
         *
         * - To open a "dataSend" stream use HAPDataSendDataStreamProtocolOpen.
         *
         * @param      server               Accessory server.
         * @param      dispatcher           HomeKit Data Stream dispatcher.
         * @param      dataStreamProtocol   HomeKit Data Stream protocol handler.
         * @param      request              Request that originated the HomeKit Data Stream.
         * @param      dataStream           HomeKit Data Stream.
         * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
         */
        void (*handleAccept)(
            HAPAccessoryServerRef *server,
            HAPDataStreamDispatcherRef *dispatcher,
            HAPDataSendDataStreamProtocol *dataStreamProtocol,
            const HAPServiceRequest *request,
            HAPDataStreamHandle dataStream,
            void *_Nullable context);
        
        /**
         * The callback used when a HomeKit Data Stream is invalidated.
         *
         * - /!\ WARNING: The HomeKit Data Stream must no longer be used after this callback returns.
         *
         * @param      server               Accessory server.
         * @param      dispatcher           HomeKit Data Stream dispatcher.
         * @param      dataStreamProtocol   HomeKit Data Stream protocol handler.
         * @param      request              Request that originated the HomeKit Data Stream.
         * @param      dataStream           HomeKit Data Stream.
         * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
         */
        void (*handleInvalidate)(
            HAPAccessoryServerRef *server,
            HAPDataStreamDispatcherRef *dispatcher,
            HAPDataSendDataStreamProtocol *dataStreamProtocol,
            const HAPServiceRequest *request,
            HAPDataStreamHandle dataStream,
            void *_Nullable context);
    } callbacks;
};
HAP_STATIC_ASSERT(HAP_OFFSETOF(HAPDataSendDataStreamProtocol, base) == 0, HAPDataSendDataStreamProtocol_base);

/**
 * "dataSend" stream type.
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 13.1.1 Start
 */
HAP_ENUM_BEGIN(uint8_t, HAPDataSendDataStreamProtocolType) {
    /** Siri. */
    kHAPDataSendDataStreamProtocolType_Audio_Siri = 1
} HAP_ENUM_END(uint8_t, HAPDataSendDataStreamProtocolType);

/**
 * "dataSend" close reason.
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 13.1.4 Close
 */
HAP_ENUM_BEGIN(uint8_t, HAPDataSendDataStreamProtocolCloseReason) {
    /** Normal Close. */
    kHAPDataSendDataStreamProtocolCloseReason_Normal = 0,
    
    /** Controller will not allow the Accessory to send this transfer (eg, this stream type is not permitted). */
    kHAPDataSendDataStreamProtocolCloseReason_NotAllowed = 1,
    
    /** Controller cannot accept this transfer right now (but normally it could have). */
    kHAPDataSendDataStreamProtocolCloseReason_Busy = 2,
    
    /** Accessory will not finish the transfer. */
    kHAPDataSendDataStreamProtocolCloseReason_Cancelled = 3,
    
    /** Controller does not support this stream type. */
    kHAPDataSendDataStreamProtocolCloseReason_Unsupported = 4,
    
    /** Some other protocol error occurred and the stream has failed. */
    kHAPDataSendDataStreamProtocolCloseReason_UnexpectedFailure = 5,
    
    /** Accessory could not start the session. */
    kHAPDataSendDataStreamProtocolCloseReason_Timeout = 6
} HAP_ENUM_END(uint8_t, HAPDataSendDataStreamProtocolCloseReason);

/**
 * "dataSend" stream.
 */
typedef HAP_OPAQUE(136) HAPDataSendDataStreamProtocolStreamRef;

/**
 * "dataSend" stream callbacks.
 */
typedef struct HAPDataSendDataStreamProtocolStreamCallbacks HAPDataSendDataStreamProtocolStreamCallbacks;
struct HAPDataSendDataStreamProtocolStreamCallbacks {
    /**
     * The callback used when a "dataSend" stream has been closed.
     *
     * - /!\ WARNING: The "dataSend" stream must no longer be used
     *   after this callback returns.
     *
     * @param      server               Accessory server.
     * @param      dispatcher           HomeKit Data Stream dispatcher.
     * @param      dataStreamProtocol   HomeKit Data Stream protocol handler.
     * @param      request              Request that originated the HomeKit Data Stream.
     * @param      dataStream           HomeKit Data Stream.
     * @param      dataSendStream       "dataSend" stream.
     * @param      error                kHAPError_None           If successful.
     *                                  kHAPError_InvalidState   If the HomeKit Data Stream is being invalidated.
     *                                                           No further operations may be started.
     *                                  kHAPError_InvalidData    If an unexpected message has been received.
     *                                  kHAPError_OutOfResources If out of resources to receive message.
     * @param      closeReason          Reason why "dataSend" stream was closed.
     * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
     */
    void (*handleClose)(
        HAPAccessoryServerRef *server,
        HAPDataStreamDispatcherRef *dispatcher,
        HAPDataSendDataStreamProtocol *dataStreamProtocol,
        const HAPDataStreamRequest *request,
        HAPDataStreamHandle dataStream,
        HAPDataSendDataStreamProtocolStreamRef *dataSendStream,
        HAPError error,
        HAPDataSendDataStreamProtocolCloseReason closeReason,
        void *_Nullable context);
    
    /**
     * The callback used to report that a "dataSend" stream has been opened.
     *
     * - Use HAPDataSendDataStreamProtocolSendData to send packets.
     *
     * @param      server               Accessory server.
     * @param      dispatcher           HomeKit Data Stream dispatcher.
     * @param      dataStreamProtocol   HomeKit Data Stream protocol handler.
     * @param      request              Request that originated the HomeKit Data Stream.
     * @param      dataStream           HomeKit Data Stream.
     * @param      dataSendStream       "dataSend" stream.
     * @param      callbacks            Callbacks supplied when opening the "dataSend" stream.
     * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
     */
    void (*handleOpen)(
        HAPAccessoryServerRef *server,
        HAPDataStreamDispatcherRef *dispatcher,
        HAPDataSendDataStreamProtocol *dataStreamProtocol,
        const HAPDataStreamRequest *request,
        HAPDataStreamHandle dataStream,
        HAPDataSendDataStreamProtocolStreamRef *dataSendStream,
        void *_Nullable context);
};

/**
 * Opens a "dataSend" stream.
 *
 * Example flow:
 * - HAPDataSendDataStreamProtocolOpenStream.
 *   - handleOpen callback on "dataSend" stream.
 *   - HAPDataSendDataStreamProtocolSendData => completion handler.
 *   - HAPDataSendDataStreamProtocolSendData => completion handler.
 *   - HAPDataSendDataStreamProtocolSendData => completion handler.
 *   - ...
 *   - HAPDataSendDataStreamProtocolSendData (endOfStream: true) => completion handler,
 *     or HAPDataSendDataStreamProtocolCancelStream.
 *   - handleClose callback on "dataSend" stream.
 *
 * @param      server               Accessory server.
 * @param      dispatcher           HomeKit Data Stream dispatcher.
 * @param      dataStream           HomeKit Data Stream.
 * @param[out] dataSendStream       "dataSend" stream.
 * @param      type                 "dataSend" stream type.
 * @param      callbacks            "dataSend" stream callbacks. Must remain valid.
 */
void HAPDataSendDataStreamProtocolOpen(
    HAPAccessoryServerRef *server,
    HAPDataStreamDispatcherRef *dispatcher,
    HAPDataStreamHandle dataStream,
    HAPDataSendDataStreamProtocolStreamRef *dataSendStream,
    HAPDataSendDataStreamProtocolType type,
    const HAPDataSendDataStreamProtocolStreamCallbacks *callbacks);

/**
 * Cancels a "dataSend" stream.
 *
 * - This operation is asynchronous. Once the stream has been cancelled the handleClose callback on the
 *   "dataSend" stream will be called.
 *
 * @param      server               Accessory server.
 * @param      dispatcher           HomeKit Data Stream dispatcher.
 * @param      dataStream           HomeKit Data Stream.
 * @param      dataSendStream       "dataSend" stream.
 */
void HAPDataSendDataStreamProtocolCancel(
    HAPAccessoryServerRef *server,
    HAPDataStreamDispatcherRef *dispatcher,
    HAPDataStreamHandle dataStream,
    HAPDataSendDataStreamProtocolStreamRef *dataSendStream);

/**
 * "dataSend" HomeKit Data Stream protocol handler packet.
 */
typedef struct {
    /**
     * Packet data.
     */
    struct {
        void *bytes;                    /**< Buffer. */
        size_t numBytes;                /**< Length of buffer. */
    } data;
    
    /**
     * Meta data for the packet.
     */
    struct {
        /**
         * Type of metadata. Must match "dataSend" stream that was specified when it was opened.
         */
        HAPDataSendDataStreamProtocolType type;
        
        /** Type-specific metadata. */
        union {
            /** Audio. */
            union {
                /**
                 * Siri.
                 *
                 * - Used when "dataSend" stream was opened with kHAPDataSendDataStreamProtocolType_Audio_Siri.
                 */
                struct {
                    /**
                     * The RMS value is set for each Opus frame. The following formula computes the RMS value:
                     * RMS = sqrt((s_0^2 + s_1^2... + s_N-1^2) / number of samples) with the s_i in the range -1 ... +1
                     */
                    float rms;
            
                    /**
                     * Integer that starts at 0 and counts up in each frame. Frames must still be delivered in order
                     * without gaps.
                     */
                    int64_t sequenceNumber;
                } siri;
            } audio;
        } _;
    } metadata;
} HAPDataSendDataStreamProtocolPacket;

/**
 * Completion handler of a "dataSend" stream send data operation.
 *
 * @param      server               Accessory server.
 * @param      dispatcher           HomeKit Data Stream dispatcher.
 * @param      dataStreamProtocol   HomeKit Data Stream protocol handler.
 * @param      request              Request that originated the HomeKit Data Stream.
 * @param      dataStream           HomeKit Data Stream.
 * @param      dataSendStream       "dataSend" stream.
 * @param      error                kHAPError_None           If successful.
 *                                  kHAPError_InvalidState   If the HomeKit Data Stream is being invalidated.
 *                                                           No further operations may be started.
 * @param      scratchBytes         Temporary buffer provided when starting the operation.
 * @param      numScratchBytes      Length of temporary buffer.
 * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
 */
typedef void (*HAPDataSendDataStreamProtocolStreamSendDataCompletionHandler)(
    HAPAccessoryServerRef *server,
    HAPDataStreamDispatcherRef *dispatcher,
    HAPDataSendDataStreamProtocol *dataStreamProtocol,
    const HAPDataStreamRequest *request,
    HAPDataStreamHandle dataStream,
    HAPDataSendDataStreamProtocolStreamRef *dataSendStream,
    HAPError error,
    void *scratchBytes,
    size_t numScratchBytes,
    void *_Nullable context);

/**
 * Sends data over a "dataSend" stream.
 *
 * - References to data buffers in the packets to send are treated as immutable references.
 *
 * @param      server               Accessory server.
 * @param      dispatcher           HomeKit Data Stream dispatcher.
 * @param      dataStream           HomeKit Data Stream.
 * @param      dataSendStream       "dataSend" stream.
 * @param      scratchBytes         Scratch buffer. Must remain valid.
 * @param      numScratchBytes      Length of scratch buffer.
 * @param      packets              Packets to send.
 * @param      numPackets           Number of packets to send.
 * @param      endOfStream          Indicates the final frame.
 * @param      completionHandler    Completion handler to call when the operation is complete.
 *
 * @return kHAPError_None           If successful.
 * @return kHAPError_OutOfResources If the scratch buffer is not large enough.
 */
HAP_RESULT_USE_CHECK
HAPError HAPDataSendDataStreamProtocolSendData(
    HAPAccessoryServerRef *server,
    HAPDataStreamDispatcherRef *dispatcher,
    HAPDataStreamHandle dataStream,
    HAPDataSendDataStreamProtocolStreamRef *dataSendStream,
    void *scratchBytes,
    size_t numScratchBytes,
    HAPDataSendDataStreamProtocolPacket *_Nullable packets,
    size_t numPackets,
    bool endOfStream,
    HAPDataSendDataStreamProtocolStreamSendDataCompletionHandler completionHandler);
    
#if __has_feature(nullability)
#pragma clang assume_nonnull end
#endif

#ifdef __cplusplus
}
#endif

#endif
