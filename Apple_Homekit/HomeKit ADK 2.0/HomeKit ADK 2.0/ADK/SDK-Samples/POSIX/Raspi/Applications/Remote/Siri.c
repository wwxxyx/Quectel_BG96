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

// Implementation of the Siri profile.
//
// /!\ It is strongly recommended not to modify Remote.c or Siri.c, to simplify the integration of revised
// versions of these files.

#include "Siri.h"

#define kSiri_StreamingTimeout ((HAPTime)(5 * HAPSecond))

static const HAPSiriDelegate *delegate;

//----------------------------------------------------------------------------------------------------------------------

// Request handlers.

HAP_RESULT_USE_CHECK
HAPError HandleSiriInputTypeRead(
    HAPAccessoryServerRef *server_,
    const HAPUInt8CharacteristicReadRequest *request,
    uint8_t *value,
    void *_Nullable context HAP_UNUSED)
{
    HAPPrecondition(server_);
    HAPPrecondition(request);
    HAPPrecondition(value);
    
    // The only supported input type is push button triggered AppleTV.
    *value = (uint8_t)kHAPCharacteristicValue_SiriInputType_PushButtonTriggeredAppleTV;
    
    HAPLog(&kHAPLog_Default, "%s: %u", __func__, *value);
    
    return kHAPError_None;
}

//----------------------------------------------------------------------------------------------------------------------

/**
 * Appends a separator to the TLV writer payload.
 *
 * @param      responseWriter       Writer to append separator TLV to.
 *
 * @return kHAPError_None           If successful.
 * @return kHAPError_OutOfResources If writer does not have enough capacity.
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 16.1.1 TLV Rules
 */
HAP_RESULT_USE_CHECK
static HAPError AppendSeparator(
    HAPTLVWriterRef *responseWriter)
{
    HAPPrecondition(responseWriter);
    
    HAPError err;
    err = HAPTLVWriterAppend(responseWriter, &(const HAPTLV) {
        .type = 0xFF,
        .value = {
            .bytes = NULL,
            .numBytes = 0
        }
    });
    if (err) {
        HAPAssert(err == kHAPError_OutOfResources);
        return err;
    }
    return kHAPError_None;
}

/**
 * Serializes audio codec type.
 *
 * @param      audioCodecType       Value.
 *
 * @return Serialized value.
 */
HAP_RESULT_USE_CHECK
static HAPCharacteristicValue_AudioCodecType SerializeAudioCodecType(
    HAPAudioCodecType audioCodecType)
{
    switch (audioCodecType) {
        case kHAPAudioCodecType_PCMU: {
            return kHAPCharacteristicValue_AudioCodecType_PCMU;
        }
        case kHAPAudioCodecType_PCMA: {
            return kHAPCharacteristicValue_AudioCodecType_PCMA;
        }
        case kHAPAudioCodecType_AAC_ELD: {
            return kHAPCharacteristicValue_AudioCodecType_AAC_ELD;
        }
        case kHAPAudioCodecType_Opus: {
            return kHAPCharacteristicValue_AudioCodecType_Opus;
        }
        case kHAPAudioCodecType_MSBC: {
            return kHAPCharacteristicValue_AudioCodecType_MSBC;
        }
        case kHAPAudioCodecType_AMR: {
            return kHAPCharacteristicValue_AudioCodecType_AMR;
        }
        case kHAPAudioCodecType_AMR_WB: {
            return kHAPCharacteristicValue_AudioCodecType_AMR_WB;
        }
    }
    HAPFatalError();
}

/**
 * Serializes audio bit-rate.
 *
 * @param      audioBitRate         Value.
 *
 * @return Serialized value.
 */
HAP_RESULT_USE_CHECK
static HAPCharacteristicValue_AudioCodecBitRateControlMode SerializeAudioBitRate(
    HAPAudioCodecBitRateControlMode audioBitRate)
{
    switch (audioBitRate) {
        case kHAPAudioCodecBitRateControlMode_Variable: {
            return kHAPCharacteristicValue_AudioCodecBitRateControlMode_Variable;
        }
        case kHAPAudioCodecBitRateControlMode_Constant: {
            return kHAPCharacteristicValue_AudioCodecBitRateControlMode_Constant;
        }
    }
    HAPFatalError();
}

/**
 * Serializes audio sample rate.
 *
 * @param      audioSampleRate      Value.
 *
 * @return Serialized value.
 */
HAP_RESULT_USE_CHECK
static HAPCharacteristicValue_AudioCodecSampleRate SerializeAudioSampleRate(
    HAPAudioCodecSampleRate audioSampleRate)
{
    switch (audioSampleRate) {
        case kHAPAudioCodecSampleRate_8KHz: {
            return kHAPCharacteristicValue_AudioCodecSampleRate_8KHz;
        }
        case kHAPAudioCodecSampleRate_16KHz: {
            return kHAPCharacteristicValue_AudioCodecSampleRate_16KHz;
        }
        case kHAPAudioCodecSampleRate_24KHz: {
            return kHAPCharacteristicValue_AudioCodecSampleRate_24KHz;
        }
    }
    HAPFatalError();
}

/**
 * Serializes a supported audio stream configuration.
 *
 * @param      configuration        Audio stream configuration.
 * @param      responseWriter       TLV writer for serializing the response.
 *
 * @return kHAPError_None           If successful.
 * @return kHAPError_OutOfResources If out of resources to process request.
 */
