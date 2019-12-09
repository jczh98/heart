/*
 * Copyright (C) 2018-2019, neverfelly <neverfelly@gmail.com>
 *
 * This software is licensed under the terms of the MIT License.
 * See COPYING for further information.
 */

#ifndef CHIHAYA_SRC_CORE_IMAGE_H_
#define CHIHAYA_SRC_CORE_IMAGE_H_

#include "common.h"

CHIHAYA_NAMESPACE_BEGIN

class Image {
 public:
  Image(int width, int height, int channel);
  bool Save(const std::string &filename, bool flip = false) const;
  void Free() noexcept ;
  bool IsValid() const;
  unsigned char& At(int x, int y, int channel);
  void SetPixel(int x, int y, int r, int g, int b);
  virtual ~Image() noexcept ;
 private:
  unsigned char *data_;
  int width_, height_, channel_;
  std::string filename_;
};
CHIHAYA_NAMESPACE_END

#endif //CHIHAYA_SRC_CORE_IMAGE_H_
