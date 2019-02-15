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

#ifndef HAP_H
#define HAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "HAPPlatform.h"

#if __has_feature(nullability)
#pragma clang assume_nonnull begin
#endif

/**
 * Compatibility version of the HAP interface.
 *
 * - If this version differs from the one returned by HAPGetCompatibilityVersion,
 *   the library is incompatible and must not be used.
 */
#define HAP_COMPATIBILITY_VERSION (4)

/**
 * Gets the compatibility version of the HAP library.
 *
 * - If the compatibility version differs from HAP_COMPATIBILITY_VERSION,
 *   the library is incompatible and may not be used.
 *
 * @return Compatibility version of the HAP library.
 */
HAP_RESULT_USE_CHECK
uint32_t HAPGetCompatibilityVersion(void);

/**
 * Gets the identification of the HAP library.
 *
 * @return HAP library identification string.
 */
HAP_RESULT_USE_CHECK
const char *HAPGetIdentification(void);

/**
 * Gets the version string of the HAP library.
 *
 * @return Version string of the HAP library.
 */
HAP_RESULT_USE_CHECK
const char *HAPGetVersion(void);

/**
 * Gets the build version string of the HAP library.
 *
 * @return Build version string of the HAP library.
 */
HAP_RESULT_USE_CHECK
const char *HAPGetBuild(void);

/**
 * 128-bit UUID.
 *
 * - The encoding of UUIDs uses reversed byte order compared to RFC 4122, i.e. network byte order backwards.
 *
 * Sample:
 *   UUID: 00112233-4455-6677-8899-AABBCCDDEEFF
 *   bytes: 0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00
 */
typedef struct {
    uint8_t bytes[16];              /**< UUID bytes in reversed network byte order. */
} HAPUUID;
HAP_STATIC_ASSERT(sizeof (HAPUUID) == 16, HAPUUID);
HAP_NONNULL_SUPPORT(HAPUUID)

/**
 * Returns whether or not two UUIDs are equal.
 *
 * @param      uuid                 UUID to compare.
 * @param      otherUUID            UUID to compare with.
 *
 * @return true                     If both UUIDs are equal.
 * @return false                    Otherwise.
 */
HAP_RESULT_USE_CHECK
bool HAPUUIDAreEqual(
    const HAPUUID *uuid,
    const HAPUUID *otherUUID);

/**
 * TLV.
 */
typedef struct {
    /**
     * Type.
     *
     * @remark The type semantics depend on the context.
     */
    uint8_t type;

    /**
     * Value.
     */
    struct {
        /**
         * Buffer containing value.
         */
        const void *_Nullable bytes;

        /**
         * Length of buffer.
         */
        size_t numBytes;
    } value;
} HAPTLV;

/**
 * TLV Reader.
 */
typedef HAP_OPAQUE(24) HAPTLVReaderRef;

/**
 * Creates a new TLV reader.
 *
 * @param      reader               An uninitialized TLV reader.
 * @param      bytes                Buffer containing raw TLV data. Will be modified by the reader. Must remain valid.
 * @param      numBytes             Length of buffer.
 */
void HAPTLVReaderCreate(
    HAPTLVReaderRef *reader,
    void *_Nullable bytes,
    size_t numBytes);

/**
 * Fetches the next TLV item from a TLV reader's buffer. TLV item content is NULL-terminated for convenience.
 *
 * @param      reader               Reader to fetch TLV item from.
 * @param[out] found                True if a TLV item has been fetched. False otherwise.
 * @param[out] tlv                  Next TLV item. Valid when @p found is true. Fragments are merged automatically.
 *
 * @return kHAPError_None           If successful.
 * @return kHAPError_InvalidData    If parsing failed (Incomplete item, or violation of TLV rules).
 */
HAP_RESULT_USE_CHECK
HAPError HAPTLVReaderGetNext(
    HAPTLVReaderRef *reader,
    bool *found,
    HAPTLV *tlv);

/**
 * Fetches TLV items by type into @p tlvs.
 *
 * On input, @p tlvs is a NULL-terminated array to TLV items.
 * For each TLV item, the type shall be specified. Types must be unique.
 *
 * On output, @p tlvs are updated to contain the actual TLV items.
 * If multiple TLV items with one of the requested types are found, an error is returned.
 * TLV item contents are NULL-terminated for convenience.
 *
 * - This API must be used on a freshly initialized TLV reader. All TLV items will be read.
 *
 * @param     reader                Reader to fetch TLV items from.
 * @param[in,out] tlvs              NULL-terminated array to TLV structures.
 *                                  On input, type filters are specified. On output, actual TLV items are filled in.
 *                                  For TLV items not found, values are set to NULL.
 *                                  Each type may only be requested once.
 *
 * @return kHAPError_None           If successful.
 * @return kHAPError_InvalidData    If parsing failed (Incomplete item, or violation of TLV rules), or if multiple TLV
 *                                  items with a requested type are found.
 */
HAP_RESULT_USE_CHECK
HAPError HAPTLVReaderGetAll(
    HAPTLVReaderRef *reader,
    HAPTLV *_Nullable const *_Nonnull tlvs);

/**
 * TLV Writer.
 */
typedef HAP_OPAQUE(32) HAPTLVWriterRef;
HAP_NONNULL_SUPPORT(HAPTLVWriterRef)

/**
 * Creates a new TLV writer.
 *
 * @param      writer               An uninitialized TLV writer.
 * @param      bytes                Buffer to serialize TLV data into. Must remain valid.
 * @param      maxBytes             Capacity of the buffer.
 */
void HAPTLVWriterCreate(
    HAPTLVWriterRef *writer,
    void *bytes,
    size_t maxBytes);

/**
 * Serializes a TLV item and appends it to the writer's buffer.
 *
 * - Multiple items of same type must be separated by an item with different type.
 *
 * @param      writer               Writer to append serialized TLV to.
 * @param      tlv                  TLV to write.
 *
 * @return kHAPError_None           If successful.
 * @return kHAPError_OutOfResources If the writer's buffer is not large enough to hold the serialized TLV item.
 */
HAP_RESULT_USE_CHECK
HAPError HAPTLVWriterAppend(
    HAPTLVWriterRef *writer,
    const HAPTLV *tlv);

/**
 * Retrieves the buffer containing the serialized TLV data.
 *
 * @param      writer               Writer to retrieve buffer from.
 * @param[out] bytes                Buffer containing serialized TLV data written so far.
 * @param[out] numBytes             Length of buffer.
 */
void HAPTLVWriterGetBuffer(
    const HAPTLVWriterRef *writer,
    void *_Nonnull *_Nonnull bytes,
    size_t *numBytes);

/**
 * Retrieves a temporary buffer of unused memory, e.g. to prepare the next TLV payload to be written.
 *
 * - /!\ The buffer becomes invalid after writing to the TLV writer again.
 *
 * @param      writer               Writer to retrieve buffer from.
 * @param[out] scratchBytes         Temporary buffer of free memory.
 * @param[out] numScratchBytes      Capacity of scratch buffer.
 */
void HAPTLVWriterGetScratchBytes(
    const HAPTLVWriterRef *writer,
    void *_Nonnull *_Nonnull scratchBytes,
    size_t *numScratchBytes);

/**
 * HomeKit Accessory server.
 */
typedef HAP_OPAQUE(2088) HAPAccessoryServerRef;
HAP_NONNULL_SUPPORT(HAPAccessoryServerRef)

/**
 * HomeKit Session.
 */
typedef HAP_OPAQUE(488) HAPSessionRef;
HAP_NONNULL_SUPPORT(HAPSessionRef)

/**
 * HomeKit Data Stream.
 */
typedef HAP_OPAQUE(504) HAPDataStreamRef;
HAP_NONNULL_SUPPORT(HAPDataStreamRef)

/**
 * Type of a data packet sent or received over a HomeKit Data Stream.
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 6.10.2 Frame Format
 */
typedef uint8_t HAPDataStreamDataType;

/**
 * Maximum size of a HomeKit Data Stream payload.
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 6.10.2 Frame Format
 */
#define kHAPDataStream_MaxPayloadBytes ((size_t) ((1u << 20u) - 1u))

/**
 * Formats that HomeKit characteristics can have.
 *
 * - IMPORTANT: The format must always match the type of the characteristic structure!
 *   For example, kHAPCharacteristicFormat_UInt8 may only be used on HAPUInt8Characteristic.
 *
 * - For Apple-defined characteristics, the format must be used exactly as defined in the specification.
 *   For example, in the specification the Brightness characteristic is defined to have type "int".
 *   Therefore, a HAPIntCharacteristic must be used for it (with format kHAPCharacteristicFormat_Int).
 *
 * - For vendor-specific characteristics, any format may be chosen.
 */
HAP_ENUM_BEGIN(uint8_t, HAPCharacteristicFormat) {
    /**
     * Opaque data blob. Raw bytes.
     *
     * - Default format in case no other format has been specified.
     */
    kHAPCharacteristicFormat_Data,

    /**
     * Boolean. True or false.
     */
    kHAPCharacteristicFormat_Bool,

    /**
     * Unsigned 8-bit integer.
     */
    kHAPCharacteristicFormat_UInt8,

    /**
     * Unsigned 16-bit integer.
     */
    kHAPCharacteristicFormat_UInt16,

    /**
     * Unsigned 32-bit integer.
     */
    kHAPCharacteristicFormat_UInt32,

    /**
     * Unsigned 64-bit integer.
     */
    kHAPCharacteristicFormat_UInt64,

    /**
     * Signed 32-bit integer.
     */
    kHAPCharacteristicFormat_Int,

    /**
     * 32-bit floating point.
     */
    kHAPCharacteristicFormat_Float,

    /**
     * UTF-8 string.
     */
    kHAPCharacteristicFormat_String,

    /**
     * One or more TLV8s.
     */
    kHAPCharacteristicFormat_TLV8
} HAP_ENUM_END(uint8_t, HAPCharacteristicFormat);

/**
 * Properties that HomeKit characteristics can have.
 *
 * For Apple-defined characteristics, the default values for the following properties are defined by the specification:
 * - readable (Paired Read)
 * - writable (Paired Write)
 * - supportsEventNotification (Notify)
 *
 * The remaining properties have to be evaluated on a case by case basis.
 */
typedef struct {
    /**
     * The characteristic is readable.
     *
     * - A read handler must be plugged into the characteristic structure's corresponding callback field.
     *   Only controllers with a secured connection can perform reads.
     */
    bool readable : 1;

    /**
     * The characteristic is writable.
     *
     * - A write handler must be plugged into the characteristic structure's corresponding callback field.
     *   Only controllers with a secured connection can perform writes.
     */
    bool writable : 1;

    /**
     * The characteristic supports notifications using the event connection established by the controller.
     * The event connection provides unidirectional communication from the accessory to the controller.
     *
     * - A read handler must be plugged into the characteristic structure's corresponding callback field.
     *   Only controllers with a secured connection can subscribe to events.
     *
     * - When the characteristic state changes, the HAPAccessoryServerRaiseEvent or
     *   HAPAccessoryServerRaiseEventOnSession function must be called.
     */
    bool supportsEventNotification : 1;

    /**
     * The characteristic should be hidden from the user.
     *
     * - This is useful for characteristics to configure the accessory or to update firmware on the accessory.
     *   Generic HomeKit applications on the controller won't show these characteristics.
     *
     * - When all characteristics in a service are marked hidden then the service must also be marked as hidden.
     */
    bool hidden : 1;

    /**
     * The characteristic will only be accessible by admin controllers.
     *
     * - Reads and writes to the characteristic will only execute if the controller has admin permissions.
     *
     * - Event notification values for the characteristic will only be delivered to controllers with admin permissions.
     */
    bool requiresAdminPermissions : 1;

    /**
     * The characteristic requires time sensitive actions.
     *
     * - Writes to the characteristic will only execute if the accessory can be reached within a short time frame.
     *   This is useful for example for security class characteristics like Lock Target State or Target Door State.
     *
     * - A write handler must be plugged into the characteristic structure's corresponding callback field.
     *   The characteristic must also be marked as writable.
     */
    bool requiresTimedWrite : 1;

    /**
     * The characteristic requires additional authorization data.
     *
     * - Additional authorization data is controller-provided data that the accessory may use to validate that the
     *   controller is authorized to perform a requested operation. The contents of the authorization data are
     *   manufacturer specific. The additional authorization data is provided by a third-party app to iOS.
     *
     * - A write handler must be plugged into the characteristic structure's corresponding callback field.
     *   The characteristic must also be marked as writable.
     *
     * - It is left up to the write handler to validate additional authorization data.
     *   If authorization is insufficient, kHAPError_NotAuthorized must be returned from the handler.
     *
     * @see iOS documentation for [HMCharacteristic updateAuthorizationData(_:completionHandler:)]
     *
     * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
     *      Section 2.3.3.2 Additional Authorization Data
     */
    bool supportsAuthorizationData : 1;

    /**
     * These properties only affect connections over IP (Ethernet / Wi-Fi).
     *
     * - If the accessory only supports Bluetooth, these properties can be ignored.
     */
    struct {
        /**
         * This flag prevents the characteristic from being read during discovery.
         *
         * - Normally, all characteristic values are being read during discovery over an IP transport.
         *   If a characteristic maintains state across multiple reads / writes, depending on the timing
         *   such discovery operations may interfere with correct operation.
         *
         * - Setting this flag prevents the characteristic from being read during discovery.
         *   Instead, a NULL value will be sent to the controller during discovery. Only explicit reads are processed.
         *
         * - This property may be useful for characteristics that handle firmware updates of the accessory.
         */
        bool controlPoint : 1;

        /**
         * The characteristic supports including a response value in the response to write request.
         *
         * - A write handler must be plugged into the characteristic structure's corresponding callback field.
         *   The characteristic must also be marked as writable.
         *
         * - A read handler must be plugged into the characteristic structure's corresponding callback field.
         *   But the characteristic does not have to be marked as readable.
         */
        bool supportsWriteResponse : 1;
    } ip;

    /**
     * These properties only affect connections over Bluetooth LE.
     *
     * - If the accessory only supports IP (Ethernet, Wi-Fi), these properties can be ignored.
     */
    struct {
        /**
         * The characteristic supports broadcast notifications. Such broadcasts happen when the characteristic state
         * changes while no controller is connected. This allows paired controllers to quickly react to the notification
         * without having to re-establish a secured connection.
         *
         * - When this property is not set, controllers may only receive state updates while connected.
         *
         * - A read handler must be plugged into the characteristic structure's corresponding callback field.
         *   Only paired controllers can decode and process the broadcasts.
         *
         * - When the characteristic state changes, the HAPAccessoryServerRaiseEvent or
         *   HAPAccessoryServerRaiseEventOnSession function must be called.
         */
        bool supportsBroadcastNotification : 1;

        /**
         * The characteristic supports disconnected notifications. When the characteristic state changes while no
         * controller is connected, paired controllers will re-establish a secured connection to the accessory and
         * fetch the updated characteristic state.
         *
         * - When this property is set, the following properties must also be set:
         *   - readable
         *   - supportsEventNotification
         *   - ble.supportsBroadcastNotification
         *
         * - This property must be set on at least one characteristic of an accessory to work around an issue
         *   in certain versions of the Home app that would otherwise claim that Additional Setup is required.
         *
         * - Disconnected events should only be used to reflect important state changes in the accessory.
         *   For example, contact sensor state changes or current door state changes should use this property.
         *   On the other hand, a temperature sensor must not use this property for changes in temperature readings.
         *
         * - A read handler must be plugged into the characteristic structure's corresponding callback field.
         *
         * - When the characteristic state changes, the HAPAccessoryServerRaiseEvent or
         *   HAPAccessoryServerRaiseEventOnSession function must be called.
         */
        bool supportsDisconnectedNotification : 1;

        /**
         * The characteristic is always readable, even before a secured session is established.
         *
         * - This is mainly an internal property that is used on characteristics that handle the pairing process.
         *
         * - A read handler must be plugged into the characteristic structure's corresponding callback field.
         */
        bool readableWithoutSecurity : 1;

        /**
         * The characteristic is always writable, even before a secured session is established.
         *
         * - This is mainly an internal property that is used on characteristics that handle the pairing process.
         *
         * - A write handler must be plugged into the characteristic structure's corresponding callback field.
         */
        bool writableWithoutSecurity : 1;
    } ble;
} HAPCharacteristicProperties;
HAP_STATIC_ASSERT(sizeof (HAPCharacteristicProperties) == 3, HAPCharacteristicProperties);

/**
 * Units that numeric HomeKit characteristics can have.
 *
 * For Apple-defined characteristics, the corresponding units are defined by the specification.
 */
HAP_ENUM_BEGIN(uint8_t, HAPCharacteristicUnits) {
    /**
     * Unitless. Used for example on enumerations.
     */
    kHAPCharacteristicUnits_None,

    /**
     * The unit of the characteristic is degrees celsius.
     */
    kHAPCharacteristicUnits_Celsius,

    /**
     * The unit of the characteristic is the degrees of an arc.
     */
    kHAPCharacteristicUnits_ArcDegrees,

    /**
     * The unit of the characteristic is a percentage %.
     */
    kHAPCharacteristicUnits_Percentage,

    /**
     * The unit of the characteristic is lux (that is, illuminance).
     */
    kHAPCharacteristicUnits_Lux,

    /**
     * The unit of the characteristic is seconds.
     */
    kHAPCharacteristicUnits_Seconds
} HAP_ENUM_END(uint8_t, HAPCharacteristicUnits);

/**
 * Transport type over which a request has been received or over which a response will be sent.
 */
HAP_ENUM_BEGIN(uint8_t, HAPTransportType) {
    /**
     * HAP over IP (Ethernet / Wi-Fi).
     */
    kHAPTransportType_IP = 1,

    /**
     * HAP over Bluetooth LE.
     */
    kHAPTransportType_BLE
} HAP_ENUM_END(uint8_t, HAPTransportType);

typedef void HAPCharacteristic;
typedef struct HAPService HAPService;
typedef struct HAPAccessory HAPAccessory;

