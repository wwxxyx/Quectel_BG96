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

// This PAL module is making use of GStreamer to obtain audio data. https://gstreamer.freedesktop.org/
// Further documentation on GStreamer usage: https://gstreamer.freedesktop.org/data/doc/gstreamer/head/manual/manual.pdf

#include "HAP.h"
#include "HAPPlatformAudio+Internal.h"

#include <math.h> // pow

static const HAPLogObject logObject = {
    .subsystem = kHAPPlatform_LogSubsystem,
    .category = "Camera"
};

/**
 * Alsa audio input device
 */
#define kAlsaInputDevice  "plughw:1"      // the alsa audio input device

/**
 * Alsa audio output device
 */
#define kAlsaOutputDevice "plughw:0"      // the alsa audio output device

/**
 * Opus RAW Samplerate
 *
 * Independent of negotiated audio sample rate
 */
#define kOpusRawSamplerate  (24000)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * FreeSafe wrapper for GStreamer g_free
 */
#define GstGFreeSafe(ptr) do { HAPAssert(ptr); g_free(ptr); ptr = NULL; } while(0)

/**
 * FreeSafe wrapper for GStreamer gst_sample_unref
 */
#define GstSampleFreeSafe(ptr) do { HAPAssert(ptr); gst_sample_unref(ptr); ptr = NULL; } while(0)

/**
 * FreeSafe wrapper for GStreamer gst_object_unref
 */
#define GstObjectFreeSafe(ptr) do { HAPAssert(ptr); gst_object_unref(GST_OBJECT(ptr)); ptr = NULL; } while(0)

/**
 * FreeSafe wrapper for GstCaps
 */
#define GstCapsFreeSafe(ptr) do { HAPAssert(ptr); gst_caps_unref(ptr); ptr = NULL; } while(0)

/**
 * FreeSafe wrapper for GStreamer gst_buffer_unref
 */
#define GstBufferFreeSafe(ptr) do { HAPAssert(ptr); gst_buffer_unref(ptr); ptr = NULL; } while(0)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Create GStreamer element and add it to GStreamer bin.
 *
 * @param      elementType          Type of the new element.
 * @param      elementName          Name of the new element.
 * @param      pipe                 GStreamer bin where the new element is added.
 *
 * @return The newly created element.
 */
HAP_RESULT_USE_CHECK
static GstElement *CreateElementAndAddToPipe(
    const gchar *elementType,
    const gchar *elementName,
    GstElement *pipe)
{
    HAPPrecondition(elementType);
    HAPPrecondition(elementName);
    HAPPrecondition(pipe);

    GstElement *element = gst_element_factory_make(elementType, elementName);
    if (!element) {
        HAPLogError(&logObject, "%s: %s (%s) creation failed", __func__, elementName, elementType);
        HAPFatalError();
    }
    gboolean success = gst_bin_add(GST_BIN(pipe), element); // bin owns element
    if (!success) {
        HAPLogError(&logObject, "%s: Pipe does not want to accept element %s (%s)", __func__, elementName, elementType);
        HAPFatalError();
    }

    return element;
}

/**
 * Link a GStreamer src to a sink.
 *
 * @param      sourceElement        GStreamer source.
 * @param      sinkElement          GStreamer sink.
 * @param[in]  filter               GStreamer structure describing sets of media formats.
 */
static void LinkElementsWithFilter(
    GstElement *sourceElement,
    GstElement *sinkElement,
    GstCaps *_Nullable filter)
{
    HAPPrecondition(sourceElement);
    HAPPrecondition(sinkElement);

    gboolean success = gst_element_link_filtered(sourceElement, sinkElement, filter);
    if (!success) {
        gchar *sourceName = gst_element_get_name(sourceElement);
        gchar *sinkName = gst_element_get_name(sinkElement);
        HAPLogError(&logObject, "%s: link from %s to %s failed", __func__, sourceName, sinkName);

        GstGFreeSafe(sourceName);
        GstGFreeSafe(sinkName);
        HAPFatalError();
    }

    if (filter) {
        GstCapsFreeSafe(filter);
    }
}

/**
 * Audio sample handler.
 *
 * This data handler is called from gstreamer thread.
 *
 * @param      inBuffer             GStreamer input buffer.
 * @param      data                 GStreamer data pointer of type HAPPlatformCameraStream*.
 *
 * @return The result of passing the data into the GStreamer input buffer.
 */
