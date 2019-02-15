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

// This PAL module is making use of GStreamer to obtain video data. https://gstreamer.freedesktop.org/
// Further documentation on GStreamer usage: https://gstreamer.freedesktop.org/data/doc/gstreamer/head/manual/manual.pdf


#include "HAP.h"
#include "HAPPlatformVideo+Internal.h"

static const HAPLogObject logObject = {
    .subsystem = kHAPPlatform_LogSubsystem,
    .category = "Camera"
};

/**
 * Video keyframe interval.
 */
#define kVideoKeyframeInterval  (5) // [s]

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
 * FreeSafe wrapper for GStreamer gst_buffer_unmap
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
 * @param[transfer full] filter     GStreamer structure describing sets of media formats.
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
 * Video sample handler
 *
 * This data handler is called from gstreamer thread.
 *
 * @param      inBuffer             GStreamer input buffer.
 * @param      data                 GStreamer data pointer of type HAPPlatformCameraStream*.
 *
 * @return The result of passing the data into the GStreamer input buffer.
 */
HAP_RESULT_USE_CHECK
static GstFlowReturn VideoSampleHandler(
    GstElement *inBuffer,
    gpointer data)
{
    HAPPrecondition(inBuffer);
    HAPPrecondition(data);

    HAPPlatformVideoStream *stream = (HAPPlatformVideoStream *) data;

    if (stream->callback) {
        GstSample *sample;
        g_signal_emit_by_name(inBuffer, "pull-sample", &sample, NULL);
        HAPAssert(sample);

        GstBuffer *buffer = gst_sample_get_buffer(sample);
        HAPAssert(buffer);

        GstClockTime ts = GST_BUFFER_DTS(buffer); // stream time [ns]
        GstMapInfo info;
        HAPAssert(gst_buffer_map(buffer, &info, (GstMapFlags)0));

        // skip NAL prefix bytes
        uint8_t *bytes = info.data;
        size_t numBytes = info.size;
        while (numBytes && bytes[0] == 0) {
            bytes++;
            numBytes--;
        }
        HAPAssert(bytes[0] == 1);
        bytes++;
        numBytes--;
        HAPAssert(numBytes);
        
        stream->callback(stream, bytes, numBytes, ts);
        
        gst_buffer_unmap(buffer, &info);
        GstSampleFreeSafe(sample);
    }

    return GST_FLOW_OK;
}

/**
 * Create a snapshot pipeline to read from camera.
 *
 * @param      width                Width of the generated snapshots.
 * @param      height               Height of the generated snapshots.
 *
 * @return GStreamer snapshot pipeline.
 */
HAP_RESULT_USE_CHECK
static GstElement *MakeSnapshotPipeline(
    int width,
    int height)
{
    HAPPrecondition(width >= 0);
    HAPPrecondition(height >= 0);

    GstElement *pipe = gst_pipeline_new("pipeline");

    GstElement *camera = CreateElementAndAddToPipe("v4l2src", "camera", pipe);
    GstCaps *cameraCaps = gst_caps_new_simple("video/x-raw",
        "width", G_TYPE_INT, width,
        "height", G_TYPE_INT, height,
        "framerate", GST_TYPE_FRACTION, 30, 1,
        NULL);

    GstElement *jpegEncoder = CreateElementAndAddToPipe("jpegenc", "jpegEncoder", pipe);
    GstElement *jpegBuffer = CreateElementAndAddToPipe("appsink", "jpegBuffer", pipe);

    // create links
    LinkElementsWithFilter(camera, jpegEncoder, cameraCaps);
    LinkElementsWithFilter(jpegEncoder, jpegBuffer, NULL);

    return pipe;
}

HAP_RESULT_USE_CHECK
HAPError HAPPlatformVideoStreamInitialize()
{
    // initialize gstreamer
    gst_init(NULL, NULL);
    guint major, minor, micro, nano;
    gst_version(&major, &minor, &micro, &nano);
    HAPLogInfo(&logObject, "gstreamer version %d.%d.%d,%d", major, minor, micro, nano);
    
    return kHAPError_None;
}

void HAPPlatformVideoStreamCleanup()
{
}