// <editor-fold desc="HAPDataCharacteristic">

typedef struct HAPDataCharacteristic HAPDataCharacteristic;

/**
 * Data characteristic read request.
 */
typedef struct {
    /**
     * Transport type over which the response will be sent.
     */
    HAPTransportType transportType;

    /**
     * The session over which the response will be sent.
     *
     * - A controller may be logged in on multiple sessions concurrently.
     * - May be NULL if a request is generated internally (e.g. for BLE broadcasts while disconnected).
     *
     * - For remote requests (e.g. via Apple TV), the associated controller may not be
     *   the one who originated the request, but instead the admin controller who set up the Apple TV.
     */
    HAPSessionRef *_Nullable session;

    /**
     * The characteristic whose value is to be read.
     */
    const HAPDataCharacteristic *characteristic;

    /**
     * The service that contains the characteristic.
     */
    const HAPService *service;

    /**
     * The accessory that provides the service.
     */
    const HAPAccessory *accessory;
} HAPDataCharacteristicReadRequest;

/**
 * Data characteristic write request.
 */
typedef struct {
    /**
     * Transport type over which the request has been received.
     */
    HAPTransportType transportType;

    /**
     * The session over which the request has been received.
     *
     * - A controller may be logged in on multiple sessions concurrently.
     *
     * - For remote requests (e.g. via Apple TV), the associated controller may not be
     *   the one who originated the request, but instead the admin controller who set up the Apple TV.
     */
    HAPSessionRef *session;

    /**
     * The characteristic whose value is to be written.
     */
    const HAPDataCharacteristic *characteristic;

    /**
     * The service that contains the characteristic.
     */
    const HAPService *service;

    /**
     * The accessory that provides the service.
     */
    const HAPAccessory *accessory;

    /**
     * Whether the request appears to have originated from a remote controller, e.g. Apple TV or iCloud.
     */
    bool remote;

    /**
     * Additional authorization data.
     */
    struct {
        const void *_Nullable bytes;    /**< Raw AAD data, if applicable. */
        size_t numBytes;                /**< Length of additional authorization data. */
    } authorizationData;
} HAPDataCharacteristicWriteRequest;

/**
 * Data characteristic subscription request.
 */
typedef struct {
    /**
     * Transport type over which the request has been received.
     */
    HAPTransportType transportType;

    /**
     * The session over which the request has been received.
     *
     * - A controller may be logged in on multiple sessions concurrently.
     */
    HAPSessionRef *session;

    /**
     * The characteristic whose value is to be subscribed or unsubscribed.
     */
    const HAPDataCharacteristic *characteristic;

    /**
     * The service that contains the characteristic.
     */
    const HAPService *service;

    /**
     * The accessory that provides the service.
     */
    const HAPAccessory *accessory;
} HAPDataCharacteristicSubscriptionRequest;

/**
 * HomeKit Data characteristic.
 */
struct HAPDataCharacteristic {
    /**
     * Format. Must be kHAPCharacteristicFormat_Data.
     */
    HAPCharacteristicFormat format;

    /**
     * Instance ID.
     *
     * - Must be unique across all service or characteristic Instance IDs of the accessory.
     * - Must not change while the accessory is paired, including over firmware updates.
     * - Must not be 0.
     * - For accessories that support Bluetooth LE, must not exceed UINT16_MAX.
     */
    uint64_t iid;

    /**
     * The type of the characteristic.
     */
    const HAPUUID *characteristicType;

    /**
     * Description for debugging (based on "Type" field of HAP specification).
     */
    const char *debugDescription;

    /**
     * Description of the characteristic provided by the manufacturer of the accessory.
     */
    const char *_Nullable manufacturerDescription;

    /**
     * Characteristic properties.
     */
    HAPCharacteristicProperties properties;

    /**
     * Value constraints.
     */
    struct {
        uint32_t maxLength;             /**< Maximum length. */
    } constraints;

    /**
     * Callbacks.
     */
    struct {
        /**
         * The callback used to handle read requests.
         * On success, the value stored in the value buffer is sent back to the controller.
         *
         * - Required if the characteristic is marked readable in the characteristic properties.
         * - The callback must not block. Consider prefetching values if it would take too long.
         * - The returned value must satisfy the constraints of the characteristic.
         *
         * @param      server               Accessory server.
         * @param      request              Request.
         * @param[out] valueBytes           Value buffer.
         * @param      maxValueBytes        Capacity of value buffer.
         * @param[out] numValueBytes        Length of value buffer.
         * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
         *
         * @return kHAPError_None           If successful.
         * @return kHAPError_Unknown        If unable to communicate with requested service.
         * @return kHAPError_InvalidState   If the request cannot be processed in the current state.
         * @return kHAPError_OutOfResources If out of resources to process request.
         * @return kHAPError_Busy           If the request failed temporarily.
         */
        HAP_RESULT_USE_CHECK
        HAPError (*_Nullable handleRead)(
            HAPAccessoryServerRef *server,
            const HAPDataCharacteristicReadRequest *request,
            void *valueBytes,
            size_t maxValueBytes,
            size_t *numValueBytes,
            void *_Nullable context);

        /**
         * The callback used to handle write requests.
         *
         * - Required if the characteristic is marked writeable in the characteristic properties.
         * - The callback must not block. Consider queueing values if it would take too long.
         * - The value is already checked against the constraints of the characteristic.
         *
         * @param      server               Accessory server.
         * @param      request              Request.
         * @param      valueBytes           Value buffer.
         * @param      numValueBytes        Length of value buffer.
         * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
         *
         * @return kHAPError_None           If successful.
         * @return kHAPError_Unknown        If unable to communicate with requested service.
         * @return kHAPError_InvalidState   If the request cannot be processed in the current state.
         * @return kHAPError_InvalidData    If the controller sent a malformed request.
         * @return kHAPError_OutOfResources If out of resources to process request.
         * @return kHAPError_NotAuthorized  If additional authorization data is insufficient.
         * @return kHAPError_Busy           If the request failed temporarily.
         */
        HAP_RESULT_USE_CHECK
        HAPError (*_Nullable handleWrite)(
            HAPAccessoryServerRef *server,
            const HAPDataCharacteristicWriteRequest *request,
            const void *valueBytes,
            size_t numValueBytes,
            void *_Nullable context);

        /**
         * The callback used to handle subscribe requests.
         *
         * - The callback must not block. Consider queueing values if it would take too long.
         *
         * @param      server               Accessory server.
         * @param      request              Request.
         * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
         */
        void (*_Nullable handleSubscribe)(
            HAPAccessoryServerRef *server,
            const HAPDataCharacteristicSubscriptionRequest *request,
            void *_Nullable context);

        /**
         * The callback used to handle unsubscribe requests.
         *
         * - The callback must not block. Consider queueing values if it would take too long.
         *
         * @param      server               Accessory server.
         * @param      request              Request.
         * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
         */
        void (*_Nullable handleUnsubscribe)(
            HAPAccessoryServerRef *server,
            const HAPDataCharacteristicSubscriptionRequest *request,
            void *_Nullable context);
    } callbacks;
};

// </editor-fold>
// <editor-fold desc="HAPBoolCharacteristic">

typedef struct HAPBoolCharacteristic HAPBoolCharacteristic;

/**
 * Bool characteristic read request.
 */
typedef struct {
    /**
     * Transport type over which the response will be sent.
     */
    HAPTransportType transportType;

    /**
     * The session over which the response will be sent.
     *
     * - A controller may be logged in on multiple sessions concurrently.
     * - May be NULL if a request is generated internally (e.g. for BLE broadcasts while disconnected).
     *
     * - For remote requests (e.g. via Apple TV), the associated controller may not be
     *   the one who originated the request, but instead the admin controller who set up the Apple TV.
     */
    HAPSessionRef *_Nullable session;

    /**
     * The characteristic whose value is to be read.
     */
    const HAPBoolCharacteristic *characteristic;

    /**
     * The service that contains the characteristic.
     */
    const HAPService *service;

    /**
     * The accessory that provides the service.
     */
    const HAPAccessory *accessory;
} HAPBoolCharacteristicReadRequest;

/**
 * Bool characteristic write request.
 */
typedef struct {
    /**
     * Transport type over which the request has been received.
     */
    HAPTransportType transportType;

    /**
     * The session over which the request has been received.
     *
     * - A controller may be logged in on multiple sessions concurrently.
     *
     * - For remote requests (e.g. via Apple TV), the associated controller may not be
     *   the one who originated the request, but instead the admin controller who set up the Apple TV.
     */
    HAPSessionRef *session;

    /**
     * The characteristic whose value is to be written.
     */
    const HAPBoolCharacteristic *characteristic;

    /**
     * The service that contains the characteristic.
     */
    const HAPService *service;

    /**
     * The accessory that provides the service.
     */
    const HAPAccessory *accessory;

    /**
     * Whether the request appears to have originated from a remote controller, e.g. Apple TV or iCloud.
     */
    bool remote;

    /**
     * Additional authorization data.
     */
    struct {
        const void *_Nullable bytes;    /**< Raw AAD data, if applicable. */
        size_t numBytes;                /**< Length of additional authorization data. */
    } authorizationData;
} HAPBoolCharacteristicWriteRequest;

/**
 * Bool characteristic subscription request.
 */
typedef struct {
    /**
     * Transport type over which the request has been received.
     */
    HAPTransportType transportType;

    /**
     * The session over which the request has been received.
     *
     * - A controller may be logged in on multiple sessions concurrently.
     */
    HAPSessionRef *session;

    /**
     * The characteristic whose value is to be subscribed or unsubscribed.
     */
    const HAPBoolCharacteristic *characteristic;

    /**
     * The service that contains the characteristic.
     */
    const HAPService *service;

    /**
     * The accessory that provides the service.
     */
    const HAPAccessory *accessory;
} HAPBoolCharacteristicSubscriptionRequest;

/**
 * HomeKit Bool characteristic.
 */
struct HAPBoolCharacteristic {
    /**
     * Format. Must be kHAPCharacteristicFormat_Bool.
     */
    HAPCharacteristicFormat format;

    /**
     * Instance ID.
     *
     * - Must be unique across all service or characteristic Instance IDs of the accessory.
     * - Must not change while the accessory is paired, including over firmware updates.
     * - Must not be 0.
     * - For accessories that support Bluetooth LE, must not exceed UINT16_MAX.
     */
    uint64_t iid;

    /**
     * The type of the characteristic.
     */
    const HAPUUID *characteristicType;

    /**
     * Description for debugging (based on "Type" field of HAP specification).
     */
    const char *debugDescription;

    /**
     * Description of the characteristic provided by the manufacturer of the accessory.
     */
    const char *_Nullable manufacturerDescription;

    /**
     * Characteristic properties.
     */
    HAPCharacteristicProperties properties;

    /**
     * Callbacks.
     */
    struct {
        /**
         * The callback used to handle read requests.
         * On success, the value stored in @p value is sent back to the controller.
         *
         * - Required if the characteristic is marked readable in the characteristic properties.
         * - The callback must not block. Consider prefetching values if it would take too long.
         * - The returned value must satisfy the constraints of the characteristic.
         *
         * @param      server               Accessory server.
         * @param      request              Request.
         * @param[out] value                Value.
         * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
         *
         * @return kHAPError_None           If successful.
         * @return kHAPError_Unknown        If unable to communicate with requested service.
         * @return kHAPError_InvalidState   If the request cannot be processed in the current state.
         * @return kHAPError_OutOfResources If out of resources to process request.
         * @return kHAPError_Busy           If the request failed temporarily.
         */
        HAP_RESULT_USE_CHECK
        HAPError (*_Nullable handleRead)(
            HAPAccessoryServerRef *server,
            const HAPBoolCharacteristicReadRequest *request,
            bool *value,
            void *_Nullable context);

        /**
         * The callback used to handle write requests.
         *
         * - Required if the characteristic is marked writeable in the characteristic properties.
         * - The callback must not block. Consider queueing values if it would take too long.
         * - The value is already checked against the constraints of the characteristic.
         *
         * @param      server               Accessory server.
         * @param      request              Request.
         * @param      val                  Value.
         * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
         *
         * @return kHAPError_None           If successful.
         * @return kHAPError_Unknown        If unable to communicate with requested service.
         * @return kHAPError_InvalidState   If the request cannot be processed in the current state.
         * @return kHAPError_InvalidData    If the controller sent a malformed request.
         * @return kHAPError_OutOfResources If out of resources to process request.
         * @return kHAPError_NotAuthorized  If additional authorization data is insufficient.
         * @return kHAPError_Busy           If the request failed temporarily.
         */
        HAP_RESULT_USE_CHECK
        HAPError (*_Nullable handleWrite)(
            HAPAccessoryServerRef *server,
            const HAPBoolCharacteristicWriteRequest *request,
            bool value,
            void *_Nullable context);

        /**
         * The callback used to handle subscribe requests.
         *
         * - The callback must not block. Consider queueing values if it would take too long.
         *
         * @param      server               Accessory server.
         * @param      request              Request.
         * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
         */
        void (*_Nullable handleSubscribe)(
            HAPAccessoryServerRef *server,
            const HAPBoolCharacteristicSubscriptionRequest *request,
            void *_Nullable context);

        /**
         * The callback used to handle unsubscribe requests.
         *
         * - The callback must not block. Consider queueing values if it would take too long.
         *
         * @param      server               Accessory server.
         * @param      request              Request.
         * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
         */
        void (*_Nullable handleUnsubscribe)(
            HAPAccessoryServerRef *server,
            const HAPBoolCharacteristicSubscriptionRequest *request,
            void *_Nullable context);
    } callbacks;
};

// </editor-fold>
// <editor-fold desc="HAPUInt8Characteristic">

typedef struct HAPUInt8Characteristic HAPUInt8Characteristic;

/**
 * UInt8 characteristic read request.
 */
typedef struct {
    /**
     * Transport type over which the response will be sent.
     */
    HAPTransportType transportType;

    /**
     * The session over which the response will be sent.
     *
     * - A controller may be logged in on multiple sessions concurrently.
     * - May be NULL if a request is generated internally (e.g. for BLE broadcasts while disconnected).
     *
     * - For remote requests (e.g. via Apple TV), the associated controller may not be
     *   the one who originated the request, but instead the admin controller who set up the Apple TV.
     */
    HAPSessionRef *_Nullable session;

    /**
     * The characteristic whose value is to be read.
     */
    const HAPUInt8Characteristic *characteristic;

    /**
     * The service that contains the characteristic.
     */
    const HAPService *service;

    /**
     * The accessory that provides the service.
     */
    const HAPAccessory *accessory;
} HAPUInt8CharacteristicReadRequest;

/**
 * UInt8 characteristic write request.
 */
typedef struct {
    /**
     * Transport type over which the request has been received.
     */
    HAPTransportType transportType;

    /**
     * The session over which the request has been received.
     *
     * - A controller may be logged in on multiple sessions concurrently.
     *
     * - For remote requests (e.g. via Apple TV), the associated controller may not be
     *   the one who originated the request, but instead the admin controller who set up the Apple TV.
     */
    HAPSessionRef *session;

    /**
     * The characteristic whose value is to be written.
     */
    const HAPUInt8Characteristic *characteristic;

    /**
     * The service that contains the characteristic.
     */
    const HAPService *service;

    /**
     * The accessory that provides the service.
     */
    const HAPAccessory *accessory;

    /**
     * Whether the request appears to have originated from a remote controller, e.g. Apple TV or iCloud.
     */
    bool remote;

    /**
     * Additional authorization data.
     */
    struct {
        const void *_Nullable bytes;    /**< Raw AAD data, if applicable. */
        size_t numBytes;                /**< Length of additional authorization data. */
    } authorizationData;
} HAPUInt8CharacteristicWriteRequest;

/**
 * UInt8 characteristic subscription request.
 */
typedef struct {
    /**
     * Transport type over which the request has been received.
     */
    HAPTransportType transportType;

    /**
     * The session over which the request has been received.
     *
     * - A controller may be logged in on multiple sessions concurrently.
     */
    HAPSessionRef *session;

    /**
     * The characteristic whose value is to be subscribed or unsubscribed.
     */
    const HAPUInt8Characteristic *characteristic;

    /**
     * The service that contains the characteristic.
     */
    const HAPService *service;

    /**
     * The accessory that provides the service.
     */
    const HAPAccessory *accessory;
} HAPUInt8CharacteristicSubscriptionRequest;

/**
 * Valid Values range for a UInt8 characteristic.
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 7.4.5.4 Valid Values Range Descriptor
 */
typedef struct {
    uint8_t start;                  /**< Starting value. */
    uint8_t end;                    /**< Ending value. */
} HAPUInt8CharacteristicValidValuesRange;

/**
 * HomeKit UInt8 characteristic.
 */
struct HAPUInt8Characteristic {
    /**
     * Format. Must be kHAPCharacteristicFormat_UInt8.
     */
    HAPCharacteristicFormat format;

    /**
     * Instance ID.
     *
     * - Must be unique across all service or characteristic Instance IDs of the accessory.
     * - Must not change while the accessory is paired, including over firmware updates.
     * - Must not be 0.
     * - For accessories that support Bluetooth LE, must not exceed UINT16_MAX.
     */
    uint64_t iid;

    /**
     * The type of the characteristic.
     */
    const HAPUUID *characteristicType;

    /**
     * Description for debugging (based on "Type" field of HAP specification).
     */
    const char *debugDescription;

    /**
     * Description of the characteristic provided by the manufacturer of the accessory.
     */
    const char *_Nullable manufacturerDescription;

    /**
     * Characteristic properties.
     */
    HAPCharacteristicProperties properties;

    /**
     * The units of the values for the characteristic.
     */
    HAPCharacteristicUnits units;

    /**
     * Value constraints.
     */
    struct {
        uint8_t minimumValue;           /**< Minimum value. */
        uint8_t maximumValue;           /**< Maximum value. */
        uint8_t stepValue;              /**< Step value. */

        /**
         * List of valid values in ascending order. NULL-terminated. Optional.
         *
         * - Only supported for Apple defined characteristics.
         *
         * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
         *      Section 7.4.5.3 Valid Values Descriptor
         */
        const uint8_t *_Nullable const *_Nullable validValues;

