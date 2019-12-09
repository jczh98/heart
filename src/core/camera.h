/*
 * Copyright (C) 2018-2019, neverfelly <neverfelly@gmail.com>
 *
 * This software is licensed under the terms of the MIT License.
 * See COPYING for further information.
 */

#ifndef CHIHAYA_SRC_CORE_CAMERA_H_
#define CHIHAYA_SRC_CORE_CAMERA_H_

#include "common.h"
#include "ray.h"

CHIHAYA_NAMESPACE_BEGIN

Vector3f RandomInUnitDisk() {
  Vector3f p;
  do {
    p = 2.0 * Vector3f(drand48(), drand48(), 0) - Vector3f(1, 1, 0);
  } while (Dot(p, p) >= 1.0);
  return p;
}

class Camera {
 public:
  Camera(const Vector3f &lookfrom,
         const Vector3f &lookat,
         const Vector3f &vup,
         Float vfov,
         Float aspect,
         Float aperture,
         Float focus_dist) {
    // vfov is top to bottom in degrees
    lens_radius = aperture / 2;
    Float theta = vfov * M_PI / 180;
    Float half_height = tan(theta / 2);
    Float half_width = aspect * half_height;
    origin = lookfrom;
    w = Normalize(lookfrom - lookat);
    u = Normalize(Cross(vup, w));
    v = Cross(w, u);
    lower_left_corner = origin - half_width * focus_dist * u - half_height * focus_dist * v - focus_dist * w;
    horizontal = 2 * half_width * focus_dist * u;
    vertical = 2 * half_height * focus_dist * v;
  }
  Ray GetRay(Float s, Float t) {
    Vector3f rd = lens_radius * RandomInUnitDisk();
    Vector3f offset = u * rd.x + v * rd.y;
    return Ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset);
  }

  Vector3f origin;
  Vector3f lower_left_corner;
  Vector3f horizontal;
  Vector3f vertical;
  Vector3f u, v, w;
  Float lens_radius;
};

CHIHAYA_NAMESPACE_END

#endif  //CHIHAYA_SRC_CORE_CAMERA_H_
