/*
 * Copyright (C) 2018-2019, neverfelly <neverfelly@gmail.com>
 *
 * This software is licensed under the terms of the MIT License.
 * See COPYING for further information.
 */

#ifndef CHIHAYA_SRC_CORE_SPHERE_H_
#define CHIHAYA_SRC_CORE_SPHERE_H_

#include "common.h"
#include "hitable.h"

CHIHAYA_NAMESPACE_BEGIN

class Sphere : public Hitable {
 public:
  Sphere(Vector3f center, Float r, Material *m) : center{center}, radius{r}, mat_ptr{m} {};
  virtual bool hit(const Ray &ray, Float t_min, Float t_max, HitRecord &rec) const;
  virtual bool BoundingBox(float t0, float t1, AABB &box) const;
  Vector3f center;
  Float radius;
  Material *mat_ptr;
};

CHIHAYA_NAMESPACE_END

#endif  //CHIHAYA_SRC_CORE_SPHERE_H_