static HAPError SerializeSupportedAudioCodecConfiguration(
    const HAPCameraSupportedAudioCodecConfiguration *configuration,
    HAPTLVWriterRef *responseWriter)
{
    HAPPrecondition(configuration);
    HAPPrecondition(responseWriter);
    
    HAPError err;
    
    // Audio Codec Type.
    err = HAPTLVWriterAppend(responseWriter, &(const HAPTLV) {
        .type = kHAPCharacteristicValue_SupportedAudioStreamConfiguration_AudioCodecConfiguration_CodecType,
        .value = {
            .bytes = (const uint8_t[]) {
                SerializeAudioCodecType(configuration->codecType)
            },
            .numBytes = 1
        }
    });
    if (err) {
        HAPAssert(err == kHAPError_OutOfResources);
        return err;
    }
    
    // Audio Codec Parameters.
    {
        HAPTLVWriterRef subWriter;
        {
            void *bytes;
            size_t maxBytes;
            HAPTLVWriterGetScratchBytes(responseWriter, &bytes, &maxBytes);
            HAPTLVWriterCreate(&subWriter, bytes, maxBytes);
        }
        
        const HAPAudioCodecParameters *parameters = configuration->codecParameters;
        
        // Audio channels.
        err = HAPTLVWriterAppend(&subWriter, &(const HAPTLV) {
            .type = kHAPCharacteristicValue_AudioCodecParameter_AudioChannels,
            .value = {
                .bytes = (const uint8_t[]) {
                    parameters->numberOfChannels
                },
                .numBytes = 1
            }
        });
        if (err) {
            HAPAssert(err == kHAPError_OutOfResources);
            return err;
        }
        
        // Bit-rate.
        bool appended = false;
        HAPAudioCodecBitRateControlMode bitRateMode = parameters->bitRateMode;
        for (size_t j = 0; bitRateMode; j++) {
            HAPAudioCodecBitRateControlMode rate = (HAPAudioCodecBitRateControlMode) (1 << j);
            
            if (bitRateMode & rate) {
                bitRateMode &= ~rate;
                
                // Append separator if necessary.
                if (appended) {
                    err = AppendSeparator(&subWriter);
                    if (err) {
                        HAPAssert(err == kHAPError_OutOfResources);
                        return err;
                    }
                } else {
                    appended = true;
                }
                
                // Serialize.
                err = HAPTLVWriterAppend(&subWriter, &(const HAPTLV) {
                    .type = kHAPCharacteristicValue_AudioCodecParameter_BitRate,
                    .value = {
                        .bytes = (const uint8_t[]) {
                            SerializeAudioBitRate(rate)
                        },
                        .numBytes = 1
                    }
                });
                if (err) {
                    HAPAssert(err == kHAPError_OutOfResources);
                    return err;
                }
            }
        }
        HAPPrecondition(appended);
        
        // Sample rate.
        appended = false;
        HAPAudioCodecSampleRate sampleRate = parameters->sampleRate;
        for (size_t j = 0; sampleRate; j++) {
            HAPAudioCodecSampleRate rate = (HAPAudioCodecSampleRate) (1 << j);
            
            if (sampleRate & rate) {
                sampleRate &= ~rate;
                
                // Append separator if necessary.
                if (appended) {
                    err = AppendSeparator(&subWriter);
                    if (err) {
                        HAPAssert(err == kHAPError_OutOfResources);
                        return err;
                    }
                } else {
                    appended = true;
                }
                
                // Serialize.
                err = HAPTLVWriterAppend(&subWriter, &(const HAPTLV) {
                    .type = kHAPCharacteristicValue_AudioCodecParameter_SampleRate,
                    .value = {
                        .bytes = (const uint8_t[]) {
                            SerializeAudioSampleRate(rate)
                        },
                        .numBytes = 1
                    }
                });
                if (err) {
                    HAPAssert(err == kHAPError_OutOfResources);
                    return err;
                }
            }
        }
        HAPPrecondition(appended);
        
        // Finalize.
        void *bytes;
        size_t numBytes;
        HAPTLVWriterGetBuffer(&subWriter, &bytes, &numBytes);
        err = HAPTLVWriterAppend(responseWriter, &(const HAPTLV) {
            .type = kHAPCharacteristicValue_SupportedAudioStreamConfiguration_AudioCodecConfiguration_CodecParameters,
            .value = {
                .bytes = bytes,
                .numBytes = numBytes
            }
        });
        if (err) {
            HAPAssert(err == kHAPError_OutOfResources);
            return err;
        }
    }
    
    return kHAPError_None;
}