        /**
         * List of valid values ranges in ascending order. NULL-terminated. Optional.
         *
         * - Only supported for Apple defined characteristics.
         *
         * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
         *      Section 7.4.5.4 Valid Values Range Descriptor
         */
        const HAPUInt8CharacteristicValidValuesRange *_Nullable const *_Nullable validValuesRanges;
    } constraints;

    /**
     * Callbacks.
     */
    struct {
        /**
         * The callback used to handle read requests.
         * On success, the value stored in @p value is sent back to the controller.
         *
         * - Required if the characteristic is marked readable in the characteristic properties.
         * - The callback must not block. Consider prefetching values if it would take too long.
         * - The returned value must satisfy the constraints of the characteristic.
         *
         * @param      server               Accessory server.
         * @param      request              Request.
         * @param[out] value                Value.
         * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
         *
         * @return kHAPError_None           If successful.
         * @return kHAPError_Unknown        If unable to communicate with requested service.
         * @return kHAPError_InvalidState   If the request cannot be processed in the current state.
         * @return kHAPError_OutOfResources If out of resources to process request.
         * @return kHAPError_Busy           If the request failed temporarily.
         */
        HAP_RESULT_USE_CHECK
        HAPError (*_Nullable handleRead)(
            HAPAccessoryServerRef *server,
            const HAPUInt8CharacteristicReadRequest *request,
            uint8_t *value,
            void *_Nullable context);

        /**
         * The callback used to handle write requests.
         *
         * - Required if the characteristic is marked writeable in the characteristic properties.
         * - The callback must not block. Consider queueing values if it would take too long.
         * - The value is already checked against the constraints of the characteristic.
         *
         * @param      server               Accessory server.
         * @param      request              Request.
         * @param      val                  Value.
         * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
         *
         * @return kHAPError_None           If successful.
         * @return kHAPError_Unknown        If unable to communicate with requested service.
         * @return kHAPError_InvalidState   If the request cannot be processed in the current state.
         * @return kHAPError_InvalidData    If the controller sent a malformed request.
         * @return kHAPError_OutOfResources If out of resources to process request.
         * @return kHAPError_NotAuthorized  If additional authorization data is insufficient.
         * @return kHAPError_Busy           If the request failed temporarily.
         */
        HAP_RESULT_USE_CHECK
        HAPError (*_Nullable handleWrite)(
            HAPAccessoryServerRef *server,
            const HAPUInt8CharacteristicWriteRequest *request,
            uint8_t value,
            void *_Nullable context);

        /**
         * The callback used to handle subscribe requests.
         *
         * - The callback must not block. Consider queueing values if it would take too long.
         *
         * @param      server               Accessory server.
         * @param      request              Request.
         * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
         */
        void (*_Nullable handleSubscribe)(
            HAPAccessoryServerRef *server,
            const HAPUInt8CharacteristicSubscriptionRequest *request,
            void *_Nullable context);

        /**
         * The callback used to handle unsubscribe requests.
         *
         * - The callback must not block. Consider queueing values if it would take too long.
         *
         * @param      server               Accessory server.
         * @param      request              Request.
         * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
         */
        void (*_Nullable handleUnsubscribe)(
            HAPAccessoryServerRef *server,
            const HAPUInt8CharacteristicSubscriptionRequest *request,
            void *_Nullable context);
    } callbacks;
};

// </editor-fold>
// <editor-fold desc="HAPUInt16Characteristic">

typedef struct HAPUInt16Characteristic HAPUInt16Characteristic;

/**
 * UInt16 characteristic read request.
 */
typedef struct {
    /**
     * Transport type over which the response will be sent.
     */
    HAPTransportType transportType;

    /**
     * The session over which the response will be sent.
     *
     * - A controller may be logged in on multiple sessions concurrently.
     * - May be NULL if a request is generated internally (e.g. for BLE broadcasts while disconnected).
     *
     * - For remote requests (e.g. via Apple TV), the associated controller may not be
     *   the one who originated the request, but instead the admin controller who set up the Apple TV.
     */
    HAPSessionRef *_Nullable session;

    /**
     * The characteristic whose value is to be read.
     */
    const HAPUInt16Characteristic *characteristic;

    /**
     * The service that contains the characteristic.
     */
    const HAPService *service;

    /**
     * The accessory that provides the service.
     */
    const HAPAccessory *accessory;
} HAPUInt16CharacteristicReadRequest;

/**
 * UInt16 characteristic write request.
 */
typedef struct {
    /**
     * Transport type over which the request has been received.
     */
    HAPTransportType transportType;

    /**
     * The session over which the request has been received.
     *
     * - A controller may be logged in on multiple sessions concurrently.
     *
     * - For remote requests (e.g. via Apple TV), the associated controller may not be
     *   the one who originated the request, but instead the admin controller who set up the Apple TV.
     */
    HAPSessionRef *session;

    /**
     * The characteristic whose value is to be written.
     */
    const HAPUInt16Characteristic *characteristic;

    /**
     * The service that contains the characteristic.
     */
    const HAPService *service;

    /**
     * The accessory that provides the service.
     */
    const HAPAccessory *accessory;

    /**
     * Whether the request appears to have originated from a remote controller, e.g. Apple TV or iCloud.
     */
    bool remote;

    /**
     * Additional authorization data.
     */
    struct {
        const void *_Nullable bytes;    /**< Raw AAD data, if applicable. */
        size_t numBytes;                /**< Length of additional authorization data. */
    } authorizationData;
} HAPUInt16CharacteristicWriteRequest;

/**
 * UInt16 characteristic subscription request.
 */
typedef struct {
    /**
     * Transport type over which the request has been received.
     */
    HAPTransportType transportType;

    /**
     * The session over which the request has been received.
     *
     * - A controller may be logged in on multiple sessions concurrently.
     */
    HAPSessionRef *session;

    /**
     * The characteristic whose value is to be subscribed or unsubscribed.
     */
    const HAPUInt16Characteristic *characteristic;

    /**
     * The service that contains the characteristic.
     */
    const HAPService *service;

    /**
     * The accessory that provides the service.
     */
    const HAPAccessory *accessory;
} HAPUInt16CharacteristicSubscriptionRequest;

/**
 * HomeKit UInt16 characteristic.
 */
struct HAPUInt16Characteristic {
    /**
     * Format. Must be kHAPCharacteristicFormat_UInt16.
     */
    HAPCharacteristicFormat format;

    /**
     * Instance ID.
     *
     * - Must be unique across all service or characteristic Instance IDs of the accessory.
     * - Must not change while the accessory is paired, including over firmware updates.
     * - Must not be 0.
     * - For accessories that support Bluetooth LE, must not exceed UINT16_MAX.
     */
    uint64_t iid;

    /**
     * The type of the characteristic.
     */
    const HAPUUID *characteristicType;

    /**
     * Description for debugging (based on "Type" field of HAP specification).
     */
    const char *debugDescription;

    /**
     * Description of the characteristic provided by the manufacturer of the accessory.
     */
    const char *_Nullable manufacturerDescription;

    /**
     * Characteristic properties.
     */
    HAPCharacteristicProperties properties;

    /**
     * The units of the values for the characteristic.
     */
    HAPCharacteristicUnits units;

    /**
     * Value constraints.
     */
    struct {
        uint16_t minimumValue;          /**< Minimum value. */
        uint16_t maximumValue;          /**< Maximum value. */
        uint16_t stepValue;             /**< Step value. */
    } constraints;

    /**
     * Callbacks.
     */
    struct {
        /**
         * The callback used to handle read requests.
         * On success, the value stored in @p value is sent back to the controller.
         *
         * - Required if the characteristic is marked readable in the characteristic properties.
         * - The callback must not block. Consider prefetching values if it would take too long.
         * - The returned value must satisfy the constraints of the characteristic.
         *
         * @param      server               Accessory server.
         * @param      request              Request.
         * @param[out] value                Value.
         * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
         *
         * @return kHAPError_None           If successful.
         * @return kHAPError_Unknown        If unable to communicate with requested service.
         * @return kHAPError_InvalidState   If the request cannot be processed in the current state.
         * @return kHAPError_OutOfResources If out of resources to process request.
         * @return kHAPError_Busy           If the request failed temporarily.
         */
        HAP_RESULT_USE_CHECK
        HAPError (*_Nullable handleRead)(
            HAPAccessoryServerRef *server,
            const HAPUInt16CharacteristicReadRequest *request,
            uint16_t *value,
            void *_Nullable context);

        /**
         * The callback used to handle write requests.
         *
         * - Required if the characteristic is marked writeable in the characteristic properties.
         * - The callback must not block. Consider queueing values if it would take too long.
         * - The value is already checked against the constraints of the characteristic.
         *
         * @param      server               Accessory server.
         * @param      request              Request.
         * @param      val                  Value.
         * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
         *
         * @return kHAPError_None           If successful.
         * @return kHAPError_Unknown        If unable to communicate with requested service.
         * @return kHAPError_InvalidState   If the request cannot be processed in the current state.
         * @return kHAPError_InvalidData    If the controller sent a malformed request.
         * @return kHAPError_OutOfResources If out of resources to process request.
         * @return kHAPError_NotAuthorized  If additional authorization data is insufficient.
         * @return kHAPError_Busy           If the request failed temporarily.
         */
        HAP_RESULT_USE_CHECK
        HAPError (*_Nullable handleWrite)(
            HAPAccessoryServerRef *server,
            const HAPUInt16CharacteristicWriteRequest *request,
            uint16_t value,
            void *_Nullable context);

        /**
         * The callback used to handle subscribe requests.
         *
         * - The callback must not block. Consider queueing values if it would take too long.
         *
         * @param      server               Accessory server.
         * @param      request              Request.
         * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
         */
        void (*_Nullable handleSubscribe)(
            HAPAccessoryServerRef *server,
            const HAPUInt16CharacteristicSubscriptionRequest *request,
            void *_Nullable context);

        /**
         * The callback used to handle unsubscribe requests.
         *
         * - The callback must not block. Consider queueing values if it would take too long.
         *
         * @param      server               Accessory server.
         * @param      request              Request.
         * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
         */
        void (*_Nullable handleUnsubscribe)(
            HAPAccessoryServerRef *server,
            const HAPUInt16CharacteristicSubscriptionRequest *request,
            void *_Nullable context);
    } callbacks;
};

// </editor-fold>
// <editor-fold desc="HAPUInt32Characteristic">

typedef struct HAPUInt32Characteristic HAPUInt32Characteristic;

/**
 * UInt32 characteristic read request.
 */
typedef struct {
    /**
     * Transport type over which the response will be sent.
     */
    HAPTransportType transportType;

    /**
     * The session over which the response will be sent.
     *
     * - A controller may be logged in on multiple sessions concurrently.
     * - May be NULL if a request is generated internally (e.g. for BLE broadcasts while disconnected).
     *
     * - For remote requests (e.g. via Apple TV), the associated controller may not be
     *   the one who originated the request, but instead the admin controller who set up the Apple TV.
     */
    HAPSessionRef *_Nullable session;

    /**
     * The characteristic whose value is to be read.
     */
    const HAPUInt32Characteristic *characteristic;

    /**
     * The service that contains the characteristic.
     */
    const HAPService *service;

    /**
     * The accessory that provides the service.
     */
    const HAPAccessory *accessory;
} HAPUInt32CharacteristicReadRequest;

/**
 * UInt32 characteristic write request.
 */
typedef struct {
    /**
     * Transport type over which the request has been received.
     */
    HAPTransportType transportType;

    /**
     * The session over which the request has been received.
     *
     * - A controller may be logged in on multiple sessions concurrently.
     *
     * - For remote requests (e.g. via Apple TV), the associated controller may not be
     *   the one who originated the request, but instead the admin controller who set up the Apple TV.
     */
    HAPSessionRef *session;

    /**
     * The characteristic whose value is to be written.
     */
    const HAPUInt32Characteristic *characteristic;

    /**
     * The service that contains the characteristic.
     */
    const HAPService *service;

    /**
     * The accessory that provides the service.
     */
    const HAPAccessory *accessory;

    /**
     * Whether the request appears to have originated from a remote controller, e.g. Apple TV or iCloud.
     */
    bool remote;

    /**
     * Additional authorization data.
     */
    struct {
        const void *_Nullable bytes;    /**< Raw AAD data, if applicable. */
        size_t numBytes;                /**< Length of additional authorization data. */
    } authorizationData;
} HAPUInt32CharacteristicWriteRequest;

/**
 * UInt32 characteristic subscription request.
 */
typedef struct {
    /**
     * Transport type over which the request has been received.
     */
    HAPTransportType transportType;

    /**
     * The session over which the request has been received.
     *
     * - A controller may be logged in on multiple sessions concurrently.
     */
    HAPSessionRef *session;

    /**
     * The characteristic whose value is to be subscribed or unsubscribed.
     */
    const HAPUInt32Characteristic *characteristic;

    /**
     * The service that contains the characteristic.
     */
    const HAPService *service;

    /**
     * The accessory that provides the service.
     */
    const HAPAccessory *accessory;
} HAPUInt32CharacteristicSubscriptionRequest;

/**
 * HomeKit UInt32 characteristic.
 */
struct HAPUInt32Characteristic {
    /**
     * Format. Must be kHAPCharacteristicFormat_UInt32.
     */
    HAPCharacteristicFormat format;

    /**
     * Instance ID.
     *
     * - Must be unique across all service or characteristic Instance IDs of the accessory.
     * - Must not change while the accessory is paired, including over firmware updates.
     * - Must not be 0.
     * - For accessories that support Bluetooth LE, must not exceed UINT16_MAX.
     */
    uint64_t iid;

    /**
     * The type of the characteristic.
     */
    const HAPUUID *characteristicType;

    /**
     * Description for debugging (based on "Type" field of HAP specification).
     */
    const char *debugDescription;

    /**
     * Description of the characteristic provided by the manufacturer of the accessory.
     */
    const char *_Nullable manufacturerDescription;

    /**
     * Characteristic properties.
     */
    HAPCharacteristicProperties properties;

    /**
     * The units of the values for the characteristic.
     */
    HAPCharacteristicUnits units;

    /**
     * Value constraints.
     */
    struct {
        uint32_t minimumValue;          /**< Minimum value. */
        uint32_t maximumValue;          /**< Maximum value. */
        uint32_t stepValue;             /**< Step value. */
    } constraints;

    /**
     * Callbacks.
     */
    struct {
        /**
         * The callback used to handle read requests.
         * On success, the value stored in @p value is sent back to the controller.
         *
         * - Required if the characteristic is marked readable in the characteristic properties.
         * - The callback must not block. Consider prefetching values if it would take too long.
         * - The returned value must satisfy the constraints of the characteristic.
         *
         * @param      server               Accessory server.
         * @param      request              Request.
         * @param[out] value                Value.
         * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
         *
         * @return kHAPError_None           If successful.
         * @return kHAPError_Unknown        If unable to communicate with requested service.
         * @return kHAPError_InvalidState   If the request cannot be processed in the current state.
         * @return kHAPError_OutOfResources If out of resources to process request.
         * @return kHAPError_Busy           If the request failed temporarily.
         */
        HAP_RESULT_USE_CHECK
        HAPError (*_Nullable handleRead)(
            HAPAccessoryServerRef *server,
            const HAPUInt32CharacteristicReadRequest *request,
            uint32_t *value,
            void *_Nullable context);

        /**
         * The callback used to handle write requests.
         *
         * - Required if the characteristic is marked writeable in the characteristic properties.
         * - The callback must not block. Consider queueing values if it would take too long.
         * - The value is already checked against the constraints of the characteristic.
         *
         * @param      server               Accessory server.
         * @param      request              Request.
         * @param      val                  Value.
         * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
         *
         * @return kHAPError_None           If successful.
         * @return kHAPError_Unknown        If unable to communicate with requested service.
         * @return kHAPError_InvalidState   If the request cannot be processed in the current state.
         * @return kHAPError_InvalidData    If the controller sent a malformed request.
         * @return kHAPError_OutOfResources If out of resources to process request.
         * @return kHAPError_NotAuthorized  If additional authorization data is insufficient.
         * @return kHAPError_Busy           If the request failed temporarily.
         */
        HAP_RESULT_USE_CHECK
        HAPError (*_Nullable handleWrite)(
            HAPAccessoryServerRef *server,
            const HAPUInt32CharacteristicWriteRequest *request,
            uint32_t value,
            void *_Nullable context);

        /**
         * The callback used to handle subscribe requests.
         *
         * - The callback must not block. Consider queueing values if it would take too long.
         *
         * @param      server               Accessory server.
         * @param      request              Request.
         * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
         */
        void (*_Nullable handleSubscribe)(
            HAPAccessoryServerRef *server,
            const HAPUInt32CharacteristicSubscriptionRequest *request,
            void *_Nullable context);

        /**
         * The callback used to handle unsubscribe requests.
         *
         * - The callback must not block. Consider queueing values if it would take too long.
         *
         * @param      server               Accessory server.
         * @param      request              Request.
         * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
         */
        void (*_Nullable handleUnsubscribe)(
            HAPAccessoryServerRef *server,
            const HAPUInt32CharacteristicSubscriptionRequest *request,
            void *_Nullable context);
    } callbacks;
};

// </editor-fold>
// <editor-fold desc="HAPUInt64Characteristic">

typedef struct HAPUInt64Characteristic HAPUInt64Characteristic;

/**
 * UInt64 characteristic read request.
 */
typedef struct {
    /**
     * Transport type over which the response will be sent.
     */
    HAPTransportType transportType;

    /**
     * The session over which the response will be sent.
     *
     * - A controller may be logged in on multiple sessions concurrently.
     * - May be NULL if a request is generated internally (e.g. for BLE broadcasts while disconnected).
     *
     * - For remote requests (e.g. via Apple TV), the associated controller may not be
     *   the one who originated the request, but instead the admin controller who set up the Apple TV.
     */
    HAPSessionRef *_Nullable session;

    /**
     * The characteristic whose value is to be read.
     */
    const HAPUInt64Characteristic *characteristic;

    /**
     * The service that contains the characteristic.
     */
    const HAPService *service;

    /**
     * The accessory that provides the service.
     */
    const HAPAccessory *accessory;
} HAPUInt64CharacteristicReadRequest;

