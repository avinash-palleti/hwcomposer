/*
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
    left_ = layer->GetDisplayFrame().left;
    top_ = layer->GetDisplayFrame().top;
    right_ = layer->GetDisplayFrame().right;
    bottom_ = layer->GetDisplayFrame().bottom;
  }

  State GetCompositionState() const {
    return state_;
  }

  int GetRight() const {
    return right_;
  }

  int GetBottom() const {
    return bottom_;
  }

  int GetLeft() const {
    return left_;
  }

  int GetTop() const {
    return top_;
  }

  void AddLayer(size_t index, const HwcRect<int> &display_frame) {
    if (left_ > display_frame.left) {
      left_ = display_frame.left;
    }

    if (top_ > display_frame.top) {
      top_ = display_frame.top;
    }

    if (right_ < display_frame.right) {
      right_ = display_frame.right;
    }

    if (bottom_ < display_frame.bottom) {
      bottom_ = display_frame.bottom;
    }

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
  int right_ = 0, bottom_ = 0, left_ = 0, top_ = 0;
  std::vector<size_t> source_layers_;
};

}  // namespace hwcomposer
#endif  // DISPLAY_PLANE_STATE_H_