HAP_RESULT_USE_CHECK
HAPError HandleAudioStreamManagementSupportedAudioStreamConfigurationRead(
    HAPAccessoryServerRef *server,
    const HAPTLV8CharacteristicReadRequest *request,
    HAPTLVWriterRef *responseWriter,
    void *_Nullable context HAP_UNUSED)
{
    HAPPrecondition(server);
    HAPPrecondition(request);
    HAPPrecondition(HAPUUIDAreEqual(request->characteristic->characteristicType,
        &kHAPCharacteristicType_SupportedAudioStreamConfiguration));
    HAPPrecondition(HAPUUIDAreEqual(request->service->serviceType,
        &kHAPServiceType_AudioStreamManagement));
    HAPPrecondition(responseWriter);
    
    HAPError err;
    
    HAPLog(&kHAPLog_Default, "%s", __func__);
    
    // We have to use Opus, VBR, 16kHz.
    // See HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
    // Section 13.1.2 Binary Data
    HAPAudioCodecParameters parameters = {
        .type = kHAPAudioCodecType_Opus,
        .numberOfChannels = 1,
        .bitRateMode = kHAPAudioCodecBitRateControlMode_Variable,
        .sampleRate = kHAPAudioCodecSampleRate_16KHz
    };
    HAPCameraSupportedAudioCodecConfiguration configuration = {
        .codecType = kHAPAudioCodecType_Opus,
        .codecParameters = &parameters
    };
    
    // Audio Codec Configuration.
    {
        HAPTLVWriterRef subWriter;
        {
            void *bytes;
            size_t maxBytes;
            HAPTLVWriterGetScratchBytes(responseWriter, &bytes, &maxBytes);
            HAPTLVWriterCreate(&subWriter, bytes, maxBytes);
        }
        
        err = SerializeSupportedAudioCodecConfiguration(&configuration, &subWriter);
        if (err) {
            HAPAssert(err == kHAPError_OutOfResources);
            return err;
        }
        
        void *bytes;
        size_t numBytes;
        HAPTLVWriterGetBuffer(&subWriter, &bytes, &numBytes);
        err = HAPTLVWriterAppend(responseWriter, &(const HAPTLV) {
            .type = kHAPCharacteristicValue_SupportedAudioStreamConfiguration_AudioCodecConfiguration,
            .value = {
                .bytes = bytes,
                .numBytes = numBytes
            }
        });
        if (err) {
            HAPAssert(err == kHAPError_OutOfResources);
            return err;
        }
    }
    
    // Comfort Noise.
    err = HAPTLVWriterAppend(responseWriter, &(const HAPTLV) {
        .type = kHAPCharacteristicValue_SupportedAudioStreamConfiguration_ComfortNoiseSupport,
        .value = {
            .bytes = (const uint8_t[]) {
                (uint8_t) 0
            },
            .numBytes = 1
        }
    });
    if (err) {
        HAPAssert(err == kHAPError_OutOfResources);
        return err;
    }
    
    return kHAPError_None;
}

HAP_RESULT_USE_CHECK
HAPError HandleAudioStreamManagementSelectedAudioStreamConfigurationRead(
    HAPAccessoryServerRef *server,
    const HAPTLV8CharacteristicReadRequest *request,
    HAPTLVWriterRef *responseWriter,
    void *_Nullable context HAP_UNUSED)
{
    HAPPrecondition(server);
    HAPPrecondition(request);
    HAPPrecondition(HAPUUIDAreEqual(request->characteristic->characteristicType,
        &kHAPCharacteristicType_SelectedAudioStreamConfiguration));
    HAPPrecondition(HAPUUIDAreEqual(request->service->serviceType,
        &kHAPServiceType_AudioStreamManagement));
    HAPPrecondition(responseWriter);
    
    HAPError err;
    
    HAPLog(&kHAPLog_Default, "%s", __func__);
    
    // Configuration is Opus, VBR, 16kHz.
    HAPAudioCodecParameters parameters = {
        .type = kHAPAudioCodecType_Opus,
        .numberOfChannels = 1,
        .bitRateMode = kHAPAudioCodecBitRateControlMode_Variable,
        .sampleRate = kHAPAudioCodecSampleRate_16KHz
    };
    HAPCameraSupportedAudioCodecConfiguration configuration = {
        .codecType = kHAPAudioCodecType_Opus,
        .codecParameters = &parameters
    };
    
    // Audio Codec Configuration.
    {
        HAPTLVWriterRef subWriter;
        {
            void *bytes;
            size_t maxBytes;
            HAPTLVWriterGetScratchBytes(responseWriter, &bytes, &maxBytes);
            HAPTLVWriterCreate(&subWriter, bytes, maxBytes);
        }
        
        err = SerializeSupportedAudioCodecConfiguration(&configuration, &subWriter);
        if (err) {
            HAPAssert(err == kHAPError_OutOfResources);
            return err;
        }
        
        void *bytes;
        size_t numBytes;
        HAPTLVWriterGetBuffer(&subWriter, &bytes, &numBytes);
        err = HAPTLVWriterAppend(responseWriter, &(const HAPTLV) {
            .type = kHAPCharacteristicValue_SelectedAudioStreamConfiguration_SelectedAudioInputStreamConfiguration,
            .value = {
                .bytes = bytes,
                .numBytes = numBytes
            }
        });
        if (err) {
            HAPAssert(err == kHAPError_OutOfResources);
            return err;
        }
    }
    
    return kHAPError_None;
}

/**
 * Parses audio codec type.
 *
 * @param[out] audioCodecType       Parsed value.
 * @param      rawValue             Raw value.
 *
 * @return kHAPError_None           If successful.
 * @return kHAPError_InvalidData    If the value is invalid.
 */
HAP_RESULT_USE_CHECK
static HAPError TryParseAudioCodecType(
    HAPAudioCodecType *audioCodecType,
    uint8_t rawValue)
{
    HAPPrecondition(audioCodecType);
    
    switch (rawValue) {
        case kHAPCharacteristicValue_AudioCodecType_PCMU: {
            *audioCodecType = kHAPAudioCodecType_PCMU;
        } return kHAPError_None;
        case kHAPCharacteristicValue_AudioCodecType_PCMA: {
            *audioCodecType = kHAPAudioCodecType_PCMA;
        } return kHAPError_None;
        case kHAPCharacteristicValue_AudioCodecType_AAC_ELD: {
            *audioCodecType = kHAPAudioCodecType_AAC_ELD;
        } return kHAPError_None;
        case kHAPCharacteristicValue_AudioCodecType_Opus: {
            *audioCodecType = kHAPAudioCodecType_Opus;
        } return kHAPError_None;
        case kHAPCharacteristicValue_AudioCodecType_MSBC: {
            *audioCodecType = kHAPAudioCodecType_MSBC;
        } return kHAPError_None;
        case kHAPCharacteristicValue_AudioCodecType_AMR: {
            *audioCodecType = kHAPAudioCodecType_AMR;
        } return kHAPError_None;
        case kHAPCharacteristicValue_AudioCodecType_AMR_WB: {
            *audioCodecType = kHAPAudioCodecType_AMR_WB;
        } return kHAPError_None;
        default: {
            HAPLogError(&kHAPLog_Default, "Audio codec type invalid: %u.", rawValue);
        } return kHAPError_InvalidData;
    }
}