HAP_RESULT_USE_CHECK
HAPError HAPPlatformVideoStreamStart(
    HAPPlatformVideoStream *stream,
    uint32_t width,
    uint32_t height,
    uint32_t framerate,
    uint32_t bitrate,
    HAPH264VideoCodecProfile profile,
    HAPH264VideoCodecProfileLevel level,
    HAPPlatformVideoStreamCallback callback,
    void *context)
{
    HAPPrecondition(stream);

    GstElement *pipe = gst_pipeline_new("video");

    GstElement *camera = CreateElementAndAddToPipe("v4l2src", "camera", pipe);

    GstElement *videoFilter = CreateElementAndAddToPipe("capsfilter", "videoFilter", pipe);
    GstCaps *sizeCaps = gst_caps_new_simple("video/x-raw",
        "width", G_TYPE_INT, width,
        "height", G_TYPE_INT, height,
        "framerate", GST_TYPE_FRACTION, framerate, 1,
        NULL);
    g_object_set(G_OBJECT(videoFilter), "caps", sizeCaps, NULL);
    GstCapsFreeSafe(sizeCaps);

    GstElement *videoEncoder = CreateElementAndAddToPipe("omxh264enc", "videoEncoder", pipe);
    g_object_set(G_OBJECT(videoEncoder),
        "control-rate", 1, // variable
        "periodicty-idr", kVideoKeyframeInterval * framerate,
        "target-bitrate", bitrate,
        NULL);
        
    const gchar *profileStr = "main";
    switch (profile) {
        case kHAPH264VideoCodecProfile_High: profileStr = "high"; break;
        case kHAPH264VideoCodecProfile_Main: profileStr = "main"; break;
        case kHAPH264VideoCodecProfile_ConstrainedBaseline: profileStr = "constrained-baseline";
    }
    
    const gchar *levelStr = "4";
    // enhance level if needed to support the given video resolution
    if (level == kHAPH264VideoCodecProfileLevel_4 || width >= 1080) {
        levelStr = "4";
    } else if (level == kHAPH264VideoCodecProfileLevel_3_2 || width >= 960) {
        levelStr = "3.2";
    } else {
        levelStr = "3.1";
    }
    
    GstCaps *videoCaps = gst_caps_new_simple("video/x-h264",
        "profile", G_TYPE_STRING, profileStr,
        "level", G_TYPE_STRING, levelStr,
        NULL);

    GstElement *videoParser = CreateElementAndAddToPipe("h264parse", "videoParser", pipe);
    GstCaps *parserCaps = gst_caps_new_simple("video/x-h264",
        "stream-format", G_TYPE_STRING, "byte-stream",
        "alignment", G_TYPE_STRING, "nal",
        NULL);

    GstElement *videoBuffer = CreateElementAndAddToPipe("appsink", "videoBuffer", pipe);
    g_object_set(G_OBJECT(videoBuffer),
        "enable-last-sample", false,
        "emit-signals", true,
        NULL);
    g_signal_connect(videoBuffer,
        "new-sample",
        G_CALLBACK(VideoSampleHandler),
        stream);

    LinkElementsWithFilter(camera, videoFilter, NULL);
    LinkElementsWithFilter(videoFilter, videoEncoder, NULL);
    LinkElementsWithFilter(videoEncoder, videoParser, videoCaps);
    LinkElementsWithFilter(videoParser, videoBuffer, parserCaps);

    gst_element_set_state(pipe, GST_STATE_READY);

    stream->inputPipe = pipe;
    stream->encoderRef = videoEncoder;
    stream->callback = callback;
    stream->context = context;

    return kHAPError_None;
}

HAP_RESULT_USE_CHECK
HAPError HAPPlatformVideoStreamReconfigure(
    HAPPlatformVideoStream *stream,
    uint32_t width,
    uint32_t height,
    uint32_t framerate,
    uint32_t bitrate)
{
    // video only
    HAPPrecondition(stream);

    if (stream->inputPipe) {
        GstElement *videoFilter = gst_bin_get_by_name(GST_BIN(stream->inputPipe), "videoFilter");
        GstCaps *sizeCaps = gst_caps_new_simple("video/x-raw",
            "width", G_TYPE_INT, width,
            "height", G_TYPE_INT, height,
            "framerate", GST_TYPE_FRACTION, framerate, 1,
            NULL);
        g_object_set(G_OBJECT(videoFilter), "caps", sizeCaps, NULL);

        GstCapsFreeSafe(sizeCaps);
        GstObjectFreeSafe(videoFilter);
    }

    if (stream->encoderRef) {
        g_object_set(G_OBJECT(stream->encoderRef),
            "target-bitrate", bitrate * 1000, // [bit/s]
            NULL);
    }
    
    return kHAPError_None;
}

void HAPPlatformVideoStreamSetBitrate(
    HAPPlatformVideoStream *stream,
    uint32_t bitrate)
{
    HAPPrecondition(stream);
    
    if (stream->encoderRef) {
        g_object_set(G_OBJECT(stream->encoderRef),
            "target-bitrate", bitrate,
            NULL);
    }
}

