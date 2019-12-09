/*
 * Copyright (C) 2018-2019, neverfelly <neverfelly@gmail.com>
 *
 * This software is licensed under the terms of the MIT License.
 * See COPYING for further information.
 */

#ifndef CHIHAYA_SRC_CORE_AABB_H_
#define CHIHAYA_SRC_CORE_AABB_H_

#include "hitable.h"
#include "ray.h"

CHIHAYA_NAMESPACE_BEGIN

inline float ffmin(float a, float b) { return a < b ? a : b; }
inline float ffmax(float a, float b) { return a > b ? a : b; }

class AABB {
 public:
  AABB() {}
  AABB(const Vector3f& a, const Vector3f& b) {
    min_ = a;
    max_ = b;
  }
  Vector3f min() const { return min_; }
  Vector3f max() const { return max_; }

  bool hit(const Ray& r, float tmin, float tmax) const {
    for (int a = 0; a < 3; a++) {
      float t0 = ffmin((min_[a] - r.o[a]) / r.d[a],
                       (max_[a] - r.o[a]) / r.d[a]);
      float t1 = ffmax((min_[a] - r.o[a]) / r.d[a],
                       (max_[a] - r.o[a]) / r.d[a]);
      tmin = ffmax(t0, tmin);
      tmax = ffmin(t1, tmax);
      if (tmax <= tmin)
        return false;
    }
    return true;
  }

 private:
  Vector3f min_, max_;
};

inline AABB SurroundingBox(AABB box0, AABB box1) {
  Vector3f small(ffmin(box0.min().x, box1.min().x),
                 ffmin(box0.min().y, box1.min().y),
                 ffmin(box0.min().z, box1.min().z));
  Vector3f big(ffmax(box0.max().x, box1.max().x),
               ffmax(box0.max().y, box1.max().y),
               ffmax(box0.max().z, box1.max().z));
  return AABB(small, big);
}

CHIHAYA_NAMESPACE_END

#endif