/**
 * Parses audio bit-rate.
 *
 * @param[out] audioBitRate         Parsed value.
 * @param      rawValue             Raw value.
 *
 * @return kHAPError_None           If successful.
 * @return kHAPError_InvalidData    If the value is invalid.
 */
HAP_RESULT_USE_CHECK
static HAPError TryParseAudioBitRate(
    HAPAudioCodecBitRateControlMode *audioBitRate,
    uint8_t rawValue)
{
    HAPPrecondition(audioBitRate);
    
    switch (rawValue) {
        case kHAPCharacteristicValue_AudioCodecBitRateControlMode_Variable: {
            *audioBitRate = kHAPAudioCodecBitRateControlMode_Variable;
        } return kHAPError_None;
        case kHAPCharacteristicValue_AudioCodecBitRateControlMode_Constant: {
            *audioBitRate = kHAPAudioCodecBitRateControlMode_Constant;
        } return kHAPError_None;
        default: {
            HAPLogError(&kHAPLog_Default, "Audio bit-rate invalid: %u.", rawValue);
        } return kHAPError_InvalidData;
    }
}

/**
 * Parses audio sample rate.
 *
 * @param[out] audioSampleRate      Parsed value.
 * @param      rawValue             Raw value.
 *
 * @return kHAPError_None           If successful.
 * @return kHAPError_InvalidData    If the value is invalid.
 */
HAP_RESULT_USE_CHECK
static HAPError TryParseAudioSampleRate(
    HAPAudioCodecSampleRate *audioSampleRate,
    uint8_t rawValue)
{
    HAPPrecondition(audioSampleRate);
    
    switch (rawValue) {
        case kHAPCharacteristicValue_AudioCodecSampleRate_8KHz: {
            *audioSampleRate = kHAPAudioCodecSampleRate_8KHz;
        } return kHAPError_None;
        case kHAPCharacteristicValue_AudioCodecSampleRate_16KHz: {
            *audioSampleRate = kHAPAudioCodecSampleRate_16KHz;
        } return kHAPError_None;
        case kHAPCharacteristicValue_AudioCodecSampleRate_24KHz: {
            *audioSampleRate = kHAPAudioCodecSampleRate_24KHz;
        } return kHAPError_None;
        default: {
            HAPLogError(&kHAPLog_Default, "Audio sample rate invalid: %u.", rawValue);
        } return kHAPError_InvalidData;
    }
}

/**
 * Parses selected audio parameters of a Selected Audio Stream Configuration command.
 *
 * @param[out] codecParameters      Parsed codec parameters.
 * @param      requestReader        TLV reader.
 *
 * @return kHAPError_None           If successful.
 * @return kHAPError_InvalidData    If the request reader contains invalid data.
 */
HAP_RESULT_USE_CHECK
static HAPError TryParseSelectedAudioStreamConfiguration(
    HAPAudioCodecParameters *codecParameters,
    HAPTLVReaderRef *requestReader)
{
    HAPPrecondition(codecParameters);
    HAPPrecondition(requestReader);
    
    HAPError err;
    
    HAPRawBufferZero(codecParameters, sizeof *codecParameters);
    
    HAPTLV codecTypeTLV =
        { .type = kHAPCharacteristicValue_SelectedAudioStreamConfiguration_Configuration_SelectedAudioCodecType };
    HAPTLV codecParametersTLV =
        { .type = kHAPCharacteristicValue_SelectedAudioStreamConfiguration_Configuration_SelectedAudioCodecParameters };
    err = HAPTLVReaderGetAll(requestReader, (HAPTLV *const[]) {
        &codecTypeTLV, &codecParametersTLV, NULL
    });
    if (err) {
        HAPAssert(err == kHAPError_InvalidData);
        return err;
    }
    
    // Selected Audio Codec type.
    if (!codecTypeTLV.value.bytes) {
        HAPLogError(&kHAPLog_Default, "Selected Audio Codec type missing.");
        return kHAPError_InvalidData;
    }
    if (codecTypeTLV.value.numBytes != 1) {
        HAPLogError(&kHAPLog_Default, "Selected Audio Codec type has invalid length (%lu).",
            (unsigned long) codecTypeTLV.value.numBytes);
        return kHAPError_InvalidData;
    }
    err = TryParseAudioCodecType(&codecParameters->type, ((const uint8_t *) codecTypeTLV.value.bytes)[0]);
    if (err) {
        HAPAssert(err == kHAPError_InvalidData);
        return err;
    }
    
    // Selected Audio Codec parameters.
    if (!codecParametersTLV.value.bytes) {
        HAPLogError(&kHAPLog_Default, "Selected Audio Codec parameters missing.");
        return kHAPError_InvalidData;
    }
    {
        HAPTLVReaderRef subReader;
        HAPTLVReaderCreate(&subReader,
            (void *) (uintptr_t) codecParametersTLV.value.bytes,
            codecParametersTLV.value.numBytes);
        
        HAPTLV audioChannelsTLV = { .type = kHAPCharacteristicValue_AudioCodecParameter_AudioChannels };
        HAPTLV bitRateTLV = { .type = kHAPCharacteristicValue_AudioCodecParameter_BitRate };
        HAPTLV sampleRateTLV = { .type = kHAPCharacteristicValue_AudioCodecParameter_SampleRate };
        err = HAPTLVReaderGetAll(&subReader, (HAPTLV *const[]) {
            &audioChannelsTLV, &bitRateTLV, &sampleRateTLV, NULL
        });
        if (err) {
            HAPAssert(err == kHAPError_InvalidData);
            return err;
        }
        
        // Audio channels.
        if (!audioChannelsTLV.value.bytes) {
            HAPLogError(&kHAPLog_Default, "Audio channels missing.");
            return kHAPError_InvalidData;
        }
        if (audioChannelsTLV.value.numBytes != 1) {
            HAPLogError(&kHAPLog_Default, "Audio channels has invalid length (%lu).",
                (unsigned long) audioChannelsTLV.value.numBytes);
            return kHAPError_InvalidData;
        }
        codecParameters->numberOfChannels = ((const uint8_t *) audioChannelsTLV.value.bytes)[0];
        
        // Bit rate.
        if (!bitRateTLV.value.bytes) {
            HAPLogError(&kHAPLog_Default, "Audio bit rate missing.");
            return kHAPError_InvalidData;
        }
        if (bitRateTLV.value.numBytes != 1) {
            HAPLogError(&kHAPLog_Default, "Audio bit rate has invalid length (%lu).",
                (unsigned long) bitRateTLV.value.numBytes);
            return kHAPError_InvalidData;
        }
        err = TryParseAudioBitRate(&codecParameters->bitRateMode, ((const uint8_t *) bitRateTLV.value.bytes)[0]);
        if (err) {
            HAPAssert(err == kHAPError_InvalidData);
            return err;
        }
        
        // Sample rate.
        if (!sampleRateTLV.value.bytes) {
            HAPLogError(&kHAPLog_Default, "Audio sample rate missing.");
            return kHAPError_InvalidData;
        }
        if (sampleRateTLV.value.numBytes != 1) {
            HAPLogError(&kHAPLog_Default, "Audio sample rate has invalid length (%lu).",
                (unsigned long) sampleRateTLV.value.numBytes);
            return kHAPError_InvalidData;
        }
        err = TryParseAudioSampleRate(&codecParameters->sampleRate, ((const uint8_t *) sampleRateTLV.value.bytes)[0]);
        if (err) {
            HAPAssert(err == kHAPError_InvalidData);
            return err;
        }
    }
    
    return kHAPError_None;
}