HAP_RESULT_USE_CHECK
static GstFlowReturn AudioSampleHandler(
    GstElement *inBuffer,
    gpointer data)
{
    HAPPrecondition(inBuffer);
    HAPPrecondition(data);

    HAPPlatformAudioStream *stream = (HAPPlatformAudioStream *) data;

    if (stream->callback) {
        GstSample *sample;
        g_signal_emit_by_name(inBuffer, "pull-sample", &sample, NULL);
        HAPAssert(sample);

        GstBuffer *buffer = gst_sample_get_buffer(sample);
        HAPAssert(buffer);

        GstClockTime ts = GST_BUFFER_DTS(buffer); // stream time [ns]
        GstMapInfo info;
        HAPAssert(gst_buffer_map(buffer, &info, (GstMapFlags)0));

        // Get RMS of raw audio data from level element.
        float rms = 0.0f;
        HAPAssert(stream->inputPipe);
        GstBus *bus = gst_pipeline_get_bus(GST_PIPELINE(stream->inputPipe));
        HAPAssert(bus);
        GstMessage *message;
        while ((message = gst_bus_pop_filtered(bus, GST_MESSAGE_ELEMENT))) {
            if (gst_message_has_name(message, "level")) {
                const GstStructure *structure = gst_message_get_structure(message);
                HAPAssert(structure);
                HAP_DIAGNOSTIC_PUSH // We have to use the deprecated type GValueArray.
                HAP_DIAGNOSTIC_IGNORED_GCC("-Wdeprecated-declarations")
                HAP_DIAGNOSTIC_IGNORED_CLANG("-Wdeprecated-declarations")
                GValueArray *levels;
                HAPAssert(gst_structure_get(structure, "rms", G_TYPE_VALUE_ARRAY, &levels, NULL));
                HAPAssert(levels);
                const GValue *level = g_value_array_get_nth(levels, 0);
                HAPAssert(level);
                HAP_DIAGNOSTIC_PUSH
                HAP_DIAGNOSTIC_IGNORED_CLANG("-Wcast-qual")
                HAPAssert(G_VALUE_TYPE(level) == G_TYPE_DOUBLE);
                HAP_DIAGNOSTIC_POP
                double rmsdb = g_value_get_double(level);
                rms = (float)pow(10, rmsdb / 20); // db -> fraction
                g_value_array_free(levels);
                HAP_DIAGNOSTIC_POP
            }
            gst_message_unref(message);
        }

        stream->callback(stream, info.data, info.size, ts, rms);
    
        gst_buffer_unmap(buffer, &info);
        GstSampleFreeSafe(sample);
    }

    return GST_FLOW_OK;
}

HAP_RESULT_USE_CHECK
HAPError HAPPlatformAudioStreamInitialize()
{
    // Initialize gstreamer.
    gst_init(NULL, NULL);
    
    return kHAPError_None;
}

void HAPPlatformAudioStreamCleanup()
{
}

