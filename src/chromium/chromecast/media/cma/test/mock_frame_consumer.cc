// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chromecast/media/cma/test/mock_frame_consumer.h"

#include "base/bind.h"
#include "base/location.h"
#include "base/message_loop/message_loop_proxy.h"
#include "base/time/time.h"
#include "chromecast/media/cma/base/coded_frame_provider.h"
#include "chromecast/media/cma/base/decoder_buffer_base.h"
#include "chromecast/media/cma/test/frame_generator_for_test.h"
#include "media/base/video_decoder_config.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace chromecast {
namespace media {

MockFrameConsumer::MockFrameConsumer(
    CodedFrameProvider* coded_frame_provider)
    : coded_frame_provider_(coded_frame_provider),
      pattern_idx_(0),
      last_read_aborted_by_flush_(false) {
}

MockFrameConsumer::~MockFrameConsumer() {
}

void MockFrameConsumer::Configure(
    const std::vector<bool>& delayed_task_pattern,
    bool last_read_aborted_by_flush,
    scoped_ptr<FrameGeneratorForTest> frame_generator) {
  delayed_task_pattern_ = delayed_task_pattern;
  last_read_aborted_by_flush_ = last_read_aborted_by_flush;
  frame_generator_.reset(frame_generator.release());
}

void MockFrameConsumer::Start(const base::Closure& done_cb) {
  done_cb_ = done_cb;

  pattern_idx_ = 0;

  base::MessageLoopProxy::current()->PostTask(
      FROM_HERE,
      base::Bind(&MockFrameConsumer::ReadFrame, base::Unretained(this)));
}

void MockFrameConsumer::ReadFrame() {
  // Once all the frames have been read, flush the frame provider.
  if (frame_generator_->RemainingFrameCount() == 0 &&
      !last_read_aborted_by_flush_) {
    coded_frame_provider_->Flush(
        base::Bind(&MockFrameConsumer::OnFlushCompleted,
                   base::Unretained(this)));
    return;
  }

  coded_frame_provider_->Read(
      base::Bind(&MockFrameConsumer::OnNewFrame,
                 base::Unretained(this)));

  // The last read is right away aborted by a Flush.
  if (frame_generator_->RemainingFrameCount() == 0 &&
      last_read_aborted_by_flush_) {
    coded_frame_provider_->Flush(
        base::Bind(&MockFrameConsumer::OnFlushCompleted,
                   base::Unretained(this)));
    return;
  }
}

void MockFrameConsumer::OnNewFrame(
    const scoped_refptr<DecoderBufferBase>& buffer,
    const ::media::AudioDecoderConfig& audio_config,
    const ::media::VideoDecoderConfig& video_config) {
  bool ref_has_config = frame_generator_->HasDecoderConfig();
  scoped_refptr<DecoderBufferBase> ref_buffer = frame_generator_->Generate();

  ASSERT_TRUE(buffer.get());
  ASSERT_TRUE(ref_buffer.get());

  EXPECT_EQ(video_config.IsValidConfig(), ref_has_config);

  EXPECT_EQ(buffer->end_of_stream(), ref_buffer->end_of_stream());
  if (!ref_buffer->end_of_stream()) {
    EXPECT_EQ(buffer->timestamp(), ref_buffer->timestamp());
    ASSERT_EQ(buffer->data_size(), ref_buffer->data_size());
    for (size_t k = 0; k < ref_buffer->data_size(); k++)
      EXPECT_EQ(buffer->data()[k], ref_buffer->data()[k]);
  }

  bool delayed = delayed_task_pattern_[pattern_idx_];
  pattern_idx_ = (pattern_idx_ + 1) % delayed_task_pattern_.size();

  if (delayed) {
    base::MessageLoopProxy::current()->PostDelayedTask(
        FROM_HERE,
        base::Bind(&MockFrameConsumer::ReadFrame,
                   base::Unretained(this)),
        base::TimeDelta::FromMilliseconds(1));
  } else {
    base::MessageLoopProxy::current()->PostTask(
        FROM_HERE,
        base::Bind(&MockFrameConsumer::ReadFrame,
                   base::Unretained(this)));
  }
}

void MockFrameConsumer::OnFlushCompleted() {
  EXPECT_EQ(frame_generator_->RemainingFrameCount(), 0);
  done_cb_.Run();
}

}  // namespace media
}  // namespace chromecast
