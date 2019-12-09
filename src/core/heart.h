/*
 * Copyright (C) 2018-2019, neverfelly <neverfelly@gmail.com>
 *
 * This software is licensed under the terms of the MIT License.
 * See COPYING for further information.
 */

#ifndef CHIHAYA_SRC_CORE_HEART_H_
#define CHIHAYA_SRC_CORE_HEART_H_

#include "common.h"
#include "hitable.h"

CHIHAYA_NAMESPACE_BEGIN

class Heart : public Hitable {
 public:
  Heart(Vector3f center, Material *m) : center(center), mat_ptr(m) {
  }
  virtual bool hit(const Ray &r, Float t_min, Float t_max, HitRecord &rec) const;
  virtual bool BoundingBox(float t0, float t1, AABB &box) const;
  Vector3f center;
  Material *mat_ptr;
};

CHIHAYA_NAMESPACE_END

#endif