HAP_RESULT_USE_CHECK
HAPError HandleAudioStreamManagementSelectedAudioStreamConfigurationWrite(
    HAPAccessoryServerRef *server_,
    const HAPTLV8CharacteristicWriteRequest *request,
    HAPTLVReaderRef *requestReader,
    void *_Nullable context HAP_UNUSED)
{
    HAPPrecondition(server_);
    HAPPrecondition(request);
    HAPPrecondition(request->session);
    HAPPrecondition(HAPUUIDAreEqual(request->characteristic->characteristicType,
        &kHAPCharacteristicType_SelectedAudioStreamConfiguration));
    HAPPrecondition(HAPUUIDAreEqual(request->service->serviceType,
        &kHAPServiceType_AudioStreamManagement));
    HAPPrecondition(requestReader);
    
    HAPError err;
    
    HAPLog(&kHAPLog_Default, "%s", __func__);
    
    HAPTLV selectedAudioTLV =
        { .type = kHAPCharacteristicValue_SelectedAudioStreamConfiguration_SelectedAudioInputStreamConfiguration };
    err = HAPTLVReaderGetAll(requestReader, (HAPTLV *const[]) {
        &selectedAudioTLV, NULL
    });
    if (err) {
        HAPAssert(err == kHAPError_InvalidData);
        return err;
    }
    
    // Selected Audio Input Stream Configuration.
    if (!selectedAudioTLV.value.bytes) {
        HAPLogError(&kHAPLog_Default, "Selected Audio Input Stream Configuration missing.");
        return kHAPError_InvalidData;
    }
    HAPAudioCodecParameters codecParameters;
    {
        HAPTLVReaderRef subReader;
        HAPTLVReaderCreate(&subReader,
            (void *) (uintptr_t) selectedAudioTLV.value.bytes,
            selectedAudioTLV.value.numBytes);
        
        err = TryParseSelectedAudioStreamConfiguration(&codecParameters, &subReader);
        if (err) {
            HAPAssert(err == kHAPError_InvalidData);
            return err;
        }
    }
    
    // Check selected codec configuration
    if (codecParameters.type != kHAPAudioCodecType_Opus ||
        codecParameters.numberOfChannels != 1 ||
        codecParameters.bitRateMode != kHAPAudioCodecBitRateControlMode_Variable ||
        codecParameters.sampleRate != kHAPAudioCodecSampleRate_16KHz) {
        HAPLogError(&kHAPLog_Default, "Invalid Selected Audio Input Stream Configuration.");
        return kHAPError_InvalidData;
    }
    
    return kHAPError_None;
}

//----------------------------------------------------------------------------------------------------------------------

// Siri Audio handling.

static const HAPDataSendDataStreamProtocolStreamCallbacks dataSendStreamCallbacks;

#define kOpusBitrate   (24000) // [bit/s]
#define kPacketTime    (20)    // [ms]
#define kMaxNumPackets (16)    // max number of audio packets sent in one data stream packet

