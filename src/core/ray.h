/*
 * Copyright (C) 2018-2019, neverfelly <neverfelly@gmail.com>
 *
 * This software is licensed under the terms of the MIT License.
 * See COPYING for further information.
 */

#ifndef CHIHAYA_SRC_CORE_RAY_H_
#define CHIHAYA_SRC_CORE_RAY_H_

#include "common.h"
#include "vector3.h"

CHIHAYA_NAMESPACE_BEGIN

// Ray Declarations
class Ray {
 public:
  // Ray Public Methods
  Ray() : t_max(Infinity), time(0.f) {}
  Ray(const Vector3f &o, const Vector3f &d, Float t_max = Infinity,
      Float time = 0.f)
      : o(o), d(d), t_max(t_max), time(time) {}
  Vector3f operator()(Float t) const { return o + d * t; }
  bool HasNaNs() const { return (o.HasNaNs() || d.HasNaNs() || isNaN(t_max)); }
  friend std::ostream &operator<<(std::ostream &os, const Ray &r) {
    os << "[o=" << r.o << ", d=" << r.d << ", t_max=" << r.t_max
       << ", time=" << r.time << "]";
    return os;
  }

  // Ray Public Data
  Vector3f o;
  Vector3f d;
  mutable Float t_max;
  Float time;
};

CHIHAYA_NAMESPACE_END

#endif //CHIHAYA_SRC_CORE_RAY_H_