HAP_RESULT_USE_CHECK
HAPError HAPPlatformAudioStreamStartMicrophone(
    HAPPlatformAudioStream          *stream,
    HAPAudioCodecSampleRate         sampleRate,
    HAPAudioCodecBitRateControlMode bitRateMode,
    uint32_t                        bitrate,
    uint32_t                        packetTime,
    HAPPlatformAudioStreamCallback  callback,
    void                            *context)
{
    HAPPrecondition(stream);
    
#if __APPLE__
    const char *audiosrc = "osxaudiosrc";
#else
    const char *audiosrc = "alsasrc";
#endif
    
    GstElement *pipe = gst_pipeline_new("Microphone");
    GstElement *microphone = CreateElementAndAddToPipe(audiosrc, "microphone", pipe);
    g_object_set(G_OBJECT(microphone), "device", kAlsaInputDevice, NULL);
    GstCaps *micCaps = gst_caps_new_simple("audio/x-raw",
        "format", G_TYPE_STRING, "S16LE",
        "rate", G_TYPE_INT, kOpusRawSamplerate,
        NULL);

    GstElement *volume = CreateElementAndAddToPipe("volume", "volume", pipe);

    GstElement *level = CreateElementAndAddToPipe("level", "level", pipe);
    g_object_set(G_OBJECT(level),
        "post-messages", true,
        "interval", packetTime * 1000000ull, // [ns]
        NULL);
    
    int bandwidth = 1103;
    switch (sampleRate) {
        case kHAPAudioCodecSampleRate_8KHz: bandwidth = 1101; break; // narrowband
        case kHAPAudioCodecSampleRate_16KHz: bandwidth = 1103; break; // wideband
        case kHAPAudioCodecSampleRate_24KHz: bandwidth = 1104; break; // superwideband
    }
    
    int bitRateType = 1;
    switch (bitRateMode) {
        case kHAPAudioCodecBitRateControlMode_Variable: bitRateType = 1; break;
        case kHAPAudioCodecBitRateControlMode_Constant: bitRateType = 0; break;
    }

    GstElement *audioEncoder = CreateElementAndAddToPipe("opusenc", "audioEncoder", pipe);
    g_object_set(G_OBJECT(audioEncoder),
        "bitrate-type", bitRateType, // cbr=0, vbr=1
        "bitrate", bitrate,
        "audio-type", 2048, // voice
        "bandwidth", bandwidth,
        "frame-size", packetTime, // [ms]
        NULL);

    GstElement *audioBuffer = CreateElementAndAddToPipe("appsink", "audioBuffer", pipe);
    g_object_set(G_OBJECT(audioBuffer),
        "enable-last-sample", false,
        "emit-signals", true,
        NULL);
    g_signal_connect(audioBuffer,
        "new-sample",
        G_CALLBACK(AudioSampleHandler),
        stream);
    
    LinkElementsWithFilter(microphone, volume, micCaps);
    LinkElementsWithFilter(volume, level, NULL);
    LinkElementsWithFilter(level, audioEncoder, NULL);
    LinkElementsWithFilter(audioEncoder, audioBuffer, NULL);
    
    gst_element_set_state(pipe, GST_STATE_READY);

    stream->inputPipe = pipe;
    stream->encoderRef = audioEncoder;
    stream->callback = callback;
    stream->context = context;

    return kHAPError_None;
}

HAP_RESULT_USE_CHECK
HAPError HAPPlatformAudioStreamEnableMicrophone(
    HAPPlatformAudioStream *stream)
{
    HAPPrecondition(stream);
    
    if (stream->inputPipe) {
        gst_element_set_state(stream->inputPipe, GST_STATE_PLAYING);
    }

    return kHAPError_None;
}

HAP_RESULT_USE_CHECK
HAPError HAPPlatformAudioStreamDisableMicrophone(
    HAPPlatformAudioStream *stream)
{
    HAPPrecondition(stream);
    
    if (stream->inputPipe) {
        gst_element_set_state(stream->inputPipe, GST_STATE_PAUSED);
    }

    return kHAPError_None;
}

void HAPPlatformAudioStreamStopMicrophone(
    HAPPlatformAudioStream *stream)
{
    HAPPrecondition(stream);
    
    if (stream->inputPipe) {
        gst_element_set_state(stream->inputPipe, GST_STATE_NULL);
        GstObjectFreeSafe(stream->inputPipe);
        stream->encoderRef = NULL;
    }
}

void HAPPlatformAudioStreamSetMicrophoneVolume(
    HAPPlatformAudioStream *stream,
    uint32_t volume,
    bool mute)
{
    HAPPrecondition(stream);
    
    if (stream->inputPipe) {
        GstElement *volumeCtrl = gst_bin_get_by_name(GST_BIN(stream->inputPipe), "volume");
        if (volumeCtrl) {
            g_object_set(G_OBJECT(volumeCtrl),
                "mute", mute,
                "volume", (double) volume / 100.0, // % -> fraction
                NULL);
        }
    }
}

