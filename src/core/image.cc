/*
 * Copyright (C) 2018-2019, neverfelly <neverfelly@gmail.com>
 *
 * This software is licensed under the terms of the MIT License.
 * See COPYING for further information.
 */

#include "image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

CHIHAYA_NAMESPACE_BEGIN

Image::Image(int width, int height, int channel) : data_{nullptr} {
  Free();
  width_ = width;
  height_ = height;
  channel_ = channel;
  data_ = (unsigned char *)malloc(width_ * height_ * channel_);
}

bool Image::Save(const std::string &filename, bool flip) const {
  if (!IsValid()) return false;
  stbi_flip_vertically_on_write(flip);
  auto rst = stbi_write_png(filename.c_str(), width_, height_, channel_, data_, width_ * channel_);
  return rst;
}

void Image::Free() noexcept {
  width_ = height_ = channel_ = 0;
  data_ = nullptr;
  filename_.clear();
}

bool Image::IsValid() const {
  return data_ != nullptr && channel_ > 0 && channel_ <= 4;
}

unsigned char &Image::At(int x, int y, int channel) {
  DCHECK(channel < channel_);
  return data_[(y * width_ + x) * channel_ + channel];
}

void Image::SetPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b) {
  DCHECK(channel_ == 3);
  At(x, y, 0) = r;
  At(x, y, 1) = g;
  At(x, y, 2) = b;
}

Image::~Image() noexcept {
  Free();
}

CHIHAYA_NAMESPACE_END