/**
 * UInt64 characteristic write request.
 */
typedef struct {
    /**
     * Transport type over which the request has been received.
     */
    HAPTransportType transportType;

    /**
     * The session over which the request has been received.
     *
     * - A controller may be logged in on multiple sessions concurrently.
     *
     * - For remote requests (e.g. via Apple TV), the associated controller may not be
     *   the one who originated the request, but instead the admin controller who set up the Apple TV.
     */
    HAPSessionRef *session;

    /**
     * The characteristic whose value is to be written.
     */
    const HAPUInt64Characteristic *characteristic;

    /**
     * The service that contains the characteristic.
     */
    const HAPService *service;

    /**
     * The accessory that provides the service.
     */
    const HAPAccessory *accessory;

    /**
     * Whether the request appears to have originated from a remote controller, e.g. Apple TV or iCloud.
     */
    bool remote;

    /**
     * Additional authorization data.
     */
    struct {
        const void *_Nullable bytes;    /**< Raw AAD data, if applicable. */
        size_t numBytes;                /**< Length of additional authorization data. */
    } authorizationData;
} HAPUInt64CharacteristicWriteRequest;

/**
 * UInt64 characteristic subscription request.
 */
typedef struct {
    /**
     * Transport type over which the request has been received.
     */
    HAPTransportType transportType;

    /**
     * The session over which the request has been received.
     *
     * - A controller may be logged in on multiple sessions concurrently.
     */
    HAPSessionRef *session;

    /**
     * The characteristic whose value is to be subscribed or unsubscribed.
     */
    const HAPUInt64Characteristic *characteristic;

    /**
     * The service that contains the characteristic.
     */
    const HAPService *service;

    /**
     * The accessory that provides the service.
     */
    const HAPAccessory *accessory;
} HAPUInt64CharacteristicSubscriptionRequest;

/**
 * HomeKit UInt64 characteristic.
 */
struct HAPUInt64Characteristic {
    /**
     * Format. Must be kHAPCharacteristicFormat_UInt64.
     */
    HAPCharacteristicFormat format;

    /**
     * Instance ID.
     *
     * - Must be unique across all service or characteristic Instance IDs of the accessory.
     * - Must not change while the accessory is paired, including over firmware updates.
     * - Must not be 0.
     * - For accessories that support Bluetooth LE, must not exceed UINT16_MAX.
     */
    uint64_t iid;

    /**
     * The type of the characteristic.
     */
    const HAPUUID *characteristicType;

    /**
     * Description for debugging (based on "Type" field of HAP specification).
     */
    const char *debugDescription;

    /**
     * Description of the characteristic provided by the manufacturer of the accessory.
     */
    const char *_Nullable manufacturerDescription;

    /**
     * Characteristic properties.
     */
    HAPCharacteristicProperties properties;

    /**
     * The units of the values for the characteristic.
     */
    HAPCharacteristicUnits units;

    /**
     * Value constraints.
     */
    struct {
        uint64_t minimumValue;          /**< Minimum value. */
        uint64_t maximumValue;          /**< Maximum value. */
        uint64_t stepValue;             /**< Step value. */
    } constraints;

    /**
     * Callbacks.
     */
    struct {
        /**
         * The callback used to handle read requests.
         * On success, the value stored in @p value is sent back to the controller.
         *
         * - Required if the characteristic is marked readable in the characteristic properties.
         * - The callback must not block. Consider prefetching values if it would take too long.
         * - The returned value must satisfy the constraints of the characteristic.
         *
         * @param      server               Accessory server.
         * @param      request              Request.
         * @param[out] value                Value.
         * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
         *
         * @return kHAPError_None           If successful.
         * @return kHAPError_Unknown        If unable to communicate with requested service.
         * @return kHAPError_InvalidState   If the request cannot be processed in the current state.
         * @return kHAPError_OutOfResources If out of resources to process request.
         * @return kHAPError_Busy           If the request failed temporarily.
         */
        HAP_RESULT_USE_CHECK
        HAPError (*_Nullable handleRead)(
            HAPAccessoryServerRef *server,
            const HAPUInt64CharacteristicReadRequest *request,
            uint64_t *value,
            void *_Nullable context);

        /**
         * The callback used to handle write requests.
         *
         * - Required if the characteristic is marked writeable in the characteristic properties.
         * - The callback must not block. Consider queueing values if it would take too long.
         * - The value is already checked against the constraints of the characteristic.
         *
         * @param      server               Accessory server.
         * @param      request              Request.
         * @param      val                  Value.
         * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
         *
         * @return kHAPError_None           If successful.
         * @return kHAPError_Unknown        If unable to communicate with requested service.
         * @return kHAPError_InvalidState   If the request cannot be processed in the current state.
         * @return kHAPError_InvalidData    If the controller sent a malformed request.
         * @return kHAPError_OutOfResources If out of resources to process request.
         * @return kHAPError_NotAuthorized  If additional authorization data is insufficient.
         * @return kHAPError_Busy           If the request failed temporarily.
         */
        HAP_RESULT_USE_CHECK
        HAPError (*_Nullable handleWrite)(
            HAPAccessoryServerRef *server,
            const HAPUInt64CharacteristicWriteRequest *request,
            uint64_t value,
            void *_Nullable context);

        /**
         * The callback used to handle subscribe requests.
         *
         * - The callback must not block. Consider queueing values if it would take too long.
         *
         * @param      server               Accessory server.
         * @param      request              Request.
         * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
         */
        void (*_Nullable handleSubscribe)(
            HAPAccessoryServerRef *server,
            const HAPUInt64CharacteristicSubscriptionRequest *request,
            void *_Nullable context);

        /**
         * The callback used to handle unsubscribe requests.
         *
         * - The callback must not block. Consider queueing values if it would take too long.
         *
         * @param      server               Accessory server.
         * @param      request              Request.
         * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
         */
        void (*_Nullable handleUnsubscribe)(
            HAPAccessoryServerRef *server,
            const HAPUInt64CharacteristicSubscriptionRequest *request,
            void *_Nullable context);
    } callbacks;
};

// </editor-fold>
// <editor-fold desc="HAPIntCharacteristic">

typedef struct HAPIntCharacteristic HAPIntCharacteristic;

/**
 * Int characteristic read request.
 */
typedef struct {
    /**
     * Transport type over which the response will be sent.
     */
    HAPTransportType transportType;

    /**
     * The session over which the response will be sent.
     *
     * - A controller may be logged in on multiple sessions concurrently.
     * - May be NULL if a request is generated internally (e.g. for BLE broadcasts while disconnected).
     *
     * - For remote requests (e.g. via Apple TV), the associated controller may not be
     *   the one who originated the request, but instead the admin controller who set up the Apple TV.
     */
    HAPSessionRef *_Nullable session;

    /**
     * The characteristic whose value is to be read.
     */
    const HAPIntCharacteristic *characteristic;

    /**
     * The service that contains the characteristic.
     */
    const HAPService *service;

    /**
     * The accessory that provides the service.
     */
    const HAPAccessory *accessory;
} HAPIntCharacteristicReadRequest;

/**
 * Int characteristic write request.
 */
typedef struct {
    /**
     * Transport type over which the request has been received.
     */
    HAPTransportType transportType;

    /**
     * The session over which the request has been received.
     *
     * - A controller may be logged in on multiple sessions concurrently.
     *
     * - For remote requests (e.g. via Apple TV), the associated controller may not be
     *   the one who originated the request, but instead the admin controller who set up the Apple TV.
     */
    HAPSessionRef *session;

    /**
     * The characteristic whose value is to be written.
     */
    const HAPIntCharacteristic *characteristic;

    /**
     * The service that contains the characteristic.
     */
    const HAPService *service;

    /**
     * The accessory that provides the service.
     */
    const HAPAccessory *accessory;

    /**
     * Whether the request appears to have originated from a remote controller, e.g. Apple TV or iCloud.
     */
    bool remote;

    /**
     * Additional authorization data.
     */
    struct {
        const void *_Nullable bytes;    /**< Raw AAD data, if applicable. */
        size_t numBytes;                /**< Length of additional authorization data. */
    } authorizationData;
} HAPIntCharacteristicWriteRequest;

/**
 * Int characteristic subscription request.
 */
typedef struct {
    /**
     * Transport type over which the request has been received.
     */
    HAPTransportType transportType;

    /**
     * The session over which the request has been received.
     *
     * - A controller may be logged in on multiple sessions concurrently.
     */
    HAPSessionRef *session;

    /**
     * The characteristic whose value is to be subscribed or unsubscribed.
     */
    const HAPIntCharacteristic *characteristic;

    /**
     * The service that contains the characteristic.
     */
    const HAPService *service;

    /**
     * The accessory that provides the service.
     */
    const HAPAccessory *accessory;
} HAPIntCharacteristicSubscriptionRequest;

/**
 * HomeKit Int characteristic.
 */
struct HAPIntCharacteristic {
    /**
     * Format. Must be kHAPCharacteristicFormat_Int.
     */
    HAPCharacteristicFormat format;

    /**
     * Instance ID.
     *
     * - Must be unique across all service or characteristic Instance IDs of the accessory.
     * - Must not change while the accessory is paired, including over firmware updates.
     * - Must not be 0.
     * - For accessories that support Bluetooth LE, must not exceed UINT16_MAX.
     */
    uint64_t iid;

    /**
     * The type of the characteristic.
     */
    const HAPUUID *characteristicType;

    /**
     * Description for debugging (based on "Type" field of HAP specification).
     */
    const char *debugDescription;

    /**
     * Description of the characteristic provided by the manufacturer of the accessory.
     */
    const char *_Nullable manufacturerDescription;

    /**
     * Characteristic properties.
     */
    HAPCharacteristicProperties properties;

    /**
     * The units of the values for the characteristic.
     */
    HAPCharacteristicUnits units;

    /**
     * Value constraints.
     */
    struct {
        int32_t minimumValue;           /**< Minimum value. */
        int32_t maximumValue;           /**< Maximum value. */
        int32_t stepValue;              /**< Step value. */
    } constraints;

    /**
     * Callbacks.
     */
    struct {
        /**
         * The callback used to handle read requests.
         * On success, the value stored in @p value is sent back to the controller.
         *
         * - Required if the characteristic is marked readable in the characteristic properties.
         * - The callback must not block. Consider prefetching values if it would take too long.
         * - The returned value must satisfy the constraints of the characteristic.
         *
         * @param      server               Accessory server.
         * @param      request              Request.
         * @param[out] value                Value.
         * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
         *
         * @return kHAPError_None           If successful.
         * @return kHAPError_Unknown        If unable to communicate with requested service.
         * @return kHAPError_InvalidState   If the request cannot be processed in the current state.
         * @return kHAPError_OutOfResources If out of resources to process request.
         * @return kHAPError_Busy           If the request failed temporarily.
         */
        HAP_RESULT_USE_CHECK
        HAPError (*_Nullable handleRead)(
            HAPAccessoryServerRef *server,
            const HAPIntCharacteristicReadRequest *request,
            int32_t *value,
            void *_Nullable context);

        /**
         * The callback used to handle write requests.
         *
         * - Required if the characteristic is marked writeable in the characteristic properties.
         * - The callback must not block. Consider queueing values if it would take too long.
         * - The value is already checked against the constraints of the characteristic.
         *
         * @param      server               Accessory server.
         * @param      request              Request.
         * @param      val                  Value.
         * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
         *
         * @return kHAPError_None           If successful.
         * @return kHAPError_Unknown        If unable to communicate with requested service.
         * @return kHAPError_InvalidState   If the request cannot be processed in the current state.
         * @return kHAPError_InvalidData    If the controller sent a malformed request.
         * @return kHAPError_OutOfResources If out of resources to process request.
         * @return kHAPError_NotAuthorized  If additional authorization data is insufficient.
         * @return kHAPError_Busy           If the request failed temporarily.
         */
        HAP_RESULT_USE_CHECK
        HAPError (*_Nullable handleWrite)(
            HAPAccessoryServerRef *server,
            const HAPIntCharacteristicWriteRequest *request,
            int32_t value,
            void *_Nullable context);

        /**
         * The callback used to handle subscribe requests.
         *
         * - The callback must not block. Consider queueing values if it would take too long.
         *
         * @param      server               Accessory server.
         * @param      request              Request.
         * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
         */
        void (*_Nullable handleSubscribe)(
            HAPAccessoryServerRef *server,
            const HAPIntCharacteristicSubscriptionRequest *request,
            void *_Nullable context);

        /**
         * The callback used to handle unsubscribe requests.
         *
         * - The callback must not block. Consider queueing values if it would take too long.
         *
         * @param      server               Accessory server.
         * @param      request              Request.
         * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
         */
        void (*_Nullable handleUnsubscribe)(
            HAPAccessoryServerRef *server,
            const HAPIntCharacteristicSubscriptionRequest *request,
            void *_Nullable context);
    } callbacks;
};

// </editor-fold>
// <editor-fold desc="HAPFloatCharacteristic">

typedef struct HAPFloatCharacteristic HAPFloatCharacteristic;

/**
 * Float characteristic read request.
 */
typedef struct {
    /**
     * Transport type over which the response will be sent.
     */
    HAPTransportType transportType;

    /**
     * The session over which the response will be sent.
     *
     * - A controller may be logged in on multiple sessions concurrently.
     * - May be NULL if a request is generated internally (e.g. for BLE broadcasts while disconnected).
     *
     * - For remote requests (e.g. via Apple TV), the associated controller may not be
     *   the one who originated the request, but instead the admin controller who set up the Apple TV.
     */
    HAPSessionRef *_Nullable session;

    /**
     * The characteristic whose value is to be read.
     */
    const HAPFloatCharacteristic *characteristic;

    /**
     * The service that contains the characteristic.
     */
    const HAPService *service;

    /**
     * The accessory that provides the service.
     */
    const HAPAccessory *accessory;
} HAPFloatCharacteristicReadRequest;

/**
 * Float characteristic write request.
 */
typedef struct {
    /**
     * Transport type over which the request has been received.
     */
    HAPTransportType transportType;

    /**
     * The session over which the request has been received.
     *
     * - A controller may be logged in on multiple sessions concurrently.
     *
     * - For remote requests (e.g. via Apple TV), the associated controller may not be
     *   the one who originated the request, but instead the admin controller who set up the Apple TV.
     */
    HAPSessionRef *session;

    /**
     * The characteristic whose value is to be written.
     */
    const HAPFloatCharacteristic *characteristic;

    /**
     * The service that contains the characteristic.
     */
    const HAPService *service;

    /**
     * The accessory that provides the service.
     */
    const HAPAccessory *accessory;

    /**
     * Whether the request appears to have originated from a remote controller, e.g. Apple TV or iCloud.
     */
    bool remote;

    /**
     * Additional authorization data.
     */
    struct {
        const void *_Nullable bytes;    /**< Raw AAD data, if applicable. */
        size_t numBytes;                /**< Length of additional authorization data. */
    } authorizationData;
} HAPFloatCharacteristicWriteRequest;

/**
 * Float characteristic subscription request.
 */
typedef struct {
    /**
     * Transport type over which the request has been received.
     */
    HAPTransportType transportType;

    /**
     * The session over which the request has been received.
     *
     * - A controller may be logged in on multiple sessions concurrently.
     */
    HAPSessionRef *session;

    /**
     * The characteristic whose value is to be subscribed or unsubscribed.
     */
    const HAPFloatCharacteristic *characteristic;

    /**
     * The service that contains the characteristic.
     */
    const HAPService *service;

    /**
     * The accessory that provides the service.
     */
    const HAPAccessory *accessory;
} HAPFloatCharacteristicSubscriptionRequest;

/**
 * HomeKit Float characteristic.
 */
struct HAPFloatCharacteristic {
    /**
     * Format. Must be kHAPCharacteristicFormat_Float.
     */
    HAPCharacteristicFormat format;

    /**
     * Instance ID.
     *
     * - Must be unique across all service or characteristic Instance IDs of the accessory.
     * - Must not change while the accessory is paired, including over firmware updates.
     * - Must not be 0.
     * - For accessories that support Bluetooth LE, must not exceed UINT16_MAX.
     */
    uint64_t iid;

    /**
     * The type of the characteristic.
     */
    const HAPUUID *characteristicType;

    /**
     * Description for debugging (based on "Type" field of HAP specification).
     */
    const char *debugDescription;

    /**
     * Description of the characteristic provided by the manufacturer of the accessory.
     */
    const char *_Nullable manufacturerDescription;

    /**
     * Characteristic properties.
     */
    HAPCharacteristicProperties properties;

    /**
     * The units of the values for the characteristic.
     */
    HAPCharacteristicUnits units;

    /**
     * Value constraints.
     */
    struct {
        float minimumValue;             /**< Minimum value. */
        float maximumValue;             /**< Maximum value. */
        float stepValue;                /**< Step value. */
    } constraints;

    /**
     * Callbacks.
     */
    struct {
        /**
         * The callback used to handle read requests.
         * On success, the value stored in @p value is sent back to the controller.
         *
         * - Required if the characteristic is marked readable in the characteristic properties.
         * - The callback must not block. Consider prefetching values if it would take too long.
         * - The returned value must satisfy the constraints of the characteristic.
         *
         * @param      server               Accessory server.
         * @param      request              Request.
         * @param[out] value                Value.
         * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
         *
         * @return kHAPError_None           If successful.
         * @return kHAPError_Unknown        If unable to communicate with requested service.
         * @return kHAPError_InvalidState   If the request cannot be processed in the current state.
         * @return kHAPError_OutOfResources If out of resources to process request.
         * @return kHAPError_Busy           If the request failed temporarily.
         */
        HAP_RESULT_USE_CHECK
        HAPError (*_Nullable handleRead)(
            HAPAccessoryServerRef *server,
            const HAPFloatCharacteristicReadRequest *request,
            float *value,
            void *_Nullable context);