void HAPPlatformVideoStreamRequestKeyFrame(
    HAPPlatformVideoStream *stream)
{
    HAPPrecondition(stream);
    
    if (stream->encoderRef) {
        GstPad *pad = gst_element_get_static_pad(stream->encoderRef, "src");
        GstEvent *event = gst_event_new_custom(
            GST_EVENT_CUSTOM_UPSTREAM,
            gst_structure_new ("GstForceKeyUnit",
                "all-headers", G_TYPE_BOOLEAN, false,
                NULL));
        gst_pad_push_event(gst_pad_get_peer(pad), event);
    }
}

HAP_RESULT_USE_CHECK
HAPError HAPPlatformVideoStreamEnable(
    HAPPlatformVideoStream *stream)
{
    HAPPrecondition(stream);
    
    if (stream->inputPipe) {
        gst_element_set_state(stream->inputPipe, GST_STATE_PLAYING);
    }

    return kHAPError_None;
}

HAP_RESULT_USE_CHECK
HAPError HAPPlatformVideoStreamDisable(
    HAPPlatformVideoStream *stream)
{
    HAPPrecondition(stream);
    
    if (stream->inputPipe) {
        gst_element_set_state(stream->inputPipe, GST_STATE_PAUSED);
    }

    return kHAPError_None;
}

void HAPPlatformVideoStreamStop(
    HAPPlatformVideoStream *stream)
{
    HAPPrecondition(stream);
    
    if (stream->inputPipe) {
        gst_element_set_state(stream->inputPipe, GST_STATE_NULL);
        GstObjectFreeSafe(stream->inputPipe);
        stream->encoderRef = NULL;
    }
}

HAP_RESULT_USE_CHECK
static HAPError SnapshotReaderGetSize(
    const HAPPlatformCameraSnapshotReader *snapshotReader,
    size_t *numBytes)
{
    HAPPrecondition(snapshotReader && snapshotReader->context);
    HAPPrecondition(numBytes);
    
    HAPPlatformVideoSnapshot *snapshot = snapshotReader->context;
    *numBytes = snapshot->size;
    
    return kHAPError_None;
}

HAP_RESULT_USE_CHECK
static HAPError SnapshotReaderRead(
    const HAPPlatformCameraSnapshotReader *snapshotReader,
    void *bytes,
    size_t maxBytes,
    size_t *numBytes)
{
    HAPPrecondition(snapshotReader && snapshotReader->context);
    HAPPrecondition(bytes);
    HAPPrecondition(numBytes);
    
    HAPPlatformVideoSnapshot *snapshot = snapshotReader->context;
    gsize read = gst_buffer_extract(snapshot->buffer, snapshot->offset, bytes, maxBytes);
    snapshot->offset += read;
    *numBytes = read;
    
    return kHAPError_None;
}

static void SnapshotReaderClose(
    const HAPPlatformCameraSnapshotReader *snapshotReader)
{
    HAPPrecondition(snapshotReader && snapshotReader->context);
    
    HAPPlatformVideoSnapshot *snapshot = snapshotReader->context;
    GstBufferFreeSafe(snapshot->buffer);
    snapshot->size = 0;
}

void HAPPlatformVideoInitializeSnapshot(
    HAPPlatformVideoSnapshot *snapshot)
{
    snapshot->size = 0;
    snapshot->reader.context = snapshot;
    snapshot->reader.getSize = SnapshotReaderGetSize;
    snapshot->reader.read = SnapshotReaderRead;
    snapshot->reader.close = SnapshotReaderClose;
}

HAP_RESULT_USE_CHECK
HAPError HAPPlatformVideoTakeSnapshot(
    HAPPlatformVideoSnapshot *snapshot,
    uint16_t width,
    uint16_t height,
    HAPPlatformCameraSnapshotReader **snapshotReader)
{
    HAPPrecondition(snapshot);
    HAPPrecondition(snapshotReader);

    GstElement *pipeline = MakeSnapshotPipeline(width, height);
    HAPAssert(pipeline);

    // start pipeline to get a sample
    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    // get a sample from the pipeline
    GstElement *jpegBuffer = gst_bin_get_by_name(GST_BIN(pipeline), "jpegBuffer");
    GstSample *sample;
    g_signal_emit_by_name(jpegBuffer, "pull-preroll", &sample, NULL);

    if (sample == NULL) {
        HAPLogError(&logObject, "snapshot generation failed");
        return kHAPError_Unknown;
    }
    GstBuffer *buffer = gst_sample_get_buffer(sample);
    gst_buffer_ref(buffer); // keep the buffer alive

    GstSampleFreeSafe(sample);
    GstObjectFreeSafe(jpegBuffer);

    gst_element_set_state(pipeline, GST_STATE_NULL);
    GstObjectFreeSafe(pipeline);

    snapshot->buffer = buffer;
    snapshot->size = gst_buffer_get_size(buffer);
    snapshot->offset = 0;
    *snapshotReader = &snapshot->reader;

    return kHAPError_None;
}
