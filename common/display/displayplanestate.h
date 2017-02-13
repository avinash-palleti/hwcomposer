﻿/*
// Copyright (c) 2016 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
*/
#ifndef DISPLAY_PLANE_STATE_H_
#define DISPLAY_PLANE_STATE_H_

#include <stdint.h>
#include <vector>
#include "hwctrace.h"
#include "overlaylayer.h"

namespace hwcomposer {

class DisplayPlane;
class DisplayPlaneState;
struct OverlayLayer;

typedef std::vector<DisplayPlaneState> DisplayPlaneStateList;

class DisplayPlaneState {
 public:
  enum class State : int32_t { kScanout, kRender };

  DisplayPlaneState() = default;
  DisplayPlaneState(DisplayPlaneState &&rhs) = default;
  DisplayPlaneState &operator=(DisplayPlaneState &&other) = default;
  DisplayPlaneState(DisplayPlane *plane, OverlayLayer *layer, uint32_t index)
      : plane_(plane), layer_(layer) {
    source_layers_.emplace_back(index);
    uint32_t width = layer->GetDisplayFrameWidth();
    uint32_t height = layer->GetDisplayFrameHeight();
    max_width_ = max_width_ > width ? max_width_ : width;
    max_height_ = max_height_ > height ? max_height_ : height;
  }

  State GetCompositionState() const {
    return state_;
  }

  uint32_t GetMaxWidth() const {
    return max_width_;
  }

  uint32_t GetMaxHeight() const {
    return max_height_;
  }

  void AddLayer(size_t index, uint32_t width, uint32_t height) {
    WTRACE("Layer is being added to width = %d, height = %d, maxWidth = %d",
           width, height, max_width_);
    max_width_ = max_width_ > width ? max_width_ : width;
    max_height_ = max_height_ > height ? max_height_ : height;
    source_layers_.emplace_back(index);
    state_ = State::kRender;
  }

  void ForceGPURendering() {
    state_ = State::kRender;
  }

  void SetOverlayLayer(OverlayLayer *layer) {
    layer_ = layer;
  }

  OverlayLayer *GetOverlayLayer() const {
    return layer_;
  }

  DisplayPlane *plane() const {
    return plane_;
  }

  const std::vector<size_t> &source_layers() const {
    return source_layers_;
  }

 private:
  State state_ = State::kScanout;
  DisplayPlane *plane_ = NULL;
  OverlayLayer *layer_ = NULL;
  uint32_t max_width_ = 0, max_height_ = 0;
  std::vector<size_t> source_layers_;
};

}  // namespace hwcomposer
#endif  // DISPLAY_PLANE_STATE_H_
