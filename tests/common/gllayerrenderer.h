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

#ifndef GL_LAYER_RENDERER_H_
#define GL_LAYER_RENDERER_H_

#include "layerrenderer.h"
#include "esUtil.h"
#include <EGL/eglext.h>
#include <GLES2/gl2ext.h>

class GLLayerRenderer : public LayerRenderer {
 public:
  GLLayerRenderer(struct gbm_device* gbm_dev);
  ~GLLayerRenderer() override;

  bool Init(uint32_t width, uint32_t height, uint32_t format,
            glContext* gl = NULL, const char* resource_path = NULL) override;
  void Draw(int64_t* pfence) override;
  virtual void glDrawFrame() = 0;

 protected:
  GLuint gl_renderbuffer_;
  GLuint gl_framebuffer_;
  EGLImageKHR egl_image_;
};

#endif
