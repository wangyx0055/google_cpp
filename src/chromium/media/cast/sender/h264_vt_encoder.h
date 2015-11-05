// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_CAST_SENDER_H264_VT_ENCODER_H_
#define MEDIA_CAST_SENDER_H264_VT_ENCODER_H_

#include "base/mac/scoped_cftyperef.h"
#include "base/threading/thread_checker.h"
#include "media/base/mac/videotoolbox_glue.h"
#include "media/cast/sender/video_encoder.h"

namespace media {
namespace cast {

// VideoToolbox implementation of the media::cast::VideoEncoder interface.
// VideoToolbox makes no guarantees that it is thread safe, so this object is
// pinned to the thread on which it is constructed.
class H264VideoToolboxEncoder : public VideoEncoder {
  typedef CoreMediaGlue::CMSampleBufferRef CMSampleBufferRef;
  typedef VideoToolboxGlue::VTCompressionSessionRef VTCompressionSessionRef;
  typedef VideoToolboxGlue::VTEncodeInfoFlags VTEncodeInfoFlags;

 public:
  H264VideoToolboxEncoder(scoped_refptr<CastEnvironment> cast_environment,
                          const VideoSenderConfig& video_config,
                          const CastInitializationCallback& initialization_cb);
  ~H264VideoToolboxEncoder() override;

  // media::cast::VideoEncoder implementation
  bool EncodeVideoFrame(
      const scoped_refptr<media::VideoFrame>& video_frame,
      const base::TimeTicks& reference_time,
      const FrameEncodedCallback& frame_encoded_callback) override;
  void SetBitRate(int new_bit_rate) override;
  void GenerateKeyFrame() override;
  void LatestFrameIdToReference(uint32 frame_id) override;
  scoped_ptr<VideoFrameFactory> CreateVideoFrameFactory() override;

 private:
  // Initialize the compression session.
  bool Initialize(const VideoSenderConfig& video_config);

  // Configure the compression session.
  void ConfigureSession(const VideoSenderConfig& video_config);

  // Teardown the encoder.
  void Teardown();

  // Set a compression session property.
  bool SetSessionProperty(CFStringRef key, int32_t value);
  bool SetSessionProperty(CFStringRef key, bool value);
  bool SetSessionProperty(CFStringRef key, CFStringRef value);

  // Compression session callback function to handle compressed frames.
  static void CompressionCallback(void* encoder_opaque,
                                  void* request_opaque,
                                  OSStatus status,
                                  VTEncodeInfoFlags info,
                                  CMSampleBufferRef sbuf);

  // The cast environment (contains worker threads & more).
  const scoped_refptr<CastEnvironment> cast_environment_;

  // VideoToolboxGlue provides access to VideoToolbox at runtime.
  const VideoToolboxGlue* videotoolbox_glue_;

  // Thread checker to enforce that this object is used on a specific thread.
  base::ThreadChecker thread_checker_;

  // The compression session.
  base::ScopedCFTypeRef<VTCompressionSessionRef> compression_session_;

  // Frame identifier counter.
  uint32 frame_id_;

  // Force next frame to be a keyframe.
  bool encode_next_frame_as_keyframe_;

  DISALLOW_COPY_AND_ASSIGN(H264VideoToolboxEncoder);
};

}  // namespace cast
}  // namespace media

#endif  // MEDIA_CAST_SENDER_H264_VT_ENCODER_H_
