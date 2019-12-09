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

Image::Image(int width, int height, int channel) :
    data_{nullptr} {
  Free();
  width_ = width;
  height_ = height;
  channel_ = channel;
  data_ = (unsigned char *) malloc(width_ * height_ * channel_);
}

bool Image::Save(const std::string &filename, bool flip) const {
  if (!IsValid()) return false;
  stbi_flip_vertically_on_write(flip);
  const int val_num = width_ * height_ * channel_;
  auto uc_data = new stbi_uc[val_num];
  for (int i = 0; i < val_num; ++i) {

    uc_data[i] = static_cast<stbi_uc>(data_[i]);
  }
  auto rst = stbi_write_png(filename.c_str(), width_, height_, channel_, data_, width_ * channel_);

  delete[] uc_data;
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

void Image::SetPixel(int x, int y, int r, int g, int b) {
  DCHECK(channel_ == 3);
  At(x, y, 0) = r;
  At(x, y, 1) = g;
  At(x, y, 2) = b;
}

Image::~Image() noexcept {
  Free();
}

CHIHAYA_NAMESPACE_END
