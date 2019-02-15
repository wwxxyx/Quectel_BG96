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

#ifndef HAP_DATA_STREAM_PROTOCOLS_H
#define HAP_DATA_STREAM_PROTOCOLS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "HAP.h"

#if __has_feature(nullability)
#pragma clang assume_nonnull begin
#endif

/**
 * HomeKit Data Stream dispatcher.
 *
 * A HomeKit Data Stream dispatcher may be used to simplify handling of messages exchanged over HomeKit Data Streams.
 * Incoming messages are delivered to their corresponding HomeKit Data Stream protocol handlers.
 * Outgoing messages from protocol handlers are multiplexed over the underlying HomeKit Data Stream.
 *
 * An accessory that opts to have its HomeKit Data Streams managed by a HomeKit Data Stream dispatcher must:
 * - Specify kHAPDataStreamDispatcher_DataStreamCallbacks as the HAPAccessory's dataStream.delegate.callbacks.
 * - Specify a HAPDataStreamDispatcherRef as the HAPAccessory's dataStream.delegate.context.
 * The same HomeKit Data Stream dispatcher may be assigned to multiple accessories.
 *
 * The HomeKit Data Stream dispatcher must be initialized before the HomeKit Accessory Server is started:
 * - For each concurrently supported HomeKit Data Stream memory for a HAPDataStreamDescriptorRef must be provided.
 * - A list of HAPDataStreamProtocol instances representing supported HomeKit Data Stream protocols must be supplied.
 */
typedef HAP_OPAQUE(32) HAPDataStreamDispatcherRef;

/**
 * HomeKit Data Stream callbacks for use with a HomeKit Data Stream dispatcher.
 *
 * - Must be set as the HAPAccessory's dataStream.delegate.callbacks when the accessory's HomeKit Data Streams
 *   are managed by a HomeKit Data Stream dispatcher.
 */
extern const HAPDataStreamCallbacks kHAPDataStreamDispatcher_DataStreamCallbacks;

/**
 * HomeKit Data Stream descriptor for use with a HomeKit Data Stream dispatcher.
 *
 * - When a HomeKit Data Stream dispatcher is initialized one of these structures must be supplied for each
 *   concurrently supported HomeKit Data Stream.
 */
typedef HAP_OPAQUE(280) HAPDataStreamDescriptorRef;

/**
 * HomeKit Data Stream handle.
 *
 * - Identifies a HomeKit Data Stream that is managed by a HomeKit Data Stream dispatcher.
 *
 * - The value of a HomeKit Data Stream handle ranges from 0 through N - 1 with N being the number of concurrently
 *   supported HomeKit Data Streams by the HomeKit Data Stream dispatcher. Values are specific to the HomeKit Data
 *   Stream dispatcher that manages the underlying HomeKit Data Stream.
 */
typedef uint16_t HAPDataStreamHandle;

/**
 * HomeKit Data Stream protocol handler for use with a HomeKit Data Stream dispatcher.
 *
 * - A HomeKit Data Stream protocol is identified by its name. The HomeKit Data Stream dispatcher delivers incoming
 *   messages based on the protocol name that they refer to. Within the context of a HomeKit Data Stream dispatcher
 *   registered protocol handlers must support protocols with different names.
 *
 * - A concrete HAPDataStreamProtocol must specify a pointer to an initialized HAPDataStreamProtocolBase as its first
 *   element. Additional data after the HAPDataStreamProtocolBase is considered opaque by the HomeKit Data Stream
 *   dispatcher.
 */
typedef void HAPDataStreamProtocol;

/**
 * HomeKit Data Stream dispatcher storage required when initializing a HomeKit Data Stream dispatcher.
 *
 * - For accessories that support HomeKit Data Stream, one of these structures needs to be allocated
 *   and provided as part of the initialization options in HAPDataStreamDispatcherCreate.
 *
 * - The provided memory (including the HAPDataStreamDispatcherStorage structure) must remain valid
 *   while the accessory server is initialized.
 */
typedef struct {
    /**
     * Number of concurrent HomeKit Data Streams that this HomeKit Data Stream dispatcher should manage.
     */
    size_t numDataStreams;
    
    /**
     * Memory for HomeKit Data Stream descriptors.
     *
     * - One HAPDataStreamDescriptorRef must be provided for each concurrently supported HomeKit Data Stream.
     */
    HAPDataStreamDescriptorRef *dataStreamDescriptors;
    
    /**
     * Array of supported HomeKit Data Stream protocol handlers. NULL-terminated.
     *
     * - /!\ HomeKit Data Stream protocol handlers must be initialized in advance.
     */
    HAPDataStreamProtocol *_Nullable const *_Nonnull dataStreamProtocols;
} HAPDataStreamDispatcherStorage;

/**
 * HomeKit Data Stream dispatcher initialization options.
 */
typedef struct {
    /**
     * HomeKit Data Stream dispatcher storage. Storage must remain valid while the accessory server is initialized.
     */
    const HAPDataStreamDispatcherStorage *storage;
} HAPDataStreamDispatcherOptions;

/**
 * Initializes a HomeKit Data Stream dispatcher.
 *
 * @param      server               Accessory server.
 * @param[out] dispatcher           HomeKit Data Stream dispatcher.
 * @param      options              Initialization options of the HomeKit Data Stream dispatcher.
 */
void HAPDataStreamDispatcherCreate(
    HAPAccessoryServerRef *server,
    HAPDataStreamDispatcherRef *dispatcher,
    const HAPDataStreamDispatcherOptions *options);

/**
 * HomeKit Data Stream protocol handler base.
 *
 * - A pointer to an initialized instance of this structure must be the first element of a concrete
 *   HAPDataStreamProtocol. Additional data after the HAPDataStreamProtocolBase is considered opaque by the HomeKit Data
 *   Stream dispatcher.
 *
 * **Example**
 
   @code{.c}
 
   typedef const HAPDataStreamProtocolBase kHAPFooDataStreamProtocol_Base;
   typedef struct {
       \/\*\*
        \* HomeKit Data Stream protocol handler base. Must refer to kHAPFooDataStreamProtocol_Base.
        \*\/
       const HAPDataStreamProtocolBase *base;
   } HAPFooDataStreamProtocol;
   HAP_STATIC_ASSERT(HAP_OFFSETOF(HAPFooDataStreamProtocol, base) == 0, HAPFooDataStreamProtocol_base);
   
   @endcode
 */
typedef struct HAPDataStreamProtocolBase HAPDataStreamProtocolBase;

#if __has_feature(nullability)
#pragma clang assume_nonnull end
#endif

#include "HAPDataStreamProtocols+DataSend.h"
#include "HAPDataStreamProtocols+TargetControl.h"

#ifdef __cplusplus
}
#endif

#endif