static void HandleSendDataComplete(
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

static void SendNextSiriPacket(
    HAPSiriAudioContext *audioContext,
    HAPSiriDataStreamContext *dataStreamContext)
{
    HAPPrecondition(audioContext);
    HAPPrecondition(dataStreamContext);
    
    if (audioContext->endOfStream) {
        // All packets sent.
        return;
    }
    
    HAPDataSendDataStreamProtocolPacket packets[kMaxNumPackets];
    HAPRawBufferZero(packets, sizeof(packets));
    
    size_t packetIndex = 0;
    size_t head = audioContext->head;
    size_t tail = audioContext->tail;
    
    while (head != tail && packetIndex < kMaxNumPackets) {
        // Get next buffer from queue.
        packets[packetIndex].data.bytes = audioContext->circularBuffer[tail].bytes;
        packets[packetIndex].data.numBytes = audioContext->circularBuffer[tail].numBytes;
        packets[packetIndex].metadata.type = kHAPDataSendDataStreamProtocolType_Audio_Siri;
        packets[packetIndex].metadata._.audio.siri.rms = audioContext->circularBuffer[tail].rms;
        packets[packetIndex].metadata._.audio.siri.sequenceNumber = audioContext->sequenceNumber;
        audioContext->sequenceNumber++;
        tail++;
        if (tail == HAPArrayCount(audioContext->circularBuffer)) {
            tail = 0;
        }
        packetIndex++;
    }
    
    if (head == tail && !audioContext->isSampling) {
        // Last packet.
        audioContext->endOfStream = true;
    }
    
    if (packetIndex || audioContext->endOfStream) {
        dataStreamContext->busy = true;
        HAPError err;
        err = HAPDataSendDataStreamProtocolSendData(
            dataStreamContext->server,
            dataStreamContext->dispatcher,
            dataStreamContext->dataStream,
            &dataStreamContext->dataSendStream,
            audioContext->scratchBytes, sizeof audioContext->scratchBytes,
            packets, packetIndex,
            audioContext->endOfStream,
            HandleSendDataComplete);
        if (err) {
            dataStreamContext->busy = false;
            HAPLogError(&kHAPLog_Default, "Siri audio data send failed");
        }
    }
    
    // Update queue and release data.
    audioContext->tail = tail;
}

static void SiriAudioContinuation(
    void *_Nullable context,
    size_t contextSize)
{
    HAPPrecondition(context);
    HAPPrecondition(contextSize == sizeof(HAPSiriAudioContext*));
    
    HAPSiriAudioContext *audioContext = * (HAPSiriAudioContext**) context;
    HAPSiriDataStreamContext *dataStreamContext = audioContext->activeStream;
    
    if (dataStreamContext && !dataStreamContext->busy) {
        // Send available data.
        SendNextSiriPacket(audioContext, dataStreamContext);
    }
}

static void SiriAudioCallback(
    void *_Nullable context,
    HAPPlatformMicrophoneRef microphone,
    HAPPlatformMicrophoneStreamRef microphoneStream,
    void *bytes,
    size_t numBytes,
    HAPTimeNS sampleTime HAP_UNUSED,
    float rms)
{
    HAPPrecondition(context);
    HAPSiriAudioContext *audioContext = (HAPSiriAudioContext*) context;
    HAPPrecondition(microphone);
    HAPPrecondition(microphoneStream);
    HAPPrecondition(bytes);
    
    if (!audioContext->isSampling) {
        // Outdated data;
        return;
    }
    
    // Store data.
    if (numBytes > sizeof audioContext->circularBuffer[0].bytes) {
        // Too much data, ignore.
        HAPLogError(&kHAPLog_Default, "Siri audio data buffer overflow");
        return;
    }
    size_t head = audioContext->head + 1;
    if (head == HAPArrayCount(audioContext->circularBuffer)) {
        head = 0;
    }
    if (head == audioContext->tail) {
        // Overflow, ignore data.
        HAPLogError(&kHAPLog_Default, "Siri audio data queue overflow");
        return;
    }
    HAPRawBufferCopyBytes(audioContext->circularBuffer[audioContext->head].bytes, bytes, numBytes);
    audioContext->circularBuffer[audioContext->head].numBytes = numBytes;
    audioContext->circularBuffer[audioContext->head].rms = rms;
    audioContext->head = head;
    
    // Synchronize execution to main thread.
    __sync_synchronize();
    HAPError err;
    err = HAPPlatformRunLoopScheduleCallback(SiriAudioContinuation, &audioContext, sizeof audioContext);
    if (err) {
        HAPLogError(&kHAPLog_Default, "Siri audio data synchronization failed");
    }
}

static const HAPDataSendDataStreamProtocolStreamCallbacks dataSendStreamCallbacks;

static void SiriAudioStart(
    HAPSiriAudioContext *audioContext,
    HAPSiriDataStreamContext *dataStreamContext)
{
    HAPPrecondition(audioContext);
    HAPPrecondition(dataStreamContext);
    
    dataStreamContext->busy = true;
    HAPDataSendDataStreamProtocolOpen(
        dataStreamContext->server,
        dataStreamContext->dispatcher,
        dataStreamContext->dataStream,
        &dataStreamContext->dataSendStream,
        kHAPDataSendDataStreamProtocolType_Audio_Siri,
        &dataSendStreamCallbacks);
    
    HAPError err;
    
    // Start audio.
    err = HAPPlatformMicrophoneStart(
        audioContext->microphone,
        &audioContext->microphoneStream,
        audioContext->codecParameters.sampleRate,
        audioContext->codecParameters.bitRateMode,
        kOpusBitrate,
        kPacketTime,
        SiriAudioCallback,
        audioContext);
    if (err) {
        HAPAssert(err == kHAPError_OutOfResources);
        HAPFatalError();
    }
    HAPPlatformMicrophoneSetVolume(audioContext->microphone, HAPNonnull(audioContext->microphoneStream), 100);
    HAPPlatformMicrophoneSetMuted(audioContext->microphone, HAPNonnull(audioContext->microphoneStream), false);
    
    audioContext->endOfStream = false;
    audioContext->newStream = NULL;
    audioContext->isSampling = true;
    audioContext->activeStream = dataStreamContext;
}

static void SiriAudioStop(
    HAPSiriAudioContext *audioContext)
{
    HAPPrecondition(audioContext);
    
    if (audioContext->isSampling) {
        HAPPlatformMicrophoneStop(audioContext->microphone, HAPNonnull(audioContext->microphoneStream));
        audioContext->microphoneStream = NULL;
        audioContext->isSampling = false;
    }
}

void SiriInitializeAudioConfiguration(
    HAPSiriAudioContext *audioContext,
    HAPPlatformMicrophoneRef microphone)
{
    HAPPrecondition(audioContext);
    HAPPrecondition(microphone);
    
    HAPRawBufferZero(audioContext, sizeof *audioContext);
    audioContext->microphone = microphone;
    audioContext->codecParameters.type = kHAPAudioCodecType_Opus;
    audioContext->codecParameters.numberOfChannels = 1;
    audioContext->codecParameters.bitRateMode = kHAPAudioCodecBitRateControlMode_Variable;
    audioContext->codecParameters.sampleRate = kHAPAudioCodecSampleRate_16KHz;
}

static void HandleStreamingTimeout(
    HAPPlatformTimerRef timer,
    void *_Nullable context)
{
    HAPPrecondition(context);
    HAPSiriDataStreamContext *dataStreamContext = (HAPSiriDataStreamContext*) context;
    HAPPrecondition(timer == dataStreamContext->timeoutTimer);
    
    dataStreamContext->timeoutTimer = 0;
    
    // Cancel data stream.
    HAPDataSendDataStreamProtocolCancel(
        dataStreamContext->server,
        dataStreamContext->dispatcher,
        dataStreamContext->dataStream,
        &dataStreamContext->dataSendStream);
}

void SiriInputStart(
    HAPSiriAudioContext *audioContext,
    HAPSiriDataStreamContext *dataStreamContext)
{
    HAPPrecondition(audioContext);
    HAPPrecondition(dataStreamContext);
    
    HAPLogInfo(&kHAPLog_Default, "%s", __func__);
    
    HAPAssert(!audioContext->isSampling);
    
    if (audioContext->activeStream) {
        // Data stream is still active, delay start of new stream.
        audioContext->newStream = dataStreamContext;
        HAPLogInfo(&kHAPLog_Default, "Audio stream start delayed");
        return;
    }
    
    SiriAudioStart(audioContext, dataStreamContext);
}

void SiriInputStop(
    HAPSiriAudioContext *audioContext,
    HAPSiriDataStreamContext *dataStreamContext)
{
    HAPPrecondition(audioContext);
    HAPPrecondition(dataStreamContext);
    
    HAPLogInfo(&kHAPLog_Default, "%s", __func__);
    
    if (audioContext->newStream) {
        // Session not yet started.
        audioContext->newStream = NULL;
        return;
    }
    
    if (audioContext->isSampling) {
        // Stop audio.
        SiriAudioStop(audioContext);
        
        if (!dataStreamContext->busy) {
            // Send remaining data.
            SendNextSiriPacket(audioContext, dataStreamContext);
        }
        
        // Start streaming timeout.
        HAPError err;
        err = HAPPlatformTimerRegister(
            &dataStreamContext->timeoutTimer,
            HAPPlatformClockGetCurrent() + kSiri_StreamingTimeout,
            HandleStreamingTimeout,
            dataStreamContext);
        if (err) {
            HAPAssert(err == kHAPError_OutOfResources);
            HAPLog(&kHAPLog_Default, "Not enough resources to start Siri timeout timer");
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

void SiriHandleDataSendAccept(
    HAPAccessoryServerRef *server,
    HAPDataStreamDispatcherRef *dispatcher,
    HAPDataSendDataStreamProtocol *dataStreamProtocol,
    const HAPServiceRequest *request,
    HAPDataStreamHandle dataStream,
    void *_Nullable context HAP_UNUSED)
{
    HAPPrecondition(server);
    HAPPrecondition(dispatcher);
    HAPPrecondition(dataStreamProtocol);
    HAPPrecondition(request);
    HAPPrecondition(dataStream < dataStreamProtocol->storage.numDataStreams);
    
    HAPLogInfo(&kHAPLog_Default, "%s", __func__);
    delegate->handleSiriAccept(server, request);
    
    // Setup siri data stream context.
    HAPSiriDataStreamContext *dataStreamContext;
    delegate->getSiriContexts(server, request, NULL, &dataStreamContext);
    HAPAssert(dataStreamContext);
    
    HAPRawBufferZero(dataStreamContext, sizeof *dataStreamContext);
    dataStreamContext->server = server;
    dataStreamContext->dispatcher = dispatcher;
    dataStreamContext->dataStream = dataStream;
}

void SiriHandleDataSendInvalidate(
    HAPAccessoryServerRef *server,
    HAPDataStreamDispatcherRef *dispatcher,
    HAPDataSendDataStreamProtocol *dataStreamProtocol,
    const HAPServiceRequest *request,
    HAPDataStreamHandle dataStream,
    void *_Nullable context HAP_UNUSED)
{
    HAPPrecondition(server);
    HAPPrecondition(dispatcher);
    HAPPrecondition(dataStreamProtocol);
    HAPPrecondition(request);
    HAPPrecondition(dataStream < dataStreamProtocol->storage.numDataStreams);
    
    HAPLogInfo(&kHAPLog_Default, "%s", __func__);
    
    HAPSiriAudioContext *audioContext;
    HAPSiriDataStreamContext *dataStreamContext;
    delegate->getSiriContexts(server, request, &audioContext, &dataStreamContext);
    HAPAssert(audioContext);
    HAPAssert(dataStreamContext);
    
    dataStreamContext->busy = false;
    
    // Abort audio.
    SiriAudioStop(audioContext);
    audioContext->activeStream = NULL;
    
    if (dataStreamContext->timeoutTimer) {
        HAPPlatformTimerDeregister(dataStreamContext->timeoutTimer);
        dataStreamContext->timeoutTimer = 0;
    }
    
    delegate->handleSiriInvalidate(server, request);
}

//----------------------------------------------------------------------------------------------------------------------

static void HandleDataSendStreamClose(
    HAPAccessoryServerRef *server,
    HAPDataStreamDispatcherRef *dispatcher,
    HAPDataSendDataStreamProtocol *dataStreamProtocol,
    const HAPDataStreamRequest *request,
    HAPDataStreamHandle dataStream,
    HAPDataSendDataStreamProtocolStreamRef *dataSendStream,
    HAPError error,
    HAPDataSendDataStreamProtocolCloseReason closeReason,
    void *_Nullable context HAP_UNUSED)
{
    HAPPrecondition(server);
    HAPPrecondition(dispatcher);
    HAPPrecondition(dataStreamProtocol);
    HAPPrecondition(request);
    HAPPrecondition(dataStream < dataStreamProtocol->storage.numDataStreams);
    HAPPrecondition(dataSendStream);
    
    HAPLogInfo(&kHAPLog_Default, "%s (error %u / reason %u)", __func__, error, closeReason);
    
    HAPSiriAudioContext *audioContext;
    HAPSiriDataStreamContext *dataStreamContext;
    delegate->getSiriContexts(server, request, &audioContext, &dataStreamContext);
    HAPAssert(audioContext);
    HAPAssert(dataStreamContext);
    
    dataStreamContext->busy = false;
    
    // Abort audio.
    SiriAudioStop(audioContext);
    audioContext->activeStream = NULL;
    
    if (dataStreamContext->timeoutTimer) {
        HAPPlatformTimerDeregister(dataStreamContext->timeoutTimer);
        dataStreamContext->timeoutTimer = 0;
    }
    
    if (audioContext->newStream) {
        SiriAudioStart(audioContext, audioContext->newStream);
    }
}

static void HandleDataSendStreamOpen(
    HAPAccessoryServerRef *server,
    HAPDataStreamDispatcherRef *dispatcher,
    HAPDataSendDataStreamProtocol *dataStreamProtocol,
    const HAPDataStreamRequest *request,
    HAPDataStreamHandle dataStream,
    HAPDataSendDataStreamProtocolStreamRef *dataSendStream,
    void *_Nullable context HAP_UNUSED)
{
    HAPPrecondition(server);
    HAPPrecondition(dispatcher);
    HAPPrecondition(dataStreamProtocol);
    HAPPrecondition(request);
    HAPPrecondition(dataStream < dataStreamProtocol->storage.numDataStreams);
    HAPPrecondition(dataSendStream);
    
    HAPLogInfo(&kHAPLog_Default, "%s", __func__);
    
    HAPSiriAudioContext *audioContext;
    HAPSiriDataStreamContext *dataStreamContext;
    delegate->getSiriContexts(server, request, &audioContext, &dataStreamContext);
    HAPAssert(audioContext);
    HAPAssert(dataStreamContext);
    
    dataStreamContext->busy = false;
    
    // Send available data.
    SendNextSiriPacket(audioContext, dataStreamContext);
}

static void HandleSendDataComplete(
    HAPAccessoryServerRef *server,
    HAPDataStreamDispatcherRef *dispatcher,
    HAPDataSendDataStreamProtocol *dataStreamProtocol,
    const HAPDataStreamRequest *request,
    HAPDataStreamHandle dataStream,
    HAPDataSendDataStreamProtocolStreamRef *dataSendStream,
    HAPError error,
    void *scratchBytes HAP_UNUSED,
    size_t numScratchBytes HAP_UNUSED,
    void *_Nullable context HAP_UNUSED)
{
    HAPPrecondition(server);
    HAPPrecondition(dispatcher);
    HAPPrecondition(dataStreamProtocol);
    HAPPrecondition(request);
    HAPPrecondition(dataStream < dataStreamProtocol->storage.numDataStreams);
    HAPPrecondition(dataSendStream);
    HAPPrecondition(!error);
    
    HAPLogInfo(&kHAPLog_Default, "%s", __func__);
    
    HAPSiriAudioContext *audioContext;
    HAPSiriDataStreamContext *dataStreamContext;
    delegate->getSiriContexts(server, request, &audioContext, &dataStreamContext);
    HAPAssert(audioContext);
    HAPAssert(dataStreamContext);
    
    dataStreamContext->busy = false;
    
    if (error) {
        HAPAssert(error == kHAPError_InvalidState);
        HAPLogError(&kHAPLog_Default, "HAPDataSendSendData failed");
        return;
    }
    
    // Send remaining data.
    SendNextSiriPacket(audioContext, dataStreamContext);
}

static const HAPDataSendDataStreamProtocolStreamCallbacks dataSendStreamCallbacks = {
    .handleClose = HandleDataSendStreamClose,
    .handleOpen = HandleDataSendStreamOpen
};

void SiriSetDelegate(
    const HAPSiriDelegate *siriDelegate)
{
    delegate = siriDelegate;
}