        /**
         * The callback used to handle write requests.
         *
         * - Required if the characteristic is marked writeable in the characteristic properties.
         * - The callback must not block. Consider queueing values if it would take too long.
         * - The value is already checked against the constraints of the characteristic.
         *
         * @param      server               Accessory server.
         * @param      request              Request.
         * @param      val                  Value.
         * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
         *
         * @return kHAPError_None           If successful.
         * @return kHAPError_Unknown        If unable to communicate with requested service.
         * @return kHAPError_InvalidState   If the request cannot be processed in the current state.
         * @return kHAPError_InvalidData    If the controller sent a malformed request.
         * @return kHAPError_OutOfResources If out of resources to process request.
         * @return kHAPError_NotAuthorized  If additional authorization data is insufficient.
         * @return kHAPError_Busy           If the request failed temporarily.
         */
        HAP_RESULT_USE_CHECK
        HAPError (*_Nullable handleWrite)(
            HAPAccessoryServerRef *server,
            const HAPFloatCharacteristicWriteRequest *request,
            float value,
            void *_Nullable context);

        /**
         * The callback used to handle subscribe requests.
         *
         * - The callback must not block. Consider queueing values if it would take too long.
         *
         * @param      server               Accessory server.
         * @param      request              Request.
         * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
         */
        void (*_Nullable handleSubscribe)(
            HAPAccessoryServerRef *server,
            const HAPFloatCharacteristicSubscriptionRequest *request,
            void *_Nullable context);

        /**
         * The callback used to handle unsubscribe requests.
         *
         * - The callback must not block. Consider queueing values if it would take too long.
         *
         * @param      server               Accessory server.
         * @param      request              Request.
         * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
         */
        void (*_Nullable handleUnsubscribe)(
            HAPAccessoryServerRef *server,
            const HAPFloatCharacteristicSubscriptionRequest *request,
            void *_Nullable context);
    } callbacks;
};

// </editor-fold>
// <editor-fold desc="HAPStringCharacteristic">

typedef struct HAPStringCharacteristic HAPStringCharacteristic;

/**
 * String characteristic read request.
 */
typedef struct {
    /**
     * Transport type over which the response will be sent.
     */
    HAPTransportType transportType;

    /**
     * The session over which the response will be sent.
     *
     * - A controller may be logged in on multiple sessions concurrently.
     * - May be NULL if a request is generated internally (e.g. for BLE broadcasts while disconnected).
     *
     * - For remote requests (e.g. via Apple TV), the associated controller may not be
     *   the one who originated the request, but instead the admin controller who set up the Apple TV.
     */
    HAPSessionRef *_Nullable session;

    /**
     * The characteristic whose value is to be read.
     */
    const HAPStringCharacteristic *characteristic;

    /**
     * The service that contains the characteristic.
     */
    const HAPService *service;

    /**
     * The accessory that provides the service.
     */
    const HAPAccessory *accessory;
} HAPStringCharacteristicReadRequest;

/**
 * String characteristic write request.
 */
typedef struct {
    /**
     * Transport type over which the request has been received.
     */
    HAPTransportType transportType;

    /**
     * The session over which the request has been received.
     *
     * - A controller may be logged in on multiple sessions concurrently.
     *
     * - For remote requests (e.g. via Apple TV), the associated controller may not be
     *   the one who originated the request, but instead the admin controller who set up the Apple TV.
     */
    HAPSessionRef *session;

    /**
     * The characteristic whose value is to be written.
     */
    const HAPStringCharacteristic *characteristic;

    /**
     * The service that contains the characteristic.
     */
    const HAPService *service;

    /**
     * The accessory that provides the service.
     */
    const HAPAccessory *accessory;

    /**
     * Whether the request appears to have originated from a remote controller, e.g. Apple TV or iCloud.
     */
    bool remote;

    /**
     * Additional authorization data.
     */
    struct {
        const void *_Nullable bytes;    /**< Raw AAD data, if applicable. */
        size_t numBytes;                /**< Length of additional authorization data. */
    } authorizationData;
} HAPStringCharacteristicWriteRequest;

/**
 * String characteristic subscription request.
 */
typedef struct {
    /**
     * Transport type over which the request has been received.
     */
    HAPTransportType transportType;

    /**
     * The session over which the request has been received.
     *
     * - A controller may be logged in on multiple sessions concurrently.
     */
    HAPSessionRef *session;

    /**
     * The characteristic whose value is to be subscribed or unsubscribed.
     */
    const HAPStringCharacteristic *characteristic;

    /**
     * The service that contains the characteristic.
     */
    const HAPService *service;

    /**
     * The accessory that provides the service.
     */
    const HAPAccessory *accessory;
} HAPStringCharacteristicSubscriptionRequest;

/**
 * HomeKit String characteristic.
 */
struct HAPStringCharacteristic {
    /**
     * Format. Must be kHAPCharacteristicFormat_String.
     */
    HAPCharacteristicFormat format;

    /**
     * Instance ID.
     *
     * - Must be unique across all service or characteristic Instance IDs of the accessory.
     * - Must not change while the accessory is paired, including over firmware updates.
     * - Must not be 0.
     * - For accessories that support Bluetooth LE, must not exceed UINT16_MAX.
     */
    uint64_t iid;

    /**
     * The type of the characteristic.
     */
    const HAPUUID *characteristicType;

    /**
     * Description for debugging (based on "Type" field of HAP specification).
     */
    const char *debugDescription;

    /**
     * Description of the characteristic provided by the manufacturer of the accessory.
     */
    const char *_Nullable manufacturerDescription;

    /**
     * Characteristic properties.
     */
    HAPCharacteristicProperties properties;

    /**
     * Value constraints.
     */
    struct {
        uint16_t maxLength;             /**< Maximum length. */
    } constraints;

    /**
     * Callbacks.
     */
    struct {
        /**
         * The callback used to handle read requests.
         * On success, the value stored in @p value is sent back to the controller.
         *
         * - Required if the characteristic is marked readable in the characteristic properties.
         * - The callback must not block. Consider prefetching values if it would take too long.
         * - The returned value must satisfy the constraints of the characteristic.
         *
         * @param      server               Accessory server.
         * @param      request              Request.
         * @param[out] value                Value. NULL-terminated.
         * @param      maxValueBytes        Capacity of value. NULL-terminator must fit within capacity.
         * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
         *
         * @return kHAPError_None           If successful.
         * @return kHAPError_Unknown        If unable to communicate with requested service.
         * @return kHAPError_InvalidState   If the request cannot be processed in the current state.
         * @return kHAPError_OutOfResources If out of resources to process request.
         * @return kHAPError_Busy           If the request failed temporarily.
         */
        HAP_RESULT_USE_CHECK
        HAPError (*_Nullable handleRead)(
            HAPAccessoryServerRef *server,
            const HAPStringCharacteristicReadRequest *request,
            char *value,
            size_t maxValueBytes,
            void *_Nullable context);

        /**
         * The callback used to handle write requests.
         *
         * - Required if the characteristic is marked writeable in the characteristic properties.
         * - The callback must not block. Consider queueing values if it would take too long.
         * - The value is already checked against the constraints of the characteristic.
         *
         * @param      server               Accessory server.
         * @param      request              Request.
         * @param      value                Value. NULL-terminated.
         * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
         *
         * @return kHAPError_None           If successful.
         * @return kHAPError_Unknown        If unable to communicate with requested service.
         * @return kHAPError_InvalidState   If the request cannot be processed in the current state.
         * @return kHAPError_InvalidData    If the controller sent a malformed request.
         * @return kHAPError_OutOfResources If out of resources to process request.
         * @return kHAPError_NotAuthorized  If additional authorization data is insufficient.
         * @return kHAPError_Busy           If the request failed temporarily.
         */
        HAP_RESULT_USE_CHECK
        HAPError (*_Nullable handleWrite)(
            HAPAccessoryServerRef *server,
            const HAPStringCharacteristicWriteRequest *request,
            const char *value,
            void *_Nullable context);

        /**
         * The callback used to handle subscribe requests.
         *
         * - The callback must not block. Consider queueing values if it would take too long.
         *
         * @param      server               Accessory server.
         * @param      request              Request.
         * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
         */
        void (*_Nullable handleSubscribe)(
            HAPAccessoryServerRef *server,
            const HAPStringCharacteristicSubscriptionRequest *request,
            void *_Nullable context);

        /**
         * The callback used to handle unsubscribe requests.
         *
         * - The callback must not block. Consider queueing values if it would take too long.
         *
         * @param      server               Accessory server.
         * @param      request              Request.
         * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
         */
        void (*_Nullable handleUnsubscribe)(
            HAPAccessoryServerRef *server,
            const HAPStringCharacteristicSubscriptionRequest *request,
            void *_Nullable context);
    } callbacks;
};

// </editor-fold>
// <editor-fold desc="HAPTLV8Characteristic">

typedef struct HAPTLV8Characteristic HAPTLV8Characteristic;

/**
 * TLV8 characteristic read request.
 */
typedef struct {
    /**
     * Transport type over which the response will be sent.
     */
    HAPTransportType transportType;

    /**
     * The session over which the response will be sent.
     *
     * - A controller may be logged in on multiple sessions concurrently.
     * - May be NULL if a request is generated internally (e.g. for BLE broadcasts while disconnected).
     *
     * - For remote requests (e.g. via Apple TV), the associated controller may not be
     *   the one who originated the request, but instead the admin controller who set up the Apple TV.
     */
    HAPSessionRef *_Nullable session;

    /**
     * The characteristic whose value is to be read.
     */
    const HAPTLV8Characteristic *characteristic;

    /**
     * The service that contains the characteristic.
     */
    const HAPService *service;

    /**
     * The accessory that provides the service.
     */
    const HAPAccessory *accessory;
} HAPTLV8CharacteristicReadRequest;

/**
 * TLV8 characteristic write request.
 */
typedef struct {
    /**
     * Transport type over which the request has been received.
     */
    HAPTransportType transportType;

    /**
     * The session over which the request has been received.
     *
     * - A controller may be logged in on multiple sessions concurrently.
     *
     * - For remote requests (e.g. via Apple TV), the associated controller may not be
     *   the one who originated the request, but instead the admin controller who set up the Apple TV.
     */
    HAPSessionRef *session;

    /**
     * The characteristic whose value is to be written.
     */
    const HAPTLV8Characteristic *characteristic;

    /**
     * The service that contains the characteristic.
     */
    const HAPService *service;

    /**
     * The accessory that provides the service.
     */
    const HAPAccessory *accessory;

    /**
     * Whether the request appears to have originated from a remote controller, e.g. Apple TV or iCloud.
     */
    bool remote;

    /**
     * Additional authorization data.
     */
    struct {
        const void *_Nullable bytes;    /**< Raw AAD data, if applicable. */
        size_t numBytes;                /**< Length of additional authorization data. */
    } authorizationData;
} HAPTLV8CharacteristicWriteRequest;

/**
 * TLV8 characteristic subscription request.
 */
typedef struct {
    /**
     * Transport type over which the request has been received.
     */
    HAPTransportType transportType;

    /**
     * The session over which the request has been received.
     *
     * - A controller may be logged in on multiple sessions concurrently.
     */
    HAPSessionRef *session;

    /**
     * The characteristic whose value is to be subscribed or unsubscribed.
     */
    const HAPTLV8Characteristic *characteristic;

    /**
     * The service that contains the characteristic.
     */
    const HAPService *service;

    /**
     * The accessory that provides the service.
     */
    const HAPAccessory *accessory;
} HAPTLV8CharacteristicSubscriptionRequest;

/**
 * HomeKit TLV8 characteristic.
 */
struct HAPTLV8Characteristic {
    /**
     * Format. Must be kHAPCharacteristicFormat_TLV8.
     */
    HAPCharacteristicFormat format;

    /**
     * Instance ID.
     *
     * - Must be unique across all service or characteristic Instance IDs of the accessory.
     * - Must not change while the accessory is paired, including over firmware updates.
     * - Must not be 0.
     * - For accessories that support Bluetooth LE, must not exceed UINT16_MAX.
     */
    uint64_t iid;

    /**
     * The type of the characteristic.
     */
    const HAPUUID *characteristicType;

    /**
     * Description for debugging (based on "Type" field of HAP specification).
     */
    const char *debugDescription;

    /**
     * Description of the characteristic provided by the manufacturer of the accessory.
     */
    const char *_Nullable manufacturerDescription;

    /**
     * Characteristic properties.
     */
    HAPCharacteristicProperties properties;

    /**
     * Callbacks.
     */
    struct {
        /**
         * The callback used to handle read requests.
         * On success, the value stored in @p value is sent back to the controller.
         *
         * - Required if the characteristic is marked readable in the characteristic properties.
         * - The callback must not block. Consider prefetching values if it would take too long.
         * - The returned value must satisfy the constraints of the characteristic.
         *
         * @param      server               Accessory server.
         * @param      request              Request.
         * @param      responseWriter       TLV writer for serializing the response.
         * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
         *
         * @return kHAPError_None           If successful.
         * @return kHAPError_Unknown        If unable to communicate with requested service.
         * @return kHAPError_InvalidState   If the request cannot be processed in the current state.
         * @return kHAPError_OutOfResources If out of resources to process request.
         * @return kHAPError_Busy           If the request failed temporarily.
         */
        HAP_RESULT_USE_CHECK
        HAPError (*_Nullable handleRead)(
            HAPAccessoryServerRef *server,
            const HAPTLV8CharacteristicReadRequest *request,
            HAPTLVWriterRef *responseWriter,
            void *_Nullable context);

        /**
         * The callback used to handle write requests.
         *
         * - Required if the characteristic is marked writeable in the characteristic properties.
         * - The callback must not block. Consider queueing values if it would take too long.
         * - The value is already checked against the constraints of the characteristic.
         *
         * @param      server               Accessory server.
         * @param      request              Request.
         * @param      requestReader        TLV reader for parsing the value.
         * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
         *
         * @return kHAPError_None           If successful.
         * @return kHAPError_Unknown        If unable to communicate with requested service.
         * @return kHAPError_InvalidState   If the request cannot be processed in the current state.
         * @return kHAPError_InvalidData    If the controller sent a malformed request.
         * @return kHAPError_OutOfResources If out of resources to process request.
         * @return kHAPError_NotAuthorized  If additional authorization data is insufficient.
         * @return kHAPError_Busy           If the request failed temporarily.
         */
        HAP_RESULT_USE_CHECK
        HAPError (*_Nullable handleWrite)(
            HAPAccessoryServerRef *server,
            const HAPTLV8CharacteristicWriteRequest *request,
            HAPTLVReaderRef *requestReader,
            void *_Nullable context);

        /**
         * The callback used to handle subscribe requests.
         *
         * - The callback must not block. Consider queueing values if it would take too long.
         *
         * @param      server               Accessory server.
         * @param      request              Request.
         * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
         */
        void (*_Nullable handleSubscribe)(
            HAPAccessoryServerRef *server,
            const HAPTLV8CharacteristicSubscriptionRequest *request,
            void *_Nullable context);

        /**
         * The callback used to handle unsubscribe requests.
         *
         * - The callback must not block. Consider queueing values if it would take too long.
         *
         * @param      server               Accessory server.
         * @param      request              Request.
         * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
         */
        void (*_Nullable handleUnsubscribe)(
            HAPAccessoryServerRef *server,
            const HAPTLV8CharacteristicSubscriptionRequest *request,
            void *_Nullable context);
    } callbacks;
};

// </editor-fold>

/**
 * Properties that HomeKit services can have.
 *
 * - If service properties are enabled, a Service Signature characteristic must be attached to the service.
 *   This is only necessary if the accessory supports Bluetooth LE, but also okay for IP accessories.
 */
typedef struct {
    /**
     * The service is the primary service on the accessory.
     *
     * - Only one service may be marked as primary.
     */
    bool primaryService : 1;

    /**
     * The service should be hidden from the user.
     *
     * - This is useful for services to configure the accessory or to update firmware on the accessory.
     *   Generic HomeKit applications on the controller won't show these services.
     *
     * - When all characteristics in a service are marked hidden then the service must also be marked as hidden.
     */
    bool hidden : 1;

    /**
     * These properties only affect connections over Bluetooth LE.
     *
     * - If the accessory only supports IP (Ethernet / Wi-Fi), these properties can be ignored.
     */
    struct {
        /**
         * The service supports configuration.
         *
         * - This must be set on the HAP Protocol Information service.
         * - This must not be set on other services.
         */
        bool supportsConfiguration : 1;
    } ble;
} HAPServiceProperties;
HAP_STATIC_ASSERT(sizeof (HAPServiceProperties) == 2, HAPServiceProperties);

/**
 * Service request.
 */
typedef struct {
    /**
     * Transport type over which the request has been received.
     */
    HAPTransportType transportType;
    
    /**
     * The session over which the request has been received.
     *
     * - A controller may be logged in on multiple sessions concurrently.
     *
     * - For remote requests (e.g. via Apple TV), the associated controller may not be
     *   the one who originated the request, but instead the admin controller who set up the Apple TV.
     */
    HAPSessionRef *session;
    
    /**
     * The service that is being accessed.
     */
    const HAPService *service;
    
    /**
     * The accessory that provides the service.
     */
    const HAPAccessory *accessory;
} HAPServiceRequest;

/**
 * HomeKit service.
 */
struct HAPService {
    /**
     * Instance ID.
     *
     * - Must be unique across all service or characteristic Instance IDs of the accessory.
     * - Must not change while the accessory is paired, including over firmware updates.
     * - Must be 1 for the Accessory Information Service.
     * - Must not be 0.
     * - For accessories that support Bluetooth LE, must not exceed UINT16_MAX.
     */
    uint64_t iid;

    /**
     * The type of the service.
     */
    const HAPUUID *serviceType;

    /**
     * Description for debugging (based on "Type" field of HAP specification).
     */
    const char *debugDescription;

