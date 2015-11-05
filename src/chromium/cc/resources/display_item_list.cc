// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "cc/resources/display_item_list.h"

#include <string>

#include "base/debug/trace_event.h"
#include "base/debug/trace_event_argument.h"
#include "cc/base/math_util.h"
#include "cc/debug/picture_debug_util.h"
#include "third_party/skia/include/core/SkCanvas.h"
#include "third_party/skia/include/core/SkPictureRecorder.h"
#include "ui/gfx/skia_util.h"

namespace cc {

DisplayItemList::DisplayItemList()
    : is_suitable_for_gpu_rasterization_(true), approximate_op_count_(0) {
}

scoped_refptr<DisplayItemList> DisplayItemList::Create() {
  return make_scoped_refptr(new DisplayItemList());
}

DisplayItemList::~DisplayItemList() {
}

void DisplayItemList::Raster(SkCanvas* canvas,
                             SkDrawPictureCallback* callback,
                             float contents_scale) const {
  canvas->save();
  canvas->scale(contents_scale, contents_scale);
  for (size_t i = 0; i < items_.size(); ++i) {
    items_[i]->Raster(canvas, callback);
  }
  canvas->restore();
}

void DisplayItemList::AppendItem(scoped_ptr<DisplayItem> item) {
  is_suitable_for_gpu_rasterization_ &= item->IsSuitableForGpuRasterization();
  approximate_op_count_ += item->ApproximateOpCount();
  items_.push_back(item.Pass());
}

bool DisplayItemList::IsSuitableForGpuRasterization() const {
  // This is more permissive than Picture's implementation, since none of the
  // items might individually trigger a veto even though they collectively have
  // enough "bad" operations that a corresponding Picture would get vetoed.
  return is_suitable_for_gpu_rasterization_;
}

int DisplayItemList::ApproximateOpCount() const {
  return approximate_op_count_;
}

size_t DisplayItemList::PictureMemoryUsage() const {
  size_t total_size = 0;

  for (const auto& item : items_) {
    total_size += item->PictureMemoryUsage();
  }

  return total_size;
}

scoped_refptr<base::debug::ConvertableToTraceFormat> DisplayItemList::AsValue()
    const {
  scoped_refptr<base::debug::TracedValue> state =
      new base::debug::TracedValue();

  state->SetInteger("length", items_.size());
  state->SetValue("params.layer_rect",
                  MathUtil::AsValue(layer_rect_).release());

  SkPictureRecorder recorder;
  SkCanvas* canvas =
      recorder.beginRecording(layer_rect_.width(), layer_rect_.height());
  canvas->translate(-layer_rect_.x(), -layer_rect_.y());
  canvas->clipRect(gfx::RectToSkRect(layer_rect_));
  for (size_t i = 0; i < items_.size(); ++i)
    items_[i]->RasterForTracing(canvas);
  skia::RefPtr<SkPicture> picture = skia::AdoptRef(recorder.endRecording());

  std::string b64_picture;
  PictureDebugUtil::SerializeAsBase64(picture.get(), &b64_picture);
  state->SetString("skp64", b64_picture);

  return state;
}

void DisplayItemList::EmitTraceSnapshot() const {
  TRACE_EVENT_OBJECT_SNAPSHOT_WITH_ID(
      TRACE_DISABLED_BY_DEFAULT("cc.debug") "," TRACE_DISABLED_BY_DEFAULT(
          "devtools.timeline.picture"),
      "cc::DisplayItemList", this, AsValue());
}

}  // namespace cc
