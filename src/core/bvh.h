/*
 * Copyright (C) 2018-2019, neverfelly <neverfelly@gmail.com>
 *
 * This software is licensed under the terms of the MIT License.
 * See COPYING for further information.
 */

#ifndef CHIHAYA_SRC_CORE_BVH_H_
#define CHIHAYA_SRC_CORE_BVH_H_

#include "hitable.h"

CHIHAYA_NAMESPACE_BEGIN

class BVHNode : public Hitable {
 public:
  BVHNode() {}
  BVHNode(Hitable** l, int n, float time0, float time1);
  virtual bool hit(const Ray& r, float tmin, float tmax, HitRecord& rec) const;
  virtual bool BoundingBox(float t0, float t1, AABB& box) const;
  Hitable* left;
  Hitable* right;
  AABB box;
};

CHIHAYA_NAMESPACE_END

#endif