    /**
     * The name of the service.
     *
     * - Must be set if the service provides user visible state or interaction.
     * - Must not be set for user invisible services like a Firmware Update service.
     * - The user may adjust the name on the controller. Such changes are local only and won't sync to the accessory.
     *
     * - If a name is set, a Name characteristic must be attached to the service.
     */
    const char *_Nullable name;

    /**
     * HAP Service properties.
     *
     * - Only one service may be marked as primary.
     * - If all characteristics in a service are marked as hidden, the service must also be marked as hidden.
     *
     * - If service properties are enabled, a Service Signature characteristic must be attached to the service.
     *   This is only necessary if the accessory supports Bluetooth LE, but also okay for IP accessories.
     */
    HAPServiceProperties properties;

    /**
     * Array containing instance IDs of linked services. 0-terminated.
     *
     * - Links are not transitive. If A links to B and B links to C, it is not implied that A links to C as well.
     * - Services may not link to themselves.
     *
     * - If linked services are used, a Service Signature characteristic must be attached to the service.
     *   This is only necessary if the accessory supports Bluetooth LE, but also okay for IP accessories.
     */
    uint16_t const *_Nullable linkedServices;

    /**
     * Array of contained characteristics. NULL-terminated.
     *
     * - All HAPCharacteristic structures can be used in this array.
     *   Please ensure that the "format" fields of each structures are correct!
     */
    const HAPCharacteristic *_Nullable const *_Nullable characteristics;
};
HAP_NONNULL_SUPPORT(HAPService)

/**
 * Accessory category.
 *
 * - An accessory with support for multiple categories should advertise the primary category. An accessory for which a
 *   primary category cannot be determined or the primary category isn't among the well defined categories falls in the
 *   `Other` category.
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 14 Accessory Categories
 */
HAP_ENUM_BEGIN(uint8_t, HAPAccessoryCategory) {
    /**
     * Accessory that is accessed through a bridge.
     */
    kHAPAccessoryCategory_BridgedAccessory = 0,

    /**
     * Other.
     */
    kHAPAccessoryCategory_Other = 1,

    /**
     * Bridges.
     */
    kHAPAccessoryCategory_Bridges = 2,

    /**
     * Fans.
     */
    kHAPAccessoryCategory_Fans = 3,

    /**
     * Garage Door Openers.
     *
     * - This accessory category must use programmable tags if NFC is supported.
     */
    kHAPAccessoryCategory_GarageDoorOpeners = 4,

    /**
     * Lighting.
     */
    kHAPAccessoryCategory_Lighting = 5,

    /**
     * Locks.
     *
     * - This accessory category must use programmable tags if NFC is supported.
     */
    kHAPAccessoryCategory_Locks = 6,

    /**
     * Outlets.
     */
    kHAPAccessoryCategory_Outlets = 7,

    /**
     * Switches.
     */
    kHAPAccessoryCategory_Switches = 8,

    /**
     * Thermostats.
     */
    kHAPAccessoryCategory_Thermostats = 9,

    /**
     * Sensors.
     */
    kHAPAccessoryCategory_Sensors = 10,

    /**
     * Security Systems.
     *
     * - This accessory category must use programmable tags if NFC is supported.
     */
    kHAPAccessoryCategory_SecuritySystems = 11,

    /**
     * Doors.
     *
     * - This accessory category must use programmable tags if NFC is supported.
     */
    kHAPAccessoryCategory_Doors = 12,

    /**
     * Windows.
     *
     * - This accessory category must use programmable tags if NFC is supported.
     */
    kHAPAccessoryCategory_Windows = 13,

    /**
     * Window Coverings.
     */
    kHAPAccessoryCategory_WindowCoverings = 14,

    /**
     * Programmable Switches.
     */
    kHAPAccessoryCategory_ProgrammableSwitches = 15,

    /**
     * Range Extenders.
     *
     * @remark Obsolete since R10.
     *
     * @see HomeKit Accessory Protocol Specification R9
     *      Table 12-3 Accessory Categories
     */
    kHAPAccessoryCategory_RangeExtenders = 16,

    /**
     * IP Cameras.
     *
     * - This accessory category must use programmable tags if NFC is supported.
     */
    kHAPAccessoryCategory_IPCameras = 17,

    /**
     * Video Doorbells.
     *
     * - This accessory category must use programmable tags if NFC is supported.
     */
    kHAPAccessoryCategory_VideoDoorbells = 18,

    /**
     * Air Purifiers.
     */
    kHAPAccessoryCategory_AirPurifiers = 19,

    /**
     * Heaters.
     */
    kHAPAccessoryCategory_Heaters = 20,

    /**
     * Air Conditioners.
     */
    kHAPAccessoryCategory_AirConditioners = 21,

    /**
     * Humidifiers.
     */
    kHAPAccessoryCategory_Humidifiers = 22,

    /**
     * Dehumidifiers.
     */
    kHAPAccessoryCategory_Dehumidifiers = 23,

    /**
     * Apple TV.
     */
    kHAPAccessoryCategory_AppleTV = 24,
    
    /**
     * Sprinklers.
     */
    kHAPAccessoryCategory_Sprinklers = 28,

    /**
     * Faucets.
     */
    kHAPAccessoryCategory_Faucets = 29,

    /**
     * Shower Systems.
     */
    kHAPAccessoryCategory_ShowerSystems = 30,

    /**
     * Remote.
     */
    kHAPAccessoryCategory_Remotes = 32,
} HAP_ENUM_END(uint8_t, HAPAccessoryCategory);

/**
 * Request that originated the HomeKit Data Stream.
 */
typedef HAPServiceRequest HAPDataStreamRequest;

/**
 * HomeKit Data Stream callbacks.
 */
typedef struct {
    /**
     * The callback used to handle accepted HomeKit Data Streams.
     *
     * @param      server               Accessory server.
     * @param      request              Request that originated the HomeKit Data Stream.
     * @param      dataStream           The newly accepted HomeKit Data Stream.
     * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
     */
    void (*handleAccept)(
        HAPAccessoryServerRef *server,
        const HAPDataStreamRequest *request,
        HAPDataStreamRef *dataStream,
        void *_Nullable context);
    
    /**
     * The callback used when a HomeKit Data Stream is invalidated.
     *
     * - /!\ WARNING: The HomeKit Data Stream must no longer be used after this callback returns.
     *
     * @param      server               Accessory server.
     * @param      request              Request that originated the HomeKit Data Stream.
     * @param      dataStream           HomeKit Data Stream.
     * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
     */
    void (*handleInvalidate)(
        HAPAccessoryServerRef *server,
        const HAPDataStreamRequest *request,
        HAPDataStreamRef *dataStream,
        void *_Nullable context);
    
    /**
     * The callback used to handle incoming data.
     *
     * - Use a series of HAPDataStreamReceiveData and HAPDataStreamSkipData calls to receive or skip data parts.
     *   Total requested length must match the total length of incoming data.
     *
     * - /!\ WARNING: Received data must only be processed after all parts have been successfully received or skipped.
     *   If a decryption error occurs, an error will be set only after the final part is received.
     *
     * @param      server               Accessory server.
     * @param      request              Request that originated the HomeKit Data Stream.
     * @param      dataStream           HomeKit Data Stream.
     * @param      dataType             Type of incoming data.
     * @param      totalDataBytes       Total length of incoming data.
     * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
     */
    void (*handleData)(
        HAPAccessoryServerRef *server,
        const HAPDataStreamRequest *request,
        HAPDataStreamRef *dataStream,
        HAPDataStreamDataType dataType,
        size_t totalDataBytes,
        void *_Nullable context);
} HAPDataStreamCallbacks;

/**
 * Invalidates a HomeKit Data Stream.
 *
 * - When invalidation is complete, the handleInvalidate callback will be called.
 *
 * - This also cancels all pending HomeKit Data Stream tasks (completion handlers will be called with an error).
 *
 * - /!\ WARNING: The HomeKit Data Stream must no longer be used after this function returns.
 *
 * @param      server               Accessory server.
 * @param      dataStream           HomeKit Data Stream.
 */
void HAPDataStreamInvalidate(
    HAPAccessoryServerRef *server,
    HAPDataStreamRef *dataStream);

/**
 * Completion handler of a HomeKit Data Stream operation.
 *
 * @param      server               Accessory server.
 * @param      request              Request that originated the HomeKit Data Stream.
 * @param      dataStream           HomeKit Data Stream.
 * @param      error                kHAPError_None           If successful.
 *                                  kHAPError_InvalidState   If the HomeKit Data Stream is being invalidated.
 *                                                           No further operations may be started.
 * @param      dataBytes            Data buffer provided when starting the HomeKit Data Stream operation.
 * @param      numDataBytes         Number of processed bytes.
 * @param      isComplete           An indication that all data parts have been processed.
 * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
 */
typedef void (*HAPDataStreamDataCompletionHandler)(
    HAPAccessoryServerRef *server,
    const HAPDataStreamRequest *request,
    HAPDataStreamRef *dataStream,
    HAPError error,
    void *_Nullable dataBytes,
    size_t numDataBytes,
    bool isComplete,
    void *_Nullable context);

/**
 * Receives data from a HomeKit Data Stream.
 *
 * - When the handleData callback is invoked, use a series of HAPDataStreamReceiveData and HAPDataStreamSkipData calls
 *   to receive or skip data parts. Total requested length must match the total length of incoming data.
 *
 * - /!\ WARNING: Received data must only be processed after all parts have been successfully received or skipped.
 *   If a decryption error occurs, an error will be set only after the final part is received.
 *
 * - /!\ WARNING: The data buffer must remain valid until the completion handler is invoked.
 *
 * - Only one receive or skip operation can be active at a time on a HomeKit Data Stream.
 *
 * @param      server               Accessory server.
 * @param      dataStream           HomeKit Data Stream.
 * @param      dataBytes            Data buffer to fill with incoming data. Must remain valid.
 * @param      numDataBytes         Number of bytes to read.
 * @param      completionHandler    Completion handler to call when the task is complete.
 */
void HAPDataStreamReceiveData(
    HAPAccessoryServerRef *server,
    HAPDataStreamRef *dataStream,
    void *dataBytes,
    size_t numDataBytes,
    HAPDataStreamDataCompletionHandler completionHandler);

/**
 * Skips incoming data from a HomeKit Data Stream.
 *
 * - When the handleData callback is invoked, use a series of HAPDataStreamReceiveData and HAPDataStreamSkipData calls
 *   to receive or skip data parts. Total requested length must match the total length of incoming data.
 *
 * - Only one receive or skip operation can be active at a time on a HomeKit Data Stream.
 *
 * @param      server               Accessory server.
 * @param      dataStream           HomeKit Data Stream.
 * @param      numDataBytes         Number of bytes to skip.
 * @param      completionHandler    Completion handler to call when the task is complete.
 */
void HAPDataStreamSkipData(
    HAPAccessoryServerRef *server,
    HAPDataStreamRef *dataStream,
    size_t numDataBytes,
    HAPDataStreamDataCompletionHandler completionHandler);

/**
 * Prepares to send data over a HomeKit Data Stream.
 *
 * - When the completion handler is invoked, use a series of HAPDataStreamSendData / HAPDataStreamSendMutableData calls
 *   to send data parts. Total sent length must match the total length of outgoing data.
 *
 * - Only one send operation can be active at a time on a HomeKit Data Stream.
 *
 * @param      server               Accessory server.
 * @param      dataStream           HomeKit Data Stream.
 * @param      dataType             Type of data.
 * @param      totalDataBytes       Total length of outgoing data.
 * @param      completionHandler    Completion handler to call when the task is complete.
 */
void HAPDataStreamPrepareData(
    HAPAccessoryServerRef *server,
    HAPDataStreamRef *dataStream,
    HAPDataStreamDataType dataType,
    size_t totalDataBytes,
    HAPDataStreamDataCompletionHandler completionHandler);

/**
 * Sends data over a HomeKit Data Stream.
 *
 * - This function is less efficient than the HAPDataStreamSendMutableData function. If possible, use the latter.
 *
 * - Before this function can be called, use HAPDataStreamPrepareData to provide the type and total length of data.
 *   Then, use a series of HAPDataStreamSendData / HAPDataStreamSendMutableData calls to send data parts.
 *   Total sent length must match the total length of outgoing data.
 *
 * - /!\ WARNING: The data buffer must remain valid until the completion handler is invoked.
 *
 * - Only one send operation can be active at a time on a HomeKit Data Stream.
 *
 * @param      server               Accessory server.
 * @param      dataStream           HomeKit Data Stream.
 * @param      dataBytes            Data buffer. Must remain valid.
 * @param      numDataBytes         Number of bytes to write.
 * @param      completionHandler    Completion handler to call when the task is complete.
 */
void HAPDataStreamSendData(
    HAPAccessoryServerRef *server,
    HAPDataStreamRef *dataStream,
    const void *dataBytes,
    size_t numDataBytes,
    HAPDataStreamDataCompletionHandler completionHandler);

/**
 * Sends mutable data over a HomeKit Data Stream.
 *
 * - Before this function can be called, use HAPDataStreamPrepareData to provide the type and total length of data.
 *   Then, use a series of HAPDataStreamSendData / HAPDataStreamSendMutableData calls to send data parts.
 *   Total sent length must match the total length of outgoing data.
 *
 * - /!\ WARNING: The data buffer must remain valid until the completion handler is invoked.
 *
 * - Only one send operation can be active at a time on a HomeKit Data Stream.
 *
 * @param      server               Accessory server.
 * @param      dataStream           HomeKit Data Stream.
 * @param      dataBytes            Data buffer. Will be modified. Must remain valid.
 * @param      numDataBytes         Number of bytes to write.
 * @param      completionHandler    Completion handler to call when the task is complete.
 */
void HAPDataStreamSendMutableData(
    HAPAccessoryServerRef *server,
    HAPDataStreamRef *dataStream,
    void *dataBytes,
    size_t numDataBytes,
    HAPDataStreamDataCompletionHandler completionHandler);

/**
 * Supported video codec configuration.
 */
typedef struct {
    /**
     * Video codec type.
     */
    HAPVideoCodecType codecType;

    /**
     * Video codec parameters.
     */
    const HAPVideoCodecParameters *codecParameters;

    /**
     * Video attributes.
     */
    const HAPVideoAttributes *_Nullable const *_Nullable attributes;
} HAPCameraSupportedVideoCodecConfiguration;

/**
 * Supported audio codec configuration.
 */
typedef struct {
    /**
     * Audio codec type.
     */
    HAPAudioCodecType codecType;

    /**
     * Audio codec parameters.
     */
    const HAPAudioCodecParameters *codecParameters;
} HAPCameraSupportedAudioCodecConfiguration;

/**
 * Collection of supported configurations for an individual IP camera stream.
 */
typedef struct {
    /**
     * Supported video parameters.
     */
    struct {
        /**
         * Array of supported video codec configurations. NULL-terminated.
         */
        const HAPCameraSupportedVideoCodecConfiguration *_Nullable const *_Nullable configurations;
    } videoStream;

    /**
     * Supported audio parameters.
     */
    struct {
        /**
         * Array of supported audio codec configurations. NULL-terminated.
         */
        const HAPCameraSupportedAudioCodecConfiguration *_Nullable const *_Nullable configurations;

        /**
         * Comfort Noise Codec parameters.
         */
        struct {
            /**
             * Whether Comfort Noise Codec is supported.
             */
            bool supported;
        } comfortNoise;
    } audioStream;

    /**
     * Supported RTP parameters.
     */
    struct {
        /**
         * Supported SRTP crypto suites.
         *
         * - If multiple SRTP crypto suites are supported, set this to a bitwise or value.
         */
        HAPSRTPCryptoSuite srtpCryptoSuites;
    } rtp;
} HAPCameraStreamSupportedConfigurations;

/**
 * Accessory identify request.
 */
typedef struct {
    /**
     * Transport type over which the request has been received.
     */
    HAPTransportType transportType;

    /**
     * The session over which the request has been received.
     *
     * - A controller may be logged in on multiple sessions concurrently.
     *
     * - For remote requests (e.g. via Apple TV), the associated controller may not be
     *   the one who originated the request, but instead the admin controller who set up the Apple TV.
     */
    HAPSessionRef *session;

    /**
     * The accessory that is being accessed.
     */
    const HAPAccessory *accessory;

    /**
     * Whether the request appears to have originated from a remote controller, e.g. Apple TV or iCloud.
     */
    bool remote;
} HAPAccessoryIdentifyRequest;

/**
 * HomeKit accessory.
 */
struct HAPAccessory {
    /**
     * Accessory instance ID.
     *
     * For regular accessories (Bluetooth LE / IP):
     * - Must be 1.
     *
     * For bridged accessories:
     * - Must be unique for the bridged accessory and not change across firmware updates or power cycles.
     */
    uint64_t aid;

    /**
     * Category information for the accessory.
     *
     * For regular accessories (Bluetooth LE / IP):
     * - Must match the functionality of the accessory's primary service.
     *
     * For bridged accessories:
     * - Must be kHAPAccessoryCategory_BridgedAccessory.
     */
    HAPAccessoryCategory category;

    /**
     * The display name of the accessory.
     *
     * - Maximum length 64 (excluding NULL-terminator).
     * - ':' and ';' characters should not be used for accessories that support Bluetooth LE.
     * - The user may adjust the name on the controller. Such changes are local only and won't sync to the accessory.
     */
    const char *name;

    /**
     * The manufacturer of the accessory.
     *
     * - Maximum length 64 (excluding NULL-terminator).
     */
    const char *manufacturer;

    /**
     * The model name of the accessory.
     *
     * - Minimum length 1 (excluding NULL-terminator).
     * - Maximum length 64 (excluding NULL-terminator).
     */
    const char *model;

    /**
     * The serial number of the accessory.
     *
     * - Minimum length 2 (excluding NULL-terminator).
     * - Maximum length 64 (excluding NULL-terminator).
     */
    const char *serialNumber;

    /**
     * The firmware version of the accessory.
     *
     * - x[.y[.z]] (e.g. "100.1.1")
     * - Each number must not be greater than UINT32_MAX.
     * - Maximum length 64 (excluding NULL-terminator).
     */
    const char *firmwareVersion;