HAP_RESULT_USE_CHECK
HAPError HAPPlatformAudioStreamStartSpeaker(
    HAPPlatformAudioStream  *stream,
    HAPAudioCodecSampleRate sampleRate,
    uint32_t                packetTime)
{
    (void)sampleRate;
    HAPPrecondition(stream);

    GstElement *pipe = gst_pipeline_new("Speaker");

    GstElement *speakerBuffer = CreateElementAndAddToPipe("appsrc", "speakerBuffer", pipe);
    GstCaps * audioCaps = gst_caps_new_simple("audio/x-opus",
        "channel-mapping-family", G_TYPE_INT, 0,
        NULL);
    g_object_set(G_OBJECT(speakerBuffer),
        "stream-type", 0, // stream
        "format", 3,      // time
        "emit-signals", false,
        "do-timestamp", false,
        "emit-signals", false,
        "is-live", true,
        "caps", audioCaps,
        NULL);
    GstCapsFreeSafe(audioCaps);
    
    GstElement *speakerDecoder = CreateElementAndAddToPipe("opusdec", "speakerDecoder", pipe);
    
    GstElement *volume = CreateElementAndAddToPipe("volume", "volume", pipe);

    GstElement *speaker = CreateElementAndAddToPipe("alsasink", "speaker", pipe);
    g_object_set(G_OBJECT(speaker), "device", kAlsaOutputDevice, NULL);
    GstCaps *speakerCaps = gst_caps_new_simple("audio/x-raw",
        "format", G_TYPE_STRING, "S16LE",
        "rate", G_TYPE_INT, kOpusRawSamplerate,
        NULL);

    LinkElementsWithFilter(speakerBuffer, speakerDecoder, NULL);
    LinkElementsWithFilter(speakerDecoder, volume, speakerCaps);
    LinkElementsWithFilter(volume, speaker, NULL);

    gst_element_set_state(pipe, GST_STATE_READY);

    stream->outputPipe = pipe;
    stream->packetTime = packetTime; // [ms]
    stream->bufferRef  = speakerBuffer;
    stream->speakerOn  = false; // delay enable

    return kHAPError_None;
}

HAP_RESULT_USE_CHECK
HAPError HAPPlatformAudioStreamEnableSpeaker(
    HAPPlatformAudioStream *stream)
{
    HAPPrecondition(stream);
    
    if (stream->outputPipe && stream->speakerOn) {
        gst_element_set_state(stream->outputPipe, GST_STATE_PLAYING);
    }

    return kHAPError_None;
}

HAP_RESULT_USE_CHECK
HAPError HAPPlatformAudioStreamDisableSpeaker(
    HAPPlatformAudioStream *stream)
{
    HAPPrecondition(stream);
    
    if (stream->outputPipe && stream->speakerOn) {
        gst_element_set_state(stream->outputPipe, GST_STATE_PAUSED);
    }

    return kHAPError_None;
}

void HAPPlatformAudioStreamStopSpeaker(
    HAPPlatformAudioStream *stream)
{
    HAPPrecondition(stream);
    
    if (stream->outputPipe) {
        gst_element_set_state(stream->outputPipe, GST_STATE_NULL);
        GstObjectFreeSafe(stream->outputPipe);
        stream->bufferRef = NULL;
    }
}

void HAPPlatformAudioPushSpeakerData(
    HAPPlatformAudioStream *stream,
    uint8_t                bytes[],
    size_t                 numBytes,
    uint64_t               sampleTime)
{
    HAPPrecondition(stream);
    HAPPrecondition(bytes);
    
    if (stream->bufferRef) {
        if (!stream->speakerOn) {
            gst_element_set_state(stream->outputPipe, GST_STATE_PLAYING);
            stream->speakerOn = true;
        }
        
        GstBuffer *buffer = gst_buffer_new_allocate(NULL, numBytes, NULL);
        HAPAssert(buffer);
        gst_buffer_fill(buffer, 0, bytes, numBytes);

        // add timestamp
        GST_BUFFER_DTS(buffer) = sampleTime;
        GST_BUFFER_DURATION(buffer) = stream->packetTime * 1000000; // [ns]

        // send to pipeline
        GstFlowReturn ret;
        g_signal_emit_by_name(stream->bufferRef, "push-buffer", buffer, &ret, NULL);
        HAPAssert(ret == GST_FLOW_OK);
        
        GstBufferFreeSafe(buffer);
    }
}

void HAPPlatformAudioStreamSetSpeakerVolume(
    HAPPlatformAudioStream *stream,
    uint32_t volume,
    bool mute)
{
    HAPPrecondition(stream);
    
    if (stream->outputPipe) {
        GstElement *volumeCtrl = gst_bin_get_by_name(GST_BIN(stream->outputPipe), "volume");
        if (volumeCtrl) {
            g_object_set(G_OBJECT(volumeCtrl),
                "mute", mute,
                "volume", (double) volume / 100.0, // % -> fraction
                NULL);
        }
    }
}
