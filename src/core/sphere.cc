/*
 * Copyright (C) 2018-2019, neverfelly <neverfelly@gmail.com>
 *
 * This software is licensed under the terms of the MIT License.
 * See COPYING for further information.
 */

#include "sphere.h"

CHIHAYA_NAMESPACE_BEGIN

bool Sphere::hit(const chihaya::Ray &ray, chihaya::Float t_min, chihaya::Float t_max, chihaya::HitRecord &rec) const {
  Vector3f oc = ray.o - center;
  Float a = Dot(ray.d, ray.d);
  Float b = Dot(oc, ray.d);
  Float c = Dot(oc, oc) - radius * radius;
  Float discriminant = b * b - a * c;
  if (discriminant > 0) {
    Float temp = (-b - sqrt(b * b - a * c)) / a;
    if (temp < t_max && temp > t_min) {
      rec.t = temp;
      rec.p = ray(rec.t);
      rec.normal = (rec.p - center) / radius;
      rec.mat_ptr = mat_ptr;
      return true;
    }
    temp = (-b + sqrt(discriminant)) / a;
    if (temp < t_max && temp > t_min) {
      rec.t = temp;
      rec.p = ray(rec.t);
      rec.normal = (rec.p - center) / radius;
      rec.mat_ptr = mat_ptr;
      return true;
    }
  }
  return false;
}

bool Sphere::BoundingBox(float t0, float t1, AABB &box) const {
  box = AABB(center - Vector3f(radius, radius, radius), center + Vector3f(radius, radius, radius));
  return true;
}

CHIHAYA_NAMESPACE_END