    /**
     * The hardware version of the accessory.
     *
     * - x[.y[.z]] (e.g. "100.1.1")
     * - Maximum length 64 (excluding NULL-terminator).
     */
    const char *_Nullable hardwareVersion;

    /**
     * Array of provided services. NULL-terminated.
     */
    const HAPService *_Nullable const *_Nullable services;
    
    /**
     * HomeKit Data Stream specific options.
     *
     * - This is only necessary if the accessory supports HomeKit Data Stream.
     */
    struct {
        /**
         * Delegate to handle HomeKit Data Stream events.
         */
        struct {
            /** Callbacks. */
            const HAPDataStreamCallbacks *_Nullable callbacks;
            
            /** Client context pointer. */
            void *_Nullable context;
        } delegate;
    } dataStream;

    /**
     * Array of IP camera stream configurations. NULL-terminated.
     *
     * - For every available camera stream, a stream configuration must be specified
     *   and a Camera RTP Stream Management Service must be added to the services array.
     *
     * - Only supported for HAP over IP (Ethernet / Wi-Fi).
     *
     * - Not allowed for bridged accessories.
     */
    const HAPCameraStreamSupportedConfigurations *_Nullable const *_Nullable cameraStreamConfigurations;

    /**
     * Callbacks.
     */
    struct {
        /**
         * The callback used to invoke the identify routine.
         *
         * @param      server               Accessory server.
         * @param      request              Request.
         * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
         *
         * @return kHAPError_None           If successful.
         * @return kHAPError_Unknown        If unable to communicate with requested service.
         * @return kHAPError_InvalidState   If the request cannot be processed in the current state.
         * @return kHAPError_InvalidData    If the controller sent a malformed request.
         * @return kHAPError_OutOfResources If out of resources to process request.
         * @return kHAPError_NotAuthorized  If additional authorization data is insufficient.
         * @return kHAPError_Busy           If the request failed temporarily.
         */
        HAP_RESULT_USE_CHECK
        HAPError (*_Nullable identify)(
            HAPAccessoryServerRef *server,
            const HAPAccessoryIdentifyRequest *request,
            void *_Nullable context);
    } callbacks;
};
HAP_NONNULL_SUPPORT(HAPAccessory)

/**
 * Minimum number of supported pairings.
 *
 * - Pairings are stored in one of the domains of the platform's key-value store.
 *   The maximum number of supported pairings is restricted to the available number of keys per domain (256).
 */
#define kHAPPairingStorage_MinElements ((HAPPlatformKeyValueStoreKey) 16)

/**
 * IP read context.
 */
typedef HAP_OPAQUE(48) HAPIPReadContextRef;

/**
 * IP write context.
 */
typedef HAP_OPAQUE(64) HAPIPWriteContextRef;

/**
 * IP session descriptor.
 */
typedef HAP_OPAQUE(824) HAPIPSessionDescriptorRef;

/**
 * IP event notification.
 */
typedef HAP_OPAQUE(24) HAPIPEventNotificationRef;

/**
 * Default size for the inbound buffer of an IP session.
 */
#define kHAPIPSession_DefaultInboundBufferSize ((size_t) 32768)

/**
 * Default size for the outbound buffer of an IP session.
 */
#define kHAPIPSession_DefaultOutboundBufferSize ((size_t) 32768)

/**
 * Default size for the outbound buffer of an IP session.
 */
#define kHAPIPSession_DefaultScratchBufferSize ((size_t) 32768)

/**
 * IP session.
 *
 * - For accessories that support IP (Ethernet / Wi-Fi), one of these structures needs to be allocated
 *   per concurrently supported IP connection, and provided as part of a HAPIPSessionStorage structure.
 *
 * - The provided memory must remain valid while the accessory server is initialized.
 */
typedef struct {
    /**
     * IP session descriptor.
     */
    HAPIPSessionDescriptorRef descriptor;

    /**
     * Buffer to store received data.
     */
    struct {
        /**
         * Inbound buffer. Memory must be allocated and must remain valid while the accessory server is initialized.
         *
         * - It is recommended to allocate at least kHAPIPSession_DefaultInboundBufferSize bytes,
         *   but the optimal size may vary depending on the accessory's attribute database.
         */
        void *bytes;

        /**
         * Size of inbound buffer.
         */
        size_t numBytes;
    } inboundBuffer;

    /**
     * Buffer to store pending data to be sent.
     */
    struct {
        /**
         * Outbound buffer. Memory must be allocated and must remain valid while the accessory server is initialized.
         *
         * - It is recommended to allocate at least kHAPIPSession_DefaultOutboundBufferSize bytes,
         *   but the optimal size may vary depending on the accessory's attribute database.
         */
        void *bytes;

        /**
         * Size of outbound buffer.
         */
        size_t numBytes;
    } outboundBuffer;

    /**
     * Event notifications.
     *
     * - At least one of these structures must be allocated per HomeKit characteristic and service and must remain
     *   valid while the accessory server is initialized.
     */
    HAPIPEventNotificationRef *eventNotifications;

    /**
     * Number of event notification structures.
     */
    size_t numEventNotifications;
} HAPIPSession;

/**
 * Default number of elements in a HAPIPSessionStorage.
 */
#define kHAPIPSessionStorage_DefaultNumElements ((size_t) 17)

/**
 * IP server storage.
 *
 * - For accessories that support IP (Ethernet / Wi-Fi), one of these structures needs to be allocated
 *   and provided as part of the initialization options in HAPAccessoryServerCreate.
 *
 * - The provided memory (including the HAPIPServerStorage structure) must remain valid
 *   while the accessory server is initialized.
 */
typedef struct {
    /**
     * IP sessions.
     *
     * - One session must be provided per concurrently supported IP connection.
     *   Each session contains additional memory that needs to be allocated. See HAPIPSession.
     *
     * - At least eight elements are required for IP (Ethernet / Wi-Fi) accessories.
     */
    HAPIPSession *sessions;

    /**
     * Number of sessions.
     */
    size_t numSessions;

    /**
     * IP read contexts.
     *
     * - At least one of these structures must be allocated per HomeKit characteristic and service and must remain
     *   valid while the accessory server is initialized.
     */
    HAPIPReadContextRef *readContexts;

    /**
     * Number of read contexts.
     */
    size_t numReadContexts;

    /**
     * IP write contexts.
     *
     * - At least one of these structures must be allocated per HomeKit characteristic and service and must remain
     *   valid while the accessory server is initialized.
     */
    HAPIPWriteContextRef *writeContexts;

    /**
     * Number of write contexts.
     */
    size_t numWriteContexts;

    /**
     * Scratch buffer.
     */
    struct {
        /**
         * Scratch buffer. Memory must be allocated and must remain valid while the accessory server is initialized.
         *
         * - It is recommended to allocate at least kHAPIPSession_DefaultScratchBufferSize bytes,
         *   but the optimal size may vary depending on the accessory's attribute database.
         */
        void *bytes;

        /**
         * Size of scratch buffer.
         */
        size_t numBytes;
    } scratchBuffer;

    /**
     * Unused protocol buffer.
     */
    HAP_DEPRECATED_MSG("Not used anymore.")
    struct {
        /**
         * Unused protocol buffer.
         */
        HAP_DEPRECATED_MSG("Not used anymore.")
        void *bytes;

        /**
         * Size of unused protocol buffer.
         */
        HAP_DEPRECATED_MSG("Not used anymore.")
        size_t numBytes;
    } protocolBuffer;
} HAPIPAccessoryServerStorage;
HAP_NONNULL_SUPPORT(HAPIPAccessoryServerStorage)

/**
 * IP Camera streaming session.
 *
 * - For IP Camera accessories, one of these structures needs to be allocated per Camera RTP Stream Management Service
 *   and provided as part of a HAPCameraStreamingSessionStorage structure.
 */
typedef HAP_OPAQUE(368) HAPCameraStreamingSessionRef;

/**
 * IP Camera streaming session setup.
 *
 * - For IP Camera accessories, one of these structures needs to be allocated per Camera RTP Stream Management Service
 *   and concurrently supported IP connection, and provided as part of a HAPCameraStreamingSessionStorage structure.
 */
typedef HAP_OPAQUE(184) HAPCameraStreamingSessionSetupRef;

/**
 * IP Camera streaming sessions.
 *
 * - For IP Camera accessories, one of these structures needs to be allocated
 *   and provided as part of the initialization options in HAPAccessoryServerCreate.
 *
 * - The provided streaming sessions must remain valid while the accessory server is initialized.
 */
typedef struct {
    /**
     * IP Camera streaming sessions.
     *
     * - One session must be provided per Camera RTP Stream Management Service.
     */
    HAPCameraStreamingSessionRef *sessions;

    /**
     * Number of sessions.
     */
    size_t numSessions;

    /**
     * IP Camera streaming session setups.
     *
     * - One setup must be provided per concurrently supported IP connection
     *   and per Camera RTP Stream Management Service.
     */
    HAPCameraStreamingSessionSetupRef *setups;

    /**
     * Number of setups.
     */
    size_t numSetups;
} HAPCameraStreamingSessionStorage;

/**
 * Element of the BLE GATT table.
 *
 * - For accessories that support Bluetooth LE, at least one of these elements must be allocated per HomeKit
 *   characteristic and service, and provided as part of a HAPBLEAccessoryServerStorage structure.
 */
typedef HAP_OPAQUE(56) HAPBLEGATTTableElementRef;

/**
 * Minimum number of BLE session cache elements in a HAPBLEAccessoryServerStorage.
 */
#define kHAPBLESessionCache_MinElements ((size_t) 8)

/**
 * Element of the BLE Pair Resume session cache.
 *
 * - For accessories that support Bluetooth LE, at least kHAPBLESessionCache_MinElements
 *   of these elements must be allocated and provided as part of a HAPBLEAccessoryServerStorage structure.
 */
typedef HAP_OPAQUE(48) HAPBLESessionCacheElementRef;

/**
 * HAP-BLE procedure.
 *
 * - For accessories that support Bluetooth LE, at least one of these procedures must be allocated
 *   and provided as part of a HAPBLEAccessoryServerStorage structure.
 */
typedef HAP_OPAQUE(152) HAPBLEProcedureRef;

/**
 * BLE accessory server storage.
 *
 * - For accessories that support Bluetooth LE, one of these structures needs to be allocated
 *   and provided as part of the initialization options in HAPAccessoryServerCreate.
 *
 * - The provided memory (including the HAPBLEAccessoryServerStorage structure) must remain valid
 *   while the accessory server is initialized.
 */
typedef struct {
    /**
     * BLE GATT table elements.
     *
     * - At least one of these elements is required per HomeKit characteristic and service
     *   and must remain valid while the accessory server is initialized.
     */
    HAPBLEGATTTableElementRef *gattTableElements;

    /**
     * Number of BLE GATT table elements.
     */
    size_t numGATTTableElements;

    /**
     * BLE Pair Resume session cache. Storage must remain valid.
     *
     * - Controllers may use the cache to speed up re-connections to the accessory by allowing them
     *   to resume previously established sessions ("Pair Resume").
     *
     * - The cache size determines how many different controllers can take advantage of this feature
     *   before they have to do a re-connection with regular speed again.
     *
     * - At least kHAPBLESessionCache_MinElements elements are required
     *   and must remain valid while the accessory server is initialized.
     */
    HAPBLESessionCacheElementRef *sessionCacheElements;

    /**
     * Number of BLE session cache elements.
     */
    size_t numSessionCacheElements;

    /**
     * BLE session storage. Storage must remain valid.
     */
    HAPSessionRef *session;

    /**
     * HAP-BLE procedures. Storage must remain valid.
     */
    HAPBLEProcedureRef *procedures;

    /**
     * Number of HAP-BLE procedures.
     */
    size_t numProcedures;

    /**
     * Buffer that the HAP-BLE procedures may use.
     *
     * - This must be large enough to fit the largest characteristic value.
     */
    struct {
        /**
         * HAP-BLE procedure buffer.
         */
        void *bytes;

        /**
         * Size of HAP-BLE procedure buffer.
         */
        size_t numBytes;
    } procedureBuffer;
} HAPBLEAccessoryServerStorage;
HAP_NONNULL_SUPPORT(HAPBLEAccessoryServerStorage)

/**
 * Minimum supported advertising interval for Bluetooth LE.
 */
#define kHAPBLEAdvertisingInterval_Minimum (HAPBLEAdvertisingIntervalCreateFromMilliseconds(160))

/**
 * Maximum supported advertising interval for Bluetooth LE.
 */
#define kHAPBLEAdvertisingInterval_Maximum (HAPBLEAdvertisingIntervalCreateFromMilliseconds(2500))

/**
 * Minimum duration of broadcast notifications and disconnected notifications in ms.
 */
#define kHAPBLENotification_MinDuration (HAPBLEAdvertisingIntervalCreateFromMilliseconds(3000))

/**
 * Accessory server initialization options.
 */
typedef struct {
    /**
     * Maximum number of allowed pairings.
     *
     * - Must be at least kHAPPairingStorage_MinElements.
     */
    HAPPlatformKeyValueStoreKey maxPairings;

    /**
     * IP specific initialization options.
     */
    struct {
        /**
         * Accessory supports HAP over IP (Ethernet / Wi-Fi).
         *
         * - If this is set to false, the other IP specific initialization options are ignored.
         */
        bool available : 1;

        /**
         * IP accessory server storage. Storage must remain valid.
         */
        HAPIPAccessoryServerStorage *_Nullable accessoryServerStorage;
        
        /**
         * HomeKit Data Stream specific initialization options.
         */
        struct {
            /**
             * TCP stream manager.
             *
             * - This is only necessary if the accessory supports HomeKit Data Stream.
             *
             * - Needs to be a separate instance other than the TCP stream manager part of HAPPlatform.
             */
            HAPPlatformTCPStreamManagerRef _Nullable tcpStreamManager;
            
            /**
             * Storage for HomeKit Data Streams. Must remain valid.
             */
            HAPDataStreamRef *_Nullable dataStreams;
            
            /**
             * Number of HomeKit Data Streams.
             */
            size_t numDataStreams;
        } dataStream;
        
        /**
         * Wi-Fi Accessory Configuration (WAC) specific initialization options.
         */
        struct {
            /**
             * Accessory supports Wi-Fi Accessory Configuration (WAC) for configuring Wi-Fi credentials.
             *
             * - Requires the platform to support authentication.
             */
            bool available : 1;
            
            /**
             * Use Wi-Fi Accessory Configuration mode that is compatible with iOS versions prior to 11.3.
             *
             * - When using compatibility mode, the user is asked for consent during pairing.
             *
             * - Compatibility mode is only available when an Apple Authentication Coprocessor is present.
             */
            HAP_DEPRECATED_MSG("Compatibility mode support will be removed in a future version.")
            bool useCompatibilityMode : 1;
        } wac;

        /**
         * IP Camera specific initialization options.
         */
        struct {
            /**
             * IP Camera streaming sessions. Storage must remain valid.
             */
            HAPCameraStreamingSessionStorage *_Nullable streamingSessionStorage;
        } camera;
    } ip;

    /**
     * BLE specific initialization options.
     */
    struct {
        /**
         * Accessory supports HAP over Bluetooth LE.
         *
         * - If this is set to false, the other BLE specific initialization options are ignored.
         */
        bool available : 1;

        /**
         * BLE accessory server storage. Storage must remain valid.
         */
        HAPBLEAccessoryServerStorage *_Nullable accessoryServerStorage;

        /**
         * Preferred Regular Advertising Interval in ms.
         *
         * Accessories must choose the regular advertising intervals in the range of kHAPBLEAdvertisingInterval_Minimum
         * ms to kHAPBLEAdvertisingInterval_Maximum ms, depending on the accessory category and its power consumption
         * characteristics.
         *
         * Mains powered accessories or accessories with larger battery capacity should use a shorter interval between
         * 160 ms to 800 ms for its regular advertisements. Battery powered accessories that do not have any
         * controllable Apple defined characteristics (such as temperature sensors, door sensors etc) are allowed to
         * use a larger regular advertising interval between 1250 ms and 2500 ms for enhanced battery life.
         *
         * Preferred advertising intervals:
         * - 211.25 ms
         * - 318.75 ms
         * - 417.5 ms
         * - 546.25 ms
         * - 760 ms
         * - 852.5 ms
         * - 1022.5 ms
         * - 1285 ms
         *
         * Note: Longer advertising intervals usually result in longer discovery and connection times.
         *
         * Use HAPBLEAdvertisingIntervalCreateFromMilliseconds to convert an advertising interval in milliseconds
         * to an advertising interval for Bluetooth LE.
         *
         * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
         *      Section 7.4.1.4 Advertising Interval
         *
         * @see Accessory Design Guidelines for Apple Devices R5
         *      Section 10.5 Advertising Interval
         */
        HAPBLEAdvertisingInterval preferredAdvertisingInterval;

        /**
         * Preferred duration of events in ms.
         *
         * Must be at least kHAPBLENotification_MinDuration.
         */
        HAPBLEAdvertisingInterval preferredNotificationDuration;
    } ble;
} HAPAccessoryServerOptions;

/**
 * HomeKit platform structure.
 *
 * The following platform modules must be implemented additionally:
 * - HAPPlatformAbort
 * - HAPPlatformLog
 * - HAPPlatformRandomNumber
 * - HAPPlatformClock
 * - HAPPlatformTimer
 * - HAPPlatformRunLoop
 */
