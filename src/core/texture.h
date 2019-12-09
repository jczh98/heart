/*
 * Copyright (C) 2018-2019, neverfelly <neverfelly@gmail.com>
 *
 * This software is licensed under the terms of the MIT License.
 * See COPYING for further information.
 */

#ifndef CHIHAYA_SRC_CORE_TEXTURE_H_
#define CHIHAYA_SRC_CORE_TEXTURE_H_

#include "vector3.h"

CHIHAYA_NAMESPACE_BEGIN

class Texture {
 public:
  virtual Vector3f Value(float u, float v, const Vector3f& p) const = 0;
};

class ConstantTexture : public Texture {
 public:
  ConstantTexture() {}
  ConstantTexture(Vector3f c) : color(c) {}
  virtual Vector3f Value(float u, float v, const Vector3f& p) const {
    return color;
  }
  Vector3f color;
};

CHIHAYA_NAMESPACE_END

#endif