typedef struct {
    /**
     * Key-Value store.
     */
    HAPPlatformKeyValueStoreRef keyValueStore;
    
    /**
     * Accessory setup manager.
     */
    HAPPlatformAccessorySetupRef accessorySetup;
    
    /**
     * These platform modules are only necessary if the accessory supports HAP over IP (Ethernet / Wi-Fi).
     */
    struct {
        /**
         * TCP stream manager.
         */
        HAPPlatformTCPStreamManagerRef _Nullable tcpStreamManager;
        
        /**
         * Service discovery.
         */
        HAPPlatformServiceDiscoveryRef _Nullable serviceDiscovery;
        
        /**
         * These platform modules are only necessary if the accessory supports
         * Wi-Fi Accessory Configuration (WAC) for configuring Wi-Fi credentials.
         */
        struct {
            /**
             * Software Access Point manager.
             */
            HAPPlatformSoftwareAccessPointRef _Nullable softwareAccessPoint;
            
            /**
             * Wi-Fi manager.
             */
            HAPPlatformWiFiManagerRef _Nullable wiFiManager;
        } wac;
        
        /**
         * IP Camera provider.
         *
         * - This platform module is only necessary if the accessory is an IP Camera.
         */
        HAPPlatformCameraRef _Nullable camera;
    } ip;
    
    /**
     * These platform modules are only necessary if the accessory supports HAP over Bluetooth LE.
     */
    struct {
        /**
         * BLE peripheral manager.
         */
        HAPPlatformBLEPeripheralManagerRef _Nullable blePeripheralManager;
    } ble;
    
    /**
     * These platform modules are only necessary for production accessories or
     * if the accessory supports Wi-Fi Accessory Configuration (WAC) for configuring Wi-Fi credentials.
     *
     * - Only one authentication method needs to be implemented.
     */
    struct {
        /**
         * Apple Authentication Coprocessor provider.
         *
         * - This platform module is only necessary if an Apple Authentication Coprocessor is connected.
         */
        HAPPlatformMFiHWAuthRef _Nullable mfiHWAuth;
        
        /**
         * Software Token provider.
         *
         * - This platform module is only necessary if Software Authentication is supported.
         */
        HAPPlatformMFiTokenAuthRef _Nullable mfiTokenAuth;
    } authentication;
} HAPPlatform;

/**
 * Accessory server callbacks.
 *
 * - Callbacks must not block.
 */
typedef struct {
    /**
     * Invoked when the accessory server state changes.
     *
     * - Updated state may be retrieved through the HAPAccessoryServerGetState, HAPAccessoryServerIsPaired and
     *   HAPAccessoryServerIsInWACMode methods.
     *
     * - The callback must not block.
     *
     * @param      server               Accessory server.
     * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
     */
    void (*handleUpdatedState)(
        HAPAccessoryServerRef *server,
        void *_Nullable context);

    /**
     * The callback used when a HomeKit Session is accepted.
     *
     * @param      server               Accessory server.
     * @param      session              The newly accepted session.
     * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
     */
    void (*handleSessionAccept)(
        HAPAccessoryServerRef *server,
        HAPSessionRef *session,
        void *_Nullable context);

    /**
     * The callback used when a HomeKit Session is invalidated.
     *
     * - /!\ WARNING: The HomeKit Session must no longer be used after this callback returns.
     *
     * @param      server               Accessory server.
     * @param      session              The session being invalidated.
     * @param      context              The context parameter given to the HAPAccessoryServerCreate function.
     */
    void (*handleSessionInvalidate)(
        HAPAccessoryServerRef *server,
        HAPSessionRef *session,
        void *_Nullable context);
} HAPAccessoryServerCallbacks;

/**
 * Creates a new HomeKit accessory server.
 *
 * - Callbacks are always invoked synchronously. They must not block.
 *   This also applies to characteristic value read / write callbacks.
 *
 * @param[out] server               An uninitialized accessory server.
 * @param      options              Initialization options for the accessory server.
 * @param      platform             Initialized HomeKit platform structure.
 * @param      callbacks            Callbacks to receive server events. Callbacks must not block.
 * @param      context              Client context pointer. Will be passed to callbacks.
 */
void HAPAccessoryServerCreate(
    HAPAccessoryServerRef *server,
    const HAPAccessoryServerOptions *options,
    const HAPPlatform *platform,
    const HAPAccessoryServerCallbacks *callbacks,
    void *_Nullable context);

/**
 * Releases resources associated with an initialized HomeKit accessory server.
 *
 * - If the accessory server is currently running, it will be ungracefully stopped by this method.
 *   Consider stopping the accessory server first and waiting for proper shutdown to occur before releasing resources.
 *
 * - IMPORTANT: Do not use this method on accessory server structures that are not initialized!
 *
 * @param      server               An initialized accessory server.
 */
void HAPAccessoryServerRelease(
    HAPAccessoryServerRef *server);

/**
 * Accessory server state.
 */
HAP_ENUM_BEGIN(uint8_t, HAPAccessoryServerState) {
    /**
     * Server is initialized but not running.
     */
    kHAPAccessoryServerState_Idle,

    /**
     * Server is running.
     */
    kHAPAccessoryServerState_Running,

    /**
     * Server is shutting down.
     */
    kHAPAccessoryServerState_Stopping
} HAP_ENUM_END(uint8_t, HAPAccessoryServerState);

/**
 * Gets the state of an initialized HomeKit accessory server.
 *
 * @param      server               An initialized accessory server.
 *
 * @return Accessory server state.
 */
HAP_RESULT_USE_CHECK
HAPAccessoryServerState HAPAccessoryServerGetState(
    HAPAccessoryServerRef *server);

/**
 * Returns whether the HomeKit accessory server is paired with any controllers.
 *
 * @param      server               An initialized accessory server.
 *
 * @return Whether the accessory server is paired with any controllers.
 */
HAP_RESULT_USE_CHECK
bool HAPAccessoryServerIsPaired(
    HAPAccessoryServerRef *server);

/**
 * Returns whether the HomeKit accessory server is in WAC mode.
 *
 * @param      server               An initialized accessory server.
 *
 * @return Whether the accessory server is in WAC mode.
 */
HAP_RESULT_USE_CHECK
bool HAPAccessoryServerIsInWACMode(
    HAPAccessoryServerRef *server);

/**
 * Starts the accessory server.
 *
 * - To start a bridge, use HAPAccessoryServerStartBridge instead.
 *
 * - The server state can be observed using the handleUpdatedState callback. The server never stops on its own.
 *
 * - The server will enter WAC mode automatically if WAC is supported by the platform and when it is started in a
 *   non-configured state (see functions HAPAccessoryServerIsInWACMode and HAPAccessoryServerEnterWACMode).
 *
 * @param      server               An initialized accessory server that is not running.
 * @param      accessory            Accessory to serve. Must remain valid while started.
 */
void HAPAccessoryServerStart(
    HAPAccessoryServerRef *server,
    const HAPAccessory *accessory);

/**
 * Maximum number of supported bridged accessories not including the bridge itself.
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 2.5.3.2 Bridges
 */
#define kHAPAccessoryServerMaxBridgedAccessories ((size_t) 149)

/**
 * Starts the accessory server for a bridge accessory. A bridge is a special type of HAP accessory that
 * bridges HAP and different RF/transport protocols, such as ZigBee or Z-Wave.
 *
 * - Only supported for HAP over IP (Ethernet / Wi-Fi).
 *
 * - A bridge accessory must not bridge more than kHAPAccessoryServerMaxBridgedAccessories accessories.
 *
 * - To change the bridged accessories (e.g. after firmware update or after modified bridge configuration),
 *   stop the server, then apply changes to the @p bridgedAccessories array, then start the server again.
 *
 * - The server state can be observed using the handleUpdatedState callback. The server never stops on its own.
 *
 * - The server will enter WAC mode automatically if WAC is supported by the platform and when it is started in a
 *   non-configured state (see functions HAPAccessoryServerIsInWACMode and HAPAccessoryServerEnterWACMode).
 *
 * @param      server               An initialized accessory server that is not running.
 * @param      bridgeAccessory      Bridge accessory to serve. Must remain valid while started.
 * @param      bridgedAccessories   Array of bridged accessories. NULL-terminated. Must remain valid while started.
 * @param      configurationChanged Whether or not the bridge configuration changed since the last start.
 *                                  This includes adding / removing accessories or updating FW of a bridged accessory.
 */
void HAPAccessoryServerStartBridge(
    HAPAccessoryServerRef *server,
    const HAPAccessory *bridgeAccessory,
    const HAPAccessory *_Nullable const *_Nullable bridgedAccessories,
    bool configurationChanged);

/**
 * Stops the accessory server.
 *
 * - When the accessory server is fully stopped, the handleUpdatedState callback will be invoked.
 *   The server can only be started again after that callback has been invoked.
 *
 * @param      server               An initialized accessory server.
 */
void HAPAccessoryServerStop(
    HAPAccessoryServerRef *server);

/**
 * Enters the pairing mode.
 * In pairing mode, setup payload is presented on applicable displays and programmable NFC tags.
 * Pairing mode deactivates automatically after 5 minutes or when a pairing attempt is registered.
 *
 * This function only has an effect if:
 * - Dynamic setup code is supported by the platform.
 * - No pairing attempt is in progress.
 * - The accessory is not paired. To reset the pairing, perform a factory reset or remove the pairing.
 *
 * @param      server               Accessory server.
 */
void HAPAccessoryServerEnterPairingMode(
    HAPAccessoryServerRef *server);

/**
 * Enters the WAC mode.
 * In Wi-Fi Accessory Configuration (WAC) mode, the accessory accepts the configuration of Wi-Fi credentials. If the
 * accessory is already configured when entering WAC mode, the existing Wi-Fi configuration will be deleted first.
 * During WAC mode, the normal HAP accessory server functionality is not available. The accessory server will exit WAC
 * mode as soon as a valid Wi-Fi configuration transaction has been completed. The accessory server will then enter
 * normal HAP mode. The accessory server will also exit WAC mode in response to calling the function
 * HAPAccessoryServerExitWACMode or if no valid configuration requests have been received for more than 15 minutes. In
 * this case, the accessory server keeps running non configured and it is possible to enter WAC mode again with the
 * function HAPAccessoryServerEnterWACMode in response to a direct user action.
 *
 * Remark: The accessory server will enter WAC mode automatically when it is started in a non-configured state.
 *
 * This function only has an effect if:
 * - WAC is supported by the platform.
 * - The accessory server is not already in WAC mode, see HAPAccessoryServerIsInWACMode.
 *
 * @param      server               Accessory server.
 */
void HAPAccessoryServerEnterWACMode(
    HAPAccessoryServerRef *server);

/**
 * Exits the WAC mode.
 * It is possible to enter WAC mode again with the function HAPAccessoryServerEnterWACMode in response to a direct user
 * action.
 *
 * This function only has an effect if:
 * - WAC is supported by the platform.
 * - The accessory server is in WAC mode, see HAPAccessoryServerIsInWACMode.
 *
 * @param      server               Accessory server.
 */
void HAPAccessoryServerExitWACMode(
    HAPAccessoryServerRef *server);

/**
 * Raises an event notification for a given characteristic in a given service provided by a given accessory object.
 *
 * @param      server               Accessory server.
 * @param      characteristic       The characteristic whose value has changed.
 * @param      service              The service that contains the characteristic.
 * @param      accessory            The accessory that provides the service.
 */
void HAPAccessoryServerRaiseEvent(
    HAPAccessoryServerRef *server,
    const HAPCharacteristic *characteristic,
    const HAPService *service,
    const HAPAccessory *accessory);

/**
 * Raises an event notification for a given characteristic in a given service provided by a given accessory object
 * on a given session.
 *
 * @param      server               Accessory server.
 * @param      characteristic       The characteristic whose value has changed.
 * @param      service              The service that contains the characteristic.
 * @param      accessory            The accessory that provides the service.
 * @param      session              The session on which to raise the event.
 */
void HAPAccessoryServerRaiseEventOnSession(
    HAPAccessoryServerRef *server,
    const HAPCharacteristic *characteristic,
    const HAPService *service,
    const HAPAccessory *accessory,
    HAPSessionRef *session);

/**
 * Restores the given key-value store to factory settings.
 *
 * - The accessory server must be stopped completely before restoring factory settings!
 *
 * - Only HomeKit related data is reset. Application specific data must be restored manually.
 *
 * @param      keyValueStore        Key-value store.
 *
 * @return kHAPError_None           If successful.
 * @return kHAPError_Unknown        If persistent store access failed.
 */
HAP_RESULT_USE_CHECK
HAPError HAPRestoreFactorySettings(
    HAPPlatformKeyValueStoreRef keyValueStore);

/**
 * Purges all HomeKit pairings from the given key-value store.
 *
 * - The accessory server must be stopped completely before issuing a pairing reset!
 *
 * @param      keyValueStore        Key-value store.
 *
 * @return kHAPError_None           If successful.
 * @return kHAPError_Unknown        If persistent store access failed.
 */
HAP_RESULT_USE_CHECK
HAPError HAPRemoveAllPairings(
    HAPPlatformKeyValueStoreRef keyValueStore);

/**
 * Device ID of an accessory server.
 */
typedef struct {
    /**
     * Device ID.
     */
    uint8_t bytes[6];
} HAPAccessoryServerDeviceID;
HAP_STATIC_ASSERT(sizeof (HAPAccessoryServerDeviceID) == 6, HAPAccessoryServerDeviceID);

/**
 * Imports a device ID into an un-provisioned key-value store.
 * This is useful to import legacy settings from a different HomeKit SDK.
 *
 * - This function must no longer be called after the initial HAPAccessoryServerCreate call.
 *
 * @param      keyValueStore        Un-provisioned key-value store.
 * @param      deviceID             Device ID of the accessory server.
 *
 * @return kHAPError_None           If successful.
 * @return kHAPError_Unknown        If an error occurred while accessing the key-value store.
 */
HAP_RESULT_USE_CHECK
HAPError HAPLegacyImportDeviceID(
    HAPPlatformKeyValueStoreRef keyValueStore,
    const HAPAccessoryServerDeviceID *deviceID);

/**
 * Imports a configuration number into an un-provisioned key-value store.
 * This is useful to import legacy settings from a different HomeKit SDK.
 *
 * - This function must no longer be called after the initial HAPAccessoryServerCreate call.
 *
 * @param      keyValueStore        Un-provisioned key-value store.
 * @param      configurationNumber  Configuration number.
 *
 * @return kHAPError_None           If successful.
 * @return kHAPError_Unknown        If an error occurred while accessing the key-value store.
 */
HAP_RESULT_USE_CHECK
HAPError HAPLegacyImportConfigurationNumber(
    HAPPlatformKeyValueStoreRef keyValueStore,
    uint32_t configurationNumber);

/**
 * Ed25519 long-term secret key of an accessory server.
 */
typedef struct {
    /**
     * Ed25519 long-term secret key.
     */
    uint8_t bytes[32];
} HAPAccessoryServerLongTermSecretKey;
HAP_STATIC_ASSERT(sizeof (HAPAccessoryServerLongTermSecretKey) == 32, HAPAccessoryServerLongTermSecretKey);

/**
 * Imports a Ed25519 long-term secret key into an un-provisioned key-value store.
 * This is useful to import legacy settings from a different HomeKit SDK.
 *
 * - This function must no longer be called after the initial HAPAccessoryServerCreate call.
 *
 * @param      keyValueStore        Un-provisioned key-value store.
 * @param      longTermSecretKey    Long-term secret key of the accessory server.
 *
 * @return kHAPError_None           If successful.
 * @return kHAPError_Unknown        If an error occurred while accessing the key-value store.
 */
HAP_RESULT_USE_CHECK
HAPError HAPLegacyImportLongTermSecretKey(
    HAPPlatformKeyValueStoreRef keyValueStore,
    const HAPAccessoryServerLongTermSecretKey *longTermSecretKey);

/**
 * Imports an unsuccessful authentication attempts counter into an un-provisioned key-value store.
 * This is useful to import legacy settings from a different HomeKit SDK.
 *
 * - This function must no longer be called after the initial HAPAccessoryServerCreate call.
 *
 * @param      keyValueStore        Un-provisioned key-value store.
 * @param      numAuthAttempts      Unsuccessful authentication attempts counter.
 *
 * @return kHAPError_None           If successful.
 * @return kHAPError_Unknown        If an error occurred while accessing the key-value store.
 */
HAP_RESULT_USE_CHECK
HAPError HAPLegacyImportUnsuccessfulAuthenticationAttemptsCounter(
    HAPPlatformKeyValueStoreRef keyValueStore,
    uint8_t numAuthAttempts);

/**
 * Pairing identifier of a paired controller.
 */
typedef struct {
    /**
     * Buffer containing pairing identifier.
     */
    uint8_t bytes[36];
    
    /**
     * Number of used bytes in buffer.
     */
    size_t numBytes;
} HAPControllerPairingIdentifier;

/**
 * Public key of a paired controller.
 */
typedef struct {
    /**
     * Public key.
     */
    uint8_t bytes[32];
} HAPControllerPublicKey;
HAP_STATIC_ASSERT(sizeof (HAPControllerPublicKey) == 32, HAPControllerPublicKey);

/**
 * Imports a controller pairing into an un-provisioned key-value store.
 * This is useful to import legacy settings from a different HomeKit SDK.
 *
 * - This function must no longer be called after the initial HAPAccessoryServerCreate call.
 *
 * @param      keyValueStore        Un-provisioned key-value store.
 * @param      pairingIndex         Key-value store pairing index. 0 ..< Max number of pairings that will be supported.
 * @param      pairingIdentifier    HomeKit pairing identifier.
 * @param      publicKey            Ed25519 long-term public key of the paired controller.
 * @param      isAdmin              Whether or not the added controller has admin permissions.
 *
 * @return kHAPError_None           If successful.
 * @return kHAPError_Unknown        If an error occurred while accessing the key-value store.
 */
HAP_RESULT_USE_CHECK
HAPError HAPLegacyImportControllerPairing(
    HAPPlatformKeyValueStoreRef keyValueStore,
    HAPPlatformKeyValueStoreKey pairingIndex,
    const HAPControllerPairingIdentifier *pairingIdentifier,
    const HAPControllerPublicKey *publicKey,
    bool isAdmin);

#if __has_feature(nullability)
#pragma clang assume_nonnull end
#endif

#include "HAPCharacteristicTypes.h"
#include "HAPDataStreamProtocols.h"
#include "HAPRequestHandlers.h"
#include "HAPRTPController.h"
#include "HAPServiceTypes.h"

#ifdef __cplusplus
}
#endif

